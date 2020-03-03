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
/// \file
/// \brief This file contains definition of Apex.OS to string functions.

#ifndef STRING__TO_STRING_HPP_
#define STRING__TO_STRING_HPP_

#include <apexutils/apexdef.h>
#include <apex_containers/visibility_control.hpp>

#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <istream>
#include <ostream>
#include <string>

#include "base_string.hpp"
#include "string_strict.hpp"
#include "string_silent.hpp"

namespace apex
{

/// \brief Create an Apex.OS static 255-character plus zero string from a C zero-terminated string.
/// \param[in] c_text_ptr a pointer to a C zero-terminated string.
/// \return return Apex.OS string that contains text copy of the input string.
/// If the c_text_ptr is nullptr (NULL), the output will be "<nullptr>" string.
/// If the imput string is larger than 255 characters,
/// the resulting string will be truncated to 255
APEX_CONTAINERS_PUBLIC apex::string256_t to_string(const char8_t * const c_text_ptr) noexcept;

/// \brief Convert a boolean value into an Apex.OS string.
/// \param[in] value is input boolean value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
APEX_CONTAINERS_PUBLIC apex::string8_t to_string(const bool value) noexcept;

/// \brief Convert 8-bit unsigned integer into an Apex.OS string.
/// \param[in] value is input 8-bit unsigned integer value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
APEX_CONTAINERS_PUBLIC apex::string16_t to_string(const ::uint8_t value) noexcept;

/// \brief Convert 16-bit unsigned integer into an Apex.OS string.
/// \param[in] value is input 16-bit unsigned integer value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
APEX_CONTAINERS_PUBLIC apex::string16_t to_string(const ::uint16_t value) noexcept;

/// \brief Convert 32-bit unsigned integer into an Apex.OS string.
/// \param[in] value is input 32-bit unsigned integer value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
APEX_CONTAINERS_PUBLIC apex::string16_t to_string(const ::uint32_t value) noexcept;

/// \brief Convert 64-bit unsigned integer into an Apex.OS string.
/// \param[in] value is an input 64-bit unsigned integer value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
APEX_CONTAINERS_PUBLIC apex::string32_t to_string(const uint64_t value) noexcept;

#ifndef APEX_WINDOWS
/// \brief Convert 64-bit unsigned integer into an Apex.OS string.
/// GCC can't automatically translate `long long unsigned int` into uint64_t.
/// \param[in] value is input 64-bit signed integer value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
inline apex::string32_t to_string(const long long unsigned int value) noexcept  // NOLINT
{
  return apex::to_string(static_cast<uint64_t>(value));
}
#endif  // APEX_WINDOWS

/// \brief Convert 8-bit signed integer into an Apex.OS string.
/// \param[in] value is input 8-bit signed integer value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
APEX_CONTAINERS_PUBLIC apex::string16_t to_string(const ::int8_t value) noexcept;

/// \brief Convert 16-bit signed integer into an Apex.OS string.
/// \param[in] value is input 16-bit signed integer value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
APEX_CONTAINERS_PUBLIC apex::string16_t to_string(const ::int16_t value) noexcept;

/// \brief Convert 32-bit signed integer into an Apex.OS string.
/// \param[in] value is input 32-bit signed integer value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
APEX_CONTAINERS_PUBLIC apex::string16_t to_string(const ::int32_t value) noexcept;

/// \brief Convert 64-bit signed integer into an Apex.OS string.
/// \param[in] value is input 64-bit signed integer value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
APEX_CONTAINERS_PUBLIC apex::string32_t to_string(const ::int64_t value) noexcept;

#ifndef APEX_WINDOWS
/// \brief Convert 64-bit signed integer into an Apex.OS string.
/// GCC can't automatically translate `long long int` into int64_t.
/// \param[in] value is input 64-bit signed integer value to convert
/// \return return Apex.OS string that contains text representation of the input value in base 10.
inline apex::string32_t to_string(const long long int value) noexcept  // NOLINT
{
  return apex::to_string(static_cast<int64_t>(value));
}
#endif  // APEX_WINDOWS

/// \brief Convert an input 32-bit float to a string in scientific notation
/// using up to 7 digits after the decimal point
/// \param[in] value is input 32-bit float value to convert
/// \return return Apex.OS string that contains text representation of the input value
/// using up to 7 digits after the decimal point
APEX_CONTAINERS_PUBLIC apex::string16_t to_string(const ::float32_t value) noexcept;

/// \brief Convert an input 64-bit float to a string in scientific notation
/// using up to 15 digits after the decimal point
/// \param[in] value is input 64-bit float value to convert
/// \return return Apex.OS string that contains text representation of the input value
/// using up to 15 digits after the decimal point
APEX_CONTAINERS_PUBLIC apex::string32_t to_string(const ::float64_t value) noexcept;

/// \brief Convert an std::string into an Apex.OS string.
/// \param[in] value is input string value to convert
/// \return return Apex.OS string that contains text representation of the input value.
inline auto to_string(const std::string & value) noexcept
{
  return apex::to_string(value.c_str());
}

/// \brief Convert an Apex.OS string into an Apex.OS string.
/// Can be an identity function or can change a type of the Apex.OS string.
/// \param[in] value is input string value to convert
/// \return return Apex.OS string that contains text representation of the input value.
template<::size64_t STRING_BUFFER_SIZE>
inline auto to_string(const StringStrict<STRING_BUFFER_SIZE> & value) noexcept
{
  return apex::to_string(value.c_str());
}

// this is implementation. See the declaration in the String class.
template<::size64_t STRING_BUFFER_SIZE>
inline StringStrict<STRING_BUFFER_SIZE>
StringStrict<STRING_BUFFER_SIZE>::to_string(const uint32_t value)
{
  StringStrict<STRING_BUFFER_SIZE> retval = apex::to_string(value);
  return retval;
}

// this is implementation. See the declaration in the String class.
template<::size64_t STRING_BUFFER_SIZE>
inline String<STRING_BUFFER_SIZE> String<STRING_BUFFER_SIZE>::to_string(const uint32_t value)
{
  apex::string16_t result = apex::to_string(value);
  if (result.size() >= String<STRING_BUFFER_SIZE>::get_buffer_size_static()) {
    throw std::overflow_error("result.size() >= this->capacity()");
  }
  String<STRING_BUFFER_SIZE> retval = result;
  return retval;
}

/// \brief Create an Apex.OS static 255-character plus zero string from a space-separated list
/// of parameters (unpacks the variadic parameter pack to a apex::string256_t)
/// \param[in] args is an input variadic parameter pack to unpack
/// \return return Apex.OS string that contains unpacked variadic parameter pack as
/// apex::string256_t
template<typename ... Args>
APEX_CONTAINERS_PUBLIC apex::string256_t varargs_to_string(Args const & ... args) noexcept
{
  // This implementation is based on variadic template parameter pack
  // See https://en.cppreference.com/w/cpp/language/parameter_pack
  apex::string256_t result_str;
  const size64_t argc = sizeof ... (args);
  size64_t counter = 0U;

  // On the every element of the array add a parameter to the result_str.
  // If the parameter is not the last one, add a space separator after
  char8_t const string_filler[]{
    '\0',      // The "value" to put into the string_filler array when the parameter list is empty
    (
      result_str += apex::to_string(args),      // Add parameter string representation to result
      result_str += (counter < (argc - 1U)) ? " " : "",                  // Add a separator
      counter++,                  // Increment the parameter counter
      '\0') ...                  // The "value" to put into the string_filler array
  };

  // We don't need string_filler array in the active code, we need its curved brackets initializer.
  // The below construct is a command to static analysis tools to ignore non-use of the array
  (void)string_filler;

  return result_str;
}

/// \class no_separator
/// A tag class which signalizes to varargs_to_string() it should not insert a separator between
/// members of the list
struct no_separator {};

/// \brief Create an Apex.OS static 255-character plus zero string from a list
/// of parameters (unpacks the variadic parameter pack to a apex::string256_t)
/// \param[in] tag a tag for choosing this overload
/// \param[in] args is an input variadic parameter pack to unpack
/// \return return Apex.OS string that contains unpacked variadic parameter pack as
/// apex::string256_t
template<typename ... Args>
APEX_CONTAINERS_PUBLIC
apex::string256_t varargs_to_string(no_separator tag, Args const & ... args) noexcept
{
  (void)tag;
  apex::string256_t result_str;
  char8_t const string_filler[]{'\0', (result_str += apex::to_string(args), '\0') ...};
  (void)string_filler;

  return result_str;
}

}  // namespace apex

#endif  // STRING__TO_STRING_HPP_
