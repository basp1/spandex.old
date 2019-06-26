#pragma once

#include <cassert>
#include <vector>
#include <functional>
#include <cmath>

namespace spandex::misc
{
	template<class T>
	class PriorityQueue
	{
	private:
		std::vector<T> values;
		std::function<T(T, T)> selectFunc;

	public:
		int size;

		PriorityQueue(std::function<T(T, T)> selectFunc, int capacity = 0)
		{
			this->values.reserve(capacity);
			this->selectFunc = selectFunc;
			this->size = 0;
		}

		void Push(const T& value)
		{
			const int index = size;

			if ((int)values.size() <= index)
			{
				values.push_back(value);
			}
			else
			{
				values[index] = value;
			}

			size += 1;

			Promote(index);
		}

		T Pop()
		{
			assert(size > 0);

			T t = GetTop();

			if (1 == size)
			{
				size = 0;
			}
			else
			{
				T last = values[size - 1];
				values[0] = last;
				size -= 1;

				Demote(0);
			}

			return t;
		}

		T GetTop() const
		{
			assert(size > 0);

			return values[0];
		}

		int GetHeight() const
		{
			return 1 + (int)std::log2(size);
		}

	private:

		void Promote(int index)
		{
			assert(index >= 0 && index < size);

			if (0 == index)
			{
				return;
			}

			int parent = (int)(index / 2);

			while (index > 0)
			{
				T t = values[index];

				if (t != selectFunc(t, values[parent]))
				{
					break;
				}

				values[index] = values[parent];
				values[parent] = t;

				int Next = parent;
				parent = (int)(index / 2);
				index = Next;
			}
		}

		void Demote(int index)
		{
			assert(index >= 0 && index < size);

			if (size == (1 + index))
			{
				return;
			}

			T value = values[index];

			while (index < size)
			{
				const int right = (1 + index) * 2;
				T rv = T();
				if (right < size)
				{
					rv = this->values[right];
				}

				const int left = right - 1;
				T lv = T();
				if (left < size)
				{
					lv = this->values[left];
				}

				int child = -1;
				if (right < size && left < size && lv == selectFunc(lv, rv))
				{
					child = left;
				}
				else if (right < size)
				{
					child = right;
				}
				else if (left < size)
				{
					child = left;
				}

				if (child < 0 || value == selectFunc(value, values[child]))
				{
					break;
				}
				else
				{
					values[index] = values[child];
					values[child] = value;
					index = child;
				}
			}
		}
	};
}