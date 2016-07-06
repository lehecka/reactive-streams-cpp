cmake_minimum_required(VERSION 3.2)
project(Reactive-Streams)

# Generate compilation database for use by YouCompleteMe.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Common configuration for all build modes.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -momit-leaf-frame-pointer")

# Configuration for Debug build mode.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")

include_directories(${CMAKE_SOURCE_DIR})

add_library(Reactive-Streams INTERFACE)
target_include_directories(Reactive-Streams INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_subdirectory(external/googletest EXCLUDE_FROM_ALL)
# EXCLUDE_FROM_ALL so 'make install' doesn't attempt installation of googletest

add_executable(
  tests
  test/Examples.cpp
  test/AllowanceSemaphoreTest.cpp
  test/OwnershipTest.cpp
  test/SmartPointersTest.cpp
  )

target_link_libraries(
  tests
  Reactive-Streams
  gtest
  gmock
  gmock_main
  glog
  )

install (FILES include/ReactiveStreams.h DESTINATION include/reactive-streams)
install (FILES utilities/AllowanceSemaphore.h utilities/Ownership.h utilities/SmartPointers.h DESTINATION include/reactive-streams/utilities)
### TODO why doesn't the following work instead of above?
#install (TARGETS Reactive-Streams DESTINATION include)

# EOF
