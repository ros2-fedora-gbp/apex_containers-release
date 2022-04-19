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
/// \brief Unit tests for the static_vector.

#include <gtest/gtest.h>

#include <apex_containers/static_vector.hpp>

#include <string>
#include <cmath>
#include <thread>
#include <utility>
#include <vector>

#include "osrf_testing_tools_cpp/memory_tools/memory_tools.hpp"

template<class T>
void fill_n(T & in, const std::size_t num)
{
  const auto max_val = static_cast<typename T::value_type>(num);
  for (typename T::value_type val = typename T::value_type(); val < max_val; ++val) {
    in.push_back(val);
  }
}

template<class T>
void check_n(T & in, const std::size_t num)
{
  const auto max_val = static_cast<typename T::value_type>(num);
  for (typename T::value_type val = typename T::value_type(); val < max_val; ++val) {
    EXPECT_EQ(in.at(val), val);
  }
  EXPECT_EQ(in.size(), num);
}

template<class T>
void test_basic(T & in, const std::size_t size)
{
  osrf_testing_tools_cpp::memory_tools::expect_no_calloc_begin();
  EXPECT_EQ(in.size(), 0);

  auto vvv3 = std::move(in);
  EXPECT_EQ(in.size(), 0);
  EXPECT_EQ(vvv3.size(), 0);

  EXPECT_THROW(in.push_back(size), std::bad_alloc);
  EXPECT_THROW(in.at(0), std::out_of_range);

  fill_n(vvv3, size);
  EXPECT_THROW(vvv3.push_back(size), std::length_error);

  check_n(vvv3, size);

  auto vvv4 = std::move(vvv3);
  check_n(vvv4, size);

  osrf_testing_tools_cpp::memory_tools::expect_no_malloc_end();

  auto vvv5 = apex::static_vector<typename T::value_type>(vvv4.size(), vvv4.cbegin(), vvv4.cend());

  check_n(vvv5, size);
}

TEST(static_vector, test_static_vector_copy_move) {
  {
    constexpr std::size_t s = 2658;
    apex::static_vector<int> vvv(s);

    test_basic(vvv, s);
  }
  {
    constexpr std::size_t s = 363;
    apex::static_vector<double> vvv(s);
    test_basic(vvv, s);
  }
}
TEST(static_vector, copy_assignment) {
  apex::static_vector<int> vvv(3);
  apex::static_vector<int> ttt(2);

  vvv.push_back(5);
  vvv.push_back(5);
  vvv.push_back(5);
  EXPECT_THROW(vvv.push_back(5), std::length_error);
  EXPECT_THROW(ttt = vvv, std::bad_alloc);
}

template<class T>
void test_reserve(T & in, const std::size_t size)
{
  for (std::size_t i = std::size_t(); i < size; ++i) {
    in.reserve(i);
    in.resize(i);
    in.clear();
    in.reserve(1);
    in.shrink_to_fit();
    EXPECT_EQ(in.capacity(), size);
  }
}

TEST(static_vector, test_static_vector_reserve) {
  {
    constexpr std::size_t s = 355;
    auto vvv = apex::static_vector<int>(s);
    osrf_testing_tools_cpp::memory_tools::expect_no_calloc_begin();
    test_reserve(vvv, s);
    osrf_testing_tools_cpp::memory_tools::expect_no_malloc_end();
  }
  {
    constexpr std::size_t s = 363;
    auto vvv = apex::static_vector<double>(s);
    osrf_testing_tools_cpp::memory_tools::expect_no_calloc_begin();
    test_reserve(vvv, s);
    osrf_testing_tools_cpp::memory_tools::expect_no_malloc_end();
  }
}

