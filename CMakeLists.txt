cmake_minimum_required(VERSION 3.19)

project(skia_experiments)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_LINK_WHAT_YOU_USE ON)

  add_compile_options(
    -Wall -Wextra -Wno-unknown-pragmas -Wwrite-strings
    -Werror=return-type -Wno-unused-parameter -Wparentheses -Wmissing-include-dirs
    $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>
    $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,4.6>:-Wno-conversion-null>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,9.0>:-Wno-deprecated-copy>>
    $<$<VERSION_LESS:$<CXX_COMPILER_VERSION>,5.0>:-Wno-missing-field-initializers>
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,4.6>:-Wenum-compare>
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,4.6>:-Wno-unused-local-typedefs>
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,4.6>:-Wno-ignored-qualifiers>
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,5.0>:-pedantic>
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,5.0>:-Wlogical-op>
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,6.0>:-Wduplicated-cond>
    $<$<CONFIG:Release>:$<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,4.9>:-fstack-protector-strong>>
    $<$<CONFIG:Release>:$<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,5.0>:-Wl,-z,relro,-z,now>>
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "^.*Clang$")
  add_compile_options(
    -Weverything
    -Wno-c++98-compat -Wno-c++98-compat-pedantic
    -Wno-global-constructors -Wno-exit-time-destructors
    -Wno-documentation
    -Wno-padded
    -Wno-weak-vtables
    -Wno-reserved-id-macro
    -Wno-attributes
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,3.8>:-fstack-protector-strong>
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  
  # enable additional compiler warnings
  add_compile_options(/w14062 /w14263 /w14264 /w14289 /w14706)
  
  if(MATH_ANIM_TREAT_WARNINGS_AS_ERRORS)
    add_compile_options(/WX)
  endif()
else()
  message(FATAL_ERROR "Unsupported or unknown compiler ${CMAKE_CXX_COMPILER_ID}")
endif()

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release MinSizeRel RelWithDebInfo)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "^.*Clang$")
  add_compile_options(-foptimize-sibling-calls)
  add_compile_options(-Wall -Wextra -Wwrite-strings -Wenum-compare)
  add_compile_options($<$<CONFIG:Release>:-D_FORTIFY_SOURCE=2>)
  
  if(MATH_ANIM_TREAT_WARNINGS_AS_ERRORS)
    add_compile_options(-Werror)
  endif()
  
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z now -pie")
    
    set(MATH_ANIM_SANITIZE "none" CACHE STRING "Sanitizer not all options are available in all compiler versions")
    set_property(CACHE MATH_ANIM_SANITIZE PROPERTY STRINGS "" none address memory thread undefined leak)
    if(NOT MATH_ANIM_SANITIZE STREQUAL "none")
      add_compile_options(-fsanitize=${MATH_ANIM_SANITIZE} -fno-omit-frame-pointer)
      link_libraries(-fsanitize=${MATH_ANIM_SANITIZE})
    endif()
  endif()
endif()

option(MATH_ANIM_ENABLE_CLANG_TIDY "" ON)
if(MATH_ANIM_ENABLE_CLANG_TIDY)
  find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
    )
  if(CLANG_TIDY_EXE)
    message(STATUS "Enable clang-tidy: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
  endif()
endif()

# Does not work well will QtCreator
# option(SKIA_EXPERIMENTS_ENABLE_CPPCHECK "" OFF)
# if(SKIA_EXPERIMENTS_ENABLE_CPPCHECK)
#   find_program(
#     CPPCHECK_EXE
#     NAMES "cppcheck"
#     DOC "Path to cppcheck executable"
#     )
#   if(CPPCHECK_EXE)
#     message(STATUS "Enable cppcheck: ${CPPCHECK_EXE}")
#             set(CMAKE_CXX_CPPCHECK "${CPPCHECK_EXE}" "--enable=all" "--quiet")
#         endif()
#     endif()
# endif()

# library
# set(SKIA_EXPERIMENTS_PUBLIC_HEADERS
#   src/skia_experiments/Extension.hpp
#   )
# add_library(skia_experiments_headers INTERFACE)
# target_include_directories(skia_experiments_headers INTERFACE
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
#   $<INSTALL_INTERFACE:include>
# )

# add_library(extension_system STATIC
#   ${SKIA_EXPERIMENTS_PUBLIC_HEADERS}
#   src/extension_system/DynamicLibrary.cpp
#   src/extension_system/filesystem.cpp
#   src/extension_system/ExtensionSystem.cpp
#   src/extension_system/filesystem.hpp
#   src/extension_system/string.hpp)
# target_link_libraries(skia_experiments
#   PUBLIC extension_system_headers
#   INTERFACE ${CMAKE_DL_LIBS})

add_executable(skia_experiments
  src/math_anim.cpp)

find_package(Boost REQUIRED)
if(Boost_FOUND)
  message(STATUS "Use boost (${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}) to speed-up searching for extensions")
  target_link_libraries(skia_experiments PRIVATE Boost::boost)
endif()


find_package(PkgConfig)
find_package(PNG REQUIRED)
find_library(LIBPTHREAD pthread)
#find_package(Skia)

set_target_properties(skia_experiments PROPERTIES PUBLIC_HEADER "${EXTENSION_SYSTEM_PUBLIC_HEADERS}")

enable_testing()


if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set_target_properties(extension_system PROPERTIES DEBUG_POSTFIX d)
  set_target_properties(extension_system_test_lib PROPERTIES DEBUG_POSTFIX d)
  set_target_properties(extension_system_test PROPERTIES DEBUG_POSTFIX d)
  set_target_properties(extension_system_example1_extension PROPERTIES DEBUG_POSTFIX d)
  set_target_properties(extension_system_example1 PROPERTIES DEBUG_POSTFIX d)
  set_target_properties(extension_system_example2_extension PROPERTIES DEBUG_POSTFIX d)
  set_target_properties(extension_system_example2 PROPERTIES DEBUG_POSTFIX d)
endif()

install(TARGETS skia_experiments
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  PUBLIC_HEADER DESTINATION include/extension_system)

find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
  add_custom_target (doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
    SOURCES ${PROJECT_SOURCE_DIR}/Doxyfile
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()
