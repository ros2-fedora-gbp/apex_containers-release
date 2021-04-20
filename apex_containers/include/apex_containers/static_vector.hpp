// Copyright 2018 Apex.AI, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/// \file
/// \brief A memory static vector.

#ifndef APEX_CONTAINERS__STATIC_VECTOR_HPP_
#define APEX_CONTAINERS__STATIC_VECTOR_HPP_

#include <algorithm>
#include <vector>
#include <typeinfo>
#include <utility>
#include <memory>
namespace apex
{
namespace _vector_impl
{

/**
 * An allocator which can only be used to allocate memory once.
 * \tparam U The type to allocate.
 */
template<class U>
class single_allocator
{
public:
  using base_type = std::allocator<U>;
  using value_type = U;

  using propagate_on_container_copy_assignment = std::true_type;  // for consistency
  using propagate_on_container_move_assignment = std::true_type;  // to avoid the pessimization
  using propagate_on_container_swap = std::true_type;  // to avoid the undefined behavior

  /**
   * Constructor
   * \param n The maximum number of allocatable elements.
   */
  explicit single_allocator(const typename base_type::size_type n)
  : m_size(n), m_consumed(false)
  {
    // Checking that we are not trying to allocate more than the base allocator supports.
    if (n > m_alloc.max_size()) {
      throw std::bad_alloc();
    }
  }

  /// Copy constructor.
  single_allocator(const single_allocator<U> & other) // NOLINT #2459
  : m_size(other.m_size), m_consumed(other.m_consumed), m_alloc(other.m_alloc) {}

  /// Copy assignment operator.
  single_allocator<U> & operator=(const _vector_impl::single_allocator<U> & other)
  {
    m_size = other.m_size;
    m_consumed = other.m_consumed;
    m_alloc = other.m_alloc;
    return *this;
  }

  /// Move constructor.
  single_allocator(single_allocator<U> && other) noexcept
  : m_size(std::move(other.m_size)),
    m_consumed(std::move(other.m_consumed)),
    m_alloc(std::move(other.m_alloc)) {}

  /// Move assignment.
  single_allocator<U> & operator=(single_allocator<U> && other) noexcept
  {
    m_size = std::move(other.m_size);
    m_consumed = std::move(other.m_consumed);
    m_alloc = std::move(other.m_alloc);
    return *this;
  }

  /**
   * Allocates memory. This special allocator imposes the following constraints:
   * * Memory can only allocated once, subsequent attempts throw std::bad_alloc.
   * * The number allocated must match the size specified when constructed the allocator.
   * \param n The number of elements to allocate memory for.
   * \param hint Pointer to nearby memory location.
   * \return Pointer to the first byte of a memory block suitably aligned and sufficient to hold
   *         an array of n objects of type T.
   * \throw std::bad_alloc if there there is no memory available.
   */
  typename base_type::pointer allocate(
    const typename base_type::size_type n, const void * hint = 0)
  {
    if (n != m_size || m_consumed) {
      throw std::bad_alloc();
    }
    // Ensuring exception safety by getting the memory before marking the allocator as used.
    auto && ret = m_alloc.allocate(n, hint);
    m_consumed = true;
    return ret;
  }

  /**
   * Deallocates memory.
   * \param ptr Pointer to the memory to deallocate.
   * \param n Number of elements to deallocate.
   */
  void deallocate(typename base_type::pointer ptr, typename base_type::size_type n)
  {
    m_alloc.deallocate(ptr, n);
  }

