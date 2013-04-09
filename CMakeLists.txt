#
#  Copyright (c) 2011-2013 Vladimir Chalupecky
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

cmake_minimum_required( VERSION 2.8 )

if( NOT CMAKE_BUILD_TYPE )
    message( STATUS "No build type selected, default to Release" )
    set( CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug
        Release RelWithDebInfo MinSizeRel." FORCE )
endif()

project( umeshu )

set( PACKAGE_BUGREPORT "vladimir.chalupecky@gmail.com" )
set( PACKAGE_NAME "Umeshu" )
set( PACKAGE_TARNAME "umeshu" )
set( PACKAGE_VERSION "0.0.1" )
set( PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}" )

if( "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}" )
  message( FATAL_ERROR "In-source build prohibited." )
endif()

if( CMAKE_COMPILER_IS_GNUCXX )
	 set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -ansi" )
   set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -mtune=native" )
endif()

option( BUILD_SHARED_LIBS "Build the umeshu library shared." ON )

################################################################################
# Find Eigen3
################################################################################
find_package( PkgConfig QUIET )
message( STATUS "Check for Eigen3" )
if( PKG_CONFIG_FOUND )
  PKG_CHECK_MODULES( PKG_EIGEN3 QUIET eigen3 ) # Try to find eigen3 with pkg-config
endif()
set( EIGEN3_FOUND FALSE )
find_path( EIGEN3_INCLUDE NAMES signature_of_eigen3_matrix_library
  PATHS ${CMAKE_INSTALL_PREFIX}/include ${PKG_EIGEN3_INCLUDE_DIRS}
  PATH_SUFFIXES eigen3
)
if( NOT EXISTS ${EIGEN3_INCLUDE} )
  message( FATAL_ERROR "Eigen3 not found in standard location. Try passing -DEIGEN3_INCLUDE=/path/to/eigen3 to cmake" )
else()
  message( STATUS "Using Eigen3 in: ${EIGEN3_INCLUDE}" )
  set( EIGEN3_FOUND TRUE )
endif()

################################################################################
# Find Boost
################################################################################
find_package( Boost COMPONENTS unit_test_framework program_options system REQUIRED )

include_directories( ${umeshu_SOURCE_DIR}/src ${Boost_INCLUDE_DIR} ${EIGEN3_INCLUDE} )

add_subdirectory( src )
add_subdirectory( tools )

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

