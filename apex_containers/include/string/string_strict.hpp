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
/// \brief This file contains definition of Apex.OS string strict class

#ifndef STRING__STRING_STRICT_HPP_
#define STRING__STRING_STRICT_HPP_

#include <apexutils/apexdef.h>
#include <apexutils/apex_string.h>
#include <apex_containers/visibility_control.hpp>

#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <istream>
#include <ostream>
#include <string>

namespace apex
{

/// \class StringStrict
/// \brief StringStrict defines a string class with constant memory footprint.
/// The internal buffer size is defined by STRING_BUFFER_SIZE template parameter.
/// The maximum length for a string is (STRING_BUFFER_SIZE - 1ULL).
/// One character is reserved for terminating zero.
/// \tparam STRING_BUFFER_SIZE defines the size of static string including its terminating zero.
template<::size64_t STRING_BUFFER_SIZE>
class StringStrict : public BaseString<STRING_BUFFER_SIZE>
{
public:
  using iterator =
    typename apex::template BaseString<STRING_BUFFER_SIZE>::template abs_iterator<::char8_t>;
  using const_iterator = typename
    apex::template BaseString<STRING_BUFFER_SIZE>::template abs_iterator<const ::char8_t>;

  /// \brief default constructor
  StringStrict(void)
  : apex::BaseString<STRING_BUFFER_SIZE>()
  {
  }

  /// \brief fill constructor
  /// Fills the string with n consecutive copies of character c.
  /// param[in] n is number of characters to copy.
  /// If n grater than the maximum capacity, std::overflow_error is thrown
  /// If n is npos, only maximum capacity is filled
  /// param[in] c is number of characters to copy.
  /// \throw std::overflow_error
  StringStrict(const ::size64_t n, const ::char8_t c)
  : StringStrict()
  {
    if ((n != StringStrict::npos) && (n > this->capacity())) {
      throw std::overflow_error("n > this->capacity()");
    }
    const ::size64_t chars_to_fill = (n < this->capacity()) ? n : this->capacity();
    // Ignore unneeded returned pointer to destination
    (void)memset(this->m_string, c, chars_to_fill);
  }

  /// \brief operator+=
  /// \param[in] src is the source string to add
  /// \return return the result of concatenation with the source string
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  StringStrict<STRING_BUFFER_SIZE> & operator+=(const ::char8_t * const src)
  {
    if (src != nullptr) {
      const ::size64_t my_length = this->length();
      const ::size64_t their_length = ::strnlen(src, this->get_buffer_size());
      if ((my_length + their_length + 1U) <= this->get_buffer_size()) {
        // Ignore returned self reference
        (void)::memmove(&(this->m_string[0U]) + my_length, src, their_length);
        this->m_string[my_length + their_length] = '\0';
      } else {
        throw std::overflow_error("Can't add too large string");
      }
    } else {
      throw std::invalid_argument("Can't add NULL string");
    }
    return *this;
  }

  /// \brief operator += chr
  /// \param[in] chr is the character to add
  /// \return return the result of concatenation with the character
  /// \throw std::overflow_error
  StringStrict<STRING_BUFFER_SIZE> & operator+=(const ::char8_t chr)
  {
    const ::size64_t my_length = this->length();
    if ((my_length + 2U) <= this->get_buffer_size()) {
      this->m_string[my_length] = chr;
      this->m_string[my_length + 1U] = '\0';
    } else {
      throw std::overflow_error("Can't add too large string");
    }
    return *this;
  }

  /// \brief constructor from an input string
  /// \param[in] str is the source string
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  StringStrict(const ::char8_t * const str)  // NOLINT: constructors should be explicit, but
  : StringStrict()  // we want to have ability to convert a classic char pointer to apex::string
  {
    *this += str;
  }

  /// \brief constructor from an Apex.OS string
  /// \param[in] str is initializing string
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  explicit StringStrict(const ::apex_string_t & str)
  : StringStrict(str.c_str)
  {
  }

