#pragma once

#include "SparseArray.h"
#include "SparseMatrix.h"
#include "Permutation.h"
#include "Normalization.h"

#include <rope/IntList.h>

#include <stack>
#include <vector>

namespace spandex
{
	template<class T>
	class CholeskySolver
	{
	private:
		rope::IntList list;

		SparseMatrix<T> ata;
		SparseMatrix<T> ld;
		std::vector<T> y;
		Permutation perm;

	public:
		int rowCount;
		int columnCount;
		Permutation::Type permutation;
		Normalization::Type normalization;

		CholeskySolver(int rowCount, int columnCount) : rowCount(rowCount), columnCount(columnCount),
			list(columnCount)
		{
			permutation = Permutation::Type::NoPermutation;
			normalization = Normalization::Type::NoNormalization;
		}

		Permutation& GetPermutation()
		{
			return perm;
		}

		void SolveSym(SparseMatrix<T>& a)
		{
			perm = Permutation::Build(a, permutation);
			ata = SparseMatrix<T>(std::move(SqrSym(a, perm)));
			ld = SparseMatrix<T>(std::move(CholSym(ata)));
			y.resize(a.rowCount);
		}

		std::vector<T> Solve(SparseMatrix<T>& a, std::vector<T>& b)
		{
			SqrTo(a, perm, ata);
			CholTo(ata, ld);

			MulTo(a, b, y);
			y = Permute(y, [&](int i) { return perm.GetPermuted(i); });

			std::vector<T> x(ld.rowCount);
			SolveTo(ld, y, x);

			x = Permute(x, [&](int i) { return perm.GetPrimary(i); });

			return std::move(x);
		}

		std::vector<T> Update(SparseArray<T>& u, T v)
		{
			assert(ld.rowCount == u.size);

			const T zero = (T)0;
			for (auto it = u.begin(); it != u.end(); ++it)
			{
				if (zero != it->second)
				{
					y[perm.GetPermuted(it->first)] += v * it->second;
				}
			}

			Update(ld, u);

			std::vector<T> x(ld.rowCount);
			SolveTo(ld, y, x);

			x = Permute(x, [&](int i) { return perm.GetPrimary(i); });

			return std::move(x);
		}

		std::vector<T> Downdate(SparseArray<T>& u, T v)
		{
			assert(ld.rowCount == u.size);

			const T zero = (T)0;
			for (auto it = u.begin(); it != u.end(); ++it)
			{
				if (zero != it->second)
				{
					y[perm.GetPermuted(it->first)] -= v * it->second;
				}
			}

			Downdate(ld, perm, u);

			std::vector<T> x(ld.rowCount);
			SolveTo(ld, y, x);

			x = Permute(x, [&](int i) { return perm.GetPrimary(i); });

			return std::move(x);
		}

		SparseMatrix<T> CholSym(SparseMatrix<T>& symm)
		{
			assert(Layout::LowerSymmetric == symm.layout);

			int n = symm.rowCount;

			rope::CommonGraph<T> g(n);
			auto it = g.GetIterator();

			std::stack<int> columns;
			list.Clear();

			for (int i = 0; i < n; i++)
			{
				for (int j = symm.rows[i]; j < symm.rows[i + 1]; j++)
				{
					int jj = symm.rowsColumns[j];
					list.Push(jj);
					columns.push(jj);
				}

				while (columns.size() > 0)
				{
					int jj = columns.top();
					columns.pop();

					if (!g.HasConnections(jj))
					{
						continue;
					}

					it.Setup(jj);
					while (it.HasNext())
					{
						int r = it.Next().first;
						if (jj == r)
						{
							continue;
						}
						if (!list.Contains(r))
						{
							list.Push(r);
							columns.push(r);
						}
					}
				}

				while (!list.IsEmpty())
				{
					g.Insert(list.Pop(), i, T());
				}
			}

			auto c = SparseMatrix<T>::FromGraph(n, n, g);
			auto ct = c.Transpose();
			ct.layout = Layout::LowerTriangle;

			return std::move(ct);
		}

