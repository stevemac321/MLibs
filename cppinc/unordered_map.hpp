/*==============================================================================
 Name        : unordered_map.hpp
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once
#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>
#include <cstdio>
#include "allocator.hpp"
#include "unordered_map_iterator.hpp"

namespace stevemac
{
template <typename K> struct is_equal {
	bool operator()(const K &k1, const K &k2) const
	{
		static_assert(std::is_integral<K>::value, "integral required.");
		return k1 == k2;
	}
};

template <typename Key, typename T, Key _empty_key = Key(),
	  typename Hash = std::hash<Key>,
	  typename Pred = stevemac::is_equal<Key>,
	  typename Allocator = stevemac::allocator<std::pair<Key, T>>>
class unordered_map
{
      public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = std::pair<key_type, mapped_type>;
	using hasher = Hash;
	using key_equal = Pred;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = value_type *;
	using const_pointer = const value_type *;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using iterator = stevemac::unordered_map_iterator<
		unordered_map<Key, T, _empty_key, Hash, Pred, Allocator>>;
	using const_iterator = const iterator;
	using allocator_type = Allocator;
	friend class stevemac::unordered_map_iterator<
		unordered_map<Key, T, _empty_key, Pred, Hash, Allocator>>;


	unordered_map() = delete;
	unordered_map(const size_t n) : _max_size(n)
	{
		allocate();
	}
	~unordered_map()
	{
		deallocate();
	}

	unordered_map(const unordered_map &m) = delete;
	unordered_map &operator=(const unordered_map &m) = delete;

	std::pair<iterator, bool> insert(const value_type &obj)
	{
		assert(_size <= _max_size);
		size_t idx = 0;

		for (size_t i = 0; i < _max_size; i++) {
			idx = (_hf(obj.first) + (i * hash2(obj.first)))
			      % _max_size;

			if (_is_equal(_table[idx].first, _empty_key)) {
				_allocator.construct(&_table[idx], obj);
				_size++;
				iterator it(&_table[idx]);
				return make_pair(it, true);
			}
		}
		return std::make_pair(end(), false);
	}

	iterator find(const Key &key) const
	{
		int idx = find_bucket(key);
                Key k = _table[idx].first;
		if (_is_equal(k, _empty_key) || !_is_equal(k, key))
			return end();

		iterator it(&_table[idx]);
		return it;
	}

	iterator begin()
	{
		iterator it(_table);
		return it;
	}

	const_iterator begin() const noexcept
	{
		const_iterator it(_table);
		return it;
	}
	const_iterator cbegin() const noexcept
	{
		const_iterator it(_table);
		return it;
	}
	iterator end()
	{
		iterator it(_end);
		return it;
	}
	const_iterator end() const noexcept
	{
		const_iterator it(_end);
		return it;
	}
	const_iterator cend() const noexcept
	{
		const_iterator it(_end);
		return it;
	}

	void stats()
	{
		printf("\nsize %zu: Max_size: %zu\n", _size, _max_size);
	}

      private:
	void allocate()
	{
		_table = _allocator.allocate(_max_size + 1);
		assert(_table);
		_end = _table + _max_size;
		for (int i = 0; i < _max_size; i++)
			_allocator.construct(&_table[i],
					     std::make_pair(_empty_key, T()));
	}
	void deallocate()
	{
		if (!std::is_trivially_constructible<
			    std::pair<const Key, T>>::value)
			for (size_t i = 0; i < _max_size; i++)
				_allocator.destroy(&_table[i]);

		_allocator.deallocate(_table);
		_table = nullptr;
		_end = nullptr;
		_max_size = 0;
		_size = 0;
	}
	size_t find_bucket(const Key &key) const
	{
		size_t idx = 0;
		for (size_t i = 0; i < _max_size; i++) {
			idx = (_hf(key) + (i * hash2(key))) % _max_size;

			if (_is_equal(_table[idx].first, key))
				return idx;
		}
		// caller needs to check if the key at this pos is _empty_key
		return idx;
	}
	size_t hash2(const size_t &primary_hash) const
	{
		size_t h2 = (primary_hash * _max_size) % _max_size;
		return h2 == 0 ? 1 : h2; // can't return 0 for step
	}

      private:
	hasher _hf = hasher();
	key_equal _is_equal = key_equal();
	allocator_type _allocator = allocator_type();

	pointer _table = nullptr;
	pointer _end = nullptr;

	size_type _resize_factor = 2;
	float _density = 1.00;   // how full until resize
	size_type _size = 0;     // how many populated entries
	size_type _max_size = 0; // the actual size of the table
};

} // end of namespace stevemac

