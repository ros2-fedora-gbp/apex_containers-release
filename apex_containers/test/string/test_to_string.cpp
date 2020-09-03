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
#include <apexutils/apexdef.h>
#include <functional>
#include <utility>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <limits>
#include "string/string.hpp"

TEST(string, string_to_string) {
  EXPECT_STREQ(apex::string16_t::to_string(0U).c_str(), "0");
  EXPECT_STREQ(apex::string16_t::to_string(1U).c_str(), "1");
  EXPECT_STREQ(apex::string16_t::to_string(12U).c_str(), "12");
  EXPECT_STREQ(apex::string16_t::to_string(123U).c_str(), "123");
  EXPECT_STREQ(apex::string16_t::to_string(1234U).c_str(), "1234");
  EXPECT_STREQ(apex::string16_t::to_string(12345U).c_str(), "12345");
  EXPECT_STREQ(apex::string16_t::to_string(123456U).c_str(), "123456");
  EXPECT_STREQ(apex::string16_t::to_string(1234567U).c_str(), "1234567");
  EXPECT_STREQ(apex::string16_t::to_string(12345678U).c_str(), "12345678");
  EXPECT_STREQ(apex::string16_t::to_string(123456789U).c_str(), "123456789");
  EXPECT_STREQ(apex::string16_t::to_string(1234567890U).c_str(), "1234567890");
  EXPECT_STREQ(apex::string16_t::to_string(uint32_t(INT32_MAX)).c_str(), "2147483647");
  EXPECT_STREQ(apex::string16_t::to_string(UINT32_MAX).c_str(), "4294967295");

  EXPECT_THROW(apex::String<1>::to_string(0U).c_str(), std::overflow_error);

  EXPECT_STREQ(apex::String<2>::to_string(0U).c_str(), "0");

  EXPECT_STREQ(apex::String<2>::to_string(1U).c_str(), "1");
  EXPECT_THROW(apex::String<2>::to_string(12U).c_str(), std::overflow_error);
  EXPECT_THROW(apex::String<2>::to_string(123U).c_str(), std::overflow_error);

  EXPECT_STREQ(apex::string8_t::to_string(1234567U).c_str(), "1234567");
  EXPECT_THROW(apex::string8_t::to_string(12345670U).c_str(), std::overflow_error);
  EXPECT_THROW(apex::string8_t::to_string(123456701U).c_str(), std::overflow_error);
  EXPECT_THROW(apex::string8_t::to_string(1234567012U).c_str(), std::overflow_error);
}

template<class T>
void uint_to_string_and_back_templated()
{
#define DO_TEST(x) EXPECT_EQ(apex::stoul(T(apex::to_string(x))), x)
  DO_TEST(0U);
  DO_TEST(1U);
  DO_TEST(12U);
  DO_TEST(123U);
  DO_TEST(1234U);
  DO_TEST(12345U);
  DO_TEST(123456U);
  DO_TEST(1234567U);
  DO_TEST(12345678U);
  DO_TEST(123456789U);
  DO_TEST(1234567890U);
  DO_TEST(INT32_MAX);
#undef DO_TEST
  EXPECT_EQ(apex::stoul(T(apex::to_string(-1))), 0xFFFFFFFF);
  EXPECT_EQ(apex::stoul(T("4294967298")), UINT32_MAX);
}

TEST(string, uint_to_string_and_back) {
  uint_to_string_and_back_templated<apex::string16_t>();
  uint_to_string_and_back_templated<apex::string32_t>();
  uint_to_string_and_back_templated<apex::string64_t>();
  uint_to_string_and_back_templated<apex::string128_t>();
  uint_to_string_and_back_templated<apex::string256_t>();

  uint_to_string_and_back_templated<apex::string_strict16_t>();
  uint_to_string_and_back_templated<apex::string_strict32_t>();
  uint_to_string_and_back_templated<apex::string_strict64_t>();
  uint_to_string_and_back_templated<apex::string_strict128_t>();
  uint_to_string_and_back_templated<apex::string_strict256_t>();
}

