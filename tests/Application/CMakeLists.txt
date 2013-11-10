# The MIT License (MIT)
#
# Copyright (c) 2013 Mateusz Kolodziejski
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

set(TEST_NAME mct_test_Application)
set(PERF_NAME mct_perf_Application)

set(TEST_SRCS
  TestApplication.hpp
  TestApplication.cpp
  )

set(PERF_SRCS
  PerfApplication.cpp
  )

link_directories(${Boost_LIBRARY_DIRS} ${MOCCPPLIB_LIBRARIES})

include_directories(
  ${CMAKE_BINARY_DIR}
  ${Boost_INCLUDE_DIRS}
  ${MOCCPPLIB_INCLUDES}
  ${CMAKE_SOURCE_DIR}/libs
  ${CPPUNIT_INCLUDES}
)

add_definitions( ${Boost_LIB_DIAGNOSTIC_DEFINITIONS} )
add_definitions( -DBOOST_ALL_DYN_LINK )
add_definitions( -DBOOST_FILESYSTEM_NO_DEPRECATED )

add_executable(${TEST_NAME} ${TEST_SRCS})
add_executable(${PERF_NAME} ${PERF_SRCS})

target_link_libraries(${TEST_NAME} mctapp ${MOCCPP_LIB} ${CPPUNIT_LIBRARY})
target_link_libraries(${PERF_NAME} mctapp ${MOCCPP_LIB})

install(TARGETS ${TEST_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
install(TARGETS ${PERF_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
