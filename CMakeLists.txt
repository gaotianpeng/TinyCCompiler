cmake_minimum_required(VERSION 3.9)
project(tinycompiler VERSION 0.0 LANGUAGES C CXX)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 11)
  set(CMAKE_C_STANDARD_REQUIRED OFF)
  set(CMAKE_C_EXTENSIONS OFF)
endif(NOT CMAKE_C_STANDARD)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT CMAKE_CXX_STANDARD)

option(TINYCOMPILER_BUILD_TESTS "Build tinycompiler's unit tests" ON)
option(TINYCOMPILER_BUILD_BENCHMARKS "Build tinycompiler's benchmarks" ON)
option(TINYCOMPILER_INSTALL "Install tinycompiler's header and library" ON)


if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Disable C++ exceptions.
  string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c-")
  add_definitions(-D_HAS_EXCEPTIONS=0)

  # Disable RTTI.
  string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
else(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Enable strict prototype warnings for C code in clang and gcc.
  if(NOT CMAKE_C_FLAGS MATCHES "-Wstrict-prototypes")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-prototypes")
  endif(NOT CMAKE_C_FLAGS MATCHES "-Wstrict-prototypes")

  # Disable C++ exceptions.
  string(REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

  # Disable RTTI.
  string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wthread-safety HAVE_CLANG_THREAD_SAFETY)

# Used by googletest.
check_cxx_compiler_flag(-Wno-missing-field-initializers
                        tinycompiler_HAVE_NO_MISSING_FIELD_INITIALIZERS)

if(BUILD_SHARED_LIBS)
    # Only export LEVELDB_EXPORT symbols from the shared library.
    add_compile_options(-fvisibility=hidden)
endif(BUILD_SHARED_LIBS)

# GoogleTest triggers a missing field initializers warning.
if(TINYCOMPILER_HAVE_NO_MISSING_FIELD_INITIALIZERS)
  set_property(TARGET gtest
      APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
  set_property(TARGET gmock
      APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
endif(TINYCOMPILER_HAVE_NO_MISSING_FIELD_INITIALIZERS)

include(GNUInstallDirs)

add_executable(tinycompiler "")
target_sources(tinycompiler
PUBLIC
  "main.cc"
)

find_package(Threads REQUIRED)
target_link_libraries(tinycompiler Threads::Threads)

enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(install_gtest OFF)
set(install_gmock OFF)
set(build_gmock ON)
add_subdirectory("third_party/googletest")

add_executable(tinycompiler_tests "")
target_include_directories(tinycompiler_tests
  PUBLIC
  ${PROJECT_SOURCE_DIR}/parser/
  ${PROJECT_SOURCE_DIR}/common/)

target_sources(tinycompiler_tests
  PUBLIC
  "ut_scanner.cc"
  "parser/scanner.h"
  "parser/scanner.cc"
  "parser/token.h"
  "parser/token.cc"
  "ut_token.cc"
)                    
target_link_libraries(tinycompiler_tests gmock gtest gtest_main)