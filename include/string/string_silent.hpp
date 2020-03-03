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
/// \brief This file contains definition of Apex.OS string class

#ifndef STRING__STRING_SILENT_HPP_
#define STRING__STRING_SILENT_HPP_

#include <apexutils/apexdef.h>
#include <apex_containers/visibility_control.hpp>

#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <istream>
#include <ostream>
#include <string>

namespace apex
{

/// \class String
/// \brief String defines a string class with constant memory footprint.
/// Objects of this class do not throw exceptions if the dst string does not have enough space.
/// In this case the dst is filled up to capacity from src.
/// The internal buffer size is defined by STRING_BUFFER_SIZE template parameter.
/// The maximum length for a string is (STRING_BUFFER_SIZE - 1ULL).
/// One character is reserved for terminating zero.
/// \tparam STRING_BUFFER_SIZE defines the size of static string including its terminating zero.
template<::size64_t STRING_BUFFER_SIZE>
class String : public BaseString<STRING_BUFFER_SIZE>
{
public:
  using iterator =
    typename apex::template BaseString<STRING_BUFFER_SIZE>::template abs_iterator<::char8_t>;
  using const_iterator = typename
    apex::template BaseString<STRING_BUFFER_SIZE>::template abs_iterator<const ::char8_t>;

  /// \brief default constructor
  String(void) noexcept
  : apex::BaseString<STRING_BUFFER_SIZE>()
  {
  }

  /// \brief fill constructor
  /// Fills the string with n consecutive copies of character c.
  /// param[in] n is number of characters to copy.
  /// If n is grater than the maximum capacity or if npos, only maximum capacity is filled
  /// param[in] c is number of characters to copy.
  String(const ::size64_t n, const ::char8_t c) noexcept
  : String()
  {
    const ::size64_t chars_to_fill = (n < this->capacity()) ? n : this->capacity();
    // Ignore unneeded returned pointer to destination
    (void)memset(this->m_string, c, chars_to_fill);
  }

  /// \brief operator+=
  String & operator+=(const char8_t * const src) noexcept
  {
    if (src != nullptr) {
      const ::size64_t my_length = this->length();
      if (my_length < this->capacity()) {
        const ::size64_t their_length = ::strnlen(src, this->capacity());
        if (their_length > 0U) {
          const ::size64_t max_copy_size = this->capacity() - my_length;
          const ::size64_t copy_size =
            (max_copy_size < their_length) ? max_copy_size : their_length;
          (void)::memmove(&(this->m_string[my_length]), src, copy_size);
          this->m_string[my_length + copy_size] = '\0';
        }
      }
    }
    return *this;
  }

  /// \brief operator += chr
  String & operator+=(const char8_t chr) noexcept
  {
    const ::size64_t my_length = this->length();
    if (my_length < this->capacity()) {
      this->m_string[my_length] = chr;
      this->m_string[my_length + 1] = '\0';
    }
    return *this;
  }

  /// \brief constructor from an input string
  String(const ::char8_t * const str) noexcept  // NOLINT: constructors should be explicit
  : String()  // but we want to have ability to convert a classic char pointer to apex::string
  {
    *this += str;
  }

  /// \brief constructor from an Apex.OS string
  explicit String(const ::apex_string_t & str) noexcept
  : String(str.c_str)
  {
  }

  /// \brief copy constructor
  template<::size64_t LEN>
  String(const apex::BaseString<LEN> & src) noexcept // NOLINT: constructors should be explicit but we
  : String(src.c_str())  // want to to convert a different legth string to apex::string
  {
  }

  /// \brief copy constructor
  /// See MISRA C++:2008 14-5-2
  explicit String(const apex::String<STRING_BUFFER_SIZE> & src) noexcept
  : String(src.c_str())
  {
  }

  /////////////////////////////////////////////////////////////////////////////
  // Addition

  /// \brief operator+
  template<::size64_t LEN>
  const apex::String<STRING_BUFFER_SIZE + LEN>
  operator+(const apex::String<LEN> & rhs) const noexcept
  {
    apex::String<STRING_BUFFER_SIZE + LEN> retval(*this);
    retval += rhs.c_str();
    return retval;
  }

  /// \brief operator+
  template<::size64_t LEN>
  auto
  operator+(const char (& rhs)[LEN]) const noexcept
  {
    apex::String<STRING_BUFFER_SIZE + LEN> retval(*this);
    retval += rhs;
    return retval;
  }

