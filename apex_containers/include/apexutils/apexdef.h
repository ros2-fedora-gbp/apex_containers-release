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
/// \brief This file contains low level definitions for all Apex.AI code

#ifndef APEXUTILS__APEXDEF_H_
#define APEXUTILS__APEXDEF_H_

////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) && defined(_WIN64)
/// \def APEX_WINDOWS
/// \brief Defined to (1) if the target compiler is MSVC
  #define APEX_WINDOWS (1)  ///< Target OS
#elif defined(__GNUC__) && defined(__linux__)
/// \def APEX_LINUX
/// \brief Defined to (1) if the target compiler is gcc and the taget OS is Linux
  #define APEX_LINUX (1)  ///< Target OS
#elif defined(__GNUC__) && defined(__APPLE__)
/// \def APEX_OSX
/// \brief Defined to (1) if the target compiler is gcc and the taget OS is OSX
  #define APEX_OSX (1)  ///< Target OS
#elif defined(QNX)
/// \def APEX_QNX
/// \brief Defined to (1) if the target compiler is gcc and the taget OS is QNX
  #define APEX_QNX (1)  ///< Target OS
#else  // !(defined(__GNUC__) && defined(__APPLE__))
  #error "Unsupported Compiler and Target OS Combination"
#endif  // _MSC_VER

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

/// \defgroup apexos-macros Apex.OS macros and definitions
/// \brief A grouping of macros and definitions provided by Apex.OS.
/// @{

/// \defgroup apex-init-defs Init definitions
/// \ingroup apexos-macros
/// \brief A grouping of definitions for Apex.OS Initialization.
/// @{

////////////////////////////////////////////////////////////////////////////////
// OR combination of Apex.OS initializing bits (flags) used during Apex.OS initialization.
// E.g. (APEX_INIT_BIT_FLAG_1 | APEX_INIT_BIT_FLAG_2).
// Note '|' operator (bitwise OR)

/// \def APEX_INIT_BIT_SCHED_FIFO
/// \brief Use SCHED_FIFO scheduling in POSIX OS.
/// This flag is overridden by APEX_INIT_BIT_SCHED_RR:
/// when both flags being used, APEX_INIT_BIT_SCHED_RR is assumed.
/// If no flag was set, SCHED_OTHER scheduler is used.
/// See https://en.wikipedia.org/wiki/Scheduling_(computing)#First_come,_first_served.
#define APEX_INIT_BIT_SCHED_FIFO    (1ULL)

/// \def APEX_INIT_BIT_SCHED_RR
/// \brief Use SCHED_RR scheduling in POSIX OS (Windows ignores this flag).
/// This flag is overrides APEX_INIT_BIT_SCHED_FIFO flag:
/// when both flags being used, APEX_INIT_BIT_SCHED_RR is assumed.
/// If no flag was set, SCHED_OTHER scheduler is used.
/// See https://en.wikipedia.org/wiki/Round-robin_scheduling for details.
#define APEX_INIT_BIT_SCHED_RR      (2ULL)

/// \def APEX_INIT_BIT_NO_CONSOLE
/// \brief Do not init Apex.OS console. All console logging calls will silently fail.
#define APEX_INIT_BIT_NO_CONSOLE    (4ULL)

/// \def APEX_INIT_BIT_TEST
/// \brief Bit no. 63 (0..63) is used for internal tests. It is ignored by Apex.OS code
#define APEX_INIT_BIT_TEST    (0x8000000000000000ULL)

/// @}  End of apex-init-defs

////////////////////////////////////////////////////////////////////////////////
/// \def APEX_ULLONG_MAX
/// \brief GCC ULLONG_MAX violates MISRA C:2012 10.4: mixes signed and unsigned
#define APEX_ULLONG_MAX (0xFFFFFFFFFFFFFFFFULL)

////////////////////////////////////////////////////////////////////////////////
/// \def APEX_MILLION
/// \brief define APEX_MILLION: too many zeroes to count
#define APEX_MILLION (1000LL * 1000LL)

/// \def APEX_BILLION
/// \brief define APEX_BILLION: too many zeroes to count
#define APEX_BILLION (APEX_MILLION * 1000LL)

////////////////////////////////////////////////////////////////////////////////
/// \defgroup apexos-return Return Value Definitions
/// \brief A grouping of Apex.OS Return Value Definitions
/// @{

