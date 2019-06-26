#pragma once

#include <cassert>
#include <vector>

namespace spandex::misc
{
	class IntList
	{
	public:
		const int NIL = -1;
		const int EMPTY = -2;
		int capacity;
		int size;

	private:
		std::vector<int> values;
		int ip;

	public:
		IntList(int capacity) : capacity(capacity), ip(NIL), size(0)
		{
			Resize(capacity);
		}

		void Resize(int capacity)
		{
			this->values.resize(capacity, EMPTY);
			this->capacity = capacity;
			this->size = 0;
			this->ip = NIL;

			int i = ip;
			int p = ip;
			while (NIL != i)
			{
				p = i;
				i = values[i];
				values[p] = EMPTY;
			}

			if (NIL != p)
			{
				values[p] = EMPTY;
			}

			ip = NIL;
			size = 0;
		}

		bool IsEmpty() const noexcept
		{
			return 0 == size;
		}

		int GetTop() const noexcept
		{
			return ip;
		}

		int Next(int key)
		{
			if (NIL == key)
			{
				return key;
			}
			else
			{
				return values[key];
			}
		}

		bool Contains(int key) const
		{
			assert(key >= 0 && key < capacity);

			return EMPTY != values[key];
		}

		void Push(int key)
		{
			assert(key >= 0 && key < capacity);

			if (Contains(key))
			{
				return;
			}

			values[key] = ip;
			ip = key;

			size += 1;
		}

		int Pop()
		{
			assert(size > 0);

			const int key = ip;
			ip = values[ip];
			values[key] = EMPTY;
			size -= 1;

			return key;
		}

		std::vector<int> PopAll()
		{
			const int n = size;
			std::vector<int> values;
			values.resize(n, 0);

			for (int i = 0; i < n; i++)
			{
				values[i] = Pop();
			}

			return values;
		}

		void Clear()
		{
			while (size > 0)
			{
				Pop();
			}
		}
	};
}