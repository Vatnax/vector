#pragma once

#include "../errors and sfinae/sfinae.h"
#include "../errors and sfinae/errors.h"

// Using declarations
using std::size_t;
using std::ptrdiff_t;

template<typename T>
class Vector
{
private:
	template<bool is_T_const>
	class VectorIteratorBase
	{
	public:
		// Just for the iterator_traits, won't be used often
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using reference = T&;
		friend class VectorIteratorBase<true>; // I don't know why this works instead of friending <false> but ok
		friend class Vector<T>;
	protected:
		const_T_or_nonconst_T<is_T_const, T>* ptr = nullptr;
		const Vector<T>& owner;
	private:
		VectorIteratorBase(T* p, const Vector<T>& own) noexcept : ptr(p), owner(own) {}
	public:
		VectorIteratorBase(const VectorIteratorBase<false>& rhs) noexcept : ptr(rhs.ptr), owner(rhs.owner) {} // non-const to const iterator conversion
		virtual const_T_or_nonconst_T<is_T_const, T>& operator*() const noexcept { return *ptr; }
		virtual const_T_or_nonconst_T<is_T_const, T>* operator->() const noexcept = 0 { return ptr; }
	};

	template<bool is_T_const>
	class VectorIterator : public VectorIteratorBase<is_T_const>
	{
	private:
		using Base = VectorIteratorBase<is_T_const>;
		friend Vector<T>;
	private:
		VectorIterator(T* p, const Vector<T>& own) noexcept : Base(p, own) {}
		bool is_begin() const noexcept { return Base::ptr == Base::owner.begin().operator->(); }
		bool is_end() const noexcept { return Base::ptr == Base::owner.end().operator->(); }
		bool is_lower_than_begin() const noexcept { return Base::ptr < Base::owner.begin().operator->(); }
		bool is_greater_than_end() const noexcept { return Base::ptr > Base::owner.end().operator->(); }
	public:
		VectorIterator(const VectorIterator<false>& rhs) noexcept : Base(rhs) {} // non-const to const iterator conversion
		VectorIterator& operator++() noexcept
		{
			err::exit_if(is_end(), err::increment_end);
			++Base::ptr;
			return *this;
		}
		VectorIterator operator++(int) noexcept
		{
			VectorIterator temp(*this);
			operator++();
			return temp;
		}
		VectorIterator& operator--() noexcept
		{
			err::exit_if(is_begin(), err::decrement_begin);
			--Base::ptr;
			return *this;
		}
		VectorIterator operator--(int) noexcept
		{
			VectorIterator temp(*this);
			operator--();
			return temp;
		}
		VectorIterator operator+(const size_t offset) noexcept
		{
			VectorIterator temp(*this);
			return temp += offset;
		}
		friend VectorIterator operator+(const size_t offset, VectorIterator& it) noexcept
		{
			return it.operator+(offset);
		}
		VectorIterator& operator+=(const size_t offset) noexcept
		{
			Base::ptr += offset;
			err::exit_if(is_greater_than_end(), err::traversed_vector); // After adding to the ptr
			return *this;
		}
		VectorIterator operator-(const size_t offset) noexcept
		{
			VectorIterator temp(*this);
			return temp -= offset;
		}
		VectorIterator& operator-=(const size_t offset) noexcept
		{
			Base::ptr -= offset;
			err::exit_if(is_lower_than_begin(), err::traversed_vector); // After subtracting to the ptr
			return *this;
		}
		ptrdiff_t operator-(const VectorIterator& it) const noexcept { return Base::ptr - it.ptr; }
		const_T_or_nonconst_T<is_T_const, T>& operator*() const noexcept
		{
			err::exit_if(is_end(), err::deref_end);
			return Base::operator*();
		}
		const_T_or_nonconst_T<is_T_const, T>& operator[](const size_t offset) const noexcept
		{
			T* temp = Base::ptr + offset;
			err::exit_if(temp > Base::owner.end().operator->(), err::traversed_vector); // After adding to the ptr
			return *(temp);
		}
		const_T_or_nonconst_T<is_T_const, T>* operator->() const noexcept override { return Base::operator->(); }
		bool operator==(const VectorIterator& it) const noexcept { return Base::ptr == it.ptr; }
		bool operator!=(const VectorIterator& it) const noexcept { return Base::ptr != it.ptr; }
		bool operator<(const VectorIterator& it) const noexcept { return Base::ptr < it.ptr; }
		bool operator<=(const VectorIterator& it) const noexcept { return Base::ptr <= it.ptr; }
		bool operator>(const VectorIterator& it) const noexcept { return Base::ptr > it.ptr; }
		bool operator>=(const VectorIterator& it) const noexcept { return Base::ptr >= it.ptr; }
	};
	template<bool is_T_const>
	class ReverseVectorIterator : public VectorIteratorBase<is_T_const>
	{
	private:
		using Base = VectorIteratorBase<is_T_const>;
		friend Vector<T>;
	private:
		ReverseVectorIterator(T* p, const Vector<T>& own) noexcept : Base(p, own) {}
		bool is_rbegin() const noexcept { return Base::ptr == Base::owner.rbegin().operator->(); }
		bool is_rend() const noexcept { return Base::ptr == Base::owner.rend().operator->(); }
		bool is_greater_than_rbegin() const noexcept { return Base::ptr > Base::owner.rbegin().operator->(); }
		bool is_lower_than_rend() const noexcept { return Base::ptr < Base::owner.rend().operator->(); }
	public:
		ReverseVectorIterator(const ReverseVectorIterator<false>& rhs) noexcept : Base(rhs) {} // non-const to const iterator conversion
		ReverseVectorIterator& operator++() noexcept
		{
			err::exit_if(is_rend(), err::increment_rend);
			--Base::ptr;
			return *this;
		}
		ReverseVectorIterator operator++(int) noexcept
		{
			ReverseVectorIterator temp(*this);
			operator++();
			return temp;
		}
		ReverseVectorIterator& operator--() noexcept
		{
			err::exit_if(is_rbegin(), err::decrement_rbegin);
			++Base::ptr;
			return *this;
		}
		ReverseVectorIterator operator--(int) noexcept
		{
			ReverseVectorIterator temp(*this);
			operator--();
			return temp;
		}
		ReverseVectorIterator operator+(const size_t offset) noexcept
		{
			ReverseVectorIterator temp(*this);
			return temp += offset;
		}
		friend ReverseVectorIterator operator+(const size_t offset, ReverseVectorIterator& it)
		{
			return it.operator+(offset);
		}
		ReverseVectorIterator& operator+=(const size_t offset) noexcept
		{
			Base::ptr -= offset;
			err::exit_if(is_lower_than_rend(), err::traversed_vector);
			return *this;
		}
		ReverseVectorIterator operator-(const size_t offset) noexcept
		{
			ReverseVectorIterator temp(*this);
			return temp -= offset;
		}
		ReverseVectorIterator& operator-=(const size_t offset) noexcept
		{
			Base::ptr += offset;
			err::exit_if(is_greater_than_rbegin(), err::traversed_vector);
			return *this;
		}
		ptrdiff_t operator-(const ReverseVectorIterator& it) const noexcept { return Base::ptr - it.ptr; }
		const_T_or_nonconst_T<is_T_const, T>& operator*() const noexcept
		{
			err::exit_if(is_rend(), err::deref_rend);
			return Base::operator*();
		}
		const_T_or_nonconst_T<is_T_const, T>& operator[](const size_t offset) const noexcept
		{
			T* temp = Base::ptr - offset;
			err::exit_if(temp < Base::owner.rend().operator->(), err::traversed_vector); // After adding to the ptr
			return *(temp);
		}
		const_T_or_nonconst_T<is_T_const, T>* operator->() const noexcept override { return Base::operator->(); }
		bool operator==(const ReverseVectorIterator& it) const noexcept { return Base::ptr == it.ptr; }
		bool operator!=(const ReverseVectorIterator& it) const noexcept { return Base::ptr != it.ptr; }
		bool operator<(const ReverseVectorIterator& it) const noexcept { return Base::ptr > it.ptr; }
		bool operator<=(const ReverseVectorIterator& it) const noexcept { return Base::ptr >= it.ptr; }
		bool operator>(const ReverseVectorIterator& it) const noexcept { return Base::ptr < it.ptr; }
		bool operator>=(const ReverseVectorIterator& it) const noexcept { return Base::ptr <= it.ptr; }
	};
public:
	using Iterator = VectorIterator<false>;
	using ConstIterator = VectorIterator<true>;
	using ReverseIterator = ReverseVectorIterator<false>;
	using ConstReverseIterator = ReverseVectorIterator<true>;
private:
	T* storage = nullptr;
	size_t vec_size = 0;
	size_t vec_capacity = 0;
private:
	void construct(size_t size) noexcept;
	void re_alloc(size_t new_cap) noexcept;
	void uninitialized_fill(const T& val) noexcept;
	bool should_re_alloc(const size_t offset = 1) const noexcept { return vec_size + offset > vec_capacity; }
	size_t double_capacity_0_prevented() const noexcept { return vec_capacity == 0 ? 1 : vec_capacity * 2; }
	bool have_diff_owner(ConstIterator it1, ConstIterator it2) const noexcept { return std::addressof(it1.owner) != std::addressof(it2.owner); }
	bool have_diff_owner(ConstIterator it) const noexcept { return this != std::addressof(it.owner); }
	bool is_same_iter(ConstIterator it1, ConstIterator it2) const noexcept { return it1 == it2; }
	void destroy();
public:
	Vector() noexcept;
	explicit Vector(const size_t siz) noexcept;
	Vector(const size_t siz, const T& val) noexcept;
	Vector(const std::initializer_list<T>& init) noexcept;
	template<typename Iter> Vector(Iter it1, Iter it2, require_forward_it<Iter>* = nullptr) noexcept;
	Vector(const Vector& rhs) noexcept;
	Vector& operator=(const Vector& rhs) noexcept;
	Vector(Vector&& rhs) noexcept;
	Vector& operator=(Vector&& rhs) noexcept;
	template<typename Iter> void assign(Iter it1, Iter it2, require_forward_it<Iter>* = nullptr) noexcept;
	void fill(const T& val) noexcept;
	size_t size() const noexcept { return vec_size; }
	size_t capacity() const noexcept { return vec_capacity; }
	bool empty() const noexcept { return vec_size == 0; }
	T* data() const noexcept { return storage; }
	T& front() const noexcept { err::exit_if(empty(), err::front_empty_vector); return storage[0]; }
	T& back() const noexcept { err::exit_if(empty(), err::back_empty_vector); return storage[vec_size - 1]; }
	T& operator[](size_t index) const noexcept { err::exit_if(index >= vec_size, err::subscript_out_of_range); return storage[index]; }
	T& at(size_t index) const noexcept { return this->operator[](index); }
	void push_back(const T& val) noexcept;
	template<typename... Args> T& emplace_back(Args&&... args) noexcept;
	Iterator insert(ConstIterator it, const T& val) noexcept;
	template<typename... Args> Iterator emplace(ConstIterator it, Args&&... args) noexcept;
	void pop_back() noexcept;
	void reserve(const size_t new_cap) noexcept;
	void resize(const size_t new_size, const T& val) noexcept;
	void shrink_to_fit() noexcept;
	void clear() noexcept;
	void swap(Vector<T>& rhs) noexcept;
	Iterator erase(Iterator it1, Iterator it2) noexcept;
	Iterator begin() const noexcept { return Iterator(storage, *this); }
	Iterator end() const noexcept { return Iterator(storage + vec_size, *this); }
	ConstIterator cbegin() const noexcept { return ConstIterator(storage, *this); }
	ConstIterator cend() const noexcept { return ConstIterator(storage + vec_size, *this); }
	ReverseIterator rbegin() const noexcept { return ReverseIterator(storage + vec_size - 1, *this); }
	ReverseIterator rend() const noexcept { return ReverseIterator(storage - 1, *this); }
	ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(storage + vec_size - 1, *this); }
	ConstReverseIterator crend() const noexcept { return ConstReverseIterator(storage - 1, *this); }
	~Vector();
};

