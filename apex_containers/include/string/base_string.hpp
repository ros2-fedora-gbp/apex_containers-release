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
/// \brief This file contains definition of Apex.OS base string classes.

#ifndef STRING__BASE_STRING_HPP_
#define STRING__BASE_STRING_HPP_

#include <apexutils/apexdef.h>
#include <apex_containers/visibility_control.hpp>

#include <stdexcept>
#include <climits>
#include <cstring>
#include <algorithm>
#include <istream>
#include <ostream>
#include <string>

namespace apex
{

/// \class BaseString
/// \brief BaseString defines a base string class with constant memory footprint.
/// The internal buffer size is defined by BUFFER_SIZE template parameter.
/// The maximum length for a string is (BUFFER_SIZE - 1ULL).
/// One character is reserved for terminating zero.
/// The class serves as a common storage and method provider for derived classes
/// \tparam BUFFER_SIZE defines the size of static string including its terminating zero.
template<::size64_t BUFFER_SIZE>
class BaseString
{
public:
  using char_type = char8_t;

  /// \brief default constructor
  BaseString(void) noexcept
  {
    // Ignore unneeded pointer to destination
    (void)::memset(&(this->m_string[0U]), 0, this->get_buffer_size());
  }

  /// \brief An unsigned integral value that indicates either "not found" or
  /// "all remaining characters" when a search function fails.
  constexpr static const ::size64_t npos = ULLONG_MAX;

  /// \brief Converts the contents of a string into a pointer to an array of characters
  /// \return return a pointer to the internal zero-terminated array of characters
  constexpr const ::char8_t * data() const noexcept
  {
    return &(m_string[0U]);
  }

