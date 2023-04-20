// Copyright 2017-2018 Apex.AI, Inc.
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

#include <string/string.hpp>
#include <gtest/gtest.h>
#include <functional>
#include <utility>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <unordered_set>

template<class T>
void check_const_out_idx(const T & str, size64_t idx)
{
  EXPECT_EQ(str[str.length()], '\0');
  EXPECT_THROW(str[idx], std::out_of_range);
}

TEST(string, all_cases) {
  apex_string_t astr256 = apex_string_create();

  memset(astr256.c_str, 0xFFU, APEX_STRING_SIZE);
  apex::String<256U> str256U(astr256);
  EXPECT_EQ(str256U.length(), 255U);
  EXPECT_EQ(str256U[255U], '\0');


  EXPECT_THROW(str256U[256U], std::out_of_range);

  apex::String<256U> str2(static_cast<char8_t *>(nullptr));
  EXPECT_EQ(str2[0U], '\0');

  apex::String<4U> str4U("123");
  EXPECT_STREQ(str4U.c_str(), "123");
  EXPECT_THROW(str4U[4U], std::out_of_range);
  EXPECT_THROW(str4U[5U], std::out_of_range);
  apex::String<3U> str3U(str4U);
  EXPECT_STREQ(str3U.c_str(), "12");
  str4U[str4U.length()] = '\0';
  check_const_out_idx(str4U, 4U);

  apex::String<4U> str4U2;
  str4U2 = str4U;
  EXPECT_STREQ(str4U2.c_str(), str4U.c_str());

  std::string big_str(MAX_APEX_STRING_LEN + 1U, '*');
  apex::String<256U> str5(big_str.c_str());
  EXPECT_STREQ(str5.c_str(), big_str.substr(0U, 255U).c_str());
  EXPECT_EQ(str5[255U], '\0');


  apex::String<16U> str16U;
  str16U = big_str.c_str();
  EXPECT_STREQ(str16U.c_str(), big_str.substr(0U, 15U).c_str());
  EXPECT_EQ(str16U[15U], '\0');

  str16U = str16U + apex::String<MAX_APEX_STRING_LEN + 2U>(big_str.c_str());
  EXPECT_STREQ(str16U.c_str(), big_str.substr(0U, 15U).c_str());
  EXPECT_EQ(str16U[15U], '\0');

  EXPECT_NO_THROW(str16U += big_str.c_str());
  EXPECT_STREQ(str16U.c_str(), big_str.substr(0U, 15U).c_str());
  EXPECT_EQ(str16U[15U], '\0');

  str3U = "0";
  str3U += "0123456789";
  EXPECT_STREQ(str3U.c_str(), "00");
  apex::String<4U> str111;
  str111 = str3U + apex::String<4U>("1");
  EXPECT_STREQ(str111.c_str(), "001");

  apex_string_t astr123 = apex_string_create();
  EXPECT_EQ(apex_string_add(&astr123, "123"), 3U);
  str16U = astr123 + str3U;
  EXPECT_STREQ(str16U.c_str(), "12300");
}

apex::string_strict16_t foo(const apex::string_strict8_t & src)
{
  apex::string_strict16_t retval(src);
  return retval;
}

apex::string16_t bar(const apex::string8_t & src)
{
  apex::string16_t retval(src);
  return retval;
}

