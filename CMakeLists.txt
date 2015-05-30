# The MIT License (MIT)
#
# Copyright (c) 2013-2014 Mateusz Kolodziejski
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


project(mct CXX)
set(MCT_VERSION 0.1.0)
set(MCT_TAG dev)
set(MCT_EMAIL matt@mattsource.com)

cmake_minimum_required(VERSION 2.8)

# Make sure that we can find all CMake includes
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_SOURCE_DIR}/cmake
)

include(MSource/MSource)

MSource_ForceOutOfSourceBuild()
MSource_DetermineAddressModel(MSOURCE_ADDRESS_MODEL)
message(STATUS "Detected ${MSOURCE_ADDRESS_MODEL}-bit address model.")

set(MINGW "TRUE" CACHE FILEPATH "MinGW environment")
set(CMAKE_INSTALL_PREFIX "C:/dev/mct/build/rtm" CACHE FILEPATH "Installation prefix")
set(CMAKE_BUILD_TYPE "Release" CACHE FILEPATH "Build type")
set(MOCCPPLIB_INCLUDES "C:/dev/deps/moccpplib_0_2_0_release32/include/" CACHE FILEPATH "moccpplib includes")
set(MOCCPPLIB_LIBRARIES "C:/dev/deps/moccpplib_0_2_0_release32/lib" CACHE FILEPATH "moccpplib libraries")
set(CPPUNIT_INCLUDES "C:/dev/deps/cppunit_1_13_2_release32/" CACHE FILEPATH "cppunit includes")
set(CPPUNIT_LIBRARY "C:/dev/deps/cppunit_1_13_2_release32/cppunit.lib" CACHE FILEPATH "cppunit library")
set(MOCTEST_LIB "C:/dev/deps/moctest_0_2_0_release32/lib" CACHE FILEPATH "moctest library")
set(MOCTEST_INCLUDES "C:/dev/deps/moctest_0_2_0_release32/include" CACHE FILEPATH "moctest includes")
set(BOOST_PATH "C:/dev/deps/boost_1_58_0_release32/" CACHE FILEPATH "boost path")
set(BOOST_VERSION "1.58.0" CACHE BOOL "boost version")

# Output description of this script
message(
  "\n\nThis script builds the mct application.\n"
  "Options that can be used in order to configure the process:\n\n"
  "  CMAKE_BUILD_TYPE: Debug / Release\n"
  "  CMAKE_INSTALL_PREFIX: Path where the application should be installed to\n"
  "  MOCCPPLIB_INCLUDES: Path where moccpp Library includes can be found\n"
  "  MOCCPPLIB_LIBRARIES: Path where moccpp Library libraries can be found\n"
  "  CPPUNIT_INCLUDES (optional): Path where CPPUNIT Library includes can be found\n"
  "  CPPUNIT_LIBRARY (optional): Path where CPPUNIT Library static can be found\n"
  "  BOOST_PATH: (optional) Path to boost installation\n"
  "  BOOST_VERSION: (optional) Used boost version (for example 1.57.0)\n\n"
  "To set an option simply type -D<OPTION>=<VALUE> after 'cmake <srcs>'.\n"
  "For example: cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local/mct -DBOOST_PATH=C:\\Boost -DCMAKE_BUILD_TYPE=Release\n\n"
)

if (NOT DEFINED MOCCPPLIB_INCLUDES)
  message(FATAL_ERROR "This project requires MOCCPPLIB (includes). Visit http://github.com/Nihilum/moccpplib and get it.")
endif()

if (NOT DEFINED MOCCPPLIB_LIBRARIES)
  message(FATAL_ERROR "This project requires MOCCPPLIB (library). Visit http://github.com/Nihilum/moccpplib and get it.")
endif()

option(CPPUNIT_INCLUDES "Path where CPPUNIT Library includes can be found" 0)
option(CPPUNIT_LIBRARY "Path where CPPUNIT Library static can be found" 0)
option(BOOST_PATH "Path to boost installation" 0)
option(BOOST_VERSION "Used boost version (for example 1.57.0)" 0)

message(STATUS "Looking for Boost...")
MSource_FindBoost(${BOOST_PATH} ${BOOST_VERSION})

if (NOT DEFINED WIN32)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/etc)
endif()

configure_file(
  ${CMAKE_SOURCE_DIR}/include/config.hpp.in
  ${CMAKE_BINARY_DIR}/config.hpp
)

set(INTERNAL_LIBS)
add_subdirectory(libs)

if (CPPUNIT_INCLUDES AND CPPUNIT_LIBRARY)
  if (NOT DEFINED MOCTEST_INCLUDES)
    message(FATAL_ERROR "When building with CPPUnit, this project requires MOCTEST_LIB (includes). Visit http://github.com/Nihilum/moctest and get it.")
  endif()

  if (NOT DEFINED MOCTEST_LIB)
    message(FATAL_ERROR "When building with CPPUnit, this project requires MOCTEST_LIB. Visit http://github.com/Nihilum/moctest and get it.")
  endif()

  add_subdirectory(tests)
  add_subdirectory(utils)
endif()

set(MCT_SRCS ${CMAKE_SOURCE_DIR}/src/main.cpp)

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(NOT DEFINED WIN32)
  SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--enable-new-dtags")
endif()

link_directories(${Boost_LIBRARY_DIRS} ${MOCCPPLIB_LIBRARIES})

