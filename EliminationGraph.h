#pragma once

#include <rope/CommonGraph.h>
#include <rope/IntList.h>
#include <rope/Placeholder.h>

#include "SparseMatrix.h"

#include <vector>

namespace spandex
{
	template<class T>
	class EliminationGraph
	{
	private:
		rope::CommonGraph<rope::Placeholder> adj;
		rope::IntList list;
		std::vector<int> size;

	public:

		EliminationGraph(SparseMatrix<T>& a) : list(a.columnCount), adj(a.columnCount, a.nnz)
		{
			assert(Layout::DefaultLayout == a.layout);

			int n = a.columnCount;
			size.resize(n, 0);

			for (int j = 0; j < n; j++)
			{
				for (int i = a.columns[j]; i < a.columns[j + 1]; i++)
				{
					int ii = a.columnsRows[i];
					for (int k = a.rows[ii]; k < a.rows[ii + 1]; k++)
					{
						int r = a.rowsColumns[k];
						if (r <= j)
						{
							continue;
						}

						list.Push(r);
					}
				}

				while (!list.IsEmpty())
				{
					int i = list.Pop();

					adj.Insert(i, j, rope::Placeholder());
					adj.Insert(j, i, rope::Placeholder());

					size[i] += 1;
					size[j] += 1;
				}
			}
		}

		decltype(adj.GetIterator()) GetIterator()
		{
			return adj.GetIterator();
		}

		int GetSize(int vertex) const
		{
			return size[vertex];
		}

		void Eliminate(int vertex)
		{
			auto primary = adj.GetIterator();
			auto secondary = adj.GetIterator();

			primary.Setup(vertex);

			while (primary.HasNext())
			{
				int i = primary.Next().first;

				secondary.Setup(i);
				while (secondary.HasNext())
				{
					int j = secondary.Next().first;

					if (j != i && j != vertex)
					{
						list.Push(j);
					}
				}

				secondary.Setup(vertex);
				while (secondary.HasNext())
				{
					int j = secondary.Next().first;

					if (j != i && j != vertex)
					{
						list.Push(j);
					}
				}

				size[i] = list.size;
				adj.RemoveFrom(i);
				while (!list.IsEmpty())
				{
					adj.Insert(i, list.Pop(), rope::Placeholder());
				}
			}

			size[vertex] = 0;
			adj.RemoveFrom(vertex);
		}

		bool IsLeaf(int vertex)
		{
			return adj.IsLeaf(vertex);
		}

	};
}