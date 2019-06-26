#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>

namespace spandex::misc
{
	template <class Key, class T, class Comparator = std::less<Key>>
	class FlatMap
	{
	public:
		typedef std::pair<Key, T> Item;
		typedef typename std::vector<Item> Container;
		typedef typename std::vector<std::pair<Key, T>>::iterator Iterator;
		typedef typename std::vector<std::pair<Key, T>>::const_iterator ConstIterator;

	private:
		struct ItemComparator;

		ItemComparator comparator;
		Container items;

	public:
		FlatMap() noexcept
		{
		}

		explicit FlatMap(const Comparator& comparator) : comparator(comparator)
		{
		}

		FlatMap(const FlatMap& that) = default;
		FlatMap(FlatMap&& that) = default;

		FlatMap& operator=(const FlatMap& that)
		{
			comparator = that.comparator;
			items = that.items;
			return *this;
		}

		FlatMap& operator=(FlatMap&& that)
		{
			comparator = std::move(that.comparator);
			items = std::move(that.items);
			return *this;
		}

		Iterator begin() noexcept
		{
			return items.begin();
		}

		ConstIterator begin() const noexcept
		{
			return items.begin();
		}

		Iterator end() noexcept
		{
			return items.end();
		}

		ConstIterator end() const noexcept
		{
			return items.end();
		}

		ConstIterator cbegin() const noexcept
		{
			return items.cbegin();
		}

		ConstIterator cend() const noexcept
		{
			return items.cend();
		}

		bool IsEmpty() const noexcept
		{
			return items.empty();
		}

		int GetSize() const noexcept
		{
			return (int)items.size();
		}

		void Reserve(int count)
		{
			return items.reserve(count);
		}

		int GetCapacity() const noexcept
		{
			return (int)items.capacity();
		}

		void Clear() noexcept
		{
			items.clear();
		}

		bool Contains(const Key& key) const
		{
			return end() != Find(key);
		}

		Iterator Find(const Key& key)
		{
			auto it = LowerBound(key);

			if (it != end() && !comparator(key, *it))
			{
				return it;
			}

			return end();
		}

		ConstIterator Find(const Key& key) const
		{
			auto it = LowerBound(key);

			if (it != end() && !comparator(key, *it))
			{
				return it;
			}

			return end();
		}

		void Insert(const Item& item)
		{
			auto it = LowerBound(item.first);

			if (it == end() || comparator(item.first, *it))
			{
				items.emplace(it, item);
			}
		}

		template <class... Args>
		void emplace(Args&& ... args)
		{
			Item item(args...);
			Insert(std::move(item));
		}

		void Remove(Iterator it)
		{
			items.erase(it);
		}

		void Remove(const Key& key)
		{
			auto it = Find(key);

			if (end() != it)
			{
				Remove(it);
			}
		}

		T& operator[](const Key& key)
		{
			auto it = LowerBound(key);

			if (it != end() && !comparator(key, *it))
			{
				return it->second;
			}

			it = items.emplace(it, key, T());

			return it->second;
		}

		T& operator[](Key&& key)
		{
			auto it = LowerBound(key);

			if (it != end() && !comparator(key, *it))
			{
				return it->second;
			}

			it = items.emplace(it, std::forward<Key>(key), T());

			return it->second;
		}

		T& Get(const Key& key)
		{
			auto it = LowerBound(key);

			if (it == end() || comparator(*it, key))
			{
				throw std::out_of_range("misc::FlatMap out of range");
			}

			return it->second;
		}

		const T& Get(const Key& k) const
		{
			auto it = LowerBound(k);

			if (it == end() || comparator(*it, k))
			{
				throw std::out_of_range("misc::FlatMap out of range");
			}

			return it->second;
		}

		const Container& container() const noexcept
		{
			return items;
		}

	private:
		Iterator LowerBound(const Key& k)
		{
			return std::lower_bound(items.begin(), items.end(), k, comparator);
		}

		ConstIterator LowerBound(const Key& k) const
		{
			return std::lower_bound(items.begin(), items.end(), k, comparator);
		}

		struct ItemComparator
		{
			Comparator comparator;

			ItemComparator() = default;

			ItemComparator(const Comparator& comparator) : comparator(comparator)
			{
			}

			bool operator()(const Item& a, const Key& b) const
			{
				return comparator(a.first, b);
			}

			bool operator()(const Key& a, const Item& b) const
			{
				return comparator(a, b.first);
			}
		};
	};

	template <typename Key, typename T, typename Comparator>
	bool operator==(const FlatMap<Key, T, Comparator>& a, const FlatMap<Key, T, Comparator>& b)
	{
		return a.container() == b.container();
	}

	template <typename Key, typename T, typename Comparator>
	bool operator!=(const FlatMap<Key, T, Comparator>& a, const FlatMap<Key, T, Comparator>& b)
	{
		return a.container() != b.container();
	}

}