  /// \brief Returns a newly constructed string object with its value initialized to a copy of a
  /// substring of this object.
  /// The substring is the portion of the object that starts at character position pos and spans
  /// len characters (or until the end of the string, whichever comes first).
  /// \param[in] pos is a position of the first character to be copied as a substring.
  /// If this is equal to the string length, the function returns an empty string.
  /// If this is greater than the string length, it throws out_of_range.
  /// Note: The first character is denoted by a value of 0 (not 1).
  /// \param[in] len is a number of characters to include in the substring.
  /// If the string is shorter, as many characters as possible are used.
  /// A value of BaseString::npos indicates all characters until the end of the string.
  /// \throw std::out_of_range
  BaseString substr(const ::size64_t pos = 0U, const ::size64_t len = npos) const
  {
    BaseString retval;
    ::size64_t my_size = size();
    if (pos > my_size) {
      throw std::out_of_range("pos > my_size");
    }
    if (pos != my_size) {
      const ::size64_t chars_to_copy = std::min(my_size - pos, len);
      // Ignore unneeded returned pointer
      (void)memmove(retval.m_string, m_string + pos, chars_to_copy);
    }
    return retval;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Comparisons

  /// \brief Compares the value of the string object (or a substring) to the sequence of characters
  /// specified by its arguments. The compared string is the value of the string object or
  /// (if the signature used has a pos1 and a count1 parameters) the substring that begins at its
  /// character in position pos1 and spans count1 characters.
  /// This string is compared to a comparing string, which is determined by the other arguments
  /// passed to the function.
  /// \param[in] pos1 is a position of the first character in the compared string.
  /// If this is greater than the string length, it throws out_of_range.
  /// Note: The first character is denoted by a value of 0 (not 1).
  /// \param[in] count1 is length of compared string.
  /// If the string is shorter, as many characters as possible.
  /// If count1 is npos, the strings length is assumed
  /// \param [in] s is pointer to an array of characters. If NULL, std::invalid_argument is thrown.
  /// \param [in] count2 is number of characters to compare. It it's npos, all characters compared.
  /// \return  Returns a signed integral indicating the relation between the strings:
  /// 0 They compare equal
  /// <0 Either the value of the first character that does not match is lower in the compared
  /// string, or all compared characters match but the compared string is shorter.
  /// >0 Either the value of the first character that does not match is greater in the compared
  /// string, or all compared characters match but the compared string is longer.
  /// \throw std::invalid_argument
  /// \throw std::out_of_range
  int32_t compare(
    const ::size64_t pos1,
    const ::size64_t count1,
    const ::char8_t * const s,
    const ::size64_t count2 = npos) const
  {
    if (nullptr == s) {
      throw std::invalid_argument("s == NULL");
    }

    const ::size64_t my_size = length();
    if (pos1 > my_size) {
      throw std::out_of_range("pos > my_len");
    }

    const ::size64_t _count1 = (npos == count1) ? my_size : count1;
    ::size64_t n_str = count2;
    if (npos == count2) {
      n_str = ::strnlen(s, APEX_STRING_SIZE);
    }

    const ::size64_t rlen = std::min(_count1, my_size - pos1);
    int32_t cmp_retval = ::strncmp(data() + pos1, s, std::min(rlen, n_str));

    if (cmp_retval == 0) {
      if (rlen < n_str) {
        cmp_retval = -1;
      }
      if (rlen > n_str) {
        cmp_retval = 1;
      }
    }
    return cmp_retval;
  }

  /// \brief Compares the value of the string object (or a substring) to the sequence of characters
  /// specified by its arguments. The compared string is the value of the string object or
  /// (if the signature used has a pos1 and a _count1 parameters) the substring that begins at its
  /// character in position pos1 and spans _count1 characters.
  /// This string is compared to a comparing string, which is determined by the other arguments
  /// passed to the function.
  /// param [in] s is pointer to an array of characters. If NULL, std::invalid_argument is thrown.
  /// \return  Returns a signed integral indicating the relation between the strings:
  /// 0 They compare equal
  /// <0 Either the value of the first character that does not match is lower in the compared
  /// string, or all compared characters match but the compared string is shorter.
  /// >0 Either the value of the first character that does not match is greater in the compared
  /// string, or all compared characters match but the compared string is longer.
  /// \throw std::invalid_argument
  /// \throw std::out_of_range
  inline int32_t compare(const ::char8_t * const s) const
  {
    return this->compare(0U, npos, s);
  }

  /// \brief operator==
  /// \param[in] rhs is the right hand comparand up to APEX_STRING_SIZE characters
  /// \return return True if the strings are equal, false otherwise
  /// \throw std::invalid_argument
  /// \throw std::out_of_range
  bool operator==(const char8_t * const rhs) const
  {
    return this->compare(rhs) == 0;
  }

  /// \brief Compares the value of the string object (or a substring) to the given string.
  /// The compared string is the value of the string object or
  /// (if the signature used has a pos1 and a _count1 parameters) the substring that begins at its
  /// character in position pos1 and spans _count1 characters.
  /// This string is compared to a comparing string, which is determined by the other arguments
  /// passed to the function.
  /// \param[in] pos1 is a position of the first character in the compared string.
  /// If this is greater than the string length, it throws out_of_range.
  /// Note: The first character is denoted by a value of 0 (not 1).
  /// \param[in] count1 is length of compared string.
  /// If the string is shorter, as many characters as possible.
  /// If count1 is npos, full string length assumed.
  /// \param [in] str is a comparand string.
  /// \return  Returns a signed integral indicating the relation between the strings:
  /// 0 They compare equal
  /// <0 Either the value of the first character that does not match is lower in the compared
  /// string, or all compared characters match but the compared string is shorter.
  /// >0 Either the value of the first character that does not match is greater in the compared
  /// string, or all compared characters match but the compared string is longer.
  /// \throw std::invalid_argument
  /// \throw std::out_of_range
  template<::size64_t LEN>
  int32_t compare(
    const ::size64_t pos1,
    const ::size64_t count1,
    const BaseString<LEN> & str) const
  {
    return compare(pos1, count1, str.c_str());
  }

  /// \brief Compares the value of the string object (or a substring) to the given string.
  /// The compared string is the value of the string object or
  /// (if the signature used has a pos1 and a _count1 parameters) the substring that begins at its
  /// character in position pos1 and spans _count1 characters.
  /// This string is compared to a comparing string, which is determined by the other arguments
  /// passed to the function.
  /// \param[in] pos1 is a position of the first character in the compared string.
  /// If this is greater than the string length, it throws out_of_range.
  /// Note: The first character is denoted by a value of 0 (not 1).
  /// \param[in] count1 is length of compared string.
  /// If the string is shorter, as many characters as possible.
  /// If count1 is npos, full string length assumed.
  /// \param [in] str is a comparand string.
  /// \return  Returns a signed integral indicating the relation between the strings:
  /// 0 They compare equal
  /// <0 Either the value of the first character that does not match is lower in the compared
  /// string, or all compared characters match but the compared string is shorter.
  /// >0 Either the value of the first character that does not match is greater in the compared
  /// string, or all compared characters match but the compared string is longer.
  /// \throw std::invalid_argument
  /// \throw std::out_of_range
  int32_t compare(
    const ::size64_t pos1,
    const ::size64_t count1,
    const std::string & str) const
  {
    return compare(pos1, count1, str.c_str());
  }

  /// \brief Compares the value of the string object to the given string
  /// specified by its arguments.
  /// \param [in] str is a comparand string.
  /// \return  Returns a signed integral indicating the relation between the strings:
  /// 0 They compare equal
  /// <0 Either the value of the first character that does not match is lower in the compared
  /// string, or all compared characters match but the compared string is shorter.
  /// >0 Either the value of the first character that does not match is greater in the compared
  /// string, or all compared characters match but the compared string is longer.
  /// \throw std::invalid_argument
  /// \throw std::out_of_range
  template<::size64_t LEN>
  int32_t compare(const BaseString<LEN> & str) const
  {
    return compare(0U, npos, str);
  }

  /// \brief Compares the value of the string object to the given string
  /// specified by its arguments.
  /// \param [in] str is a comparand string.
  /// \return  Returns a signed integral indicating the relation between the strings:
  /// 0 They compare equal
  /// <0 Either the value of the first character that does not match is lower in the compared
  /// string, or all compared characters match but the compared string is shorter.
  /// >0 Either the value of the first character that does not match is greater in the compared
  /// string, or all compared characters match but the compared string is longer.
  /// \throw std::invalid_argument
  /// \throw std::out_of_range
  int32_t compare(const std::string & str) const
  {
    return compare(0U, npos, str);
  }

  /// \brief operator==
  /// \param[in] rhs is the right hand comparand
  /// \return return True if the strings are equal, false otherwise
  template<::size64_t LEN>
  bool operator==(const BaseString<LEN> & rhs) const
  {
    const int32_t cmp_retval = compare(0U, npos, rhs);
    return cmp_retval == 0;
  }

  /// \brief operator==
  /// \param[in] rhs is the right hand comparand
  /// \return return True if the strings are equal, false otherwise
  bool operator==(const std::string & rhs) const
  {
    const int32_t cmp_retval = compare(0U, npos, rhs.c_str());
    return cmp_retval == 0;
  }

  /// \brief operator!=
  /// \param[in] rhs is the right hand comparand up to APEX_STRING_SIZE characters
  /// \return return True if the strings are not equal, false otherwise
  bool operator!=(const char8_t * const rhs) const
  {
    return !operator==(rhs);
  }

  /// \brief operator!=
  /// \param[in] rhs is the right hand comparand
  /// \return return True if the strings are not equal, false otherwise
  template<::size64_t LEN>
  bool operator!=(const BaseString<LEN> & rhs) const
  {
    return !operator==(rhs);
  }

  /// \brief operator!=
  /// \param[in] rhs is the right hand comparand
  /// \return return True if the strings are not equal, false otherwise
  bool operator!=(const std::string & rhs) const
  {
    return !operator==(rhs);
  }

  /// \brief operator<
  /// \param[in] rhs is the right hand comparand up to APEX_STRING_SIZE characters
  /// \return return True if rhs is larger
  /// \throw std::invalid_argument
  bool operator<(const char8_t * const rhs) const
  {
    return this->compare(rhs) < 0;
  }

  /// \brief operator<
  /// \param[in] rhs is the right hand comparand
  /// \return return True if rhs is larger
  template<::size64_t LEN>
  bool operator<(const BaseString<LEN> & rhs) const
  {
    const int32_t cmp_retval = compare(0U, npos, rhs);
    return cmp_retval < 0;
  }

  /// \brief operator<
  /// \param[in] rhs is the right hand comparand
  /// \return return True if rhs is larger
  bool operator<(const std::string & rhs) const
  {
    const int32_t cmp_retval = compare(0U, npos, rhs);
    return cmp_retval < 0;
  }

  /// \brief operator<=
  /// \param[in] rhs is the right hand comparand up to APEX_STRING_SIZE characters
  /// \return return True if rhs is larger or equal
  bool operator<=(const char8_t * const rhs) const
  {
    return !operator>(rhs);
  }

  /// \brief operator<
  /// \param[in] rhs is the right hand comparand
  /// \return return True if rhs is larger
  template<::size64_t LEN>
  bool operator<=(const BaseString<LEN> & rhs) const
  {
    return !operator>(rhs);
  }

  /// \brief operator<
  /// \param[in] rhs is the right hand comparand
  /// \return return True if rhs is larger
  bool operator<=(const std::string & rhs) const
  {
    return !operator>(rhs);
  }

  /// \brief operator>
  /// \param[in] rhs is the right hand comparand up to APEX_STRING_SIZE characters
  /// \return return True if rhs is smaller
  /// \throw std::invalid_argument
  bool operator>(const char8_t * const rhs) const
  {
    return this->compare(rhs) > 0;
  }

  /// \brief operator>
  /// \param[in] rhs is the right hand comparand
  /// \return return True if rhs is smaller
  template<::size64_t LEN>
  bool operator>(const BaseString<LEN> & rhs) const
  {
    const int32_t cmp_retval = compare(0U, npos, rhs);
    return cmp_retval > 0;
  }

  /// \brief operator>
  /// \param[in] rhs is the right hand comparand
  /// \return return True if rhs is smaller
  bool operator>(const std::string & rhs) const
  {
    const int32_t cmp_retval = compare(0U, npos, rhs);
    return cmp_retval > 0;
  }

  /// \brief operator>=
  /// \param[in] rhs is the right hand comparand up to APEX_STRING_SIZE characters
  /// \return return True if rhs is smaller or equal
  bool operator>=(const char8_t * const rhs) const
  {
    return !operator<(rhs);
  }

  /// \brief operator>
  /// \param[in] rhs is the right hand comparand
  /// \return return True if rhs is smaller
  template<size64_t LEN>
  bool operator>=(const BaseString<LEN> & rhs) const
  {
    return !operator<(rhs);
  }

  /// \brief operator>
  /// \param[in] rhs is the right hand comparand
  /// \return return True if rhs is smaller
  bool operator>=(const std::string & rhs) const
  {
    return !operator<(rhs);
  }

  /////////////////////////////////////////////////////////////////////////////
  // Getters

  /// \brief Test whether the string contains anything.
  /// \return true if the string contains no characters; false if it has at least one character.
  bool empty() const noexcept {return m_string[0U] == '\0';}

  /// \brief return pointer to the internal string buffer.
  const char8_t * c_str() const noexcept
  {
    return m_string;
  }

  /// \brief return non const pointer to the internal string buffer.
  char8_t * data() noexcept
  {
    return &m_string[0];
  }

  /// \brief return the largest number of elements that could be stored in the string.
  inline constexpr size64_t capacity() const
  {
    return get_buffer_size() - 1U;
  }

  /// \brief return the current number of elements in a string.
  size64_t size() const noexcept
  {
    return ::strnlen(m_string, this->capacity());
  }

  /// \brief return the current number of elements in a string.
  size64_t length() const noexcept
  {
    return size();
  }

  /// \brief a standard index operator
  /// \param[in] idx is the index of the indexed character
  /// \return reference to the indexed character
  /// \throw std::out_of_range
  char8_t & operator[](size64_t idx)
  {
    if (idx >= get_buffer_size()) {
      throw std::out_of_range("idx >= get_buffer_size()");
    }
    return m_string[idx];
  }

  /// \brief a standard index operator
  /// \param[in] idx is the index to get the value at
  /// \return return value at the index
  /// \throw std::out_of_range
  char8_t operator[](size64_t idx) const
  {
    if (idx >= get_buffer_size()) {
      throw std::out_of_range("idx >= get_buffer_size()");
    }
    return m_string[idx];
  }

  /// \brief return the total number of character in the buffer, including the terminating zero
  /// \return the total number of character in the buffer, including the terminating zero
  inline constexpr ::size64_t get_buffer_size() const noexcept
  {
    return BUFFER_SIZE;
  }

  /// \brief return the total number of character in the buffer, including the terminating zero
  /// \return the total number of character in the buffer, including the terminating zero
  static inline constexpr ::size64_t get_buffer_size_static() noexcept
  {
    return BUFFER_SIZE;
  }

  /// Convert apex::string to an STL string
  /// \return return STL string
  operator std::string() const
  {
    return std::string(this->m_string);
  }

  // Removes all elements.
  void clear() noexcept
  {
    (void)std::memset(&(this->m_string[0U]), 0, this->get_buffer_size());
  }

protected:
  // The apex::BaseString iterator abs_iterator class below is based on
  // stackoverflow.com/questions/12092448/code-for-a-basic-random-access-iterator-based-on-pointers
  // This class is for internal apex::string use only
  template<typename Type>
  class abs_iterator : public std::iterator<std::random_access_iterator_tag, Type>
  {
public:
    using difference_type =
      typename std::iterator<std::random_access_iterator_tag, Type>::difference_type;

    abs_iterator()
    : m_ptr(nullptr)
    {
    }
    explicit abs_iterator(Type * rhs)
    : m_ptr(rhs)
    {
    }
    abs_iterator(const abs_iterator & rhs)
    : m_ptr(rhs.m_ptr)
    {
    }
    inline abs_iterator & operator+=(difference_type rhs)
    {
      m_ptr += rhs; return *this;
    }
    inline abs_iterator & operator-=(difference_type rhs)
    {
      m_ptr -= rhs; return *this;
    }
    inline Type & operator*() const
    {
      return *m_ptr;
    }
    inline Type * operator->() const
    {
      return m_ptr;
    }
    inline Type & operator[](difference_type rhs)
    {
      return m_ptr[rhs];
    }
    inline const Type & operator[](difference_type rhs) const
    {
      return m_ptr[rhs];
    }
    inline abs_iterator & operator++()
    {
      ++m_ptr; return *this;
    }
    inline abs_iterator & operator--()
    {
      --m_ptr; return *this;
    }
    inline abs_iterator operator++(int)
    {
      abs_iterator tmp(*this); ++m_ptr; return tmp;
    }
    inline abs_iterator operator--(int)
    {
      abs_iterator tmp(*this); --m_ptr; return tmp;
    }
    inline difference_type operator-(const abs_iterator & rhs) const
    {
      return difference_type(m_ptr - rhs.m_ptr);
    }
    inline abs_iterator operator+(difference_type rhs) const
    {
      return abs_iterator(m_ptr + rhs);
    }
    inline abs_iterator operator-(difference_type rhs) const
    {
      return abs_iterator(m_ptr - rhs);
    }
    friend inline abs_iterator operator+(difference_type lhs, const abs_iterator & rhs)
    {
      return abs_iterator(lhs + rhs.m_ptr);
    }
    friend inline abs_iterator operator-(difference_type lhs, const abs_iterator & rhs)
    {
      return abs_iterator(lhs - rhs.m_ptr);
    }
    inline bool operator==(const abs_iterator & rhs) const
    {
      return m_ptr == rhs.m_ptr;
    }
    inline bool operator!=(const abs_iterator & rhs) const
    {
      return m_ptr != rhs.m_ptr;
    }
    inline bool operator>(const abs_iterator & rhs) const
    {
      return m_ptr > rhs.m_ptr;
    }
    inline bool operator<(const abs_iterator & rhs) const
    {
      return m_ptr < rhs.m_ptr;
    }
    inline bool operator>=(const abs_iterator & rhs) const
    {
      return m_ptr >= rhs.m_ptr;
    }
    inline bool operator<=(const abs_iterator & rhs) const
    {
      return m_ptr <= rhs.m_ptr;
    }

private:
    Type * m_ptr;
  };

  /// \brief m_string is string's storage
  char8_t m_string[BUFFER_SIZE];
};

/// \brief this redefined operator allows streaming of an apex string to a standard stream.
/// \tparam STRING_BUFFER_SIZE defines the size of static string including its terminating zero.
/// \param[in, out] out_stream is output stream
/// \param[in] str is a string to stream into
/// \return return reference to the output stream
template<size64_t STRING_BUFFER_SIZE>
typename ::std::basic_ostream<char8_t> & operator<<(
  std::basic_ostream<char8_t> & out_stream,
  const apex::BaseString<STRING_BUFFER_SIZE> & str)
{
  return out_stream.write(str.c_str(), static_cast<std::streamsize>(str.size()));
}


/// \brief Convert a string to a 32-bit float.
/// The function internally uses `strtof` standard call.
/// See POSIX manpages for the full specs: https://www.unix.com/man-page/posix/3/strtof/
/// \param[in] str is input string value
/// \return return float value corresponding to the string value. See `strtof` documenation.
/// Six significant digits are printed.
template<::size64_t STRING_BUFFER_SIZE>
inline ::float32_t stof(const apex::BaseString<STRING_BUFFER_SIZE> & str) noexcept
{
  const float32_t value = strtof(str.c_str(), nullptr);
  return value;
}

/// \brief Convert a string to a 64-bit float.
/// The function internally uses `strtod` standard call.
/// See POSIX manpages for the full specs: https://www.unix.com/man-page/posix/3/strtof/
/// \param[in] str is input string value
/// \return return float value corresponding to the string value. See `strtod` documenation.
/// Six significant digits are printed.
template<::size64_t STRING_BUFFER_SIZE>
inline ::float64_t stod(const apex::BaseString<STRING_BUFFER_SIZE> & str) noexcept
{
  const float64_t value = strtod(str.c_str(), nullptr);
  return value;
}

/// \brief Convert a string to a 32-bit integer.
/// The function internally uses `strtol` standard call.
/// \param[in] str is input string value
/// \return return 32-bit integer value corresponding to the string value. See `strtol` specs.
/// Valid range is [INT32_MIN..INT32_MAX]
template<::size64_t STRING_BUFFER_SIZE>
inline ::int32_t stol(const apex::BaseString<STRING_BUFFER_SIZE> & str) noexcept
{
  ::int64_t value = ::strtol(str.c_str(), nullptr, 10);
  if (value < static_cast<int64_t>(INT32_MIN)) {
    value = static_cast<int64_t>(INT32_MIN);
  }

  if (value > static_cast<int64_t>(INT32_MAX)) {
    value = static_cast<int64_t>(INT32_MAX);
  }
  return static_cast<int32_t>(value);
}

/// \brief Convert a string to a 64-bit integer.
/// The function internally uses `stroll` standard call.
/// \param[in] str is input string value
/// \return return 64-bit integer value corresponding to the string value. See `strtoll` specs.
/// Valid range is [INT64_MIN..INT64_MAX]
template<::size64_t STRING_BUFFER_SIZE>
inline ::int64_t stoll(const apex::BaseString<STRING_BUFFER_SIZE> & str) noexcept
{
  ::int64_t value = ::strtoll(str.c_str(), nullptr, 10);
  return value;
}

/// \brief Convert a string to a 32-bit unsigned integer.
/// The function internally uses `strtoul` standard call.
/// \param[in] str is input string value
/// \return return 32-bit unsigned integer corresponding to the string value. See `strtoul` specs.
/// Valid range is [0..UINT32_MAX]
template<::size64_t STRING_BUFFER_SIZE>
inline ::uint32_t stoul(const apex::BaseString<STRING_BUFFER_SIZE> & str) noexcept
{
  ::uint64_t value = ::strtoul(str.c_str(), nullptr, 10);
  if (value > static_cast<uint64_t>(UINT32_MAX)) {
    value = static_cast<uint64_t>(UINT32_MAX);
  }
  return static_cast<uint32_t>(value & UINT32_MAX);
}

/// \brief Convert a string to a 64-bit unsigned integer.
/// The function internally uses `strtoull` standard call.
/// \param[in] str is input string value
/// \return return 64-bit unsigned integer corresponding to the string value. See `strtoull` specs.
/// Valid range is [0..UINT64_MAX]
template<::size64_t STRING_BUFFER_SIZE>
inline ::uint64_t stoull(const apex::BaseString<STRING_BUFFER_SIZE> & str) noexcept
{
  ::uint64_t value = ::strtoull(str.c_str(), nullptr, 10);
  return value;
}

}  // namespace apex

// Extending std::hash for apex::BaseString<Buffer_Size>
// cppcheck-suppress syntaxError
namespace std
{
// hash combine function based on the boost implementation:
// https://www.boost.org/doc/libs/1_68_0/boost/container_hash/hash.hpp
template<size64_t SizeN>
struct hash<apex::BaseString<SizeN>>
{
  size_t operator()(const apex::BaseString<SizeN> & str) const noexcept
  {
    size64_t seed = str.size();
    for (size64_t idx = 0U; idx < str.size(); idx++) {
      // cast char to unsigned char to size64_t for int array hashing
      seed ^= static_cast<size64_t>(static_cast<uint8_t>(str[idx])) + 0x9e3779b9U +
        (seed << 6U) + (seed >> 2U);
    }
    return seed;
  }
};

}  // namespace std

#endif  // STRING__BASE_STRING_HPP_
