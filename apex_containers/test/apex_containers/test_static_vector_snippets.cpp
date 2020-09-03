// Copyright 2018 Apex.AI, Inc.
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
/// \brief Unit tests for the static_vector example snippets

#include <gtest/gtest.h>
#include <apex_containers/static_vector.hpp>

TEST(static_vector, documentation) {
  ASSERT_ANY_THROW(
  {
    //! [Basic Static Vector]
    // Allocates all the memory on construction for 500 elements
    apex::static_vector<int> v(500);
    // Trying to allocate more than the initial amount of memory will throw an exception.
    v.resize(501);
    //! [Basic Static Vector]
  });
}
