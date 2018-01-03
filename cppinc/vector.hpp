//===-- stevemac::vector.hpp --------------------------------------*- C -*-===//
//
// This file is distributed under the GNU General Public License, version 2
// (GPLv2). See https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
// Author: Stephen E. MacKenzie
//===----------------------------------------------------------------------===//
#pragma once
#include "iterator.hpp"
#include "allocator.hpp"

using std::swap;

namespace stevemac
{
///===----------------------------------------------------------------------===//
///
/// stevemac::vector
/// Implementation started in fall of 2014. References to the C++ standard
/// refer to N3797.  C++14 support is required.
/// stevemac::vector uses a custom iterator: stevemac::vector_iterator
///
/// § 23.3.6.1 Class Template stevemac::vector
///
//===----------------------------------------------------------------------===//
template <typename T, class Allocator = stevemac::allocator<T>> class vector
{
      public:
	using value_type = T;
	using allocator_type = Allocator;
	using reference = value_type &;
	using const_reference = const value_type &;
	using iterator = vector_iterator<vector<T, Allocator>>;
	using const_iterator = const iterator;
	using pointer = typename std::allocator_traits<allocator_type>::pointer;
	using const_pointer =
	    typename std::allocator_traits<allocator_type>::const_pointer;
	using size_type = typename allocator_type::size_type;
	using difference_type = typename allocator_type::difference_type;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	friend class vector_iterator<vector<T, Allocator>>;

	//===----------------------------------------------------------------------===//
	/// § 23.3.6.2 construct/copy/destroy:
	///
	//===----------------------------------------------------------------------===//
	/// Default constructed vector: zero size, zero capacity, invalid
	/// iterators.
	explicit vector(const allocator_type &a = allocator_type())
	    : _size(0), _capacity(0), _allocator(a)
	{
		_begin = _end = nullptr;
	}

	/// Effects: Constructs a vector with default-inserted elements.
	/// Requires: T shall be DefaultInsertible into *this.
	explicit vector(size_type n, const allocator_type &a = allocator_type())
	    : _size(n), _capacity(n), _allocator(a)
	{

		_size_n_alloc = n;
		_begin = reallocate(n);
		_end = _begin;

		for (size_type i = 0; i < _size; i++)
			_allocator.construct(_end++, T());
	}

	/// Effects: Constructs a vector with n copies of value.
	/// Requires: T shall be CopyInsertable into *this.
	vector(size_type n, const_reference value,
	       const allocator_type &a = allocator_type())
	    : _size(n), _capacity(n), _allocator(a)
	{
		assign(n, value);
	}

/// Effects: Constructs a vector equal to the range [first,last].
/// Complexity is conditional, pivots on Iter type, see 23.3.6.2.10
/// std::_RequireInputIterator takes is an enable_if shorthand that
/// I should get rid of because it takes a dependency on libstdc++
#if __cplusplus >= 201103L
	template <typename InputIterator,
		  typename = std::_RequireInputIter<InputIterator>>
	vector(InputIterator first, InputIterator last,
	       const allocator_type &a = allocator_type())
	    : _size(last - first), _capacity(_size), _allocator(a)
	{

		assign(first, last);
	}
#else
	template <typename InputIterator>
	vector(InputIterator first, InputIterator last,
	       const allocator_type &a = allocator_type())
	    : _size(last - first), _capacity(_size), _allocator(a)
	{

		assign(first, last);
	}
#endif
	/// see above ctor
	vector(iterator first, iterator last,
	       const allocator_type &a = allocator_type())
	    : _size(last - first), _capacity(_size), _allocator(a)
	{

		assign(first, last);
	}
	// copy ctors
	vector(const vector &other)
	{
		if (this != &other)
			*this = other;
	}

	vector(vector &&other)
	{
		if (this != &other)
			*this = std::move(other);
	}

	vector(const vector &other, const allocator_type &a) : _allocator(a)
	{
		if (this != &other)
			*this = other;
	}

	vector(vector &&other, const allocator_type &a) : _allocator(a)
	{
		if (this != &other)
			*this = std::move(other);
	}

	vector(std::initializer_list<T> il,
	       const allocator_type &a = allocator_type())
	    : _size(il.size()), _capacity(_size), _allocator(a)
	{
		assign(il);
	}
	/// destructor
	/// We do not want to unconditionally destroy/deallocate here in case
	/// "this"
	/// is in a "moved from" state; that is; in case "this" has been
	/// cannibalized
	/// _size = il.size();
	/// by move semantics.  The "moved to" instance sets _begin to nullptr
	/// to
	/// signal this state.  According to Hinnant, this is a reasonable
	/// implementation (cppcon 2014, talk "Ask the Authors")
	~vector()
	{
		if (_begin != nullptr) {
			range_destroy(_begin, _end);
			_allocator.deallocate(_begin, _capacity);
		}
	}

	vector &operator=(const vector &other)
	{
		if (this == &other)
			return *this;

		/// If const vector& other is empty, just reset to default
		/// constructed
		/// per 23.3.6.2.1 and 2
		if (other.capacity() == 0 && _capacity > 0) {
			_allocator.deallocate(_begin, _capacity);

			_size = _capacity = 0;
			_begin = _end = nullptr;
			return *this;
		}

		const size_type delcap = _capacity;
		const size_type delsize = _size;
		_capacity = other.capacity();
		_size = other.size();
		alloc_copy_swap(_capacity, delcap, delsize, other._begin);
		return *this;
	}

	/// \todo REVIEW this and swap
	vector &operator=(vector &&other)
	{
		_size = other._size;

		std::swap(_begin, other._begin);
		std::swap(_end, other._end);
		std::swap(_capacity, other._capacity);

		return *this;
	}
	///
	vector &operator=(const std::initializer_list<T> &il)
	{
		const size_type delcap = _capacity;
		_size = _capacity = il.size();

		pointer tmp = reallocate(_capacity);

		for (size_type i = 0; i < _size; i++)
			_allocator.construct(&tmp[i], *(il.begin() + i));

		std::swap(_begin, tmp);
		_allocator.deallocate(tmp, delcap);
		_end = _begin + _size;
		return *this;
	}
/// you have to check the state of _begin and _capacity
/// because assign can be called after _begin ha been allocated
/// @TODO the assign overloads have a lot of common code, come
/// up with a helper maybe.
#if __cplusplus >= 201103L
	template <typename InputIterator,
		  typename = std::_RequireInputIter<InputIterator>>
	void assign(InputIterator first, InputIterator last)
	{

		difference_type dist = std::distance(first, last);

		if (_begin == nullptr || dist > _capacity)
			_begin = reallocate(_capacity);

		_end = _begin;

		while (first != last)
			_allocator.construct(_end++, *(first++));
	}
#else
	template <typename InputIterator>
	void assign(InputIterator first, InputIterator last)
	{

		difference_type dist = std::distance(first, last);

		if (_begin == nullptr || dist > _capacity)
			_begin = reallocate(_capacity);

		_end = _begin;

		while (first != last)
      _allocator.construct(_end++, *(first++);
	}
#endif

	void assign(iterator first, iterator last)
	{

		difference_type dist = std::distance(first, last);

		if (_begin == nullptr || dist > _capacity)
			_begin = reallocate(_capacity);

		_end = _begin;

		while (first != last)
			_allocator.construct(_end++, *(first++));
	}

	void assign(size_type n, const T &u)
	{

		if (_begin == nullptr || n > _capacity)
			_begin = reallocate(n);

		_end = _begin;
		_size = n;

		for (size_type i = 0; i < n; i++)
			_allocator.construct(_end++, u);
	}

	void assign(const std::initializer_list<T> &il)
	{

		_size = il.size();
		if (_begin == nullptr || _size > _capacity)
			_begin = reallocate(_capacity);

		_end = _begin;

		for (auto &i : il)
			_allocator.construct(_end++, i);
	}

	allocator_type get_allocator() const noexcept { return _allocator; }
	//===----------------------------------------------------------------------===//
	/// Iterators.
	///
	//===----------------------------------------------------------------------===//
	iterator begin() noexcept { return iterator(_begin); }
	const_iterator begin() const noexcept { return const_iterator(_begin); }

	iterator end() noexcept { return iterator(_end); }
	const_iterator end() const noexcept { return const_iterator(_end); }

	reverse_iterator rbegin() noexcept { return reverse_iterator(_begin); }
	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(_begin);
	}

	reverse_iterator rend() noexcept { return reverse_iterator(_end); }
	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(_end);
	}

