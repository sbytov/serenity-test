cmake_minimum_required (VERSION 3.9.3)

if (WIN32)
  set(CMAKE_SYSTEM_VERSION 8.1)
  # Minimum supported version of Windows is Windows 7
  # DBOOST_ALL_NO_LIB=1 is workaround for boost_1_66. Remove this when cmake get fixed
  add_definitions(-D_WIN32_WINNT=0x0601 -DUNICODE -D_UNICODE -DBOOST_ALL_NO_LIB=1)
endif()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
        if(NOT DEFINED WITH_IN_SOURCE_BUILD)
                message(FATAL_ERROR
                        "CMake generation is not allowed within the source directory!"
                        "\n Remove the CMakeCache.txt file and try again from another folder"
                )
        endif()
endif()

project (SerenityTestTask CXX C)
# Build in Debug mode by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build: Debug, Release, RelWithDebInfo, MinSizeRel" FORCE)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(DEPS_BUILD_TYPE "Debug")
else()
    set(DEPS_BUILD_TYPE "Release")
endif()

option(STATIC_RUNTIME_LINKING "links to the runtime libraries statically" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${${PROJECT_NAME}_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${${PROJECT_NAME}_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${${PROJECT_NAME}_BINARY_DIR}/lib")

set(CMAKE_CXX_STANDARD 14)

if(UNIX)
  if (CMAKE_COMPILER_IS_GNUCXX)
    # Temporary measure to escape million boost "auto_ptr deprecated" warnings
    add_compile_options("-Wall" "-fPIC" "-Wno-deprecated-declarations")

    if (STATIC_RUNTIME_LINKING)
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")
    endif()
  endif()

  add_definitions(-DPOSIX)
  add_definitions(-DLINUX)
elseif(MSVC)

  add_compile_options("/W3")
  add_compile_options("/MP")

  if (STATIC_RUNTIME_LINKING)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      add_compile_options("/MTd")
    else()
      add_compile_options("/MT")
    endif()
  else()
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      add_compile_options("/MDd")
    else()
      add_compile_options("/MD")
    endif()
  endif()
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -DDEBUG_MODE")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

include(functions)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

_guess_arch(TARGET_ARCH)

if (TARGET_ARCH)
  message(STATUS "Target architecture = ${TARGET_ARCH}")
else()
  message(FATAL_ERROR "Target architecture couldn't be detected")
endif()

# Will be set to "unknown" for non-VS compilers
_extract_vs_version(VS_VERSION)
if (MSVC AND NOT VS_VERSION)
  message(FATAL_ERROR "Visual Studio version couldn't be detected")
endif()

#find_package(PythonInterp 2.7.5 REQUIRED)

include_directories(
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_SOURCE_DIR}/src/cryptoCpp"
)

link_directories(
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_SOURCE_DIR}/src/cryptoCpp"
)

add_subdirectory(src)
