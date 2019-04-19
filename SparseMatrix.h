#pragma once

#include <rope/CommonGraph.h>
#include <rope/IntList.h>

#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <numeric>
#include <vector>


namespace spandex
{
	enum Layout
	{
		DefaultLayout, LowerTriangle, UpperTriangle, LowerSymmetric, UpperSymmetric
	};

	template<class T>
	class SparseMatrix
	{
	public:
		std::vector<int> columns;
		std::vector<int> columnsRows;
		std::vector<T> values;

		std::vector<int> rows;
		std::vector<int> rowsColumns;
		std::vector<int> positions;


		Layout layout;
		int nnz;
		int rowCount;
		int columnCount;

	private:

		rope::IntList list;

		SparseMatrix(int rowCount, int columnCount, int capacity) : SparseMatrix()
		{
			Resize(rowCount, columnCount, capacity);
		}

		void Resize(int rowCount, int columnCount, int capacity)
		{
			list.Resize(std::max(rowCount, columnCount));
			layout = Layout::DefaultLayout;

			this->nnz = 0;
			this->rowCount = rowCount;
			this->columnCount = columnCount;

			columns.resize(1 + columnCount);
			columnsRows.resize(capacity);
			values.resize(capacity);

			rows.resize(1 + rowCount);
			rowsColumns.resize(capacity);
			positions.resize(capacity);

		}

	public:
		SparseMatrix() : list(0), nnz(0), layout(Layout::DefaultLayout), rowCount(0), columnCount(0)
		{
		}

		SparseMatrix(const SparseMatrix<T> & copy) :SparseMatrix()
		{
			operator=(copy);
		}

		SparseMatrix(SparseMatrix<T> && move) :SparseMatrix()
		{
			operator=(move);
		}

		SparseMatrix<T>& operator=(const SparseMatrix<T> & copy)
		{
			Resize(copy.rowCount, copy.columnCount, copy.nnz);

			layout = copy.layout;

			nnz = copy.nnz;

			columns.assign(std::begin(copy.columns), std::end(copy.columns));
			columnsRows.assign(std::begin(copy.columnsRows), std::end(copy.columnsRows));
			values.assign(std::begin(copy.values), std::end(copy.values));

			rows.assign(std::begin(copy.rows), std::end(copy.rows));
			rowsColumns.assign(std::begin(copy.rowsColumns), std::end(copy.rowsColumns));
			positions.assign(std::begin(copy.positions), std::end(copy.positions));

			return *this;
		}

		SparseMatrix<T>& operator=(SparseMatrix<T> && move)
		{
			Resize(move.rowCount, move.columnCount, move.nnz);

			layout = move.layout;
			nnz = move.nnz;

			columns = std::move(move.columns);
			columnsRows = std::move(move.columnsRows);
			values = std::move(move.values);

			rows = std::move(move.rows);
			rowsColumns = std::move(move.rowsColumns);
			positions = std::move(move.positions);

			return *this;
		}

		static SparseMatrix<T> Empty(int rowCount, int columnCount, int capacity)
		{
			return SparseMatrix<T>(rowCount, columnCount, capacity);
		}

		static SparseMatrix<T> FromCSR(int rowCount, int columnCount,
			const std::vector<int> & rows, const std::vector<int> & columns, const std::vector<T> & values)
		{
			assert(rowCount == ((int)rows.size() - 1));
			assert(values.size() == columns.size());
			assert((int)values.size() == rows.back());

			auto sparse = SparseMatrix<T>::Empty(rowCount, columnCount, (int)values.size());
			sparse.nnz = (int)values.size();
			sparse.rows = rows;

			for (int i = 0; i < rowCount; i++)
			{
				for (int j = rows[i]; j < rows[i + 1]; j++)
				{
					sparse.columns[columns[j] + 1] += 1;
				}
			}

			for (int j = 1; j < 1 + columnCount; j++)
			{
				sparse.columns[j] += sparse.columns[j - 1];
			}

			std::vector<int> tcolumns(sparse.columns);
			std::vector<int> trows(sparse.rows);
			for (int i = 0; i < rowCount; i++)
			{
				for (int j = rows[i]; j < rows[i + 1]; j++)
				{
					int c = tcolumns[columns[j]];
					sparse.columnsRows[c] = i;
					sparse.values[c] = values[j];

					sparse.rowsColumns[trows[i]] = columns[j];
					sparse.positions[trows[i]] = c;

					tcolumns[columns[j]] += 1;
					trows[i] += 1;
				}
			}

			sparse.Sort();

			return std::move(sparse);
		}