		void CholTo(SparseMatrix<T>& sym, SparseMatrix<T>& ld)
		{
			assert(Layout::LowerSymmetric == sym.layout);
			assert(Layout::LowerTriangle == ld.layout);

			int n = sym.rowCount;

			std::vector<T> acc(n, T());

			for (int j = 0; j < n; j++)
			{
				for (int i = sym.columns[j]; i < sym.columns[j + 1]; i++)
				{
					acc[sym.columnsRows[i]] = sym.values[i];
				}

				for (int k = ld.rows[j]; k < ld.rows[j + 1] && ld.rowsColumns[k] < j; k++)
				{
					int r = ld.rowsColumns[k];
					T a = ld.values[ld.positions[k]] * ld.values[ld.columns[r]];

					for (int i = ld.positions[k]; i < ld.columns[r + 1]; i++)
					{
						acc[ld.columnsRows[i]] -= a * ld.values[i];
					}
				}

				T d = ld.values[ld.columns[j]] = acc[j];

				for (int k = ld.columns[j] + 1; k < ld.columns[j + 1]; k++)
				{
					ld.values[k] = acc[ld.columnsRows[k]] / d;
					acc[ld.columnsRows[k]] = T();
				}
			}
		}

		void SolveTo(SparseMatrix<T>& ld, std::vector<T>& b, std::vector<T>& result)
		{
			assert(Layout::LowerTriangle == ld.layout);
			assert((int)b.size() == ld.rowCount);

			auto y = SolveLower(ld, b);

			auto z = SolveDiag(ld, y);

			auto x = SolveUpper(ld, z);

			result.assign(x.begin(), x.end());
		}

	private:

		SparseMatrix<T> SqrSym(SparseMatrix<T>& a, Permutation& perm)
		{
			assert(Layout::DefaultLayout == a.layout);

			rope::CommonGraph<T> g(a.columnCount, a.columnCount);
			list.Clear();

			for (int j = 0; j < a.columnCount; j++)
			{
				int jj = perm.GetPermuted(j);
				for (int i = a.columns[jj]; i < a.columns[jj + 1]; i++)
				{
					int ii = a.columnsRows[i];
					for (int k = a.rows[ii]; k < a.rows[ii + 1]; k++)
					{
						int r = perm.GetPrimary(a.rowsColumns[k]);
						if (r < j)
						{
							continue;
						}
						list.Push(r);
					}
				}

				list.Push(j);

				while (!list.IsEmpty())
				{
					g.Insert(list.Pop(), j, T());
				}
			}

			auto s = SparseMatrix<T>::FromGraph(a.columnCount, a.columnCount, g);
			s.layout = Layout::LowerSymmetric;

			return std::move(s);
		}

		void SqrTo(SparseMatrix<T>& a, Permutation& perm, SparseMatrix<T>& ata)
		{
			assert(Layout::DefaultLayout == a.layout);
			assert(Layout::LowerSymmetric == ata.layout);

			std::vector<T> acc(a.columnCount);

			for (int j = 0; j < a.columnCount; j++)
			{
				int jj = perm.GetPermuted(j);
				for (int i = a.columns[jj]; i < a.columns[jj + 1]; i++)
				{
					int ii = a.columnsRows[i];

					for (int k = a.rows[ii]; k < a.rows[ii + 1]; k++)
					{
						int r = perm.GetPrimary(a.rowsColumns[k]);

						acc[r] += a.values[i] * a.values[a.positions[k]];
					}
				}

				for (int i = ata.columns[j]; i < ata.columns[j + 1]; i++)
				{
					int r = ata.columnsRows[i];
					ata.values[i] = acc[r];
					acc[r] = T();
				}
			}
		}

		static std::vector<T> Permute(std::vector<T>& values, std::function<int(int)> indexFunc)
		{
			int n = (int)values.size();
			std::vector<T> result(n);

			for (int i = 0; i < n; i++)
			{
				result[i] = values[indexFunc(i)];
			}

			return std::move(result);
		}

		static void MulTo(SparseMatrix<T>& a, std::vector<T>& b, std::vector<T>& c)
		{
			assert(a.rowCount == (int)b.size());
			assert(a.columnCount == (int)c.size());

			std::fill(std::begin(c), std::end(c), (T)0);

			for (int i = 0; i < a.rowCount; i++)
			{
				for (int j = a.rows[i]; j < a.rows[i + 1]; j++)
				{
					c[a.rowsColumns[j]] += b[i] * a.values[a.positions[j]];
				}
			}
		}