  /// \brief copy constructor
  /// \param[in] src is initializing Apex.OS string
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  template<::size64_t LEN>
  StringStrict(const apex::BaseString<LEN> & src)  // NOLINT: constructors should be explicit, but
  : StringStrict(src.c_str())  // we want to have ability to assign another string
  {
  }

  /// \brief copy constructor
  /// See MISRA C++:2008 14-5-2
  /// \param[in] src is initializing Apex.OS string
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  explicit StringStrict(const BaseString<STRING_BUFFER_SIZE> & src)
  : StringStrict(src.c_str())
  {
  }

  /////////////////////////////////////////////////////////////////////////////
  // Addition

  /// \brief operator+
  /// \param[in] rhs is the right hand addend
  /// \return the result of concatenation with right hand addend
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  template<::size64_t LEN>
  const StringStrict<STRING_BUFFER_SIZE + LEN> operator+(const StringStrict<LEN> & rhs) const
  {
    StringStrict<STRING_BUFFER_SIZE + LEN> retval(*this);
    retval += rhs.c_str();
    return retval;
  }

  /// \brief operator+
  /// \param[in] rhs is the right hand addend
  /// \return the result of concatenation with right hand addend
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  template<::size64_t LEN>
  auto
  operator+(const char (& rhs)[LEN]) const noexcept
  {
    apex::StringStrict<STRING_BUFFER_SIZE + LEN> retval(*this);
    retval += rhs;
    return retval;
  }