		static SparseMatrix<T> FromGraph(int rowCount, int columnCount, rope::CommonGraph<T> & graph)
		{
			auto sparse = SparseMatrix<T>::Empty(rowCount, columnCount, graph.size);
			sparse.nnz = graph.size;

			sparse.rows[0] = 0;
			std::vector<T> values(sparse.nnz);

			auto it = graph.GetIterator();
			for (int i = 0, k = 0; i < rowCount; i++)
			{
				it.Setup(i);
				while (it.HasNext())
				{
					auto item = it.Next();

					sparse.rowsColumns[k] = item.first;
					sparse.columns[item.first] += 1;
					values[k] = item.second;

					k += 1;
				}
				sparse.rows[i + 1] = k;
			}

			for (int i = 0, n = 0; i <= columnCount; i++)
			{
				int t = n;
				n += sparse.columns[i];
				sparse.columns[i] = t;
			}

			std::vector<int> colIndex = sparse.columns;
			for (int i = 0, n = 0; i < rowCount; i++)
			{
				for (int j = sparse.rows[i]; j < sparse.rows[i + 1]; j++)
				{
					int col = sparse.rowsColumns[j];
					sparse.positions[j] = colIndex[col];
					sparse.values[colIndex[col]] = values[j];
					sparse.columnsRows[colIndex[col]] = i;
					colIndex[col] += 1;
				}
			}

			sparse.Sort();

			return std::move(sparse);
		}

	public:
		void Sort()
		{
			std::vector<int> oldIndices(nnz);
			std::iota(std::begin(oldIndices), std::end(oldIndices), 0);
			auto newIndices = oldIndices;

			for (int j = 0; j < columnCount; j++)
			{
				KeyValueSort(columnsRows, newIndices, columns[j], columns[j + 1]);
			}

			Reindex(values, oldIndices, newIndices);

			for (int j = 0; j < rowCount; j++)
			{
				KeyValueSort(rowsColumns, positions, rows[j], rows[j + 1]);
			}

			for (int i = 0; i < nnz; i++)
			{
				positions[i] = newIndices[positions[i]];
			}
		}

		bool Equals(const SparseMatrix<T> & that) const
		{
			if (nnz != that.nnz)
			{
				return false;
			}
			if (rowCount != that.rowCount)
			{
				return false;
			}
			if (columnCount != that.columnCount)
			{
				return false;
			}
			if (columns != that.columns)
			{
				return false;
			}
			if (columnsRows != that.columnsRows)
			{
				return false;
			}
			if (positions != that.positions)
			{
				return false;
			}
			if (values != that.values)
			{
				return false;
			}

			return true;
		}

		bool Equals(const SparseMatrix<T> & that, std::function<bool(const T&, const T&)> compareFunc) const
		{
			if (nnz != that.nnz)
			{
				return false;
			}
			if (rowCount != that.rowCount)
			{
				return false;
			}
			if (columnCount != that.columnCount)
			{
				return false;
			}
			if (columns != that.columns)
			{
				return false;
			}
			if (columnsRows != that.columnsRows)
			{
				return false;
			}
			if (positions != that.positions)
			{
				return false;
			}

			for (int i = 0; i < nnz; i++)
			{
				if (!compareFunc(values[i], that.values[i]))
				{
					return false;
				}
			}

			return true;
		}

		SparseMatrix<T> Transpose()
		{
			auto that = SparseMatrix<T>::Empty(columnCount, rowCount, nnz);

			that.nnz = nnz;
			that.columns.assign(std::begin(rows), std::end(rows));
			that.columnsRows.assign(std::begin(rowsColumns), std::end(rowsColumns));
			that.rows.assign(std::begin(columns), std::end(columns));
			that.rowsColumns.assign(std::begin(columnsRows), std::end(columnsRows));

			for (int k = 0, j = 0; j < columnCount; j++)
			{
				for (int i = columns[j]; i < columns[j + 1]; i++, k++)
				{
					that.positions[positions[i]] = k;
					that.values[k] = values[positions[i]];
				}
			}

			return std::move(that);
		}