template<class T>
void test_functions(T & in)
{
  apex::static_vector<int> ttt(43);

  osrf_testing_tools_cpp::memory_tools::expect_no_calloc_begin();
  // Just calling all the proxy functions to make sure the mapping works.
  ttt.emplace_back(32);
  ttt.emplace_back(12);
  ttt.emplace_back(85);
  ttt.emplace_back(34);

  in.begin();
  in.end();
  in.rbegin();
  in.rend();
  in.cbegin();
  in.cend();
  in.crbegin();
  in.crend();

  in.size();
  in.max_size();
  in.resize(33453);
  in.capacity();
  in.empty();
  in.reserve(5345);

  in.operator[](2345);
  in.at(541);
  in.front();
  in.back();
  in.data();

  in.assign(345, 6);
  in.push_back(435);
  in.pop_back();
  in.insert(in.begin() + 4, ttt.begin(), ttt.end());
  in.erase(in.begin() + 5);
  in.swap(ttt);
  in.clear();
  in.emplace(in.begin(), 54);
  in.emplace_back(34);

  std::swap(ttt, in);

  ASSERT_EQ(in == in, true);
  ASSERT_EQ(ttt == in || ttt != in, true);
  ASSERT_EQ(ttt <= in || ttt >= in, true);
  ASSERT_EQ(ttt<in || ttt> in || ttt == in, true);

  ttt.clear();
  ttt.emplace_back(32);
  ttt.emplace_back(12);
  ttt.emplace_back(85);
  ttt.emplace_back(34);

  EXPECT_THROW(apex::static_vector<int> iii(3, ttt.begin(), ttt.end()), std::length_error);
  EXPECT_THROW(apex::static_vector<int> iii(3, ttt.end(), ttt.begin()), std::length_error);

  osrf_testing_tools_cpp::memory_tools::expect_no_calloc_end();
}

TEST(static_vector, functions) {
  apex::static_vector<int> vvv(345364);
  test_functions(vvv);
}

template<class T>
void test_const_functions(const T & in)
{
  const apex::static_vector<int> ttt(43);
  osrf_testing_tools_cpp::memory_tools::expect_no_calloc_begin();
  // Just calling all the const proxy functions to make sure the mapping works.

  in.begin();
  in.end();
  in.rbegin();
  in.rend();
  in.cbegin();
  in.cend();
  in.crbegin();
  in.crend();

  in.size();
  in.max_size();
  in.capacity();
  in.empty();

  in.operator[](2);
  in.at(3);
  in.front();
  in.back();
  in.data();

  ASSERT_EQ(in == in, true);
  ASSERT_EQ(ttt == in || ttt != in, true);
  ASSERT_EQ(ttt <= in || ttt >= in, true);
  ASSERT_EQ(ttt<in || ttt> in || ttt == in, true);

  osrf_testing_tools_cpp::memory_tools::expect_no_calloc_end();
}

TEST(static_vector, functions_const) {
  const apex::static_vector<int> vvv(345, {234, 25, 235, 53, 24});
  test_const_functions(vvv);
}

TEST(static_vector, out_of_range_constructors) {
  constexpr std::size_t size = 456;
  std::vector<double> data;
  data.resize(size);
  EXPECT_THROW(apex::static_vector<double>(size - 1, data.begin(), data.end()), std::length_error);
  EXPECT_THROW(apex::static_vector<double>(5, {1, 2, 3, 4, 5, 6}), std::length_error);
}

TEST(static_vector, special_constructors) {
  std::vector<int> st({234, 25, 235, 53, 24});
  const apex::static_vector<int> vvv(345, {234, 25, 235, 53, 24});
  osrf_testing_tools_cpp::memory_tools::expect_no_calloc_begin();
  EXPECT_EQ(vvv.capacity(), 345);
  EXPECT_EQ(vvv.size(), st.size());
  EXPECT_EQ(std::equal(vvv.begin(), vvv.end(), st.begin()), true);
  osrf_testing_tools_cpp::memory_tools::expect_no_calloc_end();

  apex::static_vector<int> vvv2(35, st.begin(), st.end());
  osrf_testing_tools_cpp::memory_tools::expect_no_calloc_begin();
  EXPECT_EQ(vvv2.capacity(), 35);
  EXPECT_EQ(vvv2.size(), st.size());
  EXPECT_EQ(std::equal(vvv2.begin(), vvv2.end(), st.begin()), true);
  osrf_testing_tools_cpp::memory_tools::expect_no_calloc_end();
  EXPECT_THROW(apex::static_vector<int> vvv3(35, st.end(), st.begin()), std::length_error);
}