/// \def APEX_RET_OK
/// \brief Define OK return status
#define APEX_RET_OK (0)

/// \def APEX_RET_ERROR
/// \brief Define error return status (!= 0)
#define APEX_RET_ERROR (-1)

/// \def APEX_RET_TIMEOUT
/// \brief Timeout state
#define APEX_RET_TIMEOUT (-2)

#ifdef APEX_WINDOWS
/// \def APEX_RET_RESOURCE_ABANDONED
/// \brief Error state: the object in question is bad
#define APEX_RET_RESOURCE_ABANDONED (-3)
#endif  // APEX_WINDOWS

/// \def APEX_RET_RESOURCE_LEAK
/// \brief A resource freeing function failed to free a resource
#define APEX_RET_RESOURCE_LEAK (-4)

/// \def APEX_RET_WRONG_IP4
/// \brief UDP client received data from unexpected IP4 server
#define APEX_RET_WRONG_IP4 (-5)

/// \def APEX_RET_STILL_ACTIVE
/// \brief The object in question is still active
#define APEX_RET_STILL_ACTIVE (-6)

/// \def APEX_RET_OVERFLOW
/// \brief Overflow of queue_c, value or buffer happened
#define APEX_RET_OVERFLOW (-7)

/// \def APEX_RET_UNDERFLOW
/// \brief Underflow of queue_c, value or buffer happened
#define APEX_RET_UNDERFLOW (-8)

/// \def APEX_RET_RESOURCE_EXIST
/// \brief The resource in question already exists
#define APEX_RET_RESOURCE_EXIST (-9)

/// \def APEX_RET_MEM_ALIGN_ERR
/// \brief Error aligning memory
#define APEX_RET_MEM_ALIGN_ERR (-10)

/// \def APEX_RET_LOCKED_BY_OTHER
/// \brief The resource is locked by other thread
#define APEX_RET_LOCKED_BY_OTHER (-11)

#if defined(APEX_LINUX) || defined(APEX_QNX)
/// \def APEX_RET_MEM_ALLOC_OPT_ERR
/// \brief Error setting memory allocation options
#define APEX_RET_MEM_ALLOC_OPT_ERR (-11)

/// \def APEX_RET_MEM_LOCK_ERR
/// \brief Error locking memory
#define APEX_RET_MEM_LOCK_ERR (-12)

/// \def APEX_RET_SET_CPU_AFFINITY_ERR
/// \brief Error setting cpu affinities of the tasks
#define APEX_RET_SET_CPU_AFFINITY_ERR (-13)
#endif  // APEX_LINUX || APEX_QNX

/// @}

////////////////////////////////////////////////////////////////////////////////
/// \def APEX_STRING_SIZE
/// \brief Define Apex.OS fixed size string
#define APEX_STRING_SIZE (256ULL)

/// @}  End of apexos-macros group

////////////////////////////////////////////////////////////////////////////////
/// \typedef bool8_t
/// \brief define generic bool8_t
typedef bool bool8_t;

////////////////////////////////////////////////////////////////////////////////
/// \typedef char8_t
/// \brief defines generic signed 8-bit character
typedef char char8_t;

/// \typedef float64_t
/// \brief defines generic 64-bit float type
typedef double float64_t;

/// \typedef float32_t
/// \brief defines generic 32-bit float type
typedef float float32_t;

/// \typedef apex_ret_t
/// \brief defines return type of an Apex.OS function
/// It follows POSIX convention: 0 (APEX_RET_OK) means OK, non-zero means an error
typedef int32_t apex_ret_t;

#ifdef APEX_WINDOWS
/// \typedef apex_socket_t
/// \brief defines portable socket type
typedef uint64_t apex_socket_t;
#else  // APEX_WINDOWS
/// \typedef apex_socket_t
/// \brief defines portable socket type
typedef int32_t apex_socket_t;
#endif  // APEX_WINDOWS


////////////////////////////////////////////////////////////////////////////////
/// \typedef size64_t
/// \brief forces value of sizeof operator to 64 bit everywhere in the Apex.OS code
typedef uint64_t size64_t;

#ifdef __cplusplus
}
#endif  // __cplusplus

////////////////////////////////////////////////////////////////////////////////

#endif  // APEXUTILS__APEXDEF_H_
