//===-- open_map.h - open_map.h implementation---*- C++
//-*-===//
//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file open_map.h
/// \brief This is a non standard implementation of a random_access_iterator to
/// work with the stevemac::open_map class.
///
//===----------------------------------------------------------------------===//

#pragma once
#include "allocator.hpp"
#include "open_map_iterator.hpp"

namespace stevemac
{
template <typename T> struct is_equal {
	bool operator()(const T &v1, const T &v2) const { return v1 == v2; }
};
template <> struct is_equal<const char *> {
	bool operator()(const char *s1, const char *s2) const
	{
		if (s1 == nullptr && s2 == nullptr)
			return true;
		if (s1 == nullptr && s2 != nullptr)
			return false;
		if (s1 != nullptr && s2 == nullptr)
			return false;

		return (strcmp(s1, s2) == 0);
	}
};
template <typename Key, typename T, Key _empty_key = Key(),
	  typename Pred = is_equal<Key>, typename Hash = std::hash<Key>,
	  typename Allocator = stevemac::allocator<std::pair<Key, T>>>
class open_map
{
      public:
	// aliases
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
	using iterator = stevemac::open_map_iterator<
	    open_map<Key, T, _empty_key, Pred, Hash>>;
	using const_iterator = const iterator;
	using allocator_type = Allocator;
	friend class stevemac::open_map_iterator<
	    open_map<Key, T, _empty_key, Pred, Hash>>;

	// ctor, dtor, assignment
	open_map() = delete;
	open_map(size_type n) : _max_size(n) { init(); }
	// template <class InputIcterator>
	open_map(iterator f, iterator l, size_type n) : _max_size(n)
	{
		init();
		insert(f, l);
	}
	open_map(std::initializer_list<value_type> il, size_type n)
	    : _max_size(n)
	{
		init();
		for (auto p : il)
			if (!_is_equal(p.first, _empty_key))
				insert(p);
	}
	~open_map() { _allocator.deallocate(_table); }
	open_map(open_map &m) = delete;
	open_map &operator=(const open_map &m) = delete;

	void insert(iterator f, iterator l)
	{
		for (auto i = f; i != l; i++) {
			if (!_is_equal(i->first, _empty_key))
				insert(*i);
		}
	}
	// size, capacity, etc.
	bool empty() { return (_size == 0); }
	size_type size() const noexcept { return _size; }
	size_type max_size() const noexcept { return _max_size; }
	void set_resize_factor(const size_type &sz) { _resize_factor = sz; }
	void set_load_factor(const float &d) { _density = d; }
	key_type get_empty_key() { return _empty_key; }
	// element access
	mapped_type &operator[](const key_type &k)
	{
		auto i = find(k);
		if (_is_equal(i->first, _empty_key)) {
			auto p = insert(make_pair(k, mapped_type()));
			return p.first->second;
		} else
			return i->second;
	}
	mapped_type &at(const key_type &k)
	{
		auto p = find(k);
		// todo spec says this throws
		return p->second;
	}
	// iterators
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
	// element lookup
	iterator find(const key_type &k) { return find(k, _table); }
	// modifiers
	void clear() noexcept
	{ // call element dtors
		for (size_t i = 0; i < _max_size; i++)
			_allocator.destroy(&_table[i]);

		_size = 0;
	}
	std::pair<iterator, bool> insert(const value_type &val)
	{
		assert(!_is_equal(val.first, _empty_key));

		if (_size >= ((_max_size)*_density))
			resize();

		return insert(val, _table);
	}
	void stats()
	{
		printf("size %zu: max_size: %zu\n", _size, _max_size);
	}
	void resize()
	{
		stats();
		size_t old_max = _max_size;
		_max_size *= _resize_factor;
		assert(_max_size <= _allocator.max_size());

		pointer tmp = _allocator.allocate(_max_size);
		assert(tmp);

		for (int i = 0; i < _max_size; i++)
			tmp[i].first = _empty_key;

		_size = 0;

		for (int i = 0; i < old_max; i++)
			if (!_is_equal(_table[i].first, _empty_key))
				insert(_table[i], tmp);

		_allocator.deallocate(_table);
		_table = tmp;
		_end = _table + _max_size;
		stats();
	}

      private:
	iterator find(const key_type &k, pointer tab)
	{
		size_t h = _hf(k);
		size_t idx =
		    h % (_max_size - 1); // leave last slot for end iterator

		size_type step = hash2(h);

		while (!_is_equal(tab[idx].first, k) &&
		       !_is_equal(tab[idx].first, _empty_key)) {
			idx += step;
			idx = idx % (_max_size);
		}
		iterator it(&tab[idx]);
		return it;
	}

	std::pair<iterator, bool> insert(const value_type &val, pointer tab)
	{
		auto p = find(val.first, tab);
		bool found = true;

		if (!_is_equal(p->first, _empty_key))
			found = false;
		else {
			_allocator.construct(p.get_ptr(), val);
			_size++;
		}
		return make_pair(p, found);
	}


	size_type hash2(const size_type &primary_hash)
	{
		size_type h2 = (primary_hash * _max_size) % _max_size;
		return h2 == 0 ? 1 : h2; // can't return 0 for step
	}
	void init()
	{
		_table = _allocator.allocate(_max_size + 1);
		assert(_table);
		_end = _table + _max_size;
		for (int i = 0; i < _max_size; i++)
			_table[i].first = _empty_key;
	}
	size_type _max_size = 0; // the actual size of the table
	allocator_type _allocator = allocator_type();
	hasher _hf = hasher();
	size_type _resize_factor = 2;
	float _density = 1.00; // how full until resize
	pointer _table = nullptr;
	size_type _size = 0; // how many populated entries
	pointer _end = nullptr;
	key_equal _is_equal = key_equal();
};
} // end namespace