	const_iterator cbegin() const noexcept
	{
		return const_iterator(_begin);
	}
	const_iterator cend() const noexcept { return const_iterator(_end); }

	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(_begin);
	}
	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(_end);
	}

	//===----------------------------------------------------------------------===//
	/// 23.3.6.3, capacity:
	//===----------------------------------------------------------------------===//
	size_type size() const noexcept { return _size; }
	size_type max_size() const noexcept { return _max; }

	size_type capacity() const noexcept { return _capacity; }
	bool empty() const noexcept { return _size == 0; }

	/// see 23.3.6.3.2
	/// can throw length_error if n > max_size().
	void reserve(size_type n)
	{
		if (n > max_size())
			return;
			//throw std::length_error("request larger than max");

		if (n > _capacity) // otherwise do nothing, reserve nor for
				   // reducing.
			resize_helper(n, false, T());
	}

	/// T shall be MoveInsertable into this
	/// Non-binding request to reduce capacity to size
	/// \todo REVIEW what is non-binding? Optional?
	void shrink_to_fit()
	{
		if (_size < _capacity)
			alloc_move_swap(_size, _capacity, _size, _begin);

		_capacity = _size;
	}

	/// Effects: If sz <= size(), equivalent to calling pop_back()
	/// size() - sz times default-inserted elements into the sequence.
	/// Requires: T shall be MoveInsertable and DefaultInsertable into
	/// *this.
	/// Remarks: REVIEW If an exception is thrown other than move ctor of
	/// a non-CopyInsertable T there are no effects. (basic?).
	void resize(size_type sz) { resize_helper(sz, true, T()); }

	void resize(size_type sz, const T &c) { resize_helper(sz, true, c); }

	//===----------------------------------------------------------------------===//
	/// element access
	///
	//===----------------------------------------------------------------------===//
	reference operator[](size_type n) { return _begin[n]; }
	const_reference operator[](size_type n) const { return _begin[n]; }

	const_reference at(size_type n) const { return _begin[n]; }
	reference at(size_type n) { return _begin[n]; }

	reference front()
	{
		assert(_size > 0);
		return _begin[0];
	}
	const_reference front() const
	{
		assert(_size > 0);
		return _begin[0];
	}

	reference back()
	{
		assert(_size > 0);
		return _begin[_size - 1];
	}
	const_reference back() const
	{
		assert(_size > 0);
		return _begin[_size - 1];
	}
	//===----------------------------------------------------------------------===//
	/// 23.3.6.4, data access
	///
	//===----------------------------------------------------------------------===//
	T *data() noexcept { return _begin; }
	const T *data() const noexcept { return _begin; }
	//===----------------------------------------------------------------------===//
	/// 23.3.6.5 modifiers
	/// Remarks: Causes reallocation if the new size is greater than the old
	/// capacity.  If no reallocation happens, all the iterators and
	/// references
	/// before the insertion point remain valid. (before the insertion
	/// point?)
	/// If an exception is thrown other than by the copy ctor, move ctor,
	/// assign
	/// op,or move assign op of T or by any InIt operation there are no
	/// effects.
	//===----------------------------------------------------------------------===//
	/// forward preserves the const/volitile as well as the rvalue\lvalue
	/// properties of the parameter pack items.  Note that const/volitile
	/// property has a higher type dedeuction precendence.
	template <class... Args>
	iterator emplace(const_iterator position, Args &&... args)
	{
		return insert(position,
			      value_type(std::forward<Args>(args)...));
	}

	iterator insert(const_iterator position, const value_type &val)
	{
		difference_type offset = std::distance(begin(), position);

		size_type n = 1;
		_size++;

		if (_size >= _capacity)
			insert_resize(set_new_capacity(_size + n, true), offset,
				      n, val);
		else
			insert_inplace(offset, 1, val);

		return position;
	}
	/// 23.3.6.5
	iterator insert(const_iterator position, value_type &&val)
	{

		difference_type offset = std::distance(begin(), position);
		size_type n = 1;
		_size++;

		if (_size >= _capacity)
			insert_resize(set_new_capacity(n), offset, n,
				      std::move(val));
		else
			insert_inplace(offset, n, std::move(val));

		return position;
	}
	/// The pattern for insert is to either insert in place or resize.
	///
	iterator insert(const_iterator position, size_type n,
			const value_type &val)
	{

		difference_type offset = std::distance(begin(), position);
		_size += n;

		if (_size >= _capacity)
			insert_resize(set_new_capacity(_size + n, true), offset,
				      n, val);
		else
			insert_inplace(offset, n, val);

		return position;
	}

