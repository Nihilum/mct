# Install script for directory: /home/nihilum/dev/mct/tests/Configuration

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/nihilum/dev/mct/build/rtm")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_test_Configuration" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_test_Configuration")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_test_Configuration"
         RPATH "$ORIGIN")
  ENDIF()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/nihilum/dev/mct/build/rtm/tests/mct_test_Configuration")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/home/nihilum/dev/mct/build/rtm/tests" TYPE EXECUTABLE FILES "/home/nihilum/dev/mct/build/tests/Configuration/mct_test_Configuration")
  IF(EXISTS "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_test_Configuration" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_test_Configuration")
    FILE(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_test_Configuration"
         OLD_RPATH "/opt/boost/lib:/home/nihilum/Software/moccpplib_0_1_5/lib64_opt:/home/nihilum/dev/mct/build/libs/Configuration:"
         NEW_RPATH "$ORIGIN")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_test_Configuration")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_perf_Configuration" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_perf_Configuration")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_perf_Configuration"
         RPATH "$ORIGIN")
  ENDIF()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/nihilum/dev/mct/build/rtm/tests/mct_perf_Configuration")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/home/nihilum/dev/mct/build/rtm/tests" TYPE EXECUTABLE FILES "/home/nihilum/dev/mct/build/tests/Configuration/mct_perf_Configuration")
  IF(EXISTS "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_perf_Configuration" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_perf_Configuration")
    FILE(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_perf_Configuration"
         OLD_RPATH "/opt/boost/lib:/home/nihilum/Software/moccpplib_0_1_5/lib64_opt:/home/nihilum/dev/mct/build/libs/Configuration:"
         NEW_RPATH "$ORIGIN")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/nihilum/dev/mct/build/rtm/tests/mct_perf_Configuration")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

