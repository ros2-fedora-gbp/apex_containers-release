// Copyright 2019 Apex.AI, Inc.
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

#ifndef APEX_CONTAINERS__UNORDERED_SET_HPP_
#define APEX_CONTAINERS__UNORDERED_SET_HPP_

#include <apex_containers/memory/common.hpp>

#include <functional>
#include <unordered_set>
#include <utility>

namespace apex
{
namespace containers
{
/// Defines a unordered_set type that only works with static allocators. Intended for more
/// complicated use cases, such as those involving unordered_sets nested in a container
template<typename Key, typename Hash = std::hash<Key>,
  typename KeyEqual = std::equal_to<Key>, typename Mutex = memory::default_mutex>
using unordered_set =
  std::unordered_set<Key, Hash, KeyEqual, node_allocator<Key, Mutex>>;
/// Defines unordered_set type which can accept multiple allocators which can be applied to
/// different levels of nested data structures
template<typename Key, typename Hash = std::hash<Key>,
  typename KeyEqual = std::equal_to<Key>, typename Mutex = memory::default_mutex,
  typename ... InnerAlloc>
using unordered_set_scoped_alloc =
  std::unordered_set<Key, Hash, KeyEqual, scoped_node_allocator<Key, Mutex, InnerAlloc...>>;
/// Defines a unordered_multiset type that only works with static allocators. Intended for more
/// complicated use cases, such as those involving unordered_multisets nested in a container
template<typename Key, typename Hash = std::hash<Key>,
  typename KeyEqual = std::equal_to<Key>, typename Mutex = memory::default_mutex>
using unordered_multiset =
  std::unordered_multiset<Key, Hash, KeyEqual, node_allocator<Key, Mutex>>;
/// Defines unordered_multiset type which can accept multiple allocators which can be applied to
/// different levels of nested data structures
template<typename Key, typename Hash = std::hash<Key>,
  typename KeyEqual = std::equal_to<Key>, typename Mutex = memory::default_mutex,
  typename ... InnerAlloc>
using unordered_multiset_scoped_alloc =
  std::unordered_multiset<Key, Hash, KeyEqual, scoped_node_allocator<Key, Mutex, InnerAlloc...>>;

namespace memory
{
/// Helper typedef for unordered set node sizes
template<typename T>
using unordered_set_node_size = foonathan::memory::unordered_set_node_size<T>;
/// These are just pass-through parameters to any std::unordered_set
/// Specialization of node sizes for std::unordered_set
template<typename Key, typename Hash, typename KeyEqual, typename Mutex>
struct NodeSize<unordered_set<Key, Hash, KeyEqual, Mutex>>
  : std::integral_constant<std::size_t, unordered_set_node_size<Key>::value>
{
};
/// Helper typedef for unordered multiset node sizes
template<typename T>
using unordered_multiset_node_size = foonathan::memory::unordered_multiset_node_size<T>;
/// These are just pass-through parameters to any std::unordered_multiset
/// Specialization of node sizes for std::unordered_multiset
template<typename Key, typename Hash, typename KeyEqual, typename Mutex>
struct NodeSize<unordered_multiset<Key, Hash, KeyEqual, Mutex>>
  : std::integral_constant<std::size_t, unordered_multiset_node_size<Key>::value>
{
};
}  // namespace memory
}  // namespace containers
}  // namespace apex
#endif  // APEX_CONTAINERS__UNORDERED_SET_HPP_