TEST(string, string_typedefs) {
  apex::string8_t s8("123");
  apex::string16_t s16(s8);
  apex::string32_t s32 = "1234";
  apex::string64_t s64("12");
  apex::string128_t s128;
  apex::string256_t s256 = bar("12345678");

  EXPECT_FALSE(s8.empty());
  EXPECT_FALSE(s16.empty());
  EXPECT_FALSE(s32.empty());
  EXPECT_FALSE(s64.empty());
  EXPECT_TRUE(s128.empty());
  EXPECT_FALSE(s256.empty());

  EXPECT_TRUE(s8 == "123"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 != "123"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 < "123"); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 <= "123"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 > "123"); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 >= "123"); //NOLINT, we want comparison op test1

  EXPECT_TRUE(s8 != "1234"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 == "1234"); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 < "1234"); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 <= "1234"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 > "1234"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 >= "1234"); //NOLINT, we want comparison op test1

  EXPECT_TRUE(s8 != "12"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 == "12"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 < "12"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 <= "12"); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 > "12"); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 >= "12"); //NOLINT, we want comparison op test1

  EXPECT_TRUE(s8 != "abc"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 == "abc"); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 < "abc"); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 <= "abc"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 > "abc"); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 >= "abc"); //NOLINT, we want comparison op test1

  EXPECT_TRUE(s8 != ""); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 == ""); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 < ""); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 <= ""); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 > ""); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 >= ""); //NOLINT, we want comparison op test1


  EXPECT_THROW(s8 != nullptr, std::invalid_argument); //NOLINT, we want comparison op test1
  EXPECT_THROW(s8 == nullptr, std::invalid_argument); //NOLINT, we want comparison op test1
  EXPECT_THROW(s8 < nullptr, std::invalid_argument); //NOLINT, we want comparison op test1
  EXPECT_THROW(s8 <= nullptr, std::invalid_argument); //NOLINT, we want comparison op test1
  EXPECT_THROW(s8 > nullptr, std::invalid_argument); //NOLINT, we want comparison op test1
  EXPECT_THROW(s8 >= nullptr, std::invalid_argument); //NOLINT, we want comparison op test1


  EXPECT_TRUE(s8 == s8); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 != s8); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 < s8); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 <= s8); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 > s8); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 >= s8); //NOLINT, we want comparison op test1

  EXPECT_TRUE(s8 == s16); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 != s16); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 < s16); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 <= s16); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 > s16); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 >= s16); //NOLINT, we want comparison op test1

  EXPECT_FALSE(s8 == s32); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 != s32); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 < s32); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 <= s32); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 > s32); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 >= s32); //NOLINT, we want comparison op test1

  EXPECT_FALSE(s8 == s64); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 != s64); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 < s64); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 <= s64); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 > s64); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 >= s64); //NOLINT, we want comparison op test1

  EXPECT_FALSE(s8 == s128); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 != s128); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 < s128); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 <= s128); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 > s128); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 >= s128); //NOLINT, we want comparison op test1

  EXPECT_FALSE(s8 == s256); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 != s256); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 < s256); //NOLINT, we want comparison op test1
  EXPECT_TRUE(s8 <= s256); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 > s256); //NOLINT, we want comparison op test1
  EXPECT_FALSE(s8 >= s256); //NOLINT, we want comparison op test1
}

