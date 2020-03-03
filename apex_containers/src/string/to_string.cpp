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

#include <apexutils/apexdef.h>
#include <string/to_string.hpp>
#include <limits>
#include <cmath>

#ifdef APEX_OSX
namespace std
{
// OS X does not have std::isnan
int isnan(float32_t x) {return x != x;}
// OS X does not have std::isnan
int isnan(float64_t x) {return x != x;}
// OS X does not have std::isinf
int isinf(float32_t x) {return !isnan(x) && isnan(x - x);}
// OS X does not have std::isinf
int isinf(float64_t x) {return !isnan(x) && isnan(x - x);}
}  // namespace std
#endif   // APEX_OSX

namespace
{
const char8_t g_decimal_digits[10U] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

///////////////////////////////////////////////////////////////////////////////
/*
The algo in its unrolled form for uint8_t case:
Given a number X (e.g. 254):

  Divide X by 100 (== 2)
  Mod divide the above by 10 (== 2)
  Get a corresponing character from `g_decimal_digits` (= `2`)
  Add the character to the string (= "2")

  Divide X by 10 (== 25)
  Mod divide the above by 10 (== 5)
  Get a corresponing character from `g_decimal_digits` (= `5`).
  Add the character to the string (= "25")

  Divide X by 1 (== 254)
  Mod divide the above by 10 (== 4)
  Get a corresponing character from `g_decimal_digits` (= `4`).
  Add the character to the string (= "254")

  The `IF` deals with (strips) leading zeroes and zero input value.
*/

template<class STR_TYPE, class VAL_TYPE>
STR_TYPE templated_unsingned_to_string(
  const VAL_TYPE value,
  const VAL_TYPE * const dividers,
  const uint32_t MAX_DIVIDERS) noexcept
{
  static_assert(std::is_unsigned<VAL_TYPE>::value, "VAL_TYPE must be unsigned");
  //lint -e{9067} PCLint False Positive: "extern array declared without size or initializer" NOLINT
  ::char8_t retval_m_string[STR_TYPE::get_buffer_size_static()];  // NOLINT cpplint false positive

  bool digit_happened = false;
  size64_t str_idx = 0U;
  for (uint32_t i = 0U; i < MAX_DIVIDERS; ++i) {
    const VAL_TYPE digit = (value / dividers[i]) % 10U;
    if ((digit != 0U) || (i == (MAX_DIVIDERS - 1U)) || digit_happened) {
      retval_m_string[str_idx] = g_decimal_digits[digit];
      ++str_idx;
      retval_m_string[str_idx] = '\0';
      digit_happened = true;
    }
  }

  // Create a new apex::string by copying retval_m_string's content
  // A pointer is not an array and an array is not a pointer!
  const STR_TYPE retval(&retval_m_string[0U]);
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
template<class STR_TYPE, class U_TYPE, class VAL_TYPE>
STR_TYPE templated_signed_to_string(VAL_TYPE value) noexcept
{
  STR_TYPE retval;
  U_TYPE uvalue;
  if (value < 0) {
    retval += '-';
    // Intentional negation of the most negative, because conversion to unsigned will make it right
    //lint -e{2704} NOLINT potentially negating the most negative number
    value = -value;
  }
  uvalue = static_cast<U_TYPE>(value);
  retval += apex::to_string(uvalue);
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
// Strip all trailing zeroes from a decimal reprezentation of the input value:
// Convert 123 to 123
// Convert 120 to 12
// Convert 10 to 1
// Convert 10000 to 1
int64_t trim_decimal_zeroes(const int64_t value)
{
  int64_t retval = value;
  while (retval != 0LL) {
    const int64_t new_value = retval / 10LL;
    if ((new_value * 10LL) == retval) {
      retval = new_value;
    } else {
      break;
    }
  }
  return retval;
}

// Number of non-zero positions after decimal point while cponverting 32-bit floats to a string
static constexpr uint32_t float_32_pod_after_dp = 7U;
// Number of non-zero positions after decimal point while cponverting 64-bit floats to a string
static constexpr uint32_t float_64_pod_after_dp = 15U;

///////////////////////////////////////////////////////////////////////////////
// Convert an IEEE-754 float value to a normalized scientific notation string
/*
The conversion algo:

Simplified idea:
1. Input value: 123
2. Normalized input value: 1.23 * 10^2
3. String = "1.", scaled mantissa = (1.23 - 1) * 10^2 == 23
3. String += "23" + "e+" + "2" == "1.23e+2"

Detailed algo:
1. Convert the input to a positive value (if needed, add '-' to the result string)
   -123 -> 123, str = "-"
2. Assume the value is zero if it's less than the denormalized minimum value
3. Obtain mantissa in [1..10) range by dividing the value by its order of magnitude
   123 -> 1.23
4. Add the major (the first) number of the mantissa to the result string, add decimal point '.' too
   str =+ "1"
5. Obtain the after-decimal-pont minor mantissa numbers by subtracting the major from the mantissa
   mantissa = 1.23 - 1 == 0.23
   This reminder value is always in the [0..1) range
6. Add leading zeroes to the string, if the reminder contains leading zeroes.
   Shift reminder one decimal position left (*= 10) for every added leadng zero
   Do the above only for the number of required postions POS_AFTER_DP or not at all
   if the number of the leading zeroes exceed POS_AFTER_DP
   The normalized_reminder is an original reminder, multiplied by POS_AFTER_DP
   0.23 -> 2300000.0
7. Convert the normalized_reminder into integer reminder_i64 (we always have enough bits for this)
   2300000.0 -> 2300000
8. Strip possible trailing zeros from reminder_i64 by calling trim_decimal_zeroes function
   2300000 -> 23
9. Convert reminder_i64 to string using apex::to_sting(uint64) call and add to the result string
   23 -> "23"
   str += "."
   str += "23"
10. Add "e" exponent and the order of magnitude to the string
   str += "e"
   str += "+2"
*/

// Inputs:
// STR_TYPE is a return fixed string type
// POS_AFTER_DP is number of non-zero digit positions after decimal points
// VAL_TYPE is the actual IEE-754 type (32-bit or 64-bit)
// denom_value is apossibly denormalized (e.g. corrupted) IEEE-754 value
template<class STR_TYPE, uint32_t POS_AFTER_DP, typename VAL_TYPE>
STR_TYPE ieee754_to_s(const VAL_TYPE denom_value)
{
  // PCLint fires [MISRA C++ Rule 5-2-1] warning
  // "right side of logical operator '&&' is not a postfix expression"
  // Therefore I have to split the whole logical expression into constant terms
  // PCLint also fires [MISRA C++ Rule 0-1-9] operator == always evaluates to true
  // This one is a false alarm for a compile-time static_assert
  //lint --e{948} NOLINT disable [MISRA C++ Rule 0-1-9] rule check for the following code block
  {
    constexpr bool val_is_64_bit = std::is_same<VAL_TYPE, float64_t>::value;
    constexpr bool str_is_32_bytes = std::is_same<STR_TYPE, apex::string32_t>::value;
    constexpr bool after_dp_is_for_64_bits = POS_AFTER_DP == float_64_pod_after_dp;
    constexpr bool val_is_32_bit = std::is_same<VAL_TYPE, float32_t>::value;
    constexpr bool str_is_16_bytes = std::is_same<STR_TYPE, apex::string16_t>::value;
    constexpr bool after_dp_is_for_32_bits = POS_AFTER_DP == float_32_pod_after_dp;
    constexpr bool is_ieee754 = std::numeric_limits<VAL_TYPE>::is_iec559;

    // Allow only supported combinations of the template parameters
    static_assert(
      is_ieee754 &&
      ((val_is_64_bit && str_is_32_bytes && after_dp_is_for_64_bits) ||
      (val_is_32_bit && str_is_16_bytes && after_dp_is_for_32_bits)),
      "<string32_t, 15, float64_t> and <string16_t, 7, float32_t> types only");
  }

  float64_t value = static_cast<float64_t>(denom_value);
  STR_TYPE retval;
  if (std::isinf(value) && (value < 0.0)) {
    retval = "-inf";
  } else if (std::isinf(value)) {
    retval = "inf";
  } else if (std::isnan(value)) {
    retval = "nan";
  } else {
    if (value < 0.0) {
      // 1. Convert the input to a positive value (if needed, add '-' to the result string)
      retval += "-";
      value = -value;
    }

    // Now the value >= 0

    if (value < static_cast<float64_t>(std::numeric_limits<VAL_TYPE>::denorm_min())) {
      // 2. Assume the value is zero if it's less than the denormalized minimum value
      // That's the float way to do (x == 0) comparison when x is possibly broken
      retval += "0";
    } else {
      // 3. Obtain mantissa in [1..10) range by dividing the value by its order of magnitude
      // Now the value > 0, it's safe to take log10
      int32_t order_of_magnitude = static_cast<int32_t>(std::log10(value));
      #if defined(QNX) && defined(_LIBCPP_VERSION)
      float64_t divider = std::pow(10.0, static_cast<float64_t>(order_of_magnitude));
      #else
      float64_t divider = std::pow<>(10.0, static_cast<float64_t>(order_of_magnitude));
      #endif
      float64_t mantissa = value / divider;
      // Mantissa should be in [1..10) range
      // Due to IEEE-754 rounding, divider might be 999... instead of 1000...
      // Correct this situation by decrementing the order
      if (mantissa < 1.0) {
        order_of_magnitude--;
        #if defined(QNX) && defined(_LIBCPP_VERSION)
        divider = std::pow(10.0, static_cast<float64_t>(order_of_magnitude));
        #else
        divider = std::pow<>(10.0, static_cast<float64_t>(order_of_magnitude));
        #endif
        // divider can't be zero
        mantissa = value / divider;
      }

      // 4. Add the major(the first) number of the mantissa to the result string, add '.' too
      const int32_t major_number = static_cast<int32_t>(mantissa);
      retval += apex::to_string(major_number);
      retval += ".";

      // 5. Obtain the after-decimal-pont minor mantissa numbers by subtracting the major
      float64_t normalized_reminder = mantissa - static_cast<float64_t>(major_number);
      // normalized_reminder is in [0..1) range

      // 6. Add leading zeroes to the string, if the reminder contains leading zeroes.
      // The normalized_reminder is an original reminder, multiplied by POS_AFTER_DP
      if (normalized_reminder > 0.0) {
        const float64_t number_of_positions = std::abs(std::log10(normalized_reminder));
        if (number_of_positions < static_cast<float64_t>(POS_AFTER_DP)) {
          for (uint32_t i = 0U; i < POS_AFTER_DP; ++i) {
            normalized_reminder *= 10.0;
            const int32_t whole_number = int32_t(normalized_reminder);
            if (whole_number == 0) {
              retval += "0";
            }
          }
        }
      }

      // 7. Convert the normalized_reminder into 64-bit (we always have enough bits for this)
      const float64_t normalized_reminder_incremented = normalized_reminder + 0.5;
      // 8. Strip possible trailing zeros from reminder
      int64_t reminder_i64 =
        trim_decimal_zeroes(static_cast<int64_t>(normalized_reminder_incremented));
      const int64_t integer_mantissa2 =
        trim_decimal_zeroes(static_cast<int64_t>(normalized_reminder));
      if (reminder_i64 < integer_mantissa2) {
        // If normalized_reminder is 999... instead of 1000..., we need to correct it back by one
        reminder_i64 = integer_mantissa2;
      }

      // 9. Convert reminder_i64 to string using apex::to_sting call and add to the result string
      retval += apex::to_string(reminder_i64);

      // 10. Add "e" exponent and the order of magnitude to the string
      retval += "e";
      if (order_of_magnitude >= 0) {
        retval += "+";
      }
      retval += apex::to_string(order_of_magnitude);
    }
  }
  return retval;
}

}  // namespace

///////////////////////////////////////////////////////////////////////////////
apex::string16_t apex::to_string(const uint8_t value) noexcept
{
  const apex::string16_t retval = apex::to_string(static_cast<uint32_t>(value));
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string16_t apex::to_string(const uint16_t value) noexcept
{
  const apex::string16_t retval = apex::to_string(static_cast<uint32_t>(value));
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string16_t apex::to_string(const uint32_t value) noexcept
{
  static const uint32_t MAX_DIVIDERS =
    static_cast<uint32_t>(std::numeric_limits<uint32_t>::digits10) + 1U;
  static const uint32_t dividers[MAX_DIVIDERS] = {
    1000000000U,
    100000000U,
    10000000U,
    1000000U,
    100000U,
    10000U,
    1000U,
    100U,
    10U,
    1U
  };
  apex::string16_t retval;
  // I specifically want a non-typed check of length:
  //lint -e{1705} non-MISRA: static member could be accessed using a nested name specifier NOLINT
  static_assert(retval.get_buffer_size_static() >= MAX_DIVIDERS, "Return type is too small");

  retval = ::templated_unsingned_to_string<apex::string32_t>(value, &dividers[0U], MAX_DIVIDERS);
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string32_t apex::to_string(const uint64_t value) noexcept
{
  static const uint32_t MAX_DIVIDERS =
    static_cast<uint32_t>(std::numeric_limits<uint64_t>::digits10) + 1U;
  static const uint64_t dividers[MAX_DIVIDERS] = {
    10000000000000000000ULL,
    1000000000000000000ULL,
    100000000000000000ULL,
    10000000000000000ULL,
    1000000000000000ULL,
    100000000000000ULL,
    10000000000000ULL,
    1000000000000ULL,
    100000000000ULL,
    10000000000ULL,
    1000000000ULL,
    100000000ULL,
    10000000ULL,
    1000000ULL,
    100000ULL,
    10000ULL,
    1000ULL,
    100ULL,
    10ULL,
    1U
  };

  apex::string32_t retval;
  // I specifically want a non-typed check of length:
  //lint -e{1705} non-MISRA: static member could be accessed using a nested name specifier NOLINT
  static_assert(retval.get_buffer_size_static() >= MAX_DIVIDERS, "Return type is too small");

  retval = ::templated_unsingned_to_string<apex::string32_t>(value, &dividers[0U], MAX_DIVIDERS);
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string16_t apex::to_string(const int8_t value) noexcept
{
  const apex::string16_t retval = apex::to_string(static_cast<int32_t>(value));
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string16_t apex::to_string(const int16_t value) noexcept
{
  const apex::string16_t retval = apex::to_string(static_cast<int32_t>(value));
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string16_t apex::to_string(const int32_t value) noexcept
{
  const apex::string16_t retval = ::templated_signed_to_string<apex::string16_t, uint32_t>(value);
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string32_t apex::to_string(const int64_t value) noexcept
{
  const apex::string32_t retval = ::templated_signed_to_string<apex::string32_t, uint64_t>(value);
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string16_t apex::to_string(const ::float32_t value) noexcept
{
  apex::string16_t retval;
  // I specifically want a non-typed check of length:
  //lint -e{1705} non-MISRA: static member could be accessed using a nested name specifier NOLINT
  static_assert(
    // It's from limits.h: "#define FLT_MAX 3.402823466e+38F        // max value"
    // I renumbered its mantissa to `123..` to easily calculate the number of required positions
    retval.get_buffer_size_static() >= sizeof("-9.123456e+037"), "Return type is too small");
  retval = ieee754_to_s<apex::string16_t, float_32_pod_after_dp>(value);
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string32_t apex::to_string(const ::float64_t value) noexcept
{
  apex::string32_t retval;
  // I specifically want a non-typed check of length:
  //lint -e{1705} non-MISRA: static member could be accessed using a nested name specifier NOLINT
  static_assert(
    // It's from limits.h: "#define DBL_MAX 1.7976931348623158e+308 // max value"
    // I renumbered its mantissa to `123..` to easily calculate the number of required positions
    retval.get_buffer_size_static() >= sizeof("-1.123456789012345e+308"),
    "Return type is too small");
  retval = ieee754_to_s<apex::string32_t, float_64_pod_after_dp>(value);
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string8_t apex::to_string(const bool value) noexcept
{
  const apex::string8_t retval = value ? "true" : "false";
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
apex::string256_t apex::to_string(const char8_t * const c_text_ptr) noexcept
{
  apex::string256_t retval;
  if (nullptr != c_text_ptr) {
    retval = c_text_ptr;
  } else {
    retval = "<nullptr>";
  }
  return retval;
}
