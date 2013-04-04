#
#  Copyright (c) 2011-2012 Vladimir Chalupecky
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.

cmake_minimum_required( VERSION 2.4 )

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug
        Release RelWithDebInfo MinSizeRel." FORCE)
endif()

project( umeshu )

set( PACKAGE_BUGREPORT "vladimir.chalupecky@gmail.com" )
set( PACKAGE_NAME "Umeshu" )
set( PACKAGE_TARNAME "umeshu" )
set( PACKAGE_VERSION "0.0.1" )
set( PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}" )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ansi" )

if( "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}" )
  message( FATAL_ERROR "In-source build prohibited." )
endif()

include_directories(${umeshu_SOURCE_DIR}/src)
add_subdirectory(src)

find_package(Boost 1.46.1 COMPONENTS unit_test_framework program_options system REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_subdirectory(tools)
endif()

########### Tests ##############################################################
### disable testing temporarily
###############################
# enable_testing()
# add_subdirectory( tests )

########### Generate predicates_init.h #########################################
# add_executable( predicates_init src/predicates_init.c )
# add_custom_command(
   # OUTPUT predicates_init.h
   # COMMAND predicates_init > predicates_init.h
   # DEPENDS predicates_init
   # )