template<typename T>
void test_strings()
{
  T test1;

  EXPECT_EQ(test1.cbegin(), test1.cend());
  EXPECT_EQ(test1.begin(), test1.end());
  EXPECT_EQ(test1.begin() - test1.end(), 0ULL);

  // Test constant iterators
  test1 = "0123456";
  EXPECT_STREQ(test1.c_str(), "0123456");
  char8_t target = '0';
  for (auto & i : test1) {
    EXPECT_EQ(i, target);
    target++;
  }

  // Test few random std::algorithm calls

  for (auto & i : {'0', '1', '2', '3', '4', '5', '6'}) {
    EXPECT_TRUE(std::binary_search(test1.cbegin(), test1.cend(), i));
  }

  EXPECT_FALSE(std::binary_search(test1.cbegin(), test1.cend(), 'A'));

  T test2;
  std::copy(test1.cbegin(), test1.cend(), test2.begin());
  EXPECT_TRUE(test1 == test2);

  std::reverse(test1.begin(), test1.end());
  EXPECT_STREQ(test1.c_str(), "6543210");

  std::sort(test1.begin(), test1.end());
  EXPECT_STREQ(test1.c_str(), "0123456");

  std::sort(test1.begin(), test1.end(), std::greater<char8_t>());
  EXPECT_STREQ(test1.c_str(), "6543210");

  test2 = "0123456";
  EXPECT_STREQ(test2.c_str(), "0123456");

  std::swap(test1, test2);
  EXPECT_STREQ(test2.c_str(), "6543210");
  EXPECT_STREQ(test1.c_str(), "0123456");

  /////////////////////////////////////////////////////////////////////////////
  target = '0';
  std::fill(test1.begin(), test1.end(), target);
  for (auto & i : test1) {
    EXPECT_EQ(i, '0');
  }

  EXPECT_STREQ(test1.c_str(), "0000000");
  EXPECT_TRUE(std::all_of(test1.cbegin(), test1.cend(), [](char8_t i) {return i == '0';}));
  EXPECT_FALSE(std::all_of(test1.cbegin(), test1.cend(), [](char8_t i) {return i == '1';}));

  EXPECT_TRUE(std::none_of(test1.cbegin(), test1.cend(), [](char8_t i) {return i == '1';}));
  EXPECT_FALSE(std::none_of(test1.cbegin(), test1.cend(), [](char8_t i) {return i == '0';}));

  EXPECT_FALSE(std::any_of(test1.cbegin(), test1.cend(), [](char8_t i) {return i == '1';}));
  EXPECT_TRUE(std::any_of(test1.cbegin(), test1.cend(), [](char8_t i) {return i == '0';}));
}

TEST(string, iterators) {
  test_strings<apex::string8_t>();
  test_strings<apex::string16_t>();
  test_strings<apex::string32_t>();
  test_strings<apex::string64_t>();
  test_strings<apex::string128_t>();
  test_strings<apex::string256_t>();

  test_strings<apex::string_strict8_t>();
  test_strings<apex::string_strict16_t>();
  test_strings<apex::string_strict32_t>();
  test_strings<apex::string_strict64_t>();
  test_strings<apex::string_strict128_t>();
  test_strings<apex::string_strict256_t>();
}

template<typename T>
void test_string_streaming()
{
  std::stringstream ss;
  T str;

  // Stream out
  ss << str;
  str = "0123456";
  ss << str << std::endl;
  str = "6543210";
  ss << str;

  // Stream in
  ss >> str;
  EXPECT_STREQ(str.c_str(), "0123456");
  ss >> str;
  EXPECT_STREQ(str.c_str(), "6543210");
}

TEST(string, test_string_streaming) {
  test_string_streaming<apex::string8_t>();
  test_string_streaming<apex::string16_t>();
  test_string_streaming<apex::string32_t>();
  test_string_streaming<apex::string64_t>();
  test_string_streaming<apex::string128_t>();
  test_string_streaming<apex::string256_t>();

  test_string_streaming<apex::string_strict8_t>();
  test_string_streaming<apex::string_strict16_t>();
  test_string_streaming<apex::string_strict32_t>();
  test_string_streaming<apex::string_strict64_t>();
  test_string_streaming<apex::string_strict128_t>();
  test_string_streaming<apex::string_strict256_t>();
}

void test_std_string_compatibility(const std::string & param)
{
  EXPECT_STREQ(param.c_str(), "123");
}

template<typename T>
void test_std_string_compatibility_t()
{
  T str = "123";
  test_std_string_compatibility(str);
}

TEST(string, test_std_string_compatibility) {
  test_std_string_compatibility_t<apex::string8_t>();
  test_std_string_compatibility_t<apex::string16_t>();
  test_std_string_compatibility_t<apex::string32_t>();
  test_std_string_compatibility_t<apex::string64_t>();
  test_std_string_compatibility_t<apex::string128_t>();
  test_std_string_compatibility_t<apex::string256_t>();

  test_std_string_compatibility_t<apex::string_strict8_t>();
  test_std_string_compatibility_t<apex::string_strict16_t>();
  test_std_string_compatibility_t<apex::string_strict32_t>();
  test_std_string_compatibility_t<apex::string_strict64_t>();
  test_std_string_compatibility_t<apex::string_strict128_t>();
  test_std_string_compatibility_t<apex::string_strict256_t>();
}