include_directories(
  ${CMAKE_BINARY_DIR}
  ${Boost_INCLUDE_DIRS}
  ${MOCCPPLIB_INCLUDES}
  ${CMAKE_SOURCE_DIR}/libs
)

add_definitions( ${Boost_LIB_DIAGNOSTIC_DEFINITIONS} )
add_definitions( -DBOOST_ALL_DYN_LINK )
add_definitions( -DBOOST_FILESYSTEM_NO_DEPRECATED )

if(WIN32 AND NOT MINGW)
  # Disable dll-external warnings for Visual Studio; [/GS-] disable buffer overflow security checks (optimization);
  # Boost.Asio needs to know windows version [0x0501 - WinXP minimum]
  set(PROGRAM_COMPILE_FLAGS ${PROGRAM_COMPILE_FLAGS} "/wd4251 /GS- -D_WIN32_WINNT=0x0501")
elseif(WIN32 AND MINGW)
  set(PROGRAM_COMPILE_FLAGS ${PROGRAM_COMPILE_FLAGS} "-std=c++11 -D_WIN32_WINNT=0x0501")
else()
  # Activate C++11 mode for GNU/GCC
  set(PROGRAM_COMPILE_FLAGS ${PROGRAM_COMPILE_FLAGS} "-std=c++11")
endif()

add_executable(mct ${MCT_SRCS})

if(WIN32)
  target_link_libraries(mct
    moccpp
    ${INTERNAL_LIBS}
  )
else()
  target_link_libraries(mct
    moccpp
    ${INTERNAL_LIBS}
    boost_program_options
    boost_log
  )
endif()

if (DEFINED WIN32)
  install(TARGETS mct DESTINATION ${CMAKE_INSTALL_PREFIX})
else()
  install(TARGETS mct DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()

# Install Boost Libraries and moccpp Library if on Windows
if (DEFINED WIN32)
  add_definitions( -D_WIN32_WINNT=0x0501 )

  if (CPPUNIT_INCLUDES AND CPPUNIT_LIBRARY)
    file(TO_CMAKE_PATH "${MOCTEST_LIB}/moctest.dll" MOCTEST_INSTALL_PATH)
    install(FILES ${MOCTEST_INSTALL_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
  endif()

  set(MCT_BOOST_LIBS_INUSE ${MCT_BOOST_LIBS_INUSE} "boost_chrono-.*")
  set(MCT_BOOST_LIBS_INUSE ${MCT_BOOST_LIBS_INUSE} "boost_date_time-.*")
  set(MCT_BOOST_LIBS_INUSE ${MCT_BOOST_LIBS_INUSE} "boost_filesystem-.*")
  #set(MCT_BOOST_LIBS_INUSE ${MCT_BOOST_LIBS_INUSE} "boost_log_setup-.*")
  set(MCT_BOOST_LIBS_INUSE ${MCT_BOOST_LIBS_INUSE} "boost_log-.*")
  set(MCT_BOOST_LIBS_INUSE ${MCT_BOOST_LIBS_INUSE} "boost_program_options-.*")
  set(MCT_BOOST_LIBS_INUSE ${MCT_BOOST_LIBS_INUSE} "boost_regex-.*")
  set(MCT_BOOST_LIBS_INUSE ${MCT_BOOST_LIBS_INUSE} "boost_system-.*")
  set(MCT_BOOST_LIBS_INUSE ${MCT_BOOST_LIBS_INUSE} "boost_thread-.*")
  set(MCT_BOOST_LIBS_INUSE ${MCT_BOOST_LIBS_INUSE} "boost_timer-.*")


  file(GLOB_RECURSE MCT_BOOST_LIBS_INSTALL ${Boost_LIBRARY_DIRS}/ ${Boost_LIBRARY_DIRS}/*.dll)

  foreach(fName ${MCT_BOOST_LIBS_INSTALL})
    foreach(libName ${MCT_BOOST_LIBS_INUSE})
      if(${fName} MATCHES ${libName})
        install(FILES "${fName}" DESTINATION ${CMAKE_INSTALL_PREFIX})
        if (CPPUNIT_INCLUDES AND CPPUNIT_LIBRARY)
          install(FILES "${fName}" DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
        endif()
      endif()
    endforeach()
  endforeach()

  file(TO_CMAKE_PATH "${MOCCPPLIB_LIBRARIES}/moccpp.dll" MOCCPPLIB_LIBRARY_INSTALL_PATH)
  install(FILES ${MOCCPPLIB_LIBRARY_INSTALL_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})
  if (CPPUNIT_INCLUDES AND CPPUNIT_LIBRARY)
    install(FILES ${MOCCPPLIB_LIBRARY_INSTALL_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
  endif()

# Install moccpp Library on linux and prepare ./etc directory
else()
  if (CPPUNIT_INCLUDES AND CPPUNIT_LIBRARY)
    file(TO_CMAKE_PATH "${MOCTEST_LIB}/libmoctest.so" MOCTEST_INSTALL_PATH)
    install(FILES ${MOCTEST_INSTALL_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
  endif()

  file(TO_CMAKE_PATH "${MOCCPPLIB_LIBRARIES}/libmoccpp.so" MOCCPPLIB_LIBRARY_INSTALL_PATH)
  install(FILES ${MOCCPPLIB_LIBRARY_INSTALL_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
  install(DIRECTORY "${CMAKE_BINARY_DIR}/etc" DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

set_target_properties(mct PROPERTIES COMPILE_FLAGS
  "${PROGRAM_COMPILE_FLAGS}"
)