template<class T>
static void int_to_string_and_back_templated()
{
#define DO_TEST(x) EXPECT_EQ(apex::stol(T(apex::to_string(x))), x)
  DO_TEST(0);
  DO_TEST(1);
  DO_TEST(12);
  DO_TEST(123);
  DO_TEST(1234);
  DO_TEST(12345);
  DO_TEST(123456);
  DO_TEST(1234567);
  DO_TEST(12345678);
  DO_TEST(123456789);
  DO_TEST(1234567890);
  DO_TEST(INT32_MAX);
  DO_TEST(-1);
  DO_TEST(-12);
  DO_TEST(-123);
  DO_TEST(-1234);
  DO_TEST(-12345);
  DO_TEST(-123456);
  DO_TEST(-1234567);
  DO_TEST(-12345678);
  DO_TEST(-123456789);
  DO_TEST(-1234567890);
  DO_TEST(INT32_MIN);
#undef DO_TEST
  EXPECT_EQ(apex::stol(T("4294967298")), INT32_MAX);
  EXPECT_EQ(apex::stol(T("-2147483649")), INT32_MIN);
}

TEST(string, int_to_string_and_back) {
  int_to_string_and_back_templated<apex::string16_t>();
  int_to_string_and_back_templated<apex::string32_t>();
  int_to_string_and_back_templated<apex::string64_t>();
  int_to_string_and_back_templated<apex::string128_t>();
  int_to_string_and_back_templated<apex::string256_t>();

  int_to_string_and_back_templated<apex::string_strict16_t>();
  int_to_string_and_back_templated<apex::string_strict32_t>();
  int_to_string_and_back_templated<apex::string_strict64_t>();
  int_to_string_and_back_templated<apex::string_strict128_t>();
  int_to_string_and_back_templated<apex::string_strict256_t>();
}

template<class T>
void float_to_string_and_back_templated()
{
#ifndef APEX_OSX
  // OS X's POSIX gcvt call uses dynamic memory calls
#endif  // APEX_OSX

#define DO_TEST(x) EXPECT_FLOAT_EQ(apex::stof(T(apex::to_string(x))), x)
  DO_TEST(0.0F);
  DO_TEST(-0.0F);
  DO_TEST(INFINITY);
  DO_TEST(-INFINITY);
  DO_TEST(9.123456F);
  DO_TEST(-9.123456F);
  DO_TEST(9.123456E+37F);
  DO_TEST(-9.123456E+37F);
  DO_TEST(9.123456E-37F);
  DO_TEST(-9.123456E-37F);
  DO_TEST(0.1F);
  DO_TEST(0.02F);
  DO_TEST(0.003F);
  DO_TEST(9.0E-9F);
  DO_TEST(FLT_MAX);
  DO_TEST(-FLT_MAX);
  DO_TEST(FLT_MIN);
  DO_TEST(-FLT_MIN);
#undef DO_TEST

  EXPECT_TRUE(std::isnan(apex::stof(T(apex::to_string(std::nanf("0x7FC00000"))))));

#ifndef APEX_OSX
  // OS X's POSIX gcvt call uses dynamic memory calls
#endif  // APEX_OSX
}

TEST(string, float_to_string_and_back) {
  float_to_string_and_back_templated<apex::string16_t>();
  float_to_string_and_back_templated<apex::string32_t>();
  float_to_string_and_back_templated<apex::string64_t>();
  float_to_string_and_back_templated<apex::string128_t>();
  float_to_string_and_back_templated<apex::string256_t>();

  float_to_string_and_back_templated<apex::string_strict16_t>();
  float_to_string_and_back_templated<apex::string_strict32_t>();
  float_to_string_and_back_templated<apex::string_strict64_t>();
  float_to_string_and_back_templated<apex::string_strict128_t>();
  float_to_string_and_back_templated<apex::string_strict256_t>();
}

