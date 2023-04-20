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

#include <gtest/gtest.h>
#include <functional>
#include <utility>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <limits>
#include "string/string.hpp"


template<class T>
void check_const_out_idx(const T & str, size64_t idx)
{
  EXPECT_EQ(str[str.length()], '\0');
  EXPECT_THROW(str[idx], std::out_of_range);
}


TEST(string_strict, bad_cases) {
  apex_string_t astr256 = apex_string_create();
  memset(astr256.c_str, 0xFFU, APEX_STRING_SIZE);
  EXPECT_THROW(apex::StringStrict<256U> str256U(astr256), std::overflow_error);
  EXPECT_THROW(
    apex::StringStrict<256U> str2(static_cast<char8_t *>(nullptr)), std::invalid_argument);

  apex::StringStrict<4U> str4("123");
  EXPECT_THROW(apex::StringStrict<3U> str3U(str4), std::overflow_error);
  EXPECT_THROW(str4[4U], std::out_of_range);
  check_const_out_idx(str4, 4U);
  str4[str4.length()] = '\0';

  std::string big_str(MAX_APEX_STRING_LEN + 1U, '*');
  EXPECT_THROW(apex::StringStrict<256U> str16U(big_str.c_str()), std::overflow_error);

  apex::StringStrict<16U> str;
  EXPECT_THROW(str = big_str.c_str(), std::overflow_error);

  EXPECT_THROW(
    str =
    str + apex::StringStrict<MAX_APEX_STRING_LEN + 2>(big_str.c_str()), std::overflow_error);
  EXPECT_THROW(str += big_str.c_str(), std::overflow_error);
}


TEST(string_strict, good_cases) {
  apex_string_t astr256 = apex_string_create();
  EXPECT_EQ(apex_string_add_strict(&astr256, "256"), APEX_RET_OK);

  // Non-empty constructors
  apex::StringStrict<16U> str1("1");
  EXPECT_STREQ(str1.c_str(), "1");
  EXPECT_EQ(str1.capacity(), 16U - 1U);
  EXPECT_EQ(str1.length(), 1U);

  apex::StringStrict<32U> str2(str1);
  EXPECT_STREQ(str2.c_str(), "1");
  EXPECT_EQ(str2.capacity(), 32U - 1U);
  EXPECT_EQ(str2.length(), 1U);
  apex::StringStrict<32U> str22;
  str22 = str2;
  EXPECT_STREQ(str2.c_str(), str22.c_str());

  apex::StringStrict<16U> str3(astr256);
  EXPECT_STREQ(str3.c_str(), "256");
  EXPECT_EQ(str3.capacity(), 16U - 1U);
  EXPECT_EQ(str3.length(), 3U);

  apex::StringStrict<128U> str128;
  EXPECT_STREQ(str128.c_str(), "");
  EXPECT_EQ(str128.capacity(), 128U - 1U);
  EXPECT_EQ(str128.length(), 0U);

  apex::StringStrict<16U> str4(str3);
  EXPECT_STREQ(str4.c_str(), "256");
  EXPECT_EQ(str4.capacity(), 16U - 1U);
  EXPECT_EQ(str4.length(), 3U);

  // Test additions
  str128 = str128 + astr256;
  EXPECT_STREQ(str128.c_str(), "256");
  EXPECT_EQ(str128.length(), 3U);
  str128 = str128 + apex::StringStrict<2U>("2");
  EXPECT_STREQ(str128.c_str(), "2562");
  EXPECT_EQ(str128.length(), 4U);
  str128 = str128 + str128;
  EXPECT_STREQ(str128.c_str(), "25622562");
  EXPECT_EQ(str128.length(), 8U);

  // Test increments
  str128 += astr256;
  EXPECT_STREQ(str128.c_str(), "25622562256");
  str128 += "2";
  EXPECT_STREQ(str128.c_str(), "256225622562");
  str128 += str128;
  EXPECT_STREQ(str128.c_str(), "256225622562256225622562");

  // Assignments
  str128 = str1;
  EXPECT_STREQ(str128.c_str(), "1");
  EXPECT_EQ(str128.length(), 1U);

  str128 = "str1";
  EXPECT_STREQ(str128.c_str(), "str1");
  EXPECT_EQ(str128.length(), 4U);

  str128 = astr256;
  EXPECT_STREQ(str128.c_str(), "256");
  EXPECT_EQ(str128.length(), 3U);

  str1 = str3;
  EXPECT_STREQ(str3.c_str(), "256");
  EXPECT_EQ(str3.length(), 3U);

  EXPECT_EQ(str1.capacity(), 16U - 1U);
  EXPECT_EQ(str2.capacity(), 32U - 1U);
  EXPECT_EQ(str3.capacity(), 16U - 1U);
  EXPECT_EQ(str4.capacity(), 16U - 1U);
  EXPECT_EQ(str128.capacity(), 128U - 1U);

  apex_string_t astr123 = apex_string_create();
  EXPECT_EQ(apex_string_add(&astr123, "123"), 3U);
  str128 = astr123 + str128;
  EXPECT_STREQ(str128.c_str(), "123256");
}

