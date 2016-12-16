cmake_minimum_required(VERSION 3.2)
project(Reactive-Streams)

# Generate compilation database for use by YouCompleteMe.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# make sure to bail on in-source builds for cleanliness
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory)"
          "and run CMake from there. You may need to remove CMakeCache.txt.")
endif()

enable_testing()

include(ExternalProject)
include(CTest)

# gmock
ExternalProject_Add(
  gmock
  URL ${CMAKE_CURRENT_SOURCE_DIR}/googletest-master-20160722.zip
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(gmock source_dir)
set(GMOCK_SOURCE_DIR ${source_dir})
ExternalProject_Get_Property(gmock binary_dir)
set(GMOCK_BINARY_DIR ${binary_dir})

set(GMOCK_LIBS
  ${GMOCK_BINARY_DIR}/${CMAKE_CFG_INTDIR}/googlemock/${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX}
  ${GMOCK_BINARY_DIR}/${CMAKE_CFG_INTDIR}/googlemock/${CMAKE_STATIC_LIBRARY_PREFIX}gmock_main${CMAKE_STATIC_LIBRARY_SUFFIX}
  )

find_package(Threads)

# Common configuration for all build modes.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -momit-leaf-frame-pointer")

# Configuration for Debug build mode.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")

include_directories(${CMAKE_SOURCE_DIR})

include_directories(${GMOCK_SOURCE_DIR}/googlemock/include)
include_directories(${GMOCK_SOURCE_DIR}/googletest/include)

add_library(Reactive-Streams INTERFACE)
target_include_directories(Reactive-Streams INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

function(reactivestreams_test name file)
  add_executable(${name} ${file})
  target_link_libraries(${name} Reactive-Streams ${GMOCK_LIBS} ${CMAKE_THREAD_LIBS_INIT})
  add_dependencies(${name} gmock)
  add_test(NAME ${name} COMMAND ${name})
endfunction()

#add_subdirectory(external/googletest EXCLUDE_FROM_ALL)
# EXCLUDE_FROM_ALL so 'make install' doesn't attempt installation of googletest

reactivestreams_test(examples test/Examples.cpp)

install (FILES include/ReactiveStreams.h DESTINATION include/reactive-streams)
install (FILES test/Mocks.h DESTINATION include/reactive-streams/test)
### TODO why doesn't the following work instead of above?
#install (TARGETS Reactive-Streams DESTINATION include)

# EOF
