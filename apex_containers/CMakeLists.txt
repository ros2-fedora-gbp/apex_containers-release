cmake_minimum_required(VERSION 3.5)

project(apex_containers)

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

include_directories(include)

set(apex_containers_sources
    include/apex_containers/memory/common.hpp
    include/apex_containers/forward_list.hpp
    include/apex_containers/list.hpp
    include/apex_containers/map.hpp
    include/apex_containers/set.hpp
    include/apex_containers/static_vector.hpp
    include/apex_containers/string.hpp
    include/apexutils/apexdef.h
    include/apexutils/apex_string.h
    include/apexutils/visibility_control.h
    include/apex_containers/unordered_map.hpp
    include/apex_containers/unordered_set.hpp
    include/apex_containers/visibility_control.hpp
    src/string/to_string.cpp
    src/apex_string.c)

set_source_files_properties(${apex_containers_sources} PROPERTIES language "CXX")
add_library(${PROJECT_NAME} SHARED ${apex_containers_sources})
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall
  -Werror
  -Wextra
  -pedantic
  -Wcast-align
  -Wunused
  -Wconversion
  -Wsign-conversion
  -Wdouble-promotion
  -fvisibility=hidden)
  # C++-only options
target_compile_options(${PROJECT_NAME}
  PRIVATE $<$<COMPILE_LANGUAGE:CXX>: -Woverloaded-virtual -Wold-style-cast>)

ament_target_dependencies(${PROJECT_NAME}
    foonathan_memory_vendor)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  find_package(apex_test_tools REQUIRED)
  ament_lint_auto_find_test_dependencies()

  apex_test_tools_add_gtest(test_apex_containers
      test/test_main.cpp
      test/apex_containers/test_shared_ptr.cpp
      test/apex_containers/test_static_allocators_vectors.cpp
      test/apex_containers/test_static_allocators_basic.cpp
      test/apex_containers/test_static_vector_snippets.cpp
      test/string/test_string.cpp
      test/string/test_string_strict.cpp
      test/string/test_to_string.cpp)
  target_link_libraries(test_apex_containers ${PROJECT_NAME})
endif()

list(APPEND ${PROJECT_NAME}_LIBRARIES
  "${PROJECT_NAME}"
)

ament_auto_package()
