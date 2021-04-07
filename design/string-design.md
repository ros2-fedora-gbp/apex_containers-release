Apex.OS string C++ API
======================


# Purpose / Use cases

- C99 `apex_string_t` strings are 256 bytes
    - It is convenient to have strings of different sizes
- Apex.OS requires object-oriented string functionality to supplement making separate
C99 API calls
- Interaction with string should be similar to STL's `std::string` objects


# Design

`string.hpp` implements a string class with compile-time defined size.

The file contains two classes:

- `apex::String` is a string that silently trims contents to string's max size if new
contents' size exceeds string's length
- `apex::StringStrict` is a string that throws exception if new contents' size exceeds
string's length

Both classes define constructors from various types, concatenation operators (`+`) and
basic `std::string` compatible getters like `length()`, `size()`, `capacity()`,
`c_str()` and `[]`.

For both classes the main assignment functionality is in the `operator+=` functions.

During #1548, `to_string(uint32_t)` method was added, and now it is introduced into the
classes together with the corresponding unit tests. The functionality repeats C99
`apex_u32_to_string` call, plus the following overflow checks were added: the C99 call operates on
256 character string, and inability to overflow.


# References / External links

- See [apex_string-design.md](@ref md_src_apex_os_core_apexutils_design_apex_string-design).


# Assumptions / Known limits


# Future extensions / Unimplemented parts

- Apex.AI will consider creating `apex::BasicString` base abstract class to pack common
functionality from both `apex::StringStrict` and `apex::String` classes
- Apex.AI will consider adding more `std::string` compatible methods to both `apex` string
classes


# Related issues

- #778
- #1100: Update copyright statement
- #1046: Extend test coverage to 100%
- #1461: Add string comparison operators, `empty()` method, and the corresponding units
tests
- #1015: Hide struct part of typedef struct
- #1526: Update the design doc format to match the documentation style guide
- #1449: fix all warnings, put -Werror in place
- #1548: `to_string(const uint32_t value)` methods for both strict and non-strict strings
- #1770: `begin()`, `cbegin()`, `end()`, and `cend()` methods now allow use
of standard `<algorithm>` methods for both string classes.
- #1471:
    - Added support for streaming into and from standard streams including `std::stringstream`
    - Added automatic conversion to `std::string` (`std::string` uses dynamic memory!)
- #1778:
    - A subset of STL-like `compare` methods that allow substring comparison:
        ```cpp
        apex::string8_t foofoo = "foofoo";
        apex::string8_t foo = "foo";
        EXPECT_EQ(foofoo.compare(0U, 3, foo), 0);
        EXPECT_EQ(foofoo.compare(0U, 3, "foo"), 0);
        ```
    - An STL-compliant `substr` method
- #1779:
    - STL compatible calls `apex::to_string([float|int|uint])`
    - STL compatible calls `apex::stoul`, `apex::stol`, `apex::stof`
    - STL compatible fill constructor
    - STL compatible `+= chr` operator
- #2082:
    - STL compatible calls `apex::to_string()` for all integral types and for boolean too
    - STL compatible call `apex::stod`
- #2016: Style changes required for Doxygen version upgrade
- #2296: Replce the QNX-unsupported `gcvt` call with a custom Apex.OS
implementation of a value-to-string algorithm. Add unit more tests. Remove OS-dependent behavior.
- #2104: Provide `apex::to_string(const char *)` API.
- #2400: Provide `apex::stoll` and `apex::stoull` string-to-64-bit-integer conversion.
- #1991: Revise the design documentation language and format
- #2861: Fix format issue in this design document due to previous merge conflict
- #2968: Move apex_string to containers
- #3764: Added data() which returns non const pointer to internal string buffer.