TEST(string_strict, strict_string_to_string) {
  EXPECT_STREQ(apex::string_strict16_t::to_string(0U).c_str(), "0");
  EXPECT_STREQ(apex::string_strict16_t::to_string(1U).c_str(), "1");
  EXPECT_STREQ(apex::string_strict16_t::to_string(12U).c_str(), "12");
  EXPECT_STREQ(apex::string_strict16_t::to_string(123U).c_str(), "123");
  EXPECT_STREQ(apex::string_strict16_t::to_string(1234U).c_str(), "1234");
  EXPECT_STREQ(apex::string_strict16_t::to_string(12345U).c_str(), "12345");
  EXPECT_STREQ(apex::string_strict16_t::to_string(123456U).c_str(), "123456");
  EXPECT_STREQ(apex::string_strict16_t::to_string(1234567U).c_str(), "1234567");
  EXPECT_STREQ(apex::string_strict16_t::to_string(12345678U).c_str(), "12345678");
  EXPECT_STREQ(apex::string_strict16_t::to_string(123456789U).c_str(), "123456789");
  EXPECT_STREQ(apex::string_strict16_t::to_string(1234567890U).c_str(), "1234567890");
  EXPECT_STREQ(apex::string_strict16_t::to_string(uint32_t(INT32_MAX)).c_str(), "2147483647");
  EXPECT_STREQ(apex::string_strict16_t::to_string(UINT32_MAX).c_str(), "4294967295");

  EXPECT_THROW(apex::StringStrict<1>::to_string(0U).c_str(), std::overflow_error);

  EXPECT_STREQ(apex::StringStrict<2>::to_string(0U).c_str(), "0");

  EXPECT_STREQ(apex::StringStrict<2>::to_string(1U).c_str(), "1");
  EXPECT_THROW(apex::StringStrict<2>::to_string(12U).c_str(), std::overflow_error);
  EXPECT_THROW(apex::StringStrict<2>::to_string(123U).c_str(), std::overflow_error);

  EXPECT_STREQ(apex::string_strict8_t::to_string(1234567U).c_str(), "1234567");
  EXPECT_THROW(apex::string_strict8_t::to_string(12345670U).c_str(), std::overflow_error);
  EXPECT_THROW(apex::string_strict8_t::to_string(123456701U).c_str(), std::overflow_error);
  EXPECT_THROW(apex::string_strict8_t::to_string(1234567012U).c_str(), std::overflow_error);
}

apex::string_strict16_t foo_strict(const apex::string_strict8_t & src)
{
  apex::string_strict16_t retval(src);
  return retval;
}

TEST(string_strict, string_typedefs) {
  apex::string_strict8_t s8("123");
  apex::string_strict16_t s16(s8);
  apex::string_strict32_t s32 = "1234";
  apex::string_strict64_t s64 = foo_strict("12");
  apex::string_strict128_t s128;
  apex::string_strict256_t s256("12345678");

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
