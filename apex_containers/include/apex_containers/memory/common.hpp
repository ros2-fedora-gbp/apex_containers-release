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

#ifndef APEX_CONTAINERS__MEMORY__COMMON_HPP_
#define APEX_CONTAINERS__MEMORY__COMMON_HPP_

#include <foonathan/memory/config.hpp>
#include <foonathan/memory/memory_pool.hpp>
#include <foonathan/memory/std_allocator.hpp>
#include <foonathan/memory/threading.hpp>

/// \cond FALSE
namespace foonathan
{
namespace memory
{
#include <foonathan/memory/detail/container_node_sizes.hpp>
}  // namespace memory
}  // namespace foonathan
/// \endcond FALSE

#include <scoped_allocator>
#include <utility>

namespace apex
{
namespace containers
{
namespace memory
{
/// Allocates a buffer of memory for use with static allocators
using storage_t = foonathan::memory::fixed_allocator_storage;
/// A mutex type intended to be used with allocators
using default_mutex = foonathan::memory::default_mutex;
/// A typetrait for the node size of a given container
template<typename Container>
struct NodeSize;
}  // namespace memory
/// Defines the allocator intended to be used with all non-vector containers
using node_pool_t = foonathan::memory::memory_pool<
  foonathan::memory::node_pool,
  foonathan::memory::static_block_allocator>;
/// A combined storage and pool class.
/// This class over-allocates memory by a factor of two. This is because STL containers do not
/// uniformly allocate memory for each new node.
/// \tparam Container The apex::containers type to set up this pool for
template<typename Container>
class memory_pool
{
  static constexpr std::size_t node_size = memory::NodeSize<Container>::value;

public:
  /// The container type this memory pool is adaptered to
  using container = Container;
  /// \brief Constructor
  /// \param[in] capacity The number of elements for the given container type that this memory pool
  ///                     is prealloacted to store
  explicit memory_pool(const std::size_t capacity)
  : storage_(2U * node_size, capacity),
    pool_(node_size, storage_.size(), storage_)
  {
  }

  /// \brief Implicit conversion to the underlying RawAllocator
  operator node_pool_t &()
  {
    return pool_;
  }

  /// \brief Construct the container with the given arguments
  /// \tparam Args A parameter pack for forwarded constructor arguments
  /// \param[in] args Arguments passed directly to the container constructor (except for allocator)
  template<typename ... Args>
  decltype(auto) create(Args && ... args)
  {
    return container{std::forward<Args>(args)..., *this};
  }

private:
  memory::storage_t storage_;
  node_pool_t pool_;
};  // class memory_pool
/// \example static_stl_main.cpp
/// An example on how to use the memory pool for different containers

/// Adapts defined allocators into std::allocator
template<typename T, typename Mutex = memory::default_mutex>
using node_allocator = foonathan::memory::std_allocator<T, node_pool_t, Mutex>;
/// Adapts defined allocator into a scoped allocator
template<typename T, typename Mutex = memory::default_mutex, typename ... InnerAlloc>
using scoped_node_allocator =
  std::scoped_allocator_adaptor<node_allocator<T, Mutex>, InnerAlloc...>;
}  // namespace containers
}  // namespace apex
#endif  // APEX_CONTAINERS__MEMORY__COMMON_HPP_
