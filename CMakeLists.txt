cmake_minimum_required(VERSION 3.10)
project(rime-octagram)

if(MSVC AND POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG
                   CMAKE_C_FLAGS CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_DEBUG)
    if(${flag_var})
      string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      string(REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif()
  endforeach()
endif()

option(BUILD_TOOLS "Build tools" ON)
option(ENABLE_KENLM "Enable KenLM backend" ON)
set(KENLM_MAX_ORDER 6 CACHE STRING "Maximum supported ngram order for embedded KenLM headers")

set(KENLM_ROOT_DIR "" CACHE PATH "Path to KenLM installation or build tree")
if(NOT KENLM_ROOT_DIR AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../kenlm/CMakeLists.txt")
  set(KENLM_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../kenlm" CACHE PATH "Path to KenLM installation or build tree" FORCE)
endif()
set(RIME_ROOT_DIR "" CACHE PATH "Path to the librime source tree for standalone builds")
set(RIME_BUILD_DIR_VAR "" CACHE PATH "Path to the librime build directory for standalone builds")

set(octagram_plugin_deps ${rime_library})
set(KENLM_FOUND FALSE)
unset(KENLM_INCLUDE_DIR)
unset(KENLM_LIBRARY)
unset(KENLM_UTIL_LIBRARY)
unset(GLOG_LIBRARY)

if(NOT rime_library)
  if(NOT RIME_ROOT_DIR)
    set(RIME_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../librime")
  endif()
  if(NOT RIME_BUILD_DIR_VAR)
    set(RIME_BUILD_DIR_VAR "${RIME_ROOT_DIR}/build")
  endif()
  if(NOT EXISTS "${RIME_ROOT_DIR}/src/rime/common.h")
    message(FATAL_ERROR "Set RIME_ROOT_DIR to a librime source tree before building standalone.")
  endif()
  find_library(rime_library
    NAMES rime
    PATHS
      "${RIME_ROOT_DIR}/dist/lib"
      "${RIME_ROOT_DIR}/build/src/Release"
      "${RIME_ROOT_DIR}/build/bin/Release"
      "${RIME_ROOT_DIR}/build/lib/Release"
      "${RIME_ROOT_DIR}/build"
    NO_DEFAULT_PATH)
  if(NOT rime_library)
    message(FATAL_ERROR "Failed to find librime library. Build librime first or set RIME_ROOT_DIR/RIME_BUILD_DIR_VAR correctly.")
  endif()
  set(octagram_plugin_deps ${rime_library})
  if(EXISTS "${RIME_ROOT_DIR}/lib/glog.lib")
    set(GLOG_LIBRARY "${RIME_ROOT_DIR}/lib/glog.lib")
  else()
    find_library(GLOG_LIBRARY
      NAMES glog
      PATHS
        "${RIME_ROOT_DIR}/lib"
        "${RIME_ROOT_DIR}/build/deps/glog/lib/Release"
        "${RIME_ROOT_DIR}/build/deps/glog/lib"
      NO_DEFAULT_PATH)
  endif()
  if(GLOG_LIBRARY)
    list(APPEND octagram_plugin_deps ${GLOG_LIBRARY})
  endif()
endif()

if(ENABLE_KENLM)
  set(KENLM_CONFIG_HINTS)
  if(KENLM_ROOT_DIR)
    list(APPEND KENLM_CONFIG_HINTS
      "${KENLM_ROOT_DIR}/build-msvc"
      "${KENLM_ROOT_DIR}/build"
      "${KENLM_ROOT_DIR}")
  endif()
  set(KENLM_TARGETS_FILE)
  foreach(_kenlm_hint ${KENLM_CONFIG_HINTS})
    if(EXISTS "${_kenlm_hint}/kenlmTargets.cmake")
      set(KENLM_TARGETS_FILE "${_kenlm_hint}/kenlmTargets.cmake")
      break()
    endif()
  endforeach()
  if(KENLM_TARGETS_FILE)
    find_package(kenlm CONFIG QUIET HINTS ${KENLM_CONFIG_HINTS})
  endif()
  if(kenlm_FOUND)
    set(KENLM_FOUND TRUE)
  elseif(KENLM_ROOT_DIR)
    find_path(KENLM_INCLUDE_DIR
      NAMES lm/model.hh
      PATHS
        "${KENLM_ROOT_DIR}/include"
        "${KENLM_ROOT_DIR}")
    find_library(KENLM_LIBRARY
      NAMES kenlm
      PATHS
        "${KENLM_ROOT_DIR}/lib"
        "${KENLM_ROOT_DIR}/build-msvc/lib/Release"
        "${KENLM_ROOT_DIR}/build-msvc/lib"
        "${KENLM_ROOT_DIR}/build-msvc/bin/Release"
        "${KENLM_ROOT_DIR}/build/lib"
        "${KENLM_ROOT_DIR}/build/lib/Release"
        "${KENLM_ROOT_DIR}/build/lm/Release"
        "${KENLM_ROOT_DIR}/build/Release"
        "${KENLM_ROOT_DIR}/build/bin")
    find_library(KENLM_UTIL_LIBRARY
      NAMES kenlm_util
      PATHS
        "${KENLM_ROOT_DIR}/lib"
        "${KENLM_ROOT_DIR}/build-msvc/lib/Release"
        "${KENLM_ROOT_DIR}/build-msvc/lib"
        "${KENLM_ROOT_DIR}/build/lib"
        "${KENLM_ROOT_DIR}/build/lib/Release"
        "${KENLM_ROOT_DIR}/build/Release")
    if(KENLM_INCLUDE_DIR AND KENLM_LIBRARY AND KENLM_UTIL_LIBRARY)
      set(KENLM_FOUND TRUE)
    endif()
  endif()
endif()

aux_source_directory(src octagram_src)

add_library(rime-octagram-objs OBJECT ${octagram_src})
set_target_properties(rime-octagram-objs PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON)
if(MSVC)
  set_target_properties(rime-octagram-objs PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
target_compile_definitions(rime-octagram-objs PRIVATE GLOG_USE_GLOG_EXPORT)
if(BUILD_SHARED_LIBS)
  set_target_properties(rime-octagram-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
endif()

if(RIME_ROOT_DIR AND EXISTS "${RIME_ROOT_DIR}/src/rime/common.h")
  target_include_directories(rime-octagram-objs PRIVATE
    "${RIME_ROOT_DIR}/src"
    "${RIME_ROOT_DIR}/include")
  if(EXISTS "${RIME_BUILD_DIR_VAR}/rime/build_config.h")
    target_include_directories(rime-octagram-objs PRIVATE "${RIME_BUILD_DIR_VAR}")
  elseif(EXISTS "${RIME_BUILD_DIR_VAR}/src/rime/build_config.h")
    target_include_directories(rime-octagram-objs PRIVATE "${RIME_BUILD_DIR_VAR}/src")
  endif()
  if(EXISTS "${RIME_ROOT_DIR}/deps/boost-1.89.0")
    target_include_directories(rime-octagram-objs PRIVATE "${RIME_ROOT_DIR}/deps/boost-1.89.0")
  endif()
endif()

if(KENLM_FOUND)
  target_compile_definitions(rime-octagram-objs PRIVATE RIME_OCTAGRAM_ENABLE_KENLM=1)
  target_compile_definitions(rime-octagram-objs PRIVATE KENLM_MAX_ORDER=${KENLM_MAX_ORDER})
  if(kenlm_FOUND)
    if(TARGET kenlm::kenlm)
      get_target_property(KENLM_IMPORTED_INCLUDE_DIRS kenlm::kenlm INTERFACE_INCLUDE_DIRECTORIES)
      if(KENLM_IMPORTED_INCLUDE_DIRS)
        target_include_directories(rime-octagram-objs PRIVATE ${KENLM_IMPORTED_INCLUDE_DIRS})
      endif()
      list(APPEND octagram_plugin_deps kenlm::kenlm)
    elseif(TARGET kenlm)
      get_target_property(KENLM_IMPORTED_INCLUDE_DIRS kenlm INTERFACE_INCLUDE_DIRECTORIES)
      if(KENLM_IMPORTED_INCLUDE_DIRS)
        target_include_directories(rime-octagram-objs PRIVATE ${KENLM_IMPORTED_INCLUDE_DIRS})
      endif()
      list(APPEND octagram_plugin_deps kenlm)
    endif()
  else()
    target_include_directories(rime-octagram-objs PRIVATE ${KENLM_INCLUDE_DIR})
    list(APPEND octagram_plugin_deps ${KENLM_LIBRARY} ${KENLM_UTIL_LIBRARY})
  endif()
else()
  message(STATUS "KenLM support disabled or not found; only legacy octagram backend will be available.")
endif()

if(DEFINED rime_library)
  set(plugin_name rime-octagram PARENT_SCOPE)
  set(plugin_objs $<TARGET_OBJECTS:rime-octagram-objs> PARENT_SCOPE)
  set(plugin_deps ${octagram_plugin_deps} PARENT_SCOPE)
  set(plugin_modules "octagram" PARENT_SCOPE)
endif()

if(BUILD_TOOLS)
  add_subdirectory(tools)
endif()

# Enable testing if requested
option(BUILD_TESTING "Build unit tests" OFF)
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