  /// \brief operator+
  const apex::StringStrict<STRING_BUFFER_SIZE + APEX_STRING_SIZE>
  operator+(const ::apex_string_t & rhs) const
  {
    StringStrict<STRING_BUFFER_SIZE + APEX_STRING_SIZE> retval(*this);
    retval += rhs.c_str;
    return retval;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Increments

  /// \brief operator+=
  /// \param[in] rhs is the right hand addend
  /// \return the result of concatenation with right hand addend
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  template<::size64_t LEN>
  StringStrict & operator+=(const StringStrict<LEN> & rhs)
  {
    *this += rhs.c_str();
    return *this;
  }

  /// \brief operator+=
  /// \param[in] src is addend strng
  /// \return result of concatenation with addend string
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  StringStrict<STRING_BUFFER_SIZE> & operator+=(const apex_string_t & src)
  {
    *this += src.c_str;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Assignments

  /// \brief Assignment operator.
  /// See MISRA C++:2008 14-5-3
  /// \param[in] rhs is the source initializer
  /// \return return final string initialized with the source initializer
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  StringStrict<STRING_BUFFER_SIZE> & operator=(const BaseString<STRING_BUFFER_SIZE> & rhs)
  {
    // Ignore unneeded pointer to destination
    (void)::memset(&(this->m_string[0U]), 0, this->get_buffer_size());
    *this += rhs.c_str();
    return *this;
  }

  /// \brief Assignment operator
  /// \param[in] rhs is the source initializer
  /// \return return final string initialized with the source initializer
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  template<::size64_t LEN>
  StringStrict<STRING_BUFFER_SIZE> & operator=(const BaseString<LEN> & rhs)
  {
    // Ignore unneeded pointer to destination
    (void)::memset(&(this->m_string[0U]), 0, this->get_buffer_size());
    *this += rhs.c_str();
    return *this;
  }

  /// \brief Assignment operator
  /// \param[in] src is the source initializer
  /// \return return final string initialized with the source initializer
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  StringStrict<STRING_BUFFER_SIZE> & operator=(const char8_t * const src)
  {
    // Ignore unneeded pointer to destination
    (void)::memset(&(this->m_string[0U]), 0, this->get_buffer_size());
    *this += src;
    return *this;
  }

  /// \brief Assignment operator
  /// \param[in] src is the source initializer
  /// \return return final string initialized with the source initializer
  /// \throw std::invalid_argument
  /// \throw std::overflow_error
  StringStrict<STRING_BUFFER_SIZE> & operator=(const apex_string_t & src)
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
  static apex::StringStrict<STRING_BUFFER_SIZE> to_string(const uint32_t value);

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
    return const_iterator(&(BaseString<STRING_BUFFER_SIZE>::m_string[0U]));
  }

  /// \brief Retrieves the end constant iterator for this string
  /// \return the end iterator
  const_iterator cend() const noexcept
  {
    return const_iterator(
      &(BaseString<STRING_BUFFER_SIZE>::m_string[0U]) +
      BaseString<STRING_BUFFER_SIZE>::size());
  }
};

/// \brief define `apex_string_t + StringStrict` operator
/// \param[in] lhs is the left hand addend
/// \param[in] rhs is the right hand addend
/// \return return the result of lhs and rhs concatenation, possibly trimmed to the strings max size
/// \throw std::invalid_argument
/// \throw std::overflow_error
template<::size64_t LEN>
inline apex::StringStrict<APEX_STRING_SIZE + LEN>
operator+(const ::apex_string_t & lhs, const apex::StringStrict<LEN> & rhs)
{
  StringStrict<APEX_STRING_SIZE + LEN> retval(lhs.c_str);
  retval += rhs.c_str();
  return retval;
}

/// \brief define `char[] + StringStrict` operator
/// \param[in] lhs is the left hand addend
/// \param[in] rhs is the right hand addend
/// \return return the result of lhs and rhs concatenation, possibly trimmed to the strings max size
/// \throw std::invalid_argument
/// \throw std::overflow_error
template<::size64_t CSTR_LEN, ::size64_t APEX_STRING_LEN>
inline auto
operator+(const char (& lhs)[CSTR_LEN], const apex::StringStrict<APEX_STRING_LEN> & rhs)
{
  StringStrict<CSTR_LEN + APEX_STRING_LEN> retval(lhs);
  retval += rhs;
  return retval;
}


/// \typedef string_strict8_t
/// \brief string_strict8_t defines an 8 bytes string_strict
using string_strict8_t = StringStrict<8U>;
static_assert(sizeof(string_strict8_t) == 8U, "sizeof(string_strict8_t) != 8U");

/// \typedef string_strict16_t
/// \brief string_strict16_t defines a 16 bytes string_strict
using string_strict16_t = StringStrict<16U>;
static_assert(sizeof(string_strict16_t) == 16U, "sizeof(string_strict16_t) != 16U");

/// \typedef string_strict32_t
/// \brief string_strict32_t defines a 32 bytes string_strict
using string_strict32_t = StringStrict<32U>;
static_assert(sizeof(string_strict32_t) == 32U, "sizeof(string_strict32_t) != 32U");

/// \typedef string_strict64_t
/// \brief string_strict64_t defines a 64 bytes string_strict
using string_strict64_t = StringStrict<64U>;
static_assert(sizeof(string_strict64_t) == 64U, "sizeof(string_strict64_t) != 64U");

/// \typedef string_strict128_t
/// \brief string_strict128_t defines a 128 bytes string_strict
using string_strict128_t = StringStrict<128U>;
static_assert(sizeof(string_strict128_t) == 128U, "sizeof(string_strict8_t) != 128U");

/// \typedef string_strict256_t
/// \brief string_strict256_t defines a 256 bytes string_strict
using string_strict256_t = StringStrict<APEX_STRING_SIZE>;
static_assert(sizeof(string_strict256_t) == 256U, "sizeof(string_strict256_t) != 256U");

/// \brief this redefined operator allows streaming to an apex string to a standard stream.
/// \tparam STRING_BUFFER_SIZE defines the size of static string including its terminating zero.
/// \param[in, out] in_stream is input stream
/// \param[in, out] str is a string to stream from
/// \return return reference to the input stream
template<size64_t STRING_BUFFER_SIZE>
typename ::std::istream & operator>>(
  std::istream & in_stream,
  apex::StringStrict<STRING_BUFFER_SIZE> & str)
{
  typename apex::StringStrict<STRING_BUFFER_SIZE>::iterator it = str.begin();
  return in_stream.getline(&(it[0U]), str.get_buffer_size());
}

}  // namespace apex

namespace std
{
template<size64_t SizeN>
struct hash<apex::StringStrict<SizeN>>: std::hash<apex::BaseString<SizeN>> {};
}  // namespace std

#endif  // STRING__STRING_STRICT_HPP_
