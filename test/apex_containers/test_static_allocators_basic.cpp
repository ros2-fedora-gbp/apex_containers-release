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
/// \brief Unit tests for the single_allocator

#include <gtest/gtest.h>

#include <apex_containers/static_vector.hpp>

#include <string>
#include <cmath>
#include <thread>
#include <utility>
#include <limits>

template<class T>
void test_allocate(T & alloc, std::size_t size)
{
  EXPECT_EQ(alloc.max_size(), size);

  EXPECT_THROW(alloc.allocate(size + 1), std::bad_alloc);
  EXPECT_THROW(alloc.allocate(0), std::bad_alloc);

  auto mem1 = alloc.allocate(size);
  EXPECT_THROW(alloc.allocate(size), std::bad_alloc);

  alloc.deallocate(mem1, size);
  EXPECT_THROW(alloc.allocate(size), std::bad_alloc);
}

TEST(single_allocator, allocate) {
  constexpr auto size = 3453;
  apex::_vector_impl::single_allocator<int> sa(size);
  test_allocate(sa, size);

  EXPECT_THROW(
    apex::_vector_impl::single_allocator<int>
    sa(std::numeric_limits<std::size_t>::max()), std::bad_alloc);
}

TEST(single_allocator, comparison_swap) {
  constexpr auto size = 3453;
  apex::_vector_impl::single_allocator<int> sa(size);
  apex::_vector_impl::single_allocator<int> sb(2 * size);
  ASSERT_EQ(sa, sb);
  ASSERT_EQ(sa != sb, false);
}
