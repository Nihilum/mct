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

set(UTIL_NAME mct_port_blocker)

file(GLOB_RECURSE UTIL_SRCS ${CMAKE_SOURCE_DIR}/utils/PortBlocker ${CMAKE_SOURCE_DIR}/utils/PortBlocker/*.cpp ${CMAKE_SOURCE_DIR}/utils/PortBlocker/*.hpp)

link_directories(${Boost_LIBRARY_DIRS} ${MOCCPPLIB_LIBRARIES})

include_directories(
  ${CMAKE_BINARY_DIR}
  ${Boost_INCLUDE_DIRS}
  ${MOCCPPLIB_INCLUDES}
  ${CMAKE_SOURCE_DIR}/libs
)

add_definitions( ${Boost_LIB_DIAGNOSTIC_DEFINITIONS} )
add_definitions( -DBOOST_ALL_DYN_LINK )
add_definitions( -DBOOST_LOG_DYN_LINK )
add_definitions( -DBOOST_FILESYSTEM_NO_DEPRECATED )

if(WIN32 AND NOT MINGW)
  # Disable dll-external warnings for Visual Studio; [/GS-] disable buffer overflow security checks (optimization)
  set(PROGRAM_COMPILE_FLAGS ${PROGRAM_COMPILE_FLAGS} "/wd4251 /wd4275 /wd4351 /GS- -D_WIN32_WINNT=0x0501 -DBOOST_ASIO_HAS_MOVE")
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


add_executable(${UTIL_NAME} ${UTIL_SRCS})

if(WIN32)
	target_link_libraries(${UTIL_NAME})
else()
	target_link_libraries(${UTIL_NAME} boost_system)
endif()

set_target_properties(${UTIL_NAME} PROPERTIES COMPILE_FLAGS
  "${PROGRAM_COMPILE_FLAGS}"
)

install(TARGETS ${UTIL_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
