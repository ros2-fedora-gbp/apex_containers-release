// Copyright 2017-2019 Apex.AI, Inc.
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

/// All rights reserved.
/// \file
/// \brief This file provides visibility control for apexutils shared library

#ifndef APEXUTILS__VISIBILITY_CONTROL_H_
#define APEXUTILS__VISIBILITY_CONTROL_H_

#include <apexutils/apexdef.h>

////////////////////////////////////////////////////////////////////////////////
#if defined(APEX_WINDOWS)
  #define APEXUTILS_PUBLIC_DEPRECATED(msg) __declspec(dllexport)[[deprecatedmsg]]
  #if defined(APEXUTILS_BUILDING_DLL) || defined(APEXUTILS_EXPORTS)
    #define APEXUTILS_PUBLIC __declspec(dllexport)
    #define APEXUTILS_LOCAL
  #else  // defined(APEXUTILS_BUILDING_DLL) || defined(APEXUTILS_EXPORTS)
    #define APEXUTILS_PUBLIC __declspec(dllimport)
    #define APEXUTILS_LOCAL
  #endif  // defined(APEXUTILS_BUILDING_DLL) || defined(APEXUTILS_EXPORTS)
#elif defined(APEX_LINUX)
  #define APEXUTILS_PUBLIC_DEPRECATED(msg) __attribute__((visibility("default"), deprecated(msg)))
  #define APEXUTILS_PUBLIC __attribute__((visibility("default")))
  #define APEXUTILS_LOCAL __attribute__((visibility("hidden")))
#elif defined(APEX_OSX)
  #define APEXUTILS_PUBLIC_DEPRECATED(msg) __attribute__((visibility("default"), deprecated(msg)))
  #define APEXUTILS_PUBLIC __attribute__((visibility("default")))
  #define APEXUTILS_LOCAL __attribute__((visibility("hidden")))
#elif defined(APEX_QNX)
  #define APEXUTILS_PUBLIC_DEPRECATED(msg) __attribute__((visibility("default"), deprecated(msg)))
  #define APEXUTILS_PUBLIC __attribute__((visibility("default")))
  #define APEXUTILS_LOCAL __attribute__((visibility("hidden")))
#else  // defined(APEX_LINUX)
  #error "Unsupported Build Configuration"
#endif  // defined(APEX_WINDOWS)

#endif  // APEXUTILS__VISIBILITY_CONTROL_H_
