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

set(LIBRARY_NAME mctapp)

if(WIN32)
  # Disable dll-external warnings for Visual Studio; [/GS-] disable buffer overflow security checks (optimization)
  # Boost.Asio needs to know windows version [0x0501 - WinXP minimum]
  set(LIBRARY_COMPILE_FLAGS ${LIBRARY_COMPILE_FLAGS} "-DMCT_APPLICATION_DLL=1 /wd4251 /wd4275 /GS- -D_WIN32_WINNT=0x0501")
else()
  # Activate C++11 mode for GNU/GCC
  set(LIBRARY_COMPILE_FLAGS ${LIBRARY_COMPILE_FLAGS} "-std=c++11 -DMCT_APPLICATION_DLL=1")
endif()

file(GLOB_RECURSE LIBRARY_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "\$ORIGIN")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

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

add_library(${LIBRARY_NAME} SHARED
  ${LIBRARY_SRCS}
)

set(INTERNAL_LIBS ${INTERNAL_LIBS} ${LIBRARY_NAME})
set(INTERNAL_LIBS ${INTERNAL_LIBS} PARENT_SCOPE)

if (DEFINED WIN32)
  install(TARGETS ${LIBRARY_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
  )
  install(TARGETS ${LIBRARY_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/tests
  )
else()
  install(TARGETS ${LIBRARY_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  )
endif()

set_target_properties(${LIBRARY_NAME} PROPERTIES COMPILE_FLAGS
  "${LIBRARY_COMPILE_FLAGS}"
)

target_link_libraries(${LIBRARY_NAME} mctconfig mctlog mctmode mctmodefactory)