  /// \brief operator+
  const apex::String<STRING_BUFFER_SIZE + APEX_STRING_SIZE>
  operator+(const ::apex_string_t & rhs) const noexcept
  {
    apex::String<STRING_BUFFER_SIZE + APEX_STRING_SIZE> retval(*this);
    retval += rhs.c_str;
    return retval;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Increments

  /// \brief operator+=
  template<::size64_t LEN>
  apex::String<STRING_BUFFER_SIZE> & operator+=(const apex::String<LEN> & rhs) noexcept
  {
    *this += rhs.c_str();
    return *this;
  }

  /// \brief operator+=
  apex::String<STRING_BUFFER_SIZE> & operator+=(const ::apex_string_t & src) noexcept
  {
    *this += src.c_str;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Assignments

  /// \brief Assignment operator.
  /// See MISRA C++:2008 14-5-3
  String & operator=(const String & rhs) noexcept
  {
    // Ignore unneeded pointer to destination
    (void)::memset(&(this->m_string[0U]), 0, this->get_buffer_size());
    *this += rhs.c_str();
    return *this;
  }

  /// \brief Assignment operator
  template<::size64_t LEN>
  apex::String<STRING_BUFFER_SIZE> & operator=(const apex::String<LEN> & rhs) noexcept
  {
    // Ignore unneeded pointer to destination
    (void)::memset(&(this->m_string[0U]), 0, this->get_buffer_size());
    *this += rhs.c_str();
    return *this;
  }

  /// \brief Assignment operator
  apex::String<STRING_BUFFER_SIZE> & operator=(const ::char8_t * const src) noexcept
  {
    // Ignore unneeded pointer to destination
    (void)::memset(&(this->m_string[0U]), 0, this->get_buffer_size());
    *this += src;
    return *this;
  }

  /// \brief Assignment operator
  apex::String<STRING_BUFFER_SIZE> & operator=(const ::apex_string_t & src) noexcept
  {
    // Ignore unneeded pointer to destination
    (void)::memset(&(this->m_string[0U]), 0, this->get_buffer_size());
    *this += src.c_str;
    return *this;
  }

  /// \brief Convert 32-bit unsigned integer in base 10 into an Apex.OS string.
  /// The method will throw std::overflow_error exception if the string's capacity is insufficient.
  /// \param[in] value is input 32-bit unsigned integer value to convert
  /// \return return Apex.OS string that contains text representation of the input value in base 10.
  /// \throw std::overflow_error
  static apex::String<STRING_BUFFER_SIZE> to_string(const uint32_t value);

  /// \brief Retrieves the begin iterator for this string
  /// \return the begin iterator
  iterator begin() noexcept
  {
    return iterator(&(BaseString<STRING_BUFFER_SIZE>::m_string[0U]));
  }

  /// \brief Retrieves the end iterator for this string
  /// \return the end iterator
  iterator end() noexcept
  {
    return iterator(
      &(BaseString<STRING_BUFFER_SIZE>::m_string[0U]) + BaseString<STRING_BUFFER_SIZE>::size());
  }

  /// \brief Retrieves the begin constant iterator for this string
  /// \return the begin iterator
  const_iterator cbegin() const noexcept
  {
    return const_iterator(
      &(BaseString<STRING_BUFFER_SIZE>::m_string[0U]));
  }

  /// \brief Retrieves the end constant iterator for this string
  /// \return the end iterator
  const_iterator cend() const noexcept
  {
    return const_iterator(
      &(BaseString<STRING_BUFFER_SIZE>::m_string[0U]) + BaseString<STRING_BUFFER_SIZE>::size());
  }
};

///////////////////////////////////////////////////////////////////////////////
/// \brief define `apex_string_t + String` operator
template<::size64_t LEN>
inline apex::String<APEX_STRING_SIZE + LEN>
operator+(const ::apex_string_t & lhs, const apex::String<LEN> & rhs) noexcept
{
  String<APEX_STRING_SIZE + LEN> retval(lhs.c_str);
  retval += rhs.c_str();
  return retval;
}

/// \brief define `char[] + String` operator
/// \param[in] lhs is the left hand addend
/// \param[in] rhs is the right hand addend
/// \return return the result of lhs and rhs concatenation, possibly trimmed to the strings max size
/// \throw std::invalid_argument
/// \throw std::overflow_error
template<::size64_t CSTR_LEN, ::size64_t APEX_STRING_LEN>
inline auto
operator+(const char (& lhs)[CSTR_LEN], const apex::String<APEX_STRING_LEN> & rhs)
{
  apex::String<CSTR_LEN + APEX_STRING_LEN> retval(lhs);
  retval += rhs;
  return retval;
}


/// \typedef string8_t
/// \brief string8_t defines an 8 bytes string
using string8_t = String<8U>;
static_assert(sizeof(string8_t) == 8U, "sizeof(string8_t) != 8U");

/// \typedef string16_t
/// \brief string16_t defines a 16 bytes string
using string16_t = String<16U>;
static_assert(sizeof(string16_t) == 16U, "sizeof(string16_t) != 16U");

/// \typedef string32_t
/// \brief string32_t defines a 32 bytes string
using string32_t = String<32U>;
static_assert(sizeof(string32_t) == 32U, "sizeof(string32_t) != 32U");

/// \typedef string64_t
/// \brief string64_t defines a 64 bytes string
using string64_t = String<64U>;
static_assert(sizeof(string64_t) == 64U, "sizeof(string64_t) != 64U");

/// \typedef string128_t
/// \brief string128_t defines a 128 bytes string
using string128_t = String<128U>;
static_assert(sizeof(string128_t) == 128U, "sizeof(string8_t) != 128U");

/// \typedef string256_t
/// \brief string256_t defines a 256 bytes string
using string256_t = String<APEX_STRING_SIZE>;
static_assert(sizeof(string256_t) == 256U, "sizeof(string256_t) != 256U");

/// \brief this redefined operator allows streaming to an apex string to a standard stream.
/// \tparam STRING_BUFFER_SIZE defines the size of static string including its terminating zero.
/// \param[in, out] in_stream is input stream
/// \param[in, out] str is a string to stream from
/// \return return reference to the input stream
template<size64_t STRING_BUFFER_SIZE>
typename ::std::istream & operator>>(
  std::istream & in_stream,
  apex::String<STRING_BUFFER_SIZE> & str)
{
  typename apex::String<STRING_BUFFER_SIZE>::iterator it = str.begin();
  return in_stream.getline(&(it[0U]), str.get_buffer_size());
}

}  // namespace apex

namespace std
{
template<size64_t SizeN>
struct hash<apex::String<SizeN>>: std::hash<apex::BaseString<SizeN>> {};
}  // namespace std

#endif  // STRING__STRING_SILENT_HPP_
