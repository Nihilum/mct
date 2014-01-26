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

function(MSource_FindBoost MSOURCE_OUTVAR_1 MSOURCE_OUTVAR_2)
  file(TO_CMAKE_PATH "${MSOURCE_OUTVAR_1}" MSOURCE_TMPVAR_1)

  set(Boost_USE_STATIC_LIBS OFF)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME OFF)

  if (DEFINED MSOURCE_TMPVAR_1)
    set(BOOST_ROOT ${MSOURCE_TMPVAR_1})
  endif()

  if (${MSOURCE_OUTVAR_2} STREQUAL "OFF")
    set(MSOURCE_OUTVAR_2 1.55.0)
  endif()

  find_package(Boost ${MSOURCE_OUTVAR_2})

  if (NOT Boost_FOUND)
    message(FATAL_ERROR
      "This project requires Boost installed. Please download Boost from http://www.boost.org/ and set -DBOOST_PATH=path_to_boost variable."
      "Consult documentation for more information."
    )
  endif()

  set(Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} PARENT_SCOPE)
  set(Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIRS} PARENT_SCOPE)

  unset(MSOURCE_TMPVAR_1)
endfunction()
