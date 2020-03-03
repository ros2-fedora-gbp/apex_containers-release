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

/// \file
/// \brief This file provides visibility control for apex containers shared library

#ifndef APEX_CONTAINERS__VISIBILITY_CONTROL_HPP_
#define APEX_CONTAINERS__VISIBILITY_CONTROL_HPP_

#include <apexutils/apexdef.h>

////////////////////////////////////////////////////////////////////////////////
#if defined(APEX_WINDOWS)
  #if defined(APEXCPP_BUILDING_DLL) || defined(APEXCPP_EXPORTS)
    #define APEX_CONTAINERS_PUBLIC __declspec(dllexport)
    #define APEX_CONTAINERS_LOCAL
  #else  // defined(APEXCPP_BUILDING_DLL) || defined(APEXCPP_EXPORTS)
    #define APEX_CONTAINERS_PUBLIC __declspec(dllimport)
    #define APEX_CONTAINERS_LOCAL
  #endif  // defined(APEXCPP_BUILDING_DLL) || defined(APEXCPP_EXPORTS)
#elif defined(APEX_LINUX)
  #define APEX_CONTAINERS_PUBLIC __attribute__((visibility("default")))
  #define APEX_CONTAINERS_LOCAL __attribute__((visibility("hidden")))
#elif defined(APEX_OSX)
  #define APEX_CONTAINERS_PUBLIC __attribute__((visibility("default")))
  #define APEX_CONTAINERS_LOCAL __attribute__((visibility("hidden")))
#elif defined(APEX_QNX)
  #define APEX_CONTAINERS_PUBLIC __attribute__((visibility("default")))
  #define APEX_CONTAINERS_LOCAL __attribute__((visibility("hidden")))
#else  // defined(APEX_LINUX)
  #error "Unsupported Build Configuration"
#endif  // defined(APEX_WINDOWS)

#endif  // APEX_CONTAINERS__VISIBILITY_CONTROL_HPP_