template<typename T1, typename T2>
void test_compare_funcs()
{
  T1 str = "123";
  std::string stl_str = "123";


  EXPECT_NO_THROW(str.compare(0, 0, "1", 0U));
  EXPECT_NO_THROW(str.compare(1, 0, "1", 0U));
  EXPECT_NO_THROW(str.compare(2, 0, "1", 0U));
  EXPECT_NO_THROW(str.compare(3, 0, "1", 0U));

  EXPECT_THROW(str.compare(0, 0, nullptr, 0U), std::invalid_argument);
  EXPECT_THROW(str.compare(4, 0, "1", 0U), std::out_of_range);

  EXPECT_EQ(str.compare(0, 0, "123", 0U), 0);
  EXPECT_EQ(str.compare(3, 0, "123", 0U), 0);
  EXPECT_EQ(str.compare(3, 10, "123", 0U), 0);

  EXPECT_LT(str.compare(3, 10, "123", std::string::npos), 0);
  EXPECT_GT(str.compare(2, 10, "123", std::string::npos), 0);
  EXPECT_EQ(str.compare(2, 10, "123", 0), stl_str.compare(2, 10, "123", 0));
  EXPECT_EQ(str.compare(0, 1, "1", 1), stl_str.compare(0, 1, "1", 1));
  EXPECT_EQ(str.compare(0, 10, "123", 10) < 0, stl_str.compare(0, 10, "123", 10) < 0);
  EXPECT_EQ(str.compare(0, 10, "0123", 10) > 0, stl_str.compare(0, 10, "0123", 10) > 0);

  EXPECT_LT(str.compare(3, apex::string8_t::npos, "123", std::string::npos), 0);
  EXPECT_GT(str.compare(2, apex::string8_t::npos, "123", std::string::npos), 0);
  EXPECT_EQ(str.compare(2, apex::string8_t::npos, "123", 0), stl_str.compare(2, 10, "123", 0));
  EXPECT_GT(str.compare(0, apex::string8_t::npos, "1", 1), 0);
  EXPECT_EQ(
    str.compare(0, apex::string8_t::npos, "123", 10) > 0, stl_str.compare(
      0, 10, "123",
      10) > 0);
  EXPECT_EQ(
    str.compare(0, apex::string8_t::npos, "0123", 10) > 0, stl_str.compare(
      0, 10, "0123",
      10) > 0);

  EXPECT_EQ(str.compare("123"), 0);
  EXPECT_GT(str.compare("1"), 0);
  EXPECT_LT(str.compare("1234"), 0);

  /////////////////////////////////////////////////////////////////////////////
  // String vs string comparison
  T2 s1 = "1";
  T2 s123 = "123";
  T2 s1234 = "1234";

  EXPECT_NO_THROW(str.compare(0, 0, s1));
  EXPECT_NO_THROW(str.compare(1, 0, s1));
  EXPECT_NO_THROW(str.compare(2, 0, s1));
  EXPECT_NO_THROW(str.compare(3, 0, s1));

  EXPECT_THROW(str.compare(4, 0, s1), std::out_of_range);

  EXPECT_LT(str.compare(0, 0, s123), 0);
  EXPECT_LT(str.compare(3, 0, s123), 0);
  EXPECT_LT(str.compare(3, 10, s123), 0);

  EXPECT_LT(str.compare(3, 10, s123), 0);
  EXPECT_GT(str.compare(2, 10, s123), 0);
  EXPECT_EQ(str.compare(2, 10, s123), stl_str.compare(2, 10, "123"));
  EXPECT_EQ(str.compare(0, 1, s1), 0);
  EXPECT_EQ(str.compare(0, 10, s123), stl_str.compare(0, 10, "123"));
  EXPECT_EQ(str.compare(0, 10, T2("0123")) > 0, stl_str.compare(0, 10, "0123") > 0);

  EXPECT_LT(str.compare(3, apex::string8_t::npos, s123), 0);
  EXPECT_GT(str.compare(2, apex::string8_t::npos, s123), 0);
  EXPECT_EQ(str.compare(2, apex::string8_t::npos, s123) > 0, stl_str.compare(2, 10, "123", 0) > 0);
  EXPECT_GT(str.compare(0, apex::string8_t::npos, s1), 0);
  EXPECT_EQ(str.compare(0, apex::string8_t::npos, s123), stl_str.compare(0, 10, "123"));
  EXPECT_EQ(
    str.compare(0, apex::string8_t::npos, T2("0123")) > 0, stl_str.compare(
      0, 10,
      "0123") > 0);

  EXPECT_EQ(str.compare(s123), 0);
  EXPECT_GT(str.compare(s1), 0);
  EXPECT_LT(str.compare(s1234), 0);


  std::string ss1 = s1;
  std::string ss123 = s123;
  std::string ss1234 = s1234;

  EXPECT_EQ(str.compare(ss123), 0);
  EXPECT_GT(str.compare(ss1), 0);
  EXPECT_LT(str.compare(ss1234), 0);

  EXPECT_TRUE(str == ss123);  //NOLINT, we want comparison op test
  EXPECT_TRUE(str != ss1);  //NOLINT, we want comparison op test

  EXPECT_TRUE(str > ss1);  //NOLINT, we want comparison op test
  EXPECT_TRUE(str >= ss1);  //NOLINT, we want comparison op test
  EXPECT_TRUE(str >= ss123);  //NOLINT, we want comparison op test

  EXPECT_TRUE(str < ss1234);  //NOLINT, we want comparison op test
  EXPECT_TRUE(str <= ss1234);  //NOLINT, we want comparison op test
}