template<typename T>
void Vector<T>::construct(size_t cap) noexcept
{
	storage = (T*) malloc(cap * sizeof(T));
	vec_capacity = cap;
	vec_size = vec_capacity;
}

template<typename T>
void Vector<T>::re_alloc(size_t new_cap) noexcept
{
	// Allocate a chunk of memory
	T* temp = (T*) malloc(new_cap * sizeof(T));

	// Move data from current storage to the temporary one
	for (size_t i = 0; i < vec_size; ++i)
		temp[i] = std::move(storage[i]);

	// Destroy (free) memory from current storage (all information have been moved to the other storage so there is no risk in destroying this object)
	destroy();

	// Restore data from the temporary object
	storage = temp;
	vec_capacity = new_cap;
}

template<typename T>
Vector<T>::Vector() noexcept
{
	construct(0);
}

template<typename T>
Vector<T>::Vector(const size_t siz) noexcept
{
	construct(siz);
	this->uninitialized_fill(T());
}

template<typename T>
Vector<T>::Vector(const size_t siz, const T& val) noexcept
{
	construct(siz);
	this->uninitialized_fill(val);
}

template<typename T>
Vector<T>::Vector(const std::initializer_list<T>& init) noexcept
{
	construct(init.size());

	size_t i = 0;
	// Copy data
	for (const auto& val : init)
		storage[i++] = val;
}

