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

function(MSource_ForceOutOfSourceBuild)
  string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" BUILDING_IN_SOURCE)

  if (EXISTS ${CMAKE_SOURCE_DIR}/CMakeCache.txt)
    set (BUILDING_IN_SOURCE 1)
  endif()

  if (EXISTS ${CMAKE_SOURCE_DIR}/CMakeFiles)
    set (BUILDING_IN_SOURCE 1)
  endif()

  if (BUILDING_IN_SOURCE)
    message(FATAL_ERROR
      "\nThis project requires an out of source build. Remove the file 'CMakeCache.txt' and the folder 'CMakeFiles' "
      "found in this directory before continuing, create a separate build directory and "
      "run 'cmake <srcs> [options]' from there (e.g. 'cmake ..').\n"
    )
  endif()
endfunction()