TEST(string, test_compare_funcs)
{
  test_compare_funcs<apex::string8_t, apex::string16_t>();
  test_compare_funcs<apex::string16_t, apex::string32_t>();
  test_compare_funcs<apex::string32_t, apex::string64_t>();
  test_compare_funcs<apex::string64_t, apex::string128_t>();
  test_compare_funcs<apex::string128_t, apex::string256_t>();
  test_compare_funcs<apex::string256_t, apex::string8_t>();

  test_compare_funcs<apex::string_strict8_t, apex::string16_t>();
  test_compare_funcs<apex::string_strict16_t, apex::string32_t>();
  test_compare_funcs<apex::string_strict32_t, apex::string64_t>();
  test_compare_funcs<apex::string_strict64_t, apex::string128_t>();
  test_compare_funcs<apex::string_strict128_t, apex::string256_t>();
  test_compare_funcs<apex::string_strict256_t, apex::string8_t>();

  test_compare_funcs<apex::string_strict8_t, apex::string_strict16_t>();
  test_compare_funcs<apex::string_strict16_t, apex::string_strict32_t>();
  test_compare_funcs<apex::string_strict32_t, apex::string_strict64_t>();
  test_compare_funcs<apex::string_strict64_t, apex::string_strict128_t>();
  test_compare_funcs<apex::string_strict128_t, apex::string_strict256_t>();
  test_compare_funcs<apex::string_strict256_t, apex::string_strict8_t>();

  test_compare_funcs<apex::string8_t, apex::string_strict16_t>();
  test_compare_funcs<apex::string16_t, apex::string_strict32_t>();
  test_compare_funcs<apex::string32_t, apex::string_strict64_t>();
  test_compare_funcs<apex::string64_t, apex::string_strict128_t>();
  test_compare_funcs<apex::string128_t, apex::string_strict256_t>();
  test_compare_funcs<apex::string256_t, apex::string_strict8_t>();

  apex::string8_t foofoo = "foofoo";
  apex::string8_t foo = "foo";
  EXPECT_EQ(foofoo.compare(0U, 3, foo), 0);
  EXPECT_EQ(foofoo.compare(0U, 3, "foo"), 0);
}