TEST(string, variadic_template_to_string) {
  // Test a list of various parameters of all possible types
  apex::string256_t st = apex::varargs_to_string(
    "Parameters:", 1, 2U, 3.0F, "four", 5ULL, 6LL, 7.0,
    false);
  EXPECT_STREQ(st.c_str(), "Parameters: 1 2 3.0e+0 four 5 6 7.0e+0 false");

  // with characters > 255
  EXPECT_STREQ(
    apex::varargs_to_string(
      "Variadic parameters: " \
      "int32_t min:", -2147483648, "int32_t max:", 2147483647, "uint32_t max:", 4294967295, \
      "float max:", std::numeric_limits<float32_t>::max(), \
      "double max:", std::numeric_limits<float64_t>::max(), \
      "int64_t max:", std::numeric_limits<int64_t>::max(), \
      "uint64_t max:", std::numeric_limits<uint64_t>::max(), \
      "string: useless stray test string out of bounds").c_str(),
    "Variadic parameters: " \
    "int32_t min: -2147483648 int32_t max: 2147483647 uint32_t max: 4294967295 " \
    "float max: 3.4028235e+38 " \
    "double max: 1.797693134862316e+308 " \
    "int64_t max: 9223372036854775807 " \
    "uint64_t max: 18446744073709551615 string: useless stray test strin");

  // Test empty variadic parameters list
  EXPECT_STREQ(apex::varargs_to_string().c_str(), "");
}

// The following test takes about 30 minutes on my 12-core PC.
// I disable it until we have provisions for super-exhaustive and super-long tests
// #define LONG_RUN_IS_OK
# ifdef LONG_RUN_IS_OK

typedef union {
  uint32_t uval;
  int32_t ival;
  float32_t fval;
} varval_t;

TEST(string, exhaustive_to_string)
{
  volatile uint64_t counter = 0ULL;

  #pragma omp parallel for
  for (int64_t i = INT32_MIN; i <= INT32_MAX; ++i) {
    varval_t varval;
    varval.ival = static_cast<int32_t>(i);
    apex::string16_t uval_str = apex::to_string(varval.uval);
    apex::string16_t ival_str = apex::to_string(varval.ival);
    apex::string16_t fval_str = apex::to_string(varval.fval);
    uint32_t uval = apex::stoul(uval_str);
    int32_t ival = apex::stol(ival_str);
    float32_t fval = apex::stof(fval_str);
    EXPECT_EQ(uval, varval.uval);
    EXPECT_EQ(ival, varval.ival);

    if (std::isnan(varval.fval)) {
      EXPECT_TRUE(std::isnan(fval));
    } else if (std::isinf(varval.fval)) {
      EXPECT_TRUE(std::isinf(fval));
    } else if (std::abs(varval.fval) > FLT_TRUE_MIN) {
      float32_t delta = std::abs((varval.fval / fval) - 1.0F);
      EXPECT_TRUE(delta < 1.0e-6F);
      if (delta >= 1.0e-6F) {
        printf("************** %g\n", delta);
      }
    } else {
      // varval.fval <= FLT_TRUE_MIN
      EXPECT_FLOAT_EQ(fval, varval.fval);
    }

    uint64_t lc = apex::atomics::inc(counter);
    if ((lc % 10000000ULL) == 0ULL) {
      printf("lc is %llu\n", lc);
    }
  }
}
#endif  // LONG_RUN_IS_OK

template<typename T>
void test_integer_templated()
{
  T test_val;

  test_val = std::numeric_limits<T>::min();
  EXPECT_STREQ(apex::to_string(test_val).c_str(), std::to_string(test_val).c_str());

  test_val = 0;
  EXPECT_STREQ(apex::to_string(test_val).c_str(), std::to_string(test_val).c_str());

  test_val = std::numeric_limits<T>::max();
  EXPECT_STREQ(apex::to_string(test_val).c_str(), std::to_string(test_val).c_str());
}