		static std::vector<T> SolveLower(SparseMatrix<T>& ld, std::vector<T>& b)
		{
			assert(Layout::LowerTriangle == ld.layout);
			assert((int)b.size() == ld.rowCount);

			int n = ld.rowCount;
			std::vector<T> y(n);

			for (int i = 0; i < n; i++)
			{
				T sum = (T)0;
				for (int j = ld.rows[i]; j < (ld.rows[i + 1] - 1); j++)
				{
					sum += ld.values[ld.positions[j]] * y[ld.rowsColumns[j]];
				}
				y[i] = b[i] - sum;
			}

			return std::move(y);
		}

		static std::vector<T> SolveDiag(SparseMatrix<T>& ld, std::vector<T>& y)
		{
			assert(Layout::LowerTriangle == ld.layout);
			assert((int)y.size() == ld.rowCount);

			int n = ld.rowCount;
			std::vector<T> x(n);

			for (int j = (n - 1); j >= 0; j--)
			{
				x[j] = y[j] / ld.values[ld.columns[j]];
			}

			return std::move(x);
		}

		static std::vector<T> SolveUpper(SparseMatrix<T>& ld, std::vector<T>& z)
		{
			assert(Layout::LowerTriangle == ld.layout);
			assert((int)z.size() == ld.rowCount);

			int n = ld.rowCount;
			std::vector<T> x(n);

			for (int j = (n - 1); j >= 0; j--)
			{
				T sum = (T)0;
				for (int i = ld.columns[j] + 1; i < ld.columns[j + 1]; i++)
				{
					sum += ld.values[i] * x[ld.columnsRows[i]];
				}
				x[j] = z[j] - sum;
			}

			return std::move(x);
		}

		static void Update(SparseMatrix<T>& ld, const SparseArray<T>& u)
		{
			assert(Layout::LowerTriangle == ld.layout);
			assert(ld.rowCount == u.size);

			const T zero = (T)0;
			T a = (T)1, b = zero, c = zero;
			std::vector<T> vals(u.size, zero);
			for (auto it = u.begin(); it != u.end(); ++it)
			{
				vals[it->first] = it->second;
			}

			for (auto it = u.begin(); it != u.end(); ++it)
			{
				int j = it->first;

				int jj = ld.columns[j];

				T diag = ld.values[jj];
				T x = vals[j];
				b = a + x * x / diag;
				ld.values[jj] = diag * b / a;
				c = x / (diag * b);
				a = b;

				for (int i = jj + 1; i < ld.columns[j + 1]; i++)
				{
					int ii = ld.columnsRows[i];

					vals[ii] -= x * ld.values[i];
					ld.values[i] += c * vals[ii];
				}
			}
		}

		static void Downdate(SparseMatrix<T>& ld, Permutation& perm, const SparseArray<T>& u)
		{
			assert(Layout::LowerTriangle == ld.layout);
			assert(ld.rowCount == u.size);

			const T zero = (T)0;
			const T one = (T)1;

			std::vector<T> vals(u.size, zero);
			for (auto it = u.begin(); it != u.end(); ++it)
			{
				vals[it->first] = it->second;
			}

			auto diag = ld.GetDiag();
			for (int i = 0; i < u.size; i++)
			{
				ld.SetColumnwise(i, i, one);
			}

			auto p = SolveDiag(ld, vals);

			for (int i = 0; i < u.size; i++)
			{
				ld.SetColumnwise(i, i, diag[i]);
			}

			T sum = zero;
			for (int i = 0; i < u.size; i++)
			{
				sum += p[i] / ld.GetColumnwise(i, i) * p[i];
			}

			T a = one - sum;

			for (int j = u.size - 1; -1 != j; j--)
			{
				int jj = ld.columns[j];
				T d = ld.values[jj];

				T b = a + p[j] * p[j] / d;
				ld.values[jj] = d * a / b;
				T c = -p[j] / (d * a);
				vals[j] = p[j];

				a = b;

				for (int i = jj + 1; i < ld.columns[j + 1]; i++)
				{
					int ii = ld.columnsRows[i];

					T v = vals[ii];
					vals[ii] += p[j] * ld.values[i];
					ld.values[i] += c * v;
				}
			}
		}
	};
}