template<class T>
void string_test_substr()
{
  T str = "123";
  EXPECT_STREQ(str.substr(0U).c_str(), "123");
  EXPECT_STREQ(str.substr(1U).c_str(), "23");
  EXPECT_STREQ(str.substr(2U).c_str(), "3");
  EXPECT_STREQ(str.substr(str.size()).c_str(), "");
  EXPECT_STREQ(str.substr(str.size(), 1U).c_str(), "");
  EXPECT_STREQ(str.substr(str.size(), T::npos).c_str(), "");


  EXPECT_THROW(str.substr(str.size() + 1U), std::out_of_range);
  EXPECT_THROW(str.substr(str.size() + 1U, 1U), std::out_of_range);
  EXPECT_THROW(str.substr(str.size() + 1U, T::npos), std::out_of_range);

  EXPECT_THROW(str.substr(ULLONG_MAX), std::out_of_range);
  EXPECT_THROW(str.substr(ULLONG_MAX, 1U), std::out_of_range);
  EXPECT_THROW(str.substr(ULLONG_MAX, T::npos), std::out_of_range);


  EXPECT_STREQ(str.substr(0U, 0U).c_str(), "");
  EXPECT_STREQ(str.substr(0U, 1U).c_str(), "1");
  EXPECT_STREQ(str.substr(0U, 2U).c_str(), "12");
  EXPECT_STREQ(str.substr(0U, 3U).c_str(), "123");
  EXPECT_STREQ(str.substr(0U, 4U).c_str(), "123");
  EXPECT_STREQ(str.substr(0U, T::npos).c_str(), "123");

  EXPECT_STREQ(str.substr(1U, 0U).c_str(), "");
  EXPECT_STREQ(str.substr(1U, 1U).c_str(), "2");
  EXPECT_STREQ(str.substr(1U, 2U).c_str(), "23");
  EXPECT_STREQ(str.substr(1U, 3U).c_str(), "23");
  EXPECT_STREQ(str.substr(1U, 4U).c_str(), "23");
  EXPECT_STREQ(str.substr(1U, T::npos).c_str(), "23");

  EXPECT_STREQ(str.substr(2U, 0U).c_str(), "");
  EXPECT_STREQ(str.substr(2U, 1U).c_str(), "3");
  EXPECT_STREQ(str.substr(2U, 2U).c_str(), "3");
  EXPECT_STREQ(str.substr(2U, 3U).c_str(), "3");
  EXPECT_STREQ(str.substr(2U, 4U).c_str(), "3");
  EXPECT_STREQ(str.substr(2U, T::npos).c_str(), "3");

  EXPECT_STREQ(str.substr(3U, 0U).c_str(), "");
  EXPECT_STREQ(str.substr(3U, 1U).c_str(), "");
  EXPECT_STREQ(str.substr(3U, 2U).c_str(), "");
  EXPECT_STREQ(str.substr(3U, 3U).c_str(), "");
  EXPECT_STREQ(str.substr(3U, 4U).c_str(), "");
  EXPECT_STREQ(str.substr(3U, T::npos).c_str(), "");

  EXPECT_THROW(str.substr(4U, 0U).c_str(), std::out_of_range);
  EXPECT_THROW(str.substr(4U, 1U).c_str(), std::out_of_range);
  EXPECT_THROW(str.substr(4U, 2U).c_str(), std::out_of_range);
  EXPECT_THROW(str.substr(4U, 3U).c_str(), std::out_of_range);
  EXPECT_THROW(str.substr(4U, 4U).c_str(), std::out_of_range);
}

TEST(string, test_substr)
{
  string_test_substr<apex::string8_t>();
  string_test_substr<apex::string16_t>();
  string_test_substr<apex::string32_t>();
  string_test_substr<apex::string64_t>();
  string_test_substr<apex::string128_t>();
  string_test_substr<apex::string256_t>();

  string_test_substr<apex::string_strict8_t>();
  string_test_substr<apex::string_strict16_t>();
  string_test_substr<apex::string_strict32_t>();
  string_test_substr<apex::string_strict64_t>();
  string_test_substr<apex::string_strict128_t>();
  string_test_substr<apex::string_strict256_t>();
}

