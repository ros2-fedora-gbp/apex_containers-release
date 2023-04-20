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

#include <string.h>
#include <apexutils/apex_string.h>

/// fill the apex_string_t object with zeroes
apex_ret_t apex_string_clean(apex_string_t * const str)
{
  apex_ret_t retval = APEX_RET_ERROR;
  if (str != NULL) {
    // Ignore returned self reference
    (void)memset(str->c_str, 0, sizeof(str->c_str));
    retval = APEX_RET_OK;
  }
  return retval;
}

/// Create apex_string_t object on the stack
apex_string_t apex_string_create(void)
{
  apex_string_t retval;
  // Ignore returned self reference
  (void)memset(&retval, 0, sizeof(retval.c_str));
  return retval;
}

/// Add a string or fail if overflow
apex_ret_t apex_string_add_strict(apex_string_t * const dst, const char8_t * const src)
{
  apex_ret_t retval = APEX_RET_ERROR;
  if ((dst != NULL) && (src != NULL)) {
    size64_t my_length = strnlen(dst->c_str, APEX_STRING_SIZE);
    size64_t their_length = strnlen(src, APEX_STRING_SIZE);
    if ((my_length + their_length + 1U) <= APEX_STRING_SIZE) {
      // Ignore returned self refrence
      (void)strncat(dst->c_str, src, their_length);
      dst->c_str[my_length + their_length] = '\0';
      retval = APEX_RET_OK;
    }
  }
  return retval;
}

/// Add strings or fail if overflow
apex_ret_t apex_string_add2_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2)
{
  apex_ret_t retval = apex_string_add_strict(dst, src1);
  if (retval == APEX_RET_OK) {
    retval = apex_string_add_strict(dst, src2);
  }
  return retval;
}

/// Add strings or fail if overflow
apex_ret_t apex_string_add3_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3
)
{
  apex_ret_t retval = apex_string_add2_strict(dst, src1, src2);
  if (retval == APEX_RET_OK) {
    retval = apex_string_add_strict(dst, src3);
  }
  return retval;
}

/// Add strings or fail if overflow
apex_ret_t apex_string_add4_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4
)
{
  apex_ret_t retval = apex_string_add3_strict(dst, src1, src2, src3);
  if (retval == APEX_RET_OK) {
    retval = apex_string_add_strict(dst, src4);
  }
  return retval;
}

/// Add strings or fail if overflow
apex_ret_t apex_string_add5_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5
)
{
  apex_ret_t retval = apex_string_add4_strict(dst, src1, src2, src3, src4);
  if (retval == APEX_RET_OK) {
    retval = apex_string_add_strict(dst, src5);
  }
  return retval;
}

/// Add strings or fail if overflow
apex_ret_t apex_string_add6_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6
)
{
  apex_ret_t retval = apex_string_add5_strict(dst, src1, src2, src3, src4, src5);
  if (retval == APEX_RET_OK) {
    retval = apex_string_add_strict(dst, src6);
  }
  return retval;
}

/// Add strings or fail if overflow
apex_ret_t apex_string_add7_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7
)
{
  apex_ret_t retval = apex_string_add6_strict(dst, src1, src2, src3, src4, src5, src6);
  if (retval == APEX_RET_OK) {
    retval = apex_string_add_strict(dst, src7);
  }
  return retval;
}

/// Add strings or fail if overflow
apex_ret_t apex_string_add8_strict(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7,
  const char8_t * const src8
)
{
  apex_ret_t retval = apex_string_add7_strict(dst, src1, src2, src3, src4, src5, src6, src7);
  if (retval == APEX_RET_OK) {
    retval = apex_string_add_strict(dst, src8);
  }
  return retval;
}

/// Add strings or fail if overflow
apex_ret_t apex_string_add9_strict(
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
)
{
  apex_ret_t retval = apex_string_add8_strict(dst, src1, src2, src3, src4, src5, src6, src7, src8);
  if (retval == APEX_RET_OK) {
    retval = apex_string_add_strict(dst, src9);
  }
  return retval;
}

/// Add strings or fail if overflow
apex_ret_t apex_string_add10_strict(
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
)
{
  apex_ret_t retval = apex_string_add9_strict(
    dst, src1, src2, src3, src4, src5, src6, src7, src8, src9);
  if (retval == APEX_RET_OK) {
    retval = apex_string_add_strict(dst, src10);
  }
  return retval;
}

///////////////////////////////////////////////////////////////////////////////
// "Silent" string add functions, possibly with trimming

size64_t apex_string_add(
  apex_string_t * const dst,
  const char8_t * const src)
{
  size64_t copy_size = 0;
  if ((dst != NULL) && (src != NULL)) {
    const size64_t my_length = strnlen(dst->c_str, APEX_STRING_SIZE);
    if (my_length < (APEX_STRING_SIZE - 1U)) {
      const size64_t their_length = strnlen(src, APEX_STRING_SIZE);
      if (their_length > 0U) {
        const size64_t max_copy_size = (APEX_STRING_SIZE - 1U) - my_length;
        copy_size = (max_copy_size < their_length) ? max_copy_size : their_length;
        (void)memmove(dst->c_str + my_length, src, copy_size);
        dst->c_str[my_length + copy_size] = '\0';
      }
    }
  }
  return copy_size;
}

size64_t apex_string_add2(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2)
{
  size64_t retval = apex_string_add(dst, src1);
  retval += apex_string_add(dst, src2);
  return retval;
}

size64_t apex_string_add3(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3
)
{
  size64_t retval = apex_string_add2(dst, src1, src2);
  retval += apex_string_add(dst, src3);
  return retval;
}

size64_t apex_string_add4(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4
)
{
  size64_t retval = apex_string_add3(dst, src1, src2, src3);
  retval += apex_string_add(dst, src4);
  return retval;
}

size64_t apex_string_add5(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5
)
{
  size64_t retval = apex_string_add4(dst, src1, src2, src3, src4);
  retval += apex_string_add(dst, src5);
  return retval;
}

size64_t apex_string_add6(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6
)
{
  size64_t retval = apex_string_add5(dst, src1, src2, src3, src4, src5);
  retval += apex_string_add(dst, src6);
  return retval;
}

size64_t apex_string_add7(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7
)
{
  size64_t retval = apex_string_add6(dst, src1, src2, src3, src4, src5, src6);
  retval += apex_string_add(dst, src7);
  return retval;
}

size64_t apex_string_add8(
  apex_string_t * const dst,
  const char8_t * const src1,
  const char8_t * const src2,
  const char8_t * const src3,
  const char8_t * const src4,
  const char8_t * const src5,
  const char8_t * const src6,
  const char8_t * const src7,
  const char8_t * const src8
)
{
  size64_t retval = apex_string_add7(dst, src1, src2, src3, src4, src5, src6, src7);
  retval += apex_string_add(dst, src8);
  return retval;
}

size64_t apex_string_add9(
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
)
{
  size64_t retval = apex_string_add8(dst, src1, src2, src3, src4, src5, src6, src7, src8);
  retval += apex_string_add(dst, src9);
  return retval;
}

size64_t apex_string_add10(
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
)
{
  size64_t retval = apex_string_add9(dst, src1, src2, src3, src4, src5, src6, src7, src8, src9);
  retval += apex_string_add(dst, src10);
  return retval;
}
