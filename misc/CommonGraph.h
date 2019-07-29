#pragma once

#include "FlatMap.h"

#include <cassert>
#include <functional>
#include <numeric>
#include <vector>

namespace spandex::misc
{
	template<class T>
	class CommonGraph
	{
	public:
		class Iterator;
		friend class Iterator;

		const static int NIL = -1;

		std::vector<int> start;
		std::vector<int> next;
		std::vector<int> vertices;
		std::vector<T> edges;

		int size;
		int free = NIL;

	public:

		CommonGraph(int vertexCount, int capacity = 0)
		{
			size = 0;
			free = NIL;

			start.resize(vertexCount, NIL);

			next.reserve(capacity);
			vertices.reserve(capacity);
			edges.reserve(capacity);
		}

		void Resize(int vertexCount)
		{
			start.resize(vertexCount);
			Clear();
		}

		void Clear()
		{
			size = 0;
			free = NIL;

			std::fill(std::begin(start), std::end(start), NIL);
			next.resize(0);
			vertices.resize(0);
			edges.resize(0);
		}

		void AddVertex()
		{
			start.push_back(NIL);
		}

		Iterator GetIterator()
		{
			return Iterator(*this);
		}

		bool HasConnections(int vertex)
		{
			assert(vertex >= 0 && vertex < (int)start.size());

			if (NIL == start[vertex])
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		bool AreConnected(const int& from, const int& to)
		{
			assert(from >= 0 && from < (int)start.size());
			assert(to >= 0);

			if (!HasConnections(from))
			{
				return false;
			}

			for (int j = start[from]; NIL != j; j = next[j])
			{
				if (to == vertices[j])
				{
					return true;
				}
			}

			return false;
		}

		bool Equals(CommonGraph<T>& that)
		{
			if (size != that.size)
			{
				return false;
			}

			const int n = (int)start.size();
			for (int i = 0; i < n; i++)
			{
				int j = start[i];
				int k = that.start[i];

				while (NIL != j && NIL != k)
				{
					if (vertices[j] != that.vertices[k])
					{
						return false;
					}
					if (edges[j] != that.edges[k])
					{
						return false;
					}

					j = next[j];
					k = that.next[k];
				}

				if (NIL != j || NIL != k)
				{
					return false;
				}
			}

			return true;
		}

		void Insert(int from, int to, const T& edge)
		{
			assert(from >= 0 && from < (int)start.size());
			assert(to >= 0);

			int n;
			if (free >= 0)
			{
				n = free;
				vertices[free] = to;
				edges[free] = edge;
				free = next[free];
			}
			else
			{
				n = size;
				next.push_back(NIL);
				vertices.push_back(to);
				edges.push_back(edge);
			}

			if (NIL == start[from])
			{
				start[from] = NIL;
			}

			next[n] = start[from];
			start[from] = n;

			size += 1;
		}

		void InsertOrAssign(int from, int to, const T& edge)
		{
			assert(from >= 0 && from < (int)start.size());
			assert(to >= 0);

			if (!AreConnected(from, to))
			{
				Insert(from, to, edge);
			}
		}

		void RemoveFrom(int from, int to)
		{
			assert(from >= 0 && from < (int)start.size());
			assert(to >= 0);

			if (!HasConnections(from))
			{
				return;
			}

			int k = NIL;
			int p = NIL;
			int j = start[from];

			while (NIL != j)
			{
				if (to == vertices[j])
				{
					k = j;
					break;
				}
				p = j;
				j = next[j];
			}

			if (NIL == k)
			{
				return;
			}

			if (start[from] == k)
			{
				start[from] = next[k];
				next[k] = free;
				free = k;
			}
			else
			{
				next[p] = next[k];
				next[k] = free;
				free = k;
			}

			size -= 1;
		}

		void RemoveFrom(int vertex)
		{
			assert(vertex >= 0 && vertex < (int)start.size());

			if (!HasConnections(vertex))
			{
				return;
			}

			int n = 1;
			int p = start[vertex];

			while (NIL != next[p])
			{
				p = next[p];
				n += 1;
			}

			next[p] = free;
			free = start[vertex];
			start[vertex] = NIL;
			size -= n;
		}

		bool IsLeaf(int vertex)
		{
			assert(vertex >= 0 && vertex < (int)start.size());

			if (!HasConnections(vertex))
			{
				return true;
			}

			int firstNeighbor = vertex;

			for (int i = start[vertex]; NIL != i; i = next[i])
			{
				if (vertex != vertices[i])
				{
					firstNeighbor = vertices[i];
					break;
				}
			}

			if (vertex == firstNeighbor)
			{
				return true;
			}

			for (int i = start[vertex]; NIL != i; i = next[i])
			{
				if (firstNeighbor != vertices[i] && vertex != vertices[i])
				{
					return false;
				}
			}

			return true;
		}

		void Sort()
		{
			std::vector<int> sorted;
			std::vector<int> index;

			const int n = (int)start.size();
			for (int i = 0; i < n; i++)
			{
				int count = 0;
				sorted.resize(0);
				index.resize(0);
				int j = start[i];

				while (NIL != j)
				{
					sorted.push_back(vertices[j]);
					index.push_back(j);
					j = next[j];
					count += 1;
				}

				if (count < 2)
				{
					continue;
				}

				KeyValueSort(sorted, index, 0, count);

				for (j = 1; j < count; j++)
				{
					next[index[j - 1]] = index[j];

					start[i] = index[0];
					next[index[count - 1]] = NIL;
				}
			}
		}

		class Iterator
		{
		private:
			CommonGraph<T>& graph;
			int next;

		public:
			Iterator(CommonGraph<T>& graph) : graph(graph)
			{
				next = NIL;
			}

			void Setup(int start)
			{
				if (!graph.HasConnections(start))
				{
					next = NIL;
				}
				else
				{
					next = graph.start[start];
				}
			}

			bool HasNext() const noexcept
			{
				return NIL != next;
			}

			std::pair<int, T> Next()
			{
				int prev = next;
				next = graph.next[next];
				return std::move(std::pair<int, T>(graph.vertices[prev], graph.edges[prev]));
			}
		};

	private:
		template<class Key, class Value>
		static void KeyValueSort(std::vector<Key>& keys, std::vector<Value>& values, int begin, int end)
		{
			assert(keys.size() == values.size());
			assert(begin >= 0);
			assert(end >= begin && end <= (int)keys.size());

			std::vector<int> indices(end - begin);
			std::iota(std::begin(indices), std::end(indices), begin);

			std::vector<int> sorted = indices;
			std::sort(std::begin(sorted), std::end(sorted),
				[&](int a, int b) { return keys[a] < keys[b]; });

			reindex(values, indices, sorted);
			reindex(keys, indices, sorted);
		}

		template<class Y>
		static void reindex(std::vector<Y>& values,
			const std::vector<int>& oldIndices, const std::vector<int>& newIndices)
		{
			assert(oldIndices.size() == newIndices.size());
			assert(oldIndices.size() <= values.size());

			const int n = (int)oldIndices.size();
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