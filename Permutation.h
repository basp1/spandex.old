#pragma once

#include "SparseMatrix.h"
#include "EliminationGraph.h"

#include <limits>
#include <vector>

#include <libra/SegmentTree.h>

namespace spandex
{
	class Permutation
	{
	public:
		enum Type
		{
			NoPermutation,
			AMD
		};

	private:
		int size;
		std::vector<int> primary;
		std::vector<int> permuted;

		Permutation(int size) : size(size), primary(size, -1), permuted(size, -1)
		{
		}

		void Insert(int prime, int perm)
		{
			assert(prime < size);
			assert(perm < size);
			assert(primary[perm] < 0 && permuted[prime] < 0);

			primary[perm] = prime;
			permuted[prime] = perm;
		}

	public:
		Permutation() : size(0)
		{}

		int GetPrimary(int perm)
		{
			assert(perm < size);

			return primary[perm];
		}

		int GetPermuted(int prime)
		{
			assert(prime < size);

			return permuted[prime];
		}

		bool Equals(Permutation& that) const
		{
			return primary == that.primary && permuted == that.permuted;
		}

		template<class T>
		static Permutation Build(SparseMatrix<T>& a, Permutation::Type type)
		{
			if (type == Permutation::Type::AMD)
			{
				return BuildAmd<T>(a);
			}
			else
			{
				int n = a.columnCount;
				Permutation pt(n);

				for (int i = 0; i < n; i++)
				{
					pt.Insert(i, i);
				}

				return std::move(pt);
			}
		}

	private:
		template<class T>
		static Permutation BuildAmd(SparseMatrix<T>& a)
		{
			using item = std::pair<int, double>;

			int n = a.columnCount;

			Permutation pt(n);
			EliminationGraph<T> eg(a);

			std::vector<item> inits(n);
			for (int i = 0; i < n; i++)
			{
				inits[i] = item(i, eg.GetSize(i));
			}

			libra::SegmentTree<item> st(std::make_pair<int, double>(-1, std::numeric_limits<int>::max()),
				[](const item& x, const item& y) { return x.second < y.second ? x : y; },
				inits);

			auto it = eg.GetIterator();
			std::vector<int> vertices;

			for (int i = 0; i < n; i++)
			{
				int min = st.GetTop().first;

				pt.Insert(i, min);

				vertices.clear();
				it.Setup(min);
				while (it.HasNext())
				{
					vertices.push_back(it.Next().first);
				}

				eg.Eliminate(min);

				for (int j = 0; j < (int)vertices.size(); j++)
				{
					int jj = vertices[j];
					st.Set(jj, std::move(item(jj, eg.GetSize(jj))));
				}

				st.Set(min, std::make_pair<int, double>(-1, std::numeric_limits<int>::max()));
			}

			return std::move(pt);
		}
	};
}