  /// The maximum number of elements allocatable.
  typename base_type::size_type max_size() const
  {
    return m_size;
  }

private:
  typename base_type::size_type m_size;
  bool m_consumed;
  base_type m_alloc;
};  // _vector_impl::single_allocator

/// Equal operator
template<typename T, typename U>
inline bool operator==(const single_allocator<T> &, const single_allocator<U> &)
{
  // Memory fom a _vector_impl::single_allocator can always be deallocated by a different
  // single_allocator.
  return true;
}

/// Non-equal operator
template<typename T, typename U>
inline bool operator!=(const single_allocator<T> & a, const single_allocator<U> & b)
{
  return !(a == b);
}

}  // namespace _vector_impl
/**
 * Macro which generates perfect forwarding function without parameters.
 * \param member Member name to use.
 * \param name Member function to use.
 */
#define APEX_STATIC_VECTOR_FWD(member, name) \
  decltype(auto) name() {return member.name();} \
  decltype(auto) name() const {return member.name();}

/**
 * Macro which generates perfect forwarding function with parameters.
 * \param member Member name to use.
 * \param name Member function to use.
 */
#define APEX_STATIC_VECTOR_FWD_PARAM(member, name) \
  template<class ... Params> \
  decltype(auto) name(Params && ... params) { \
    return member.name(std::forward<Params>(params) ...);} \
  template<class ... Params> \
  decltype(auto) name(Params && ... params) const { \
    return member.name(std::forward<Params>(params) ...);}

/**
 * Helper macro to forward definitions.
 * /param type The type to get the definition from.
 * /param name The definition inside the type that should be forwarded.
 */
#define APEX_STATIC_VECTOR_DEFINITIONS_GEN(type, name) \
  using name = typename type::name;

/**
 * Static vector which only allocated memory on construction. Copying this vector is not possible
 * as this would require additional memory.
 * \tparam T Type to store in the vector.
 */
template<class T>
class static_vector
{
private:
  using vector_type = std::vector<T, _vector_impl::single_allocator<T>>;
  std::vector<T, _vector_impl::single_allocator<T>> m_v;

public:
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, value_type)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, allocator_type)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, reference)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, const_reference)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, pointer)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, const_pointer)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, iterator)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, const_iterator)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, reverse_iterator)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, const_reverse_iterator)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, difference_type)
  APEX_STATIC_VECTOR_DEFINITIONS_GEN(vector_type, size_type)
  /**
   * Creates an empty vector.
   * \param capacity Capacity of the vector meaning the maximum number of elements it can hold.
   */
  explicit static_vector(const size_type capacity)
  : m_v(_vector_impl::single_allocator<T>(capacity))
  {
    m_v.reserve(capacity);
  }

  /**
   * Constructs the container with the contents of the range [first, last).
   * \param capacity Capacity of the vector meaning the maximum number of elements it can hold.
   * \param first Begin of the iterator.
   * \param last End of the iterator.
   * \tparam IT Iterator type.
   */
  template<class IT>
  explicit static_vector(
    const size_type capacity,
    const IT first,
    const IT last)
  : m_v(_vector_impl::single_allocator<T>(capacity))
  {
    const auto dist = std::distance(first, last);
    if (dist > static_cast<decltype(dist)>(capacity)) {
      throw std::length_error("[first, last) range error");
    }
    m_v.reserve(capacity);
    // Distance is positive per above condition.
    m_v.resize(std::distance(first, last));
    std::copy(first, last, m_v.begin());
  }
  /**
   * Constructs the container from an initialization list.
   * \param capacity Capacity of the vector meaning the maximum number of elements it can hold.
   * \param il Initialization list to copy to the vector.
   */
  explicit static_vector(
    const size_type capacity,
    std::initializer_list<value_type> il)
  : m_v(_vector_impl::single_allocator<T>(capacity))
  {
    if (il.size() > capacity) {
      throw std::length_error("initializer list too long");
    }
    m_v.reserve(capacity);
    m_v.resize(il.size());
    std::copy(il.begin(), il.end(), m_v.begin());
  }

  static_vector(const static_vector<T> & other) = delete;
  /// Assignment operator.
  static_vector<T> & operator=(const static_vector<T> & other)
  {
    m_v = other.m_v;
    return *this;
  }

  /// Move constructor.
  static_vector(static_vector<T> && other) noexcept
  : m_v(std::move(other.m_v)) {}

  /// Move assignment.
  static_vector<T> & operator=(static_vector<T> && other) noexcept
  {
    m_v = std::move(other.m_v);
    return *this;
  }

  /// Noop. This is just implemented for compatibility reasons.
  void shrink_to_fit()
  {
    // The C++ standard allows shrink to fit to be a noop which is the case here to prevent any
    // reallocation.
  }

  /**
   * Exchanges the content of the container by the content another vector
   * object of the same type. Sizes may differ.
   * \param x The vector to swap with.
   */
  void swap(static_vector<T> & x)
  {
    m_v.swap(x.m_v);
  }

  APEX_STATIC_VECTOR_FWD(m_v, begin)
  APEX_STATIC_VECTOR_FWD(m_v, end)
  APEX_STATIC_VECTOR_FWD(m_v, rbegin)
  APEX_STATIC_VECTOR_FWD(m_v, rend)
  APEX_STATIC_VECTOR_FWD(m_v, cbegin)
  APEX_STATIC_VECTOR_FWD(m_v, cend)
  APEX_STATIC_VECTOR_FWD(m_v, crbegin)
  APEX_STATIC_VECTOR_FWD(m_v, crend)

  APEX_STATIC_VECTOR_FWD(m_v, size)
  APEX_STATIC_VECTOR_FWD(m_v, max_size)
  APEX_STATIC_VECTOR_FWD_PARAM(m_v, resize)
  APEX_STATIC_VECTOR_FWD(m_v, capacity)
  APEX_STATIC_VECTOR_FWD(m_v, empty)
  APEX_STATIC_VECTOR_FWD_PARAM(m_v, reserve)

  APEX_STATIC_VECTOR_FWD_PARAM(m_v, operator[])
  APEX_STATIC_VECTOR_FWD_PARAM(m_v, at)
  APEX_STATIC_VECTOR_FWD(m_v, front)
  APEX_STATIC_VECTOR_FWD(m_v, back)
  APEX_STATIC_VECTOR_FWD(m_v, data)

  APEX_STATIC_VECTOR_FWD_PARAM(m_v, assign)
  APEX_STATIC_VECTOR_FWD_PARAM(m_v, push_back)
  APEX_STATIC_VECTOR_FWD(m_v, pop_back)
  APEX_STATIC_VECTOR_FWD_PARAM(m_v, insert)
  APEX_STATIC_VECTOR_FWD_PARAM(m_v, erase)
  APEX_STATIC_VECTOR_FWD(m_v, clear)
  APEX_STATIC_VECTOR_FWD_PARAM(m_v, emplace)
  APEX_STATIC_VECTOR_FWD_PARAM(m_v, emplace_back)

  template<class K>
  friend bool operator==(const static_vector<K> & lhs, const static_vector<K> & rhs);
  template<class K>
  friend bool operator!=(const static_vector<K> & lhs, const static_vector<K> & rhs);
  template<class K>
  friend bool operator<(const static_vector<K> & lhs, const static_vector<K> & rhs);
  template<class K>
  friend bool operator<=(const static_vector<K> & lhs, const static_vector<K> & rhs);
  template<class K>
  friend bool operator>(const static_vector<K> & lhs, const static_vector<K> & rhs);
  template<class K>
  friend bool operator>=(const static_vector<K> & lhs, const static_vector<K> & rhs);
};

/**
 * Macro to forward binary operators.
 * /param op Operator to forward.
 * /param member Member to apply the operator on.
 */
#define APEX_VECTOR_CMP_OP(op, member) \
  template<class K> \
  bool operator op( \
    const static_vector<K> & lhs, \
    const static_vector<K> & rhs) { \
    return operator op(lhs.member, rhs.member); \
  }

APEX_VECTOR_CMP_OP(==, m_v)
APEX_VECTOR_CMP_OP(!=, m_v)
APEX_VECTOR_CMP_OP(<, m_v) // NOLINT
APEX_VECTOR_CMP_OP(<=, m_v)
APEX_VECTOR_CMP_OP(>, m_v) // NOLINT
APEX_VECTOR_CMP_OP(>=, m_v)
}  // namespace apex

namespace std
{
template<class T>
void swap(apex::static_vector<T> & rhs, apex::static_vector<T> & lhs)
{
  rhs.swap(lhs);
}
}  // namespace std
#endif  // APEX_CONTAINERS__STATIC_VECTOR_HPP_