#if __cplusplus >= 201103L
	template <typename InputIterator,
		  typename = std::_RequireInputIter<InputIterator>>
	iterator insert(const_iterator position, InputIterator first,
			InputIterator last)
	{

		difference_type offset = std::distance(begin(), position);

		_size += std::distance(first, last);

		if (_size >= _capacity)
			insert_resize(set_new_capacity(offset, true), offset,
				      first, last);
		else
			insert_inplace(offset, first, last);

		return position;
	}
#else
	template <typename InputIterator>
	iterator insert(const_iterator position, InputIterator first,
			InputIterator last)
	{

		difference_type offset = std::distance(begin(), position);

		_size += std::distance(first, last);

		if (_size >= _capacity)
			insert_resize(set_new_capacity(offset, true), offset,
				      first, last);
		else
			insert_inplace(offset, first, last);

		return position;
	}
#endif

	iterator insert(const_iterator position,
			const std::initializer_list<value_type> il)
	{

		difference_type offset = std::distance(begin(), position);

		_size += il.size();

		if (_size >= _capacity)
			insert_resize(set_new_capacity(offset, true), offset,
				      il);
		else
			insert_inplace(offset, il);

		return position;
		;
	}
	/// Exception is thrown by the move ctor of a non-CopyInsertable T, the
	/// effects are unspecified.  REVIEW strong guarantee.
	/// forward preserves the const/volitile as well as the rvalue\lvalue
	/// properties of the parameter pack items.  Note that const/volitile
	/// property has a higher type dedeuction precendence.
	template <class... Args> void emplace_back(Args &&... args)
	{
		push_back(value_type(std::forward<Args>(args)...));
	}

	/// Strong exception guarantee  REVIEW
	/// see push_back_resize also.
	/// 23.3.6.5.1
	void push_back(const T &val)
	{
		if (_capacity <= 0)
			push_back_initial_alloc();

		if (_size >= _capacity) {
			const size_type oldcap = _capacity;
			const size_type oldsize = _size;
			set_new_capacity(1, true);
			alloc_move_swap(_capacity, oldcap, oldsize, _begin);
		}

		_allocator.construct(_end++, val);
		_size++;
	}
	/// Qualfied strong guarantee, if move ctor of a non-CopyInsertable T,
	/// affects are unspecified.  REVIEW
	void push_back(T &&val)
	{
		if (_capacity <= 0)
			push_back_initial_alloc();

		if (_size >= _capacity) {
			const size_type oldcap = _capacity;
			const size_type oldsize = _size;
			set_new_capacity(1, true);
			alloc_move_swap(_capacity, oldcap, oldsize, _begin);
		}

		_allocator.construct(_end++, std::move(val));
		_size++;
	}

	void pop_back()
	{
		pointer del = _end;
		_size--;
		_end--;
		_allocator.destroy(del);
	}
	/// erase
	/// see 23.3.6.5.3,4,5
	iterator erase(const_iterator position)
	{
		iterator p = position; // non-const iterator for +1 in move
		_allocator.destroy(
		    &(*p)); // * op converts iterator to address of T&
		std::move(p + 1, end(), p);
		_end--;
		_size--;
		return position;
	}

	/// Range erase, delete first inclusive up to but excluding last
	/// Refer to 23.3.6.5.3
	iterator erase(const_iterator first, const_iterator last)
	{

		std::move(last, end(), first);
		const size_type offset = const_cast<iterator &>(last) -
					 const_cast<iterator &>(first);

		range_destroy(_begin + _size - offset, _begin + _size - 1);

		_end -= offset;
		_size -= offset;
		return last;
	}

	/// capacity remains unchanged
	void clear() noexcept
	{

		_size = 0;
		_size_n_alloc = 0;
		_end = _begin;
	}
	//===----------------------------------------------------------------------===//
	/// 23.3.6.6 specialized algorithms
	///
	//===----------------------------------------------------------------------===//
	/// \todo REVIEW
	///
	void swap(vector &other)
	{
		if (this != &other) {
			std::swap(_size, other._size);
			std::swap(_begin, other._begin);
			std::swap(_end, other._end);
			std::swap(_capacity, other._capacity);
		}
	}
	//===----------------------------------------------------------------------===//
	/// vector private implementation
	///
	//===----------------------------------------------------------------------===//
      private:
	void range_destroy(pointer first, pointer last)
	{
		pointer p = first;
		while (p != nullptr && p != last)
			_allocator.destroy(p++);
	}
	/// This is used when size reaches capacity.
	/// need to check a couple boundary cases
	size_type set_new_capacity(const size_type n, bool refactor = true)
	{
		if (n >= _max)
			return _max;

		size_type newsize = 0;
		if (refactor)
			newsize = (_size * _resize_factor) >= n
				      ? _size * _resize_factor
				      : n * _resize_factor;
		else
			newsize = n;

		_capacity = newsize < _max ? newsize : _max;
		return _capacity;
	}

	/// This function does the heavy lifting in copy, assign, resize, and
	/// shrink
	/// push_back methods.
	void alloc_copy_swap(const size_type newcap, const size_type oldcap,
			     const size_type oldsize, const pointer &srcBuf)
	{
		pointer tmp = reallocate(newcap);

		for (size_type i = 0; i < _size; i++)
			_allocator.construct(&tmp[i], srcBuf[i]);

		std::swap(_begin, tmp);
		_allocator.deallocate(tmp, oldcap);
		_end = _begin + _size;
	}
	void alloc_move_swap(const size_type newcap, const size_type oldcap,
			     const size_type oldsize, const pointer &srcBuf)
	{
		pointer tmp = reallocate(newcap);

		for (size_type i = 0; i < _size; i++)
			_allocator.construct(&tmp[i], std::move(srcBuf[i]));

		std::swap(_begin, tmp);
		_allocator.deallocate(tmp, oldcap);
		_end = _begin + _size;
	}

	/// The two stage construct here prevents us from using alloc_move_swap.
	///
	void resize_alloc(const size_type n, const size_type numval,
			  const T &val, bool refactor = true)
	{
		size_type oldcap = _capacity;
		set_new_capacity(n, refactor);
		pointer tmp = reallocate(_capacity);

		// if this is getting called, we should always have existing
		// elements
		for (size_type i = 0; i < _size; ++i)
			_allocator.construct(&tmp[i], std::move(_begin[i]));

		for (size_type i = _size; i < _size + numval; i++)
			_allocator.construct(&tmp[i], val);

		// ok, if we get here we did not throw :-)
		std::swap(_begin, tmp);
		_allocator.deallocate(tmp, oldcap);
	}

	/// Insert support
	/// four overloads of insert resize, all these do very close to the
	/// same thing, with slight variations.  If there is not enough room
	/// to insert into the current capacity, we have to allocate and restore
	/// the existing elements up to the insertion point into the new buffer,
	/// then insert the new elements, then append the remaining pre-existing
	/// elements that get pushed end-ward by the insert operator.  Then we
	/// swap in the new buffer and nuke the old.
	/// First overload, other overloads follow basic pattern with slight
	/// variations.
	void insert_resize(const size_type sz, const difference_type offset,
			   const size_type n, const T &val)
	{
		// restore existing elements up to insertion point
		pointer tmp = insert_resize_to_offset(sz, offset);
		// insert new elements
		for (size_type i = 0; i < n; i++)
			_allocator.construct(&tmp[offset + i], val);
		// relocate pre-exisiting elements that were after the insertion
		// point
		insert_resize_construct_end(tmp, offset, n);
	}
	/// Second overload, see first overload for comment.
	///
	void insert_resize(const size_type sz, const difference_type offset,
			   const size_type n, T &&val)
	{
		pointer tmp = insert_resize_to_offset(sz, offset);

		for (size_type i = 0; i < n; i++)
			_allocator.construct(&tmp[offset + i], val);

		insert_resize_construct_end(tmp, offset, n);
	}
	/// Third overload, first, last, see first overload for comment.
	///
	template <class InputIterator>
	void insert_resize(const size_type sz, const difference_type offset,
			   InputIterator first, InputIterator last)
	{
		pointer tmp = insert_resize_to_offset(sz, offset);

		difference_type n = std::distance(first, last);

		for (size_type i = 0; i < n; i++)
			_allocator.construct(&tmp[offset + i], *(first++));

		insert_resize_construct_end(tmp, offset, n);
	}
	/// Fourth overload, init_list, see first overload for comment.
	///
	void insert_resize(const size_type sz, const difference_type offset,
			   const std::initializer_list<T> &il)
	{
		pointer tmp = insert_resize_to_offset(sz, offset);

		size_type i = 0;
		for (auto &val : il)
			_allocator.construct(&tmp[offset + i++], val);

		insert_resize_construct_end(tmp, offset, il.size());
	}
	/// This relocates pre-existing elements that followed the insertion
	/// point.
	///
	void insert_resize_construct_end(pointer &buf,
					 const difference_type offset,
					 const size_type n)
	{
		for (size_type i = offset + n; i < _size; i++)
			_allocator.construct(&buf[i], std::move(_begin[i - n]));

		insert_resize_swap(buf, offset);
	}
	/// This restores the pre-existing elements up to the insertion point.
	///
	pointer insert_resize_to_offset(const size_type &n,
					const difference_type &offset)
	{
		pointer tmp = reallocate(n);

		for (size_type i = 0; i < offset; ++i)
			_allocator.construct(&tmp[i], std::move(_begin[i]));

		return tmp;
	}
	/// This is the post-insertion step that swaps in the new buffer and
	/// nukes the old, fixes up the the iterator vars.
	void insert_resize_swap(pointer &buf, const difference_type offset)
	{
		std::swap(_begin, buf);
		_allocator.deallocate(buf, _capacity);
		_end = _begin + _size;
	}
	/// This is for inplace insertion; it shifts the pre-existing elements
	/// that followed the insertion point end-ward.
	void insert_inplace_end_to_offset(const difference_type &offset)
	{
		// construct _end with the last element
		_allocator.construct(&_begin[_size], std::move(back()));

		for (int i = _size - 1; i > offset; i--)
			_allocator.construct(&_begin[i],
					     std::move(_begin[i - 1]));
	}
	/// Four overloads for insert_inplace
	/// first it shifts the pre-exisiting elements that followed the
	/// insertion
	/// endward, then does the insertion.  It gets called by insert, since
	/// we
	/// are inserting in place, the pre-existing elements priorty to the
	/// insertion point are just left alone.  Capacity is unchanged.
	/// First overload, other overloads follow this pattern with slight
	/// variations.

	void insert_inplace(const difference_type offset, const size_type n,
			    const T &&val)
	{
		insert_inplace_end_to_offset(offset);

		for (size_type i = 0; i < n; i++)
			_begin[offset + i] = std::move(val);

		_end = _begin + _size;
	}

	/// Second overload, see comment on first overload.
	///
	void insert_inplace(const difference_type offset, const size_type n,
			    const T &val)
	{
		insert_inplace_end_to_offset(offset);

		for (size_type i = 0; i < n; i++)
			_begin[offset + i] = val;

		_end = _begin + _size;
	}
	/// Third overload, see first overload for comment.
	///
	void insert_inplace(const difference_type offset,
			    const std::initializer_list<T> &il)
	{
		insert_inplace_end_to_offset(offset);

		size_type i = 0;
		for (auto &val : il)
			_begin[offset + i++] = val;

		_end = _begin + _size;
	}
