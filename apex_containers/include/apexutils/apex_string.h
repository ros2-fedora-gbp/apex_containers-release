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
/// \brief This file declares fixed strings API used in Apex.OS applications

#ifndef APEXUTILS__APEX_STRING_H_
#define APEXUTILS__APEX_STRING_H_

#include <apexutils/visibility_control.h>
#include <apexutils/apexdef.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

///////////////////////////////////////////////////////////////////////////////
/// \def APEX_STRING_SIZE
/// \brief Define Apex.OS fixed string size including terminating zero
#define APEX_STRING_SIZE (256ULL)

/// \def MAX_APEX_STRING_LEN
/// \brief Define maximum actual string length
#define MAX_APEX_STRING_LEN (APEX_STRING_SIZE - 1ULL)

/// \typedef apex_string_t
/// \brief defines a fixed size string
/// \struct _apex_string_s
/// \brief see apex_string_t
typedef struct _apex_string_s
{
  char8_t c_str[APEX_STRING_SIZE];  ///< fixed array of characters
} apex_string_t;

///////////////////////////////////////////////////////////////////////////////

/// \brief Create clean, empty apex_string_t object on the stack
/// \return return clean, empty apex_string_t object on the stack
APEXUTILS_PUBLIC apex_string_t apex_string_create(void);

/// \brief fill the apex_string_t object with zeroes
/// \param[in, out] str pointer to apex_string to clean up
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if str is NULL
APEXUTILS_PUBLIC apex_ret_t apex_string_clean(apex_string_t * const str);

/// \brief Add a string or fail if overflow
/// If overflow predicted, dst not changed
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src pointer to a C string to add to dst
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if dst or src are NULL or if overflow predicted
APEXUTILS_PUBLIC apex_ret_t apex_string_add_strict(
  apex_string_t * const dst,
  const char8_t * const src);

/// \brief Add strings or fail if overflow
/// If overflow predicted, dst not changed
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if dst or srcX are NULL or if overflow predicted
APEXUTILS_PUBLIC apex_ret_t apex_string_add2_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2
);

/// \brief Add strings or fail if overflow
/// If overflow predicted, dst not changed
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if dst or srcX are NULL or if overflow predicted
APEXUTILS_PUBLIC apex_ret_t apex_string_add3_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3
);

/// \brief Add strings or fail if overflow
/// If overflow predicted, dst not changed
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if dst or srcX are NULL or if overflow predicted
APEXUTILS_PUBLIC apex_ret_t apex_string_add4_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4
);

/// \brief Add strings or fail if overflow
/// If overflow predicted, dst not changed
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if dst or srcX are NULL or if overflow predicted
APEXUTILS_PUBLIC apex_ret_t apex_string_add5_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5
);

/// \brief Add strings or fail if overflow
/// If overflow predicted, dst not changed
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \param[in] src6 pointer to a C string to add to dst
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if dst or srcX are NULL or if overflow predicted
APEXUTILS_PUBLIC apex_ret_t apex_string_add6_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6
);

/// \brief Add strings or fail if overflow
/// If overflow predicted, dst not changed
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \param[in] src6 pointer to a C string to add to dst
/// \param[in] src7 pointer to a C string to add to dst
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if dst or srcX are NULL or if overflow predicted
APEXUTILS_PUBLIC apex_ret_t apex_string_add7_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7
);

/// \brief Add strings or fail if overflow
/// If overflow predicted, dst not changed
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \param[in] src6 pointer to a C string to add to dst
/// \param[in] src7 pointer to a C string to add to dst
/// \param[in] src8 pointer to a C string to add to dst
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if dst or srcX are NULL or if overflow predicted
APEXUTILS_PUBLIC apex_ret_t apex_string_add8_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7,
  const char8_t * const src8
);

/// \brief Add strings or fail if overflow
/// If overflow predicted, dst not changed
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \param[in] src6 pointer to a C string to add to dst
/// \param[in] src7 pointer to a C string to add to dst
/// \param[in] src8 pointer to a C string to add to dst
/// \param[in] src9 pointer to a C string to add to dst
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if dst or srcX are NULL or if overflow predicted
APEXUTILS_PUBLIC apex_ret_t apex_string_add9_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7,
  const char8_t * const src8,
  const char8_t * const src9
);

