# CMakeLists.txt for unit tests
cmake_minimum_required(VERSION 3.10)

# Enable testing
enable_testing()

# Find or download Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include directories
include_directories(BEFORE
  ${googletest_SOURCE_DIR}/googletest/include
  ${googletest_SOURCE_DIR}/googletest)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Test executables
set(TEST_SOURCES
  test_gram_encoding.cc
  test_gram_db.cc
  test_octagram.cc
)

# Create test executable
add_executable(rime_octagram_tests ${TEST_SOURCES})
target_compile_definitions(rime_octagram_tests PRIVATE GLOG_USE_GLOG_EXPORT)

if(RIME_ROOT_DIR AND EXISTS "${RIME_ROOT_DIR}/src/rime/common.h")
  target_include_directories(rime_octagram_tests PRIVATE
    "${RIME_ROOT_DIR}/src"
    "${RIME_ROOT_DIR}/include")
  if(EXISTS "${RIME_BUILD_DIR_VAR}/rime/build_config.h")
    target_include_directories(rime_octagram_tests PRIVATE "${RIME_BUILD_DIR_VAR}")
  elseif(EXISTS "${RIME_BUILD_DIR_VAR}/src/rime/build_config.h")
    target_include_directories(rime_octagram_tests PRIVATE "${RIME_BUILD_DIR_VAR}/src")
  endif()
  if(EXISTS "${RIME_ROOT_DIR}/deps/boost-1.89.0")
    target_include_directories(rime_octagram_tests PRIVATE "${RIME_ROOT_DIR}/deps/boost-1.89.0")
  endif()
endif()

# Set C++ standard (required for std::filesystem)
set_target_properties(rime_octagram_tests PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

target_link_libraries(rime_octagram_tests
  PRIVATE
  $<TARGET_OBJECTS:rime-octagram-objs>
  GTest::gtest
  GTest::gtest_main
  ${octagram_plugin_deps}
  dbghelp
)

if(KENLM_FOUND)
  if(TARGET kenlm::kenlm)
    target_link_libraries(rime_octagram_tests PRIVATE kenlm::kenlm)
  elseif(TARGET kenlm)
    target_link_libraries(rime_octagram_tests PRIVATE kenlm)
  elseif(KENLM_LIBRARY)
    target_link_libraries(rime_octagram_tests PRIVATE ${KENLM_LIBRARY})
  endif()
endif()
if(RIME_ROOT_DIR AND EXISTS "${RIME_ROOT_DIR}/lib/glog.lib")
  target_link_libraries(rime_octagram_tests PRIVATE "${RIME_ROOT_DIR}/lib/glog.lib")
endif()

# For older compilers, may need to link filesystem library
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(rime_octagram_tests PRIVATE stdc++fs)
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(rime_octagram_tests PRIVATE c++fs)
endif()

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(rime_octagram_tests)

add_executable(rime_octagram_smoke smoke_backend.cc)
set_target_properties(rime_octagram_smoke PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON)
if(MSVC)
  set_target_properties(rime_octagram_smoke PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
target_compile_definitions(rime_octagram_smoke PRIVATE GLOG_USE_GLOG_EXPORT)
if(RIME_ROOT_DIR AND EXISTS "${RIME_ROOT_DIR}/src/rime/common.h")
  target_include_directories(rime_octagram_smoke PRIVATE
    "${RIME_ROOT_DIR}/src"
    "${RIME_ROOT_DIR}/include")
  if(EXISTS "${RIME_BUILD_DIR_VAR}/rime/build_config.h")
    target_include_directories(rime_octagram_smoke PRIVATE "${RIME_BUILD_DIR_VAR}")
  elseif(EXISTS "${RIME_BUILD_DIR_VAR}/src/rime/build_config.h")
    target_include_directories(rime_octagram_smoke PRIVATE "${RIME_BUILD_DIR_VAR}/src")
  endif()
  if(EXISTS "${RIME_ROOT_DIR}/deps/boost-1.89.0")
    target_include_directories(rime_octagram_smoke PRIVATE "${RIME_ROOT_DIR}/deps/boost-1.89.0")
  endif()
endif()
target_link_libraries(rime_octagram_smoke
  PRIVATE
  $<TARGET_OBJECTS:rime-octagram-objs>
  ${octagram_plugin_deps}
  dbghelp)
if(KENLM_FOUND)
  if(TARGET kenlm::kenlm)
    target_link_libraries(rime_octagram_smoke PRIVATE kenlm::kenlm)
  elseif(TARGET kenlm)
    target_link_libraries(rime_octagram_smoke PRIVATE kenlm)
  elseif(KENLM_LIBRARY)
    target_link_libraries(rime_octagram_smoke PRIVATE ${KENLM_LIBRARY})
  endif()
endif()
if(RIME_ROOT_DIR AND EXISTS "${RIME_ROOT_DIR}/lib/glog.lib")
  target_link_libraries(rime_octagram_smoke PRIVATE "${RIME_ROOT_DIR}/lib/glog.lib")
endif()
add_test(NAME rime_octagram_smoke COMMAND rime_octagram_smoke)

