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

#ifndef APEX_CONTAINERS__MAP_HPP_
#define APEX_CONTAINERS__MAP_HPP_

#include <apex_containers/memory/common.hpp>

#include <functional>
#include <map>
#include <utility>

namespace apex
{
namespace containers
{
/// Defines a map type that only works with static allocators. Intended for more complicated
/// use cases, such as those involving maps nested in a container
template<typename Key, typename Value, typename Comp = std::less<Key>,
  typename Mutex = memory::default_mutex>
using map = std::map<Key, Value, Comp, node_allocator<std::pair<const Key, Value>, Mutex>>;
/// Defines map type which can accept multiple allocators which can be applied to different
/// levels of nested data structures
template<typename Key, typename Value, typename Comp = std::less<Key>,
  typename Mutex = memory::default_mutex, typename ... InnerAlloc>
using map_scoped_alloc = std::map<Key, Value, Comp,
    scoped_node_allocator<std::pair<const Key, Value>, Mutex, InnerAlloc...>>;
/// Defines a multimap type that only works with static allocators. Intended for more complicated
/// use cases, such as those involving multimaps nested in a container
template<typename Key, typename Value, typename Comp = std::less<Key>,
  typename Mutex = memory::default_mutex>
using multimap =
  std::multimap<Key, Value, Comp, node_allocator<std::pair<const Key, Value>, Mutex>>;
/// Defines multimap type which can accept multiple allocators which can be applied to different
/// levels of nested data structures
template<typename Key, typename Value, typename Comp = std::less<Key>,
  typename Mutex = memory::default_mutex, typename ... InnerAlloc>
using multimap_scoped_alloc = std::multimap<Key, Value, Comp,
    scoped_node_allocator<std::pair<const Key, Value>, Mutex, InnerAlloc...>>;

namespace memory
{
/// Helper typedef for map node sizes
template<typename T>
using map_node_size = foonathan::memory::map_node_size<T>;
/// These are just pass-through parameters to any std::map
/// Specialization of node sizes for std::map
template<typename Key, typename Value, typename Comp, typename Mutex>
struct NodeSize<map<Key, Value, Comp, Mutex>>
  : std::integral_constant<std::size_t, map_node_size<std::pair<const Key, Value>>::value>
{
};
/// Helper typedef for multimap node sizes
template<typename T>
using multimap_node_size = foonathan::memory::multimap_node_size<T>;
/// These are just pass-through parameters to any std::multimap
/// Specialization of node sizes for std::multimap
template<typename Key, typename Value, typename Comp, typename Mutex>
struct NodeSize<multimap<Key, Value, Comp, Mutex>>
  : std::integral_constant<std::size_t, multimap_node_size<std::pair<const Key, Value>>::value>
{
};
}  // namespace memory

}  // namespace containers
}  // namespace apex
#endif  // APEX_CONTAINERS__MAP_HPP_