template<class T>
void string_test_fill_and_increments(bool is_strict = false)
{
  T test(T::npos, '*');
  EXPECT_EQ(T::get_buffer_size_static(), test.get_buffer_size());
  EXPECT_EQ(test.c_str()[T::get_buffer_size_static() - 1U], '\0');

  T test2(0U, '*');
  EXPECT_EQ(test2.c_str()[0U], '\0');
  test2 += '*';
  EXPECT_STREQ(test2.c_str(), "*");
  test2 += '*';
  EXPECT_STREQ(test2.c_str(), "**");
  test2 += '*';
  EXPECT_STREQ(test2.c_str(), "***");

  for (uint32_t i = 3U; i < test2.capacity(); ++i) {
    test2 += '*';
  }
  EXPECT_STREQ(test.c_str(), test2.c_str());

  if (is_strict) {
    EXPECT_THROW(test2 += '*', std::overflow_error);
    EXPECT_NO_THROW(T test3(T::get_buffer_size_static() - 1U, '*'));
    EXPECT_THROW(T test4(T::get_buffer_size_static(), '*'), std::overflow_error);
    EXPECT_THROW(T test5(T::get_buffer_size_static() + 1U, '*'), std::overflow_error);
  } else {
    EXPECT_NO_THROW(test2 += '*');
  }
  EXPECT_EQ(test2.c_str()[test2.get_buffer_size() - 2U], '*');
  EXPECT_EQ(test2.c_str()[test2.get_buffer_size() - 1U], '\0');

  test2.clear();
  EXPECT_STREQ(test2.c_str(), "");
  EXPECT_EQ(test2.size(), 0);
  test2 = "hello" + T("__") + "world";
  EXPECT_STREQ(test2.c_str(), "hello__world");
}

TEST(string, test_fill_constructors)
{
  string_test_fill_and_increments<apex::string_strict16_t>(true);
  string_test_fill_and_increments<apex::string_strict32_t>(true);
  string_test_fill_and_increments<apex::string_strict64_t>(true);
  string_test_fill_and_increments<apex::string_strict128_t>(true);
  string_test_fill_and_increments<apex::string_strict256_t>(true);

  string_test_fill_and_increments<apex::string16_t>();
  string_test_fill_and_increments<apex::string32_t>();
  string_test_fill_and_increments<apex::string64_t>();
  string_test_fill_and_increments<apex::string128_t>();
  string_test_fill_and_increments<apex::string256_t>();
}

namespace
{
template<class T>
void test_hash()
{
  std::unordered_set<T> set;
  std::unordered_set<std::string> ref_set;

  static auto get_char = []
    {
      static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
      static const auto max_index = (sizeof(charset) - 1);
      return charset[rand() % max_index];
    };

  for (auto i = 0; i < 1000; ++i) {
    T str;
    for (size64_t j = 1; j < str.capacity(); ++j) {
      std::generate_n(str.begin(), j, get_char);
      ASSERT_EQ(ref_set.emplace(str.c_str()).second, set.insert(str).second);
    }
  }

  ASSERT_EQ(set.size(), ref_set.size());
}
}  // namespace

TEST(string, hash)
{
  test_hash<apex::string8_t>();
  test_hash<apex::string16_t>();
  test_hash<apex::string32_t>();
  test_hash<apex::string64_t>();
  test_hash<apex::string128_t>();
  test_hash<apex::string256_t>();
  test_hash<apex::string_strict8_t>();
  test_hash<apex::string_strict16_t>();
  test_hash<apex::string_strict32_t>();
  test_hash<apex::string_strict64_t>();
  test_hash<apex::string_strict128_t>();
  test_hash<apex::string_strict256_t>();
}
