#pragma once

namespace spandex::misc
{
	struct Range
	{
		struct Iterator;

		int from;
		int to;

		Range(int count) : Range(0, count)
		{
		}

		Range(int from, int to) : from(from), to(to)
		{
		}

		Iterator begin()
		{
			return Iterator(from, to);
		}

		Iterator end()
		{
			return Iterator(to, to);
		}

		bool operator==(const Range& that) const
		{
			return from == that.from && to == that.to;
		}

		struct Iterator
		{
			int from;
			int to;

			Iterator(int from, int to) : from(from), to(to)
			{
			}

			const int& operator*() const
			{
				return from;
			}

			Iterator& operator++()
			{
				from++;
				return *this;
			}

			bool operator==(const Iterator& that) const
			{
				return from == that.from && to == that.to;
			}

			bool operator!=(const Iterator& that) const
			{
				return !operator==(that);
			}
		};
	};
}