		bool Contains(int row, int column) const
		{
			assert(column >= 0 && column < columnCount);
			assert(row >= 0 && row < rowCount);

			for (int j = columns[column]; j < columns[column + 1]; j++)
			{
				if (row == columnsRows[j])
				{
					return true;
				}
			}

			return false;
		}

		T GetRowwise(int row, int column) const
		{
			assert(column >= 0 && column < columnCount);
			assert(row >= 0 && row < rowCount);

			for (int i = rows[row]; i < rows[row + 1]; i++)
			{
				if (column == rowsColumns[i])
				{
					return values[positions[i]];
				}
			}

			return T();
		}

		void SetRowwise(int row, int column, const T & value)
		{
			assert(column >= 0 && column < columnCount);
			assert(row >= 0 && row < rowCount);

			for (int i = rows[row]; i < rows[row + 1]; i++)
			{
				if (column == rowsColumns[i])
				{
					values[positions[i]] = value;
					return;
				}
			}

			throw std::out_of_range("");
		}

		T GetColumnwise(int row, int column) const
		{
			assert(column >= 0 && column < columnCount);
			assert(row >= 0 && row < rowCount);

			for (int i = columns[column]; i < columns[column + 1]; i++)
			{
				if (row == columnsRows[i])
				{
					return values[i];
				}
			}

			return T();
		}

		void SetColumnwise(int row, int column, const T & value)
		{
			assert(column >= 0 && column < columnCount);
			assert(row >= 0 && row < rowCount);

			for (int i = columns[column]; i < columns[column + 1]; i++)
			{
				if (row == columnsRows[i])
				{
					values[i] = value;
					return;
				}
			}

			throw std::out_of_range("");
		}

	public:
		SparseMatrix<T> Add(SparseMatrix<T> & b)
		{
			assert(rowCount == b.rowCount);
			assert(columnCount == b.columnCount);

			auto c = AddSym(b);
			AddTo(b, c);
			return std::move(c);
		}

		SparseMatrix<T> AddSym(SparseMatrix<T> & b)
		{
			assert(rowCount == b.rowCount);
			assert(columnCount == b.columnCount);

			rope::CommonGraph<T> c(rowCount, nnz);

			list.Clear();

			for (int j = 0; j < columnCount; j++)
			{
				for (int i = columns[j]; i < columns[j + 1]; i++)
				{
					list.Push(columnsRows[i]);
				}
				for (int i = b.columns[j]; i < b.columns[j + 1]; i++)
				{
					list.Push(b.columnsRows[i]);
				}
				while (!list.IsEmpty())
				{
					c.Insert(list.Pop(), j, T());
				}
			}

			return std::move(FromGraph(rowCount, columnCount, c));
		}

		void AddTo(SparseMatrix<T> & b, SparseMatrix<T> & c)
		{
			assert(rowCount == b.rowCount);
			assert(columnCount == b.columnCount);
			assert(rowCount == c.rowCount);
			assert(columnCount == c.columnCount);

			std::vector<T> acc(columnCount, (T)0);

			for (int i = 0; i < rowCount; i++)
			{
				for (int j = columns[i]; j < columns[i + 1]; j++)
				{
					acc[columnsRows[j]] = values[j];
				}

				for (int j = b.columns[i]; j < b.columns[i + 1]; j++)
				{
					acc[b.columnsRows[j]] += b.values[j];
				}

				for (int j = c.columns[i]; j < c.columns[i + 1]; j++)
				{
					int jj = c.columnsRows[j];
					c.values[j] = acc[jj];
					acc[jj] = (T)0;
				}
			}
		}

		SparseMatrix<T> Mul(SparseMatrix<T> & b)
		{
			assert(columnCount == b.rowCount);

			auto c = MulSym(b);
			MulTo(b, c);
			return std::move(c);
		}

