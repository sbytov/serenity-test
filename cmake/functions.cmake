function(_is_subproject _var)
  if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    # Base source dir is our source dir - we are top-level
    set(${_var} NO PARENT_SCOPE)
  else()
    # Base source dir is not our source dir - we are a subproject
    set(${_var} YES PARENT_SCOPE)
  endif()
endfunction()

#==============================================================================

function(_add_groups FILE)
      get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
      string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

      # TODO betters solution for complicated paths
      string(REPLACE "../" "" GROUP "${GROUP}")

      string(REPLACE "/" "\\" GROUP "${GROUP}")

      if ("${FILE}" MATCHES ".*\\.{cpp, h}")
         set(GROUP "${GROUP}\\Source Files")
      elseif("${FILE}" MATCHES ".*\\Interface.h")
         set(GROUP "${GROUP}\\Header Files")
      endif()

      source_group("${GROUP}" FILES "${FILE}")
endfunction()

#==============================================================================

function(_add_sources_recurse path sources)
  file(GLOB_RECURSE temp "${path}/*.cpp" "${path}/*.h")

  set(${sources}
      ${${sources}}
      ${temp}
      PARENT_SCOPE
  )
  foreach(FILE ${temp})
    _add_groups(${FILE})
  endforeach()
endfunction()

#==============================================================================

function(_add_sources path sources)
  file(GLOB temp "${path}/*.cpp" "${path}/*.h" "${path}/*.c")
  #include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${path}" PARENT_SCOPE)
  set(${sources}
      ${${sources}}
      ${temp}
      PARENT_SCOPE
  )
  foreach(FILE ${temp})
    _add_groups(${FILE})
  endforeach()
endfunction()

#==============================================================================

macro(add_boost_test args)

message(STATUS "CREATE boost unittest ${ARGV0}")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable(${ARGV0} ${ARGN})
set_target_properties(${ARGV0} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${ARGV0} PROPERTIES FOLDER ${TESTS_FOLDERS})
add_test(NAME ${ARGV0} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ARGV0})

if(MSVC)
    set_target_properties(${ARGV0} PROPERTIES COMPILE_FLAGS "/EHs")
endif(MSVC)

target_link_libraries(${ARGV0} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

if(WIN32)
    target_link_libraries(
        ${ARGV0}
        ws2_32
        winmm
        crypt32
        psapi
    )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(
        ${ARGV0}
        pthread
        rt
    )
endif()

endmacro(add_boost_test)

#==============================================================================

function(_guess_arch var)
  if (MSVC)
    # Guess based on the generator
    string(REGEX MATCH "^Visual Studio .* Win64$" GENERATOR_MATCH "${CMAKE_GENERATOR}")
    if (GENERATOR_MATCH)
      set(guessed_arch "x64")
    else()
      set(guessed_arch "x86")
    endif()
  else()
    # Guess based on the flags or host architecture
    set(TARGET_ARCH_REGEX "^.*-march[= ]([^ ]+).*$")
    string(REGEX MATCH "${TARGET_ARCH_REGEX}" TARGET_ARCH_MATCH "${CMAKE_C_FLAGS}" "${CMAKE_CXX_FLAGS}")
    if (TARGET_ARCH_MATCH)
      string(REGEX REPLACE "${TARGET_ARCH_REGEX}" "\\1" TARGET_ARCH "${CMAKE_C_FLAGS}" "${CMAKE_CXX_FLAGS}")
    else()
      set(TARGET_ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
    endif()

    string(REGEX MATCH "64$" TARGET_ARCH_MATCH "${TARGET_ARCH}")
    if (TARGET_ARCH_MATCH)
      set(guessed_arch "x64")
    else()
      set(guessed_arch "x86")
    endif()
  endif()

  if (guessed_arch)
    set(${var} ${guessed_arch} PARENT_SCOPE)
  endif()

endfunction()

#==============================================================================

function(_extract_vs_version var)
  if (MSVC)
    string(REGEX MATCH "^Visual Studio [0-9]+ ([0-9]+)" VS_VERSION_MATCH "${CMAKE_GENERATOR}")
    if (VS_VERSION_MATCH AND CMAKE_MATCH_1)
      set(vs_version ${CMAKE_MATCH_1})
    elseif()
      set(vs_version "unknown")
    endif()
  endif()

  if (vs_version)
    set(${var} ${vs_version} PARENT_SCOPE)
  endif()

endfunction()