/// \brief Add strings or fail if overflow
/// If overflow predicted, dst not changed
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \param[in] src6 pointer to a C string to add to dst
/// \param[in] src7 pointer to a C string to add to dst
/// \param[in] src8 pointer to a C string to add to dst
/// \param[in] src9 pointer to a C string to add to dst
/// \param[in] src10 pointer to a C string to add to dst
/// \return return APEX_RET_OK if OK
/// \return return APEX_RET_ERROR if dst or srcX are NULL or if overflow predicted
APEXUTILS_PUBLIC apex_ret_t apex_string_add10_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7,
  const char8_t * const src8,
  const char8_t * const src9,
  const char8_t * const src10
);

///////////////////////////////////////////////////////////////////////////////
// "Silent" string add functions, possibly with trimming

/// \brief Add a string and return number of copied characters
/// If dst does not have space only part of src got copied or zero if there no place at all
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src pointer to a C string to add to dst
/// \return return number of copied characters, possibly zero
APEXUTILS_PUBLIC size64_t apex_string_add(
  apex_string_t * const dst,
  const char8_t * const src);

/// \brief Add strings or fail if overflow
/// If dst does not have space only part of src got copied or zero if there no place at all
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \return return number of copied characters, possibly zero
APEXUTILS_PUBLIC size64_t apex_string_add2(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2
);

/// \brief Add strings and return number of copied characters
/// If dst does not have space only part of src got copied or zero if there no place at all
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \return return number of copied characters, possibly zero
APEXUTILS_PUBLIC size64_t apex_string_add3(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3
);

/// \brief Add strings and return number of copied characters
/// If dst does not have space only part of src got copied or zero if there no place at all
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \return return number of copied characters, possibly zero
APEXUTILS_PUBLIC size64_t apex_string_add4(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4
);

/// \brief Add strings and return number of copied characters
/// If dst does not have space only part of src got copied or zero if there no place at all
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \return return number of copied characters, possibly zero
APEXUTILS_PUBLIC size64_t apex_string_add5(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5
);

/// \brief Add strings and return number of copied characters
/// If dst does not have space only part of src got copied or zero if there no place at all
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \param[in] src6 pointer to a C string to add to dst
/// \return return number of copied characters, possibly zero
APEXUTILS_PUBLIC size64_t apex_string_add6(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6
);

/// \brief Add strings and return number of copied characters
/// If dst does not have space only part of src got copied or zero if there no place at all
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \param[in] src6 pointer to a C string to add to dst
/// \param[in] src7 pointer to a C string to add to dst
/// \return return number of copied characters, possibly zero
APEXUTILS_PUBLIC size64_t apex_string_add7(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7
);

/// \brief Add strings and return number of copied characters
/// If dst does not have space only part of src got copied or zero if there no place at all
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \param[in] src6 pointer to a C string to add to dst
/// \param[in] src7 pointer to a C string to add to dst
/// \param[in] src8 pointer to a C string to add to dst
/// \return return number of copied characters, possibly zero
APEXUTILS_PUBLIC size64_t apex_string_add8(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7,
  const char8_t * const src8
);

/// \brief Add strings and return number of copied characters
/// If dst does not have space only part of src got copied or zero if there no place at all
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \param[in] src6 pointer to a C string to add to dst
/// \param[in] src7 pointer to a C string to add to dst
/// \param[in] src8 pointer to a C string to add to dst
/// \param[in] src9 pointer to a C string to add to dst
/// \return return number of copied characters, possibly zero
APEXUTILS_PUBLIC size64_t apex_string_add9(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7,
  const char8_t * const src8,
  const char8_t * const src9
);

/// \brief Add strings and return number of copied characters
/// If dst does not have space only part of src got copied or zero if there no place at all
/// \param[in, out] dst pointer to apex_string to modify
/// \param[in] src1 pointer to a C string to add to dst
/// \param[in] src2 pointer to a C string to add to dst
/// \param[in] src3 pointer to a C string to add to dst
/// \param[in] src4 pointer to a C string to add to dst
/// \param[in] src5 pointer to a C string to add to dst
/// \param[in] src6 pointer to a C string to add to dst
/// \param[in] src7 pointer to a C string to add to dst
/// \param[in] src8 pointer to a C string to add to dst
/// \param[in] src9 pointer to a C string to add to dst
/// \param[in] src10 pointer to a C string to add to dst
/// \return return number of copied characters, possibly zero
APEXUTILS_PUBLIC size64_t apex_string_add10(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7,
  const char8_t * const src8,
  const char8_t * const src9,
  const char8_t * const src10
);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // APEXUTILS__APEX_STRING_H_