		SparseMatrix<T> MulSym(SparseMatrix<T> & b)
		{
			assert(columnCount == b.rowCount);

			list.Clear();

			int initcap = (int)(nnz / 3);
			rope::CommonGraph<T> c(rowCount, initcap);

			for (int j = 0; j < b.columnCount; j++)
			{
				for (int i = b.columns[j]; i < b.columns[j + 1]; i++)
				{
					int ii = b.columnsRows[i];
					for (int k = columns[ii]; k < columns[ii + 1]; k++)
					{
						list.Push(columnsRows[k]);
					}
				}

				while (!list.IsEmpty())
				{
					c.Insert(list.Pop(), j, T());
				}
			}

			return FromGraph(rowCount, b.columnCount, c);
		}

		void MulTo(SparseMatrix<T> & b, SparseMatrix<T> & c)
		{
			assert(rowCount == c.rowCount);
			assert(columnCount == b.rowCount);
			assert(b.columnCount == c.columnCount);

			std::vector<T> acc(c.rowCount, (T)0);

			for (int j = 0; j < b.columnCount; j++)
			{
				for (int i = b.columns[j]; i < b.columns[j + 1]; i++)
				{
					int ii = b.columnsRows[i];
					for (int k = columns[ii]; k < columns[ii + 1]; k++)
					{
						acc[columnsRows[k]] += b.values[i] * values[k];
					}
				}

				for (int i = c.columns[j]; i < c.columns[j + 1]; i++)
				{
					int jj = c.columnsRows[i];
					c.values[i] = acc[jj];
					acc[jj] = (T)0;
				}
			}
		}

		SparseMatrix<T> Sqr()
		{
			auto s = SqrSym();
			SqrTo(s);
			return std::move(s);
		}

		SparseMatrix<T> SqrSym()
		{
			rope::CommonGraph<T> g(columnCount, columnCount);
			list.Clear();

			for (int j = 0; j < columnCount; j++)
			{
				for (int i = columns[j]; i < columns[j + 1]; i++)
				{
					int ii = columnsRows[i];
					for (int k = rows[ii]; k < rows[ii + 1]; k++)
					{
						int r = rowsColumns[k];
						list.Push(r);
					}
				}

				list.Push(j);

				while (!list.IsEmpty())
				{
					g.Insert(list.Pop(), j, T());
				}
			}

			auto ata = FromGraph(columnCount, columnCount, g);

			return std::move(ata);
		}

		void SqrTo(SparseMatrix<T> & ata)
		{
			assert(Layout::DefaultLayout == ata.layout);

			std::vector<T> acc(columnCount);

			for (int j = 0; j < columnCount; j++)
			{
				for (int i = columns[j]; i < columns[j + 1]; i++)
				{
					int ii = columnsRows[i];

					for (int k = rows[ii]; k < rows[ii + 1]; k++)
					{
						int r = rowsColumns[k];

						acc[r] += values[i] * values[positions[k]];
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

	private:
		template<class Key, class Value>
		static void KeyValueSort(std::vector<Key> & keys, std::vector<Value> & values, int begin, int end)
		{
			assert(keys.size() == values.size());
			assert(begin >= 0);
			assert(end >= begin && end <= (int)keys.size());

			std::vector<int> indices(end - begin);
			std::iota(std::begin(indices), std::end(indices), begin);

			std::vector<int> sorted = indices;
			std::sort(std::begin(sorted), std::end(sorted),
				[&](int a, int b) { return keys[a] < keys[b]; });

			Reindex(values, indices, sorted);
			Reindex(keys, indices, sorted);
		}

		template<class Y>
		static void Reindex(std::vector<Y> & values,
			const std::vector<int> & oldIndices, const std::vector<int> & newIndices)
		{
			assert(oldIndices.size() == newIndices.size());
			assert(oldIndices.size() <= values.size());

			int n = (int)oldIndices.size();
			std::vector<Y> t = values;

			for (int i = 0; i < n; i++)
			{
				t[i] = values[oldIndices[i]];
			}

			for (int i = 0; i < n; i++)
			{
				values[newIndices[i]] = t[i];
			}
		}
	};
}