template<typename T>
template<typename Iter>
Vector<T>::Vector(Iter it1, Iter it2, require_forward_it<Iter>*) noexcept
{
	construct(std::distance(it1, it2));

	// Copy data
	for (size_t i = 0; it1 != it2; ++i, ++it1)
		storage[i] = *it1;
}

template<typename T>
template<typename Iter>
void Vector<T>::assign(Iter it1, Iter it2, require_forward_it<Iter>*) noexcept
{
	destroy();
	construct(std::distance(it1, it2));

	// Copy data
	for (size_t i = 0; it1 != it2; ++i, ++it1)
		storage[i] = *it1;
}

template<typename T>
Vector<T>::Vector(const Vector& rhs) noexcept
{
	construct(rhs.vec_capacity);

	// Copy data
	for (size_t i = 0; i < vec_size; ++i)
		storage[i] = rhs.storage[i];
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs) noexcept
{
	if (this != &rhs)
	{
		destroy();
		construct(rhs.vec_capacity);

		// Copy data
		for (size_t i = 0; i < vec_size; ++i)
			storage[i] = rhs.storage[i];
	}
	return *this;
}

template<typename T>
Vector<T>::Vector(Vector&& rhs) noexcept
{
	// This
	storage = rhs.storage;
	vec_capacity = rhs.vec_capacity;
	vec_size = rhs.vec_size;

	// Rhs
	rhs.construct(0);
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
{
	if (this == &rhs)
	{
		destroy();

		// This
		storage = rhs.storage;
		vec_capacity = rhs.vec_capacity;
		vec_size = rhs.vec_size;

		// Rhs
		rhs.construct(0);
	}
	return *this;
}

template<typename T>
void Vector<T>::uninitialized_fill(const T& val) noexcept
{
	// Copy data
	for (size_t i = 0; i < vec_size; ++i)
		storage[i] = val;
}

template<typename T>
void Vector<T>::fill(const T& val) noexcept
{
	destroy(); // Preventing memory leak
	construct(vec_capacity);

	// Copy data
	for (size_t i = 0; i < vec_size; ++i)
		storage[i] = val;
}

template<typename T>
void Vector<T>::push_back(const T& val) noexcept
{
	if (should_re_alloc())
		re_alloc(double_capacity_0_prevented());

	storage[vec_size++] = val;
}


template<typename T>
template<typename... Args>
T& Vector<T>::emplace_back(Args&&... args) noexcept
{
	if (should_re_alloc())
		re_alloc(double_capacity_0_prevented());

	new (&storage[vec_size++]) T(std::forward<Args>(args)...);
	return storage[vec_size];
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::insert(ConstIterator it, const T& val) noexcept
{
	err::exit_if(have_diff_owner(it), err::diff_vectors);

	// Sneaky index calculation
	const ptrdiff_t index = it - begin();

	if (should_re_alloc())
		re_alloc(double_capacity_0_prevented());

	// Moving backwards and making space to a single value
	for (ptrdiff_t i = vec_size - 1; i >= index; --i)
		storage[i + 1] = std::move(storage[i]);

	// Move value into the created space
	storage[index] = val;
	++vec_size;
	return Iterator(storage + index, *this);
}

template<typename T>
template<typename... Args>
typename Vector<T>::Iterator Vector<T>::emplace(ConstIterator it, Args&&... args) noexcept
{
	err::exit_if(have_diff_owner(it), err::diff_vectors);

	// Sneaky index calculation
	const ptrdiff_t index = it - begin();

	if (should_re_alloc())
		re_alloc(double_capacity_0_prevented());

	// Moving backwards and making space to a single value
	for (ptrdiff_t i = vec_size - 1; i >= index; --i)
		storage[i + 1] = std::move(storage[i]);

	// Move value into the created space
	new (&storage[index]) T(std::forward<Args>(args)...);
	++vec_size;
	return Iterator(storage + index, *this);
}

template<typename T>
void Vector<T>::pop_back() noexcept
{
	err::exit_if(empty(), err::pop_empty_vector);
	storage[--vec_size].~T();
}

template<typename T>
void Vector<T>::reserve(const size_t new_cap) noexcept
{
	if (new_cap > vec_capacity)
		re_alloc(new_cap);
}

template<typename T>
void Vector<T>::resize(const size_t new_size, const T& val) noexcept
{
	if (new_size == vec_size)
		return; // Nothing to do if equal

	// Basically re_alloc with some differences if the new size is less than the current one
	if (new_size < vec_size)
	{
		T* temp = (T*) malloc(vec_capacity * sizeof(T));

		for (size_t i = 0; i < new_size; ++i)
			temp[i] = std::move(storage[i]);

		destroy();
		storage = temp;
		vec_size = new_size;
		return;
	}

	// If the new size is bigger than the current one

	const size_t overall_size = new_size + vec_size;

	if (should_re_alloc(new_size))
		re_alloc(overall_size);

	for (size_t i = vec_size; i < overall_size; ++i)
		storage[i] = val;

	vec_size = new_size;
}

template<typename T>
void Vector<T>::shrink_to_fit() noexcept
{
	if (vec_capacity > vec_size)
		re_alloc(vec_size);
}

template<typename T>
void Vector<T>::clear() noexcept
{
	destroy();
	construct(0);
}

template<typename T>
void Vector<T>::swap(Vector<T>& rhs) noexcept
{
	Vector<T> temp = std::move(*this);
	*this = std::move(rhs);
	rhs = std::move(temp);
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::erase(Iterator it1, Iterator it2) noexcept
{
	err::exit_if(have_diff_owner(it1, it2) || have_diff_owner(it1), err::diff_vectors);

	// Removing this if-statement would cause harm due to the second loop in this function. It would move value stored in it2 into it1
	// (which are the same memory addresses), call destructor on it2 (and it1 also since they are pointing to the same memory location),
	// so the value would be unusable and later in the destroy() function it would call destructors second time on the same object(s).
	if (is_same_iter(it1, it2))
		return it2;

	for (auto it = it1; it != it2; ++it)
		(*it).~T();

	auto _it1 = it1;
	auto _it2 = it2;

	for (; _it2 != end(); ++_it2, ++_it1)
	{
		*_it1 = std::move(*_it2);
		(*_it2).~T();
	}

	vec_size = vec_size - (it2 - it1); // I think it is safe to do because (subtraction of int and unsigned int), it2 - it1 will never be < 0 unless
								      // it2 < it1 but even if, abort will be called

	return it2;
}

template<typename T>
void Vector<T>::destroy()
{
	// Calling destructors
	if (!empty()) // Do only if not empty, otherwise vec_size == 0, vec_size - 1 == ~ 4 mld what causes problems
		for (size_t i = 0; i < vec_size - 1; ++i)
			storage[i].~T();
	free(storage);
}


template<typename T>
Vector<T>::~Vector()
{
	destroy();
}
