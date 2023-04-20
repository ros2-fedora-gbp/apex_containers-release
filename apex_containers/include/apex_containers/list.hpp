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

#ifndef APEX_CONTAINERS__LIST_HPP_
#define APEX_CONTAINERS__LIST_HPP_

#include <apex_containers/memory/common.hpp>

#include <list>

namespace apex
{
namespace containers
{
/// Defines a list type that only works with static allocators. Intended for more
/// complicated use cases, such as those involving lists nested in a container
template<typename T, typename Mutex = memory::default_mutex>
using list = std::list<T, node_allocator<T, Mutex>>;
/// Defines list type which can accept multiple allocators which can be applied to
/// different levels of nested data structures
template<typename T, typename Mutex = memory::default_mutex, typename ... InnerAlloc>
using list_scoped_alloc = std::list<T, scoped_node_allocator<T, Mutex, InnerAlloc...>>;

namespace memory
{
/// Helper typedef for unordered set node sizes
template<typename T>
using list_node_size = foonathan::memory::list_node_size<T>;
/// These are just pass-through parameters to any std::list
/// Specialization of node sizes for std::list
template<typename T, typename Mutex>
struct NodeSize<list<T, Mutex>>: std::integral_constant<std::size_t, list_node_size<T>::value>
{
};
}  // namespace memory
}  // namespace containers
}  // namespace apex
#endif  // APEX_CONTAINERS__LIST_HPP_
