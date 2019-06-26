#pragma once

#include "CommonGraph.h"
#include "Placeholder.h"

#include <list>
#include <stack>
#include <unordered_set>
#include <vector>

namespace spandex::misc
{
	class DirectedGraph
	{
	private:
		CommonGraph<Placeholder> adj;

	public:
		DirectedGraph(int size, int capacity = 0) : adj(size, capacity)
		{
		}

		void Insert(const int& from, const int& to)
		{
			adj.Insert(from, to, Placeholder());
		}

		bool AreConnected(const int& a, const int& b)
		{
			std::stack<int> vertices({ a });
			std::unordered_set<int> visited({ a });
			auto it = adj.GetIterator();

			while (vertices.size() > 0)
			{
				auto start = vertices.top();
				vertices.pop();

				if (start == b)
				{
					return true;
				}

				it.Setup(start);
				while (it.HasNext())
				{
					auto Next = it.Next().first;
					if (visited.end() == visited.find(Next))
					{
						vertices.push(Next);
						visited.insert(Next);
					}
				}
			}

			return false;
		}

		auto FindAllPaths(const int& from, const int& to)
		{
			std::list<std::vector<int>> paths;

			std::stack<int> path;
			auto it = adj.GetIterator();

			std::stack<int> levels({ 0 });
			std::stack<int> vertices({ from });
			std::unordered_set<int> visited({ from });

			while (vertices.size() > 0)
			{
				auto start = vertices.top();
				vertices.pop();

				int level = levels.top();
				levels.pop();

				int n = (int)path.size() - level;
				for (int i = 0; i < n; i++)
				{
					visited.erase(path.top());
					path.pop();
				}

				path.push(start);
				visited.insert(start);

				if (start == to)
				{
					auto end = 1 + &path.top();
					auto begin = end - path.size();
					paths.push_back(std::vector<int>(begin, end));
					visited.erase(to);
				}
				else
				{
					it.Setup(start);

					while (it.HasNext())
					{
						auto Next = it.Next().first;

						if (visited.end() == visited.find(Next))
						{
							vertices.push(Next);
							levels.push(1 + level);
						}
					}
				}
			}

			return paths;
		}
	};
}