TEST(string, test_to_string) {
  // Test all Apex MISRA-compatible integer types
  test_integer_templated<uint8_t>();
  test_integer_templated<uint16_t>();
  test_integer_templated<uint32_t>();
  test_integer_templated<uint64_t>();

  test_integer_templated<int8_t>();
  test_integer_templated<int16_t>();
  test_integer_templated<int32_t>();
  test_integer_templated<int64_t>();

  // test all C++ MISRA-incompatibleinteger types (they don't have size in their type names)
  // from https://en.cppreference.com/w/cpp/language/types
  test_integer_templated<short>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<short int>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<signed short>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<signed short int>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<unsigned short>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<unsigned short int>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<unsigned short int>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<int>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<signed>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<signed int>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<unsigned>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<unsigned int>();  // NOLINT: use intXX, rather than the C type int Y
#ifndef APEX_OSX
  // Apparently Apple compiler has a bug and can't find a templated call for the following types
  test_integer_templated<long>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<long int>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<signed long>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<signed long int>();  // NOLINT: use intXX, rather than the C type int Y
#endif  // APEX_OSX
#if !defined(APEX_WINDOWS) && !defined(APEX_OSX)
  // Apparently both MS and OSX compilers have a bug
  // and can't find a templated call for the following types
  test_integer_templated<unsigned long>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<unsigned long int>();  // NOLINT: use intXX, rather than the C type int Y
#endif  // APEX_WINDOWS
#if !defined(APEX_LINUX) && !defined(APEX_QNX)
  // Apparently G++ compiler has a bug and can't find a templated call for the following types
  test_integer_templated<long long>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<long long int>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<long long int>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<signed long long>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<signed long long int>();  // NOLINT: use intXX, rather than the C type
  test_integer_templated<unsigned long long>();  // NOLINT: use intXX, rather than the C type int Y
  test_integer_templated<unsigned long long int>();  // NOLINT: use intXX, rather than the C type
#endif  // APEX_LINUX
}

TEST(string, test_float_to_string) {
  float32_t test_val;
  apex::string16_t str_val;

  EXPECT_STREQ(apex::to_string(0.1F).c_str(), "1.0e-1");
  EXPECT_STREQ(apex::to_string(0.123F).c_str(), "1.23e-1");
  EXPECT_STREQ(apex::to_string(99.0F).c_str(), "9.9e+1");
  EXPECT_STREQ(apex::to_string(std::numeric_limits<float32_t>::infinity()).c_str(), "inf");
  EXPECT_STREQ(apex::to_string(-std::numeric_limits<float32_t>::infinity()).c_str(), "-inf");
  EXPECT_STREQ(apex::to_string(-std::numeric_limits<float32_t>::quiet_NaN()).c_str(), "nan");

  test_val = 1.0F;
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "1.0e+0");
  EXPECT_FLOAT_EQ(test_val, apex::stof(str_val));

  test_val = 101.0F;
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "1.01e+2");
  EXPECT_DOUBLE_EQ(test_val, apex::stof(str_val));

  test_val = std::numeric_limits<float32_t>::min();
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "1.1754944e-38");
  EXPECT_FLOAT_EQ(test_val, apex::stof(str_val));

  test_val = -std::numeric_limits<float32_t>::min();
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "-1.1754944e-38");
  EXPECT_FLOAT_EQ(test_val, apex::stof(str_val));

  test_val = 0;
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "0");
  EXPECT_FLOAT_EQ(test_val, apex::stof(str_val));

  test_val = -0.0F;
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "0");
  EXPECT_FLOAT_EQ(test_val, apex::stof(str_val));

  test_val = std::numeric_limits<float32_t>::max();
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "3.4028235e+38");
  EXPECT_FLOAT_EQ(test_val, apex::stof(str_val));

  test_val = -std::numeric_limits<float32_t>::max();
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "-3.4028235e+38");
  EXPECT_FLOAT_EQ(test_val, apex::stof(str_val));

  EXPECT_TRUE(std::isnan(apex::stof(apex::to_string(std::nanf("0x7FC00000")))));
}

TEST(string, test_double_to_string) {
  float64_t test_val;
  apex::string32_t str_val;

  EXPECT_STREQ(apex::to_string(0.1).c_str(), "1.0e-1");
  EXPECT_STREQ(apex::to_string(0.123).c_str(), "1.23e-1");
  EXPECT_STREQ(apex::to_string(99999.0).c_str(), "9.9999e+4");
  EXPECT_STREQ(apex::to_string(std::numeric_limits<double>::infinity()).c_str(), "inf");
  EXPECT_STREQ(apex::to_string(-std::numeric_limits<double>::infinity()).c_str(), "-inf");
  EXPECT_STREQ(apex::to_string(-std::numeric_limits<double>::quiet_NaN()).c_str(), "nan");

  test_val = 1.0;
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "1.0e+0");
  EXPECT_DOUBLE_EQ(test_val, apex::stod(str_val));

  test_val = 101.0;
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "1.01e+2");
  EXPECT_DOUBLE_EQ(test_val, apex::stod(str_val));

  test_val = std::numeric_limits<double>::min();
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "2.225073858507201e-308");
  EXPECT_DOUBLE_EQ(test_val, apex::stod(str_val));

  test_val = -std::numeric_limits<double>::min();
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "-2.225073858507201e-308");
  EXPECT_DOUBLE_EQ(test_val, apex::stod(str_val));

  test_val = 0;
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "0");
  EXPECT_DOUBLE_EQ(test_val, apex::stod(str_val));

  test_val = -0.0;
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "0");
  EXPECT_DOUBLE_EQ(test_val, apex::stod(str_val));

  test_val = std::numeric_limits<double>::max();
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "1.797693134862316e+308");
  EXPECT_DOUBLE_EQ(test_val, apex::stod(str_val));

  test_val = -std::numeric_limits<double>::max();
  str_val = apex::to_string(test_val);
  EXPECT_STREQ(str_val.c_str(), "-1.797693134862316e+308");
  EXPECT_DOUBLE_EQ(test_val, apex::stod(str_val));

  EXPECT_TRUE(std::isnan(apex::stod(apex::to_string(std::nanf("0x7FC0000000000000")))));
}

TEST(string, test_boolean_to_string) {
  EXPECT_STREQ(apex::to_string(true).c_str(), "true");
  EXPECT_STREQ(apex::to_string(false).c_str(), "false");
}

TEST(string, test_cstring_to_string) {
  std::string str512(512, '*');
  EXPECT_EQ(str512.length(), 512U);
  std::string str255(255, '*');
  EXPECT_EQ(str255.length(), 255U);

  // Check trimming to max size
  EXPECT_STREQ(apex::to_string(str512.c_str()).c_str(), str255.c_str());
  EXPECT_STREQ(apex::to_string("abc").c_str(), "abc");
  EXPECT_STREQ(apex::to_string("").c_str(), "");
  EXPECT_STREQ(apex::to_string(nullptr).c_str(), "<nullptr>");
}

TEST(string, to_string_from_string) {
  const apex::string8_t not_a_number("????");
  const apex::string32_t ten_power_20("100000000000000000000");
  const apex::string32_t minus_ten_power_20("-100000000000000000000");
  const apex::string_strict8_t not_a_number_strict("????");

  EXPECT_EQ(apex::stoul(not_a_number), 0U);
  EXPECT_EQ(apex::stoul(not_a_number_strict), 0U);
  EXPECT_EQ(apex::stoul(apex::to_string(0U)), 0U);
  EXPECT_EQ(apex::stoul(apex::to_string(UINT32_MAX)), UINT32_MAX);
  EXPECT_EQ(apex::stoul(apex::to_string(UINT64_MAX)), UINT32_MAX);
  EXPECT_EQ(apex::stoul(apex::to_string("4294967296")), UINT32_MAX);  // UINT32_MAX + 1
  EXPECT_EQ(apex::stoul(minus_ten_power_20), UINT32_MAX);
  EXPECT_EQ(apex::stoul(ten_power_20), UINT32_MAX);

  EXPECT_EQ(apex::stol(not_a_number), 0);
  EXPECT_EQ(apex::stol(not_a_number_strict), 0);
  EXPECT_EQ(apex::stol(apex::to_string(0)), 0);
  EXPECT_EQ(apex::stol(apex::to_string(INT32_MAX)), INT32_MAX);
  EXPECT_EQ(apex::stol(apex::to_string(INT32_MIN)), INT32_MIN);
  EXPECT_EQ(apex::stol(apex::to_string(INT64_MAX)), INT32_MAX);
  EXPECT_EQ(apex::stol(apex::to_string(INT64_MIN)), INT32_MIN);
  EXPECT_EQ(apex::stol(apex::to_string("-2147483649")), INT32_MIN);  // INT32_MIN - 1
  EXPECT_EQ(apex::stol(apex::to_string("2147483648")), INT32_MAX);  // INT32_MAX + 1
  EXPECT_EQ(apex::stol(minus_ten_power_20), INT32_MIN);
  EXPECT_EQ(apex::stol(ten_power_20), INT32_MAX);

  EXPECT_EQ(apex::stoull(not_a_number), 0ULL);
  EXPECT_EQ(apex::stoull(not_a_number_strict), 0ULL);
  EXPECT_EQ(apex::stoull(apex::to_string(0U)), 0U);
  EXPECT_EQ(apex::stoull(apex::to_string(0ULL)), 0ULL);
  EXPECT_EQ(apex::stoull(apex::to_string(UINT32_MAX)), UINT32_MAX);
  EXPECT_EQ(apex::stoull(apex::to_string(UINT64_MAX)), UINT64_MAX);
  EXPECT_EQ(apex::stoull(apex::string64_t("18446744073709551616")), UINT64_MAX);  // UINT64_MAX + 1
  EXPECT_EQ(apex::stoull(minus_ten_power_20), UINT64_MAX);
  EXPECT_EQ(apex::stoull(ten_power_20), UINT64_MAX);

  EXPECT_EQ(apex::stoll(not_a_number), 0LL);
  EXPECT_EQ(apex::stoll(not_a_number_strict), 0LL);
  EXPECT_EQ(apex::stoll(apex::to_string(0)), 0LL);
  EXPECT_EQ(apex::stoll(apex::to_string(0LL)), 0LL);
  EXPECT_EQ(apex::stoll(apex::to_string(INT32_MAX)), INT32_MAX);
  EXPECT_EQ(apex::stoll(apex::to_string(INT32_MIN)), INT32_MIN);
  EXPECT_EQ(apex::stoll(apex::to_string(INT64_MAX)), INT64_MAX);
  EXPECT_EQ(apex::stoll(apex::to_string(INT64_MIN)), INT64_MIN);
  EXPECT_EQ(apex::stoll(apex::string64_t("-9223372036854775809")), INT64_MIN);  // INT64_MIN - 1
  EXPECT_EQ(apex::stoll(apex::string64_t("9223372036854775808")), INT64_MAX);
  EXPECT_EQ(apex::stoll(minus_ten_power_20), INT64_MIN);
  EXPECT_EQ(apex::stoll(ten_power_20), INT64_MAX);
}

TEST(string, test_std_string_to_string) {
  std::string str512(512, '*');
  std::string str255(255, '*');
  apex::string128_t a_str127(127, '*');
  apex::string256_t a_str255(255, '*');

  EXPECT_STREQ(apex::to_string(str512).c_str(), str255.c_str());
  EXPECT_STREQ(apex::to_string(a_str127).c_str(), a_str127.c_str());
  EXPECT_STREQ(apex::to_string(a_str255).c_str(), a_str255.c_str());

  std::string hello = "Hello";
  apex::string128_t world = "world";
  apex::string8_t exclm = "!";
  apex::string256_t lucky = "My lucky number is";
  std::int32_t num = 42;

  ASSERT_STREQ(
    apex::varargs_to_string(hello, world, exclm, lucky, num).c_str(),
    "Hello world ! My lucky number is 42"
  );
}
