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

#ifndef APEX_CONTAINERS__SET_HPP_
#define APEX_CONTAINERS__SET_HPP_

#include <apex_containers/memory/common.hpp>

#include <functional>
#include <set>
#include <utility>

namespace apex
{
namespace containers
{
/// Defines a set type that only works with static allocators. Intended for more complicated
/// use cases, such as those involving sets nested in a container
template<typename Key, typename Comp = std::less<Key>, typename Mutex = memory::default_mutex>
using set = std::set<Key, Comp, node_allocator<Key, Mutex>>;
/// Defines set type which can accept multiple allocators which can be applied to different
/// levels of nested data structures
template<typename Key, typename Comp = std::less<Key>,
  typename Mutex = memory::default_mutex, typename ... InnerAlloc>
using set_scoped_alloc =
  std::set<Key, Comp, scoped_node_allocator<Key, Mutex, InnerAlloc...>>;
/// Defines a multiset type that only works with static allocators. Intended for more complicated
/// use cases, such as those involving multisets nested in a container
template<typename Key, typename Comp = std::less<Key>, typename Mutex = memory::default_mutex>
using multiset = std::multiset<Key, Comp, node_allocator<Key, Mutex>>;
/// Defines multiset type which can accept multiple allocators which can be applied to different
/// levels of nested data structures
template<typename Key, typename Comp = std::less<Key>,
  typename Mutex = memory::default_mutex, typename ... InnerAlloc>
using multiset_scoped_alloc =
  std::multiset<Key, Comp, scoped_node_allocator<Key, Mutex, InnerAlloc...>>;

namespace memory
{
/// Helper typedef for set node sizes
template<typename T>
using set_node_size = foonathan::memory::set_node_size<T>;
/// These are just pass-through parameters to any std::set
/// Specialization of node sizes for std::set
template<typename Key, typename Comp, typename Mutex>
struct NodeSize<set<Key, Comp, Mutex>>
  : std::integral_constant<std::size_t, set_node_size<Key>::value>
{
};
/// Helper typedef for multiset node sizes
template<typename T>
using multiset_node_size = foonathan::memory::multiset_node_size<T>;
/// These are just pass-through parameters to any std::multiset
/// Specialization of node sizes for std::multiset
template<typename Key, typename Comp, typename Mutex>
struct NodeSize<multiset<Key, Comp, Mutex>>
  : std::integral_constant<std::size_t, multiset_node_size<Key>::value>
{
};
}  // namespace memory
}  // namespace containers
}  // namespace apex
#endif  // APEX_CONTAINERS__SET_HPP_
