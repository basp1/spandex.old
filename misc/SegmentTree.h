#pragma once

#include <cassert>
#include <vector>
#include <functional>
#include <cmath>

namespace spandex::misc
{
	template<class T>
	class SegmentTree
	{
	private:
		int capacity;
		T limit;
		std::function<T(T, T)> selectFunc;
		std::vector<T> values;

	public:
		SegmentTree(const T limit, std::function<T(T, T)> selectFunc, const std::vector<T>& values) :
			SegmentTree(limit, selectFunc, (int)values.size())
		{
			for (int i = 0; i < (int)values.size(); i++)
			{
				this->values[capacity - 1 + i] = values[i];
			}

			for (int i = capacity - 2; i >= 0; i--)
			{
				this->values[i] = selectFunc(this->values[1 + (i << 1)], this->values[2 + (i << 1)]);
			}
		}

		SegmentTree(const T limit, std::function<T(T, T)> selectFunc, int n)
		{
			this->limit = limit;
			this->selectFunc = selectFunc;
			this->capacity = 1 << (int)std::ceil(std::log2(n));
			this->values.resize(capacity << 1);

			std::fill(std::begin(values), std::end(values), limit);
		}

		bool Equals(const SegmentTree<T> & that) const
		{
			if (capacity != that.capacity)
			{
				return false;
			}

			if (values != that.values)
			{
				return false;
			}

			return true;
		}

		void Set(int index, T newValue)
		{
			assert(index < capacity);

			index += (capacity - 1);
			values[index] = newValue;

			while (index > 0)
			{
				T value = values[index];
				int neighbor;
				if (1 == index % 2)
				{
					neighbor = index + 1;
					index = index >> 1;
				}
				else
				{
					neighbor = index - 1;
					index = (index >> 1) - 1;
				}

				newValue = selectFunc(value, values[neighbor]);
				if (values[index] != newValue)
				{
					values[index] = newValue;
				}
				else
				{
					break;
				}
			}
		}

		T GetTop() const
		{
			return values[0];
		}

		T Get(int index) const
		{
			return Range(index, index);
		}

		T Range(int left, int right) const
		{
			assert(left <= right);
			assert(left < capacity);
			assert(right < capacity);

			left += (capacity - 1);
			right += (capacity - 1);
			T leftValue = limit;
			T rightValue = limit;

			while (left < right)
			{
				if (0 == left % 2)
				{
					leftValue = selectFunc(leftValue, values[left]);
				}
				left = left >> 1;

				if (1 == right % 2)
				{
					rightValue = selectFunc(values[right], rightValue);
				}
				right = (right >> 1) - 1;
			}

			if (left == right)
			{
				leftValue = selectFunc(leftValue, values[left]);
			}

			return selectFunc(leftValue, rightValue);
		}
	};
}