cmake_minimum_required(VERSION 3.19)

project(math_anim)

set(CMAKE_CXX_STANDARD 20)

add_executable(main
  src/main.cpp)

# find_package(Boost REQUIRED)
# if(Boost_FOUND)
#   message(STATUS "Use boost (${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}) to speed-up searching for extensions")
#   target_link_libraries(math_anim PRIVATE Boost::boost)
# endif()


# find_package(PkgConfig)
# find_package(PNG REQUIRED)
# find_library(LIBPTHREAD pthread)

# add_library(Skia INTERFACE)
# target_include_directories(Skia
# 	INTERFACE
# 	vendor/skia/include)
