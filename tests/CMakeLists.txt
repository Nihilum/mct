# The MIT License (MIT)
#
# Copyright (c) 2014 Mateusz Kolodziejski
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

set(TEST_NAME mct_tests)

file(GLOB_RECURSE TEST_SRCS ${CMAKE_SOURCE_DIR}/tests/ ${CMAKE_SOURCE_DIR}/tests/*.cpp ${CMAKE_SOURCE_DIR}/tests/*.hpp)

link_directories(${Boost_LIBRARY_DIRS} ${MOCCPPLIB_LIBRARIES} ${MOCTEST_LIB})

include_directories(
  ${CMAKE_BINARY_DIR}
  ${Boost_INCLUDE_DIRS}
  ${MOCCPPLIB_INCLUDES}
  ${MOCTEST_INCLUDES}
  ${CPPUNIT_INCLUDES}
  ${CMAKE_SOURCE_DIR}/libs
)

add_definitions( ${Boost_LIB_DIAGNOSTIC_DEFINITIONS} )
add_definitions( -DBOOST_ALL_DYN_LINK )
add_definitions( -DBOOST_LOG_DYN_LINK )
add_definitions( -DBOOST_FILESYSTEM_NO_DEPRECATED )

if(WIN32 AND NOT MINGW)
  # Disable dll-external warnings for Visual Studio; [/GS-] disable buffer overflow security checks (optimization)
  set(PROGRAM_COMPILE_FLAGS ${PROGRAM_COMPILE_FLAGS} "/wd4251 /wd4275 /wd4351 /GS-")
elseif(WIN32 AND MINGW)
  set(PROGRAM_COMPILE_FLAGS ${PROGRAM_COMPILE_FLAGS} "-std=c++11")
else()
  # Activate C++11 mode for GNU/GCC; set rpath to $ORIGIN so the shared library can be easily found
  set(PROGRAM_COMPILE_FLAGS ${PROGRAM_COMPILE_FLAGS} "-std=c++11")
endif()

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "\$ORIGIN:\$ORIGIN/../lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(NOT DEFINED WIN32)
  SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--enable-new-dtags")
endif()


add_executable(${TEST_NAME} ${TEST_SRCS})

if(WIN32)
	target_link_libraries(${TEST_NAME} moccpp ${INTERNAL_LIBS} ${CPPUNIT_LIBRARY} moctest)
else()
	target_link_libraries(${TEST_NAME} moccpp ${INTERNAL_LIBS} ${CPPUNIT_LIBRARY} moctest boost_log boost_filesystem boost_system boost_regex boost_thread boost_program_options)
endif()

set_target_properties(${TEST_NAME} PROPERTIES COMPILE_FLAGS
  "${PROGRAM_COMPILE_FLAGS}"
)

install(TARGETS ${TEST_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