/// Fourth overload, see first overload for comment.
///
#if __cplusplus >= 201103L
	template <typename InputIterator,
		  typename = std::_RequireInputIter<InputIterator>>
	void insert_inplace(const difference_type offset, InputIterator first,
			    InputIterator last)
	{
		insert_inplace_end_to_offset(offset);

		size_type i = 0;
		while (first != last)
			_begin[offset + i++] = *(first++);

		_end = _begin + _size;
	}
#else
	template <typename InputIterator>
	void insert_inplace(const difference_type offset, InputIterator first,
			    InputIterator last)
	{
		insert_inplace_end_to_offset(offset);

		size_type i = 0;
		while (first != last)
			_begin[offset + i++] = *(first++);

		_end = _begin + _size;
	}
#endif
	void insert_inplace(const difference_type offset, iterator first,
			    iterator last)
	{
		insert_inplace_end_to_offset(offset);

		size_type i = 0;
		while (first != last)
			_begin[offset + i++] = *(first++);

		_end = _begin + _size;
	}

	// resize does the heavy lifting, decides where a recalloc is needed
	void resize_helper(size_type sz, bool refactor, const T &c)
	{

		auto numvals = sz - _size;
		if (sz <= _size) {
			size_type offset = _size - sz;

			for (size_type i = 0; i < offset; ++i)
				pop_back();

		} else
			resize_alloc(sz, numvals, T(), false);

		_size = sz;
		_end = _begin + _size;
	}
	/// push_back support.
	/// This has to support strong guarantee for push_back.
	/// \todo REVIEW
	void push_back_initial_alloc()
	{
		_capacity = _push_back_init_cap; // default starting value
		_begin = _allocator.allocate(_capacity);
		_end = _begin;
	}
	/// For vector::reserve.  We have to check to see if the user has
	/// reserved
	/// a buffer.  If so, we use it, otherwise, we do an allocation.
	pointer reallocate(const size_type n)
	{
		_capacity = n;
		return _allocator.allocate(n);
	}
	//===----------------------------------------------------------------------===//
	/// vector private data members
	//===----------------------------------------------------------------------===//
      private:
	size_type _size = 0;
	size_type _capacity = 0;
	Allocator _allocator;
	pointer _begin = nullptr;
	pointer _end = nullptr;
	size_type _size_n_alloc = 0;
	const size_type _push_back_init_cap = 10;
	const size_type _resize_factor = 2;
	const size_type _max = std::numeric_limits<int>::max();
};
//===----------------------------------------------------------------------===//
/// non-member vector helpers
//===----------------------------------------------------------------------===//
template <class T, class Allocator>
bool operator==(const vector<T, Allocator> &x, const vector<T, Allocator> &y)
{
	return (x.size() == y.size()) &&
	       (std::equal(x.cbegin(), x.cend(), y.cbegin()));
}

template <class T, class Allocator>
bool operator<(const vector<T, Allocator> &x, const vector<T, Allocator> &y)
{
	return std::lexicographical_compare(x.cbegin(), x.cend(), y.cbegin(),
					    y.cend());
}

template <class T, class Allocator>
bool operator!=(const vector<T, Allocator> &x, const vector<T, Allocator> &y)
{
	return !(x == y);
}

template <class T, class Allocator>
bool operator>(const vector<T, Allocator> &x, const vector<T, Allocator> &y)
{
	return y < x;
}

template <class T, class Allocator>
bool operator>=(const vector<T, Allocator> &x, const vector<T, Allocator> &y)
{
	return !(x < y);
}
template <class T, class Allocator>
bool operator<=(const vector<T, Allocator> &x, const vector<T, Allocator> &y)
{
	return !(y < x);
}
} // end stevemac
