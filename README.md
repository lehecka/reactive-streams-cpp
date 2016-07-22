# reactive-streams-cpp

Reactive Streams Specification and Virtual Classes for C++

NOTE: This is a temporary location for this project. Intent is to move to https://github.com/reactive-streams/reactive-streams-cpp

# Build Status

<a href='https://travis-ci.org/ReactiveSocket/reactive-streams-cpp/builds'><img src='https://travis-ci.org/ReactiveSocket/reactive-streams-cpp.svg?branch=master'></a>

# Dependencies

None.

# Build & Install

A script to build and test is included that installs a Release build in `build/Release/reactivesocket`

```
$ cd reactive-streams-cpp
$ bash scripts/cppbuild
```

or to build by hand and install in a different location

```
cd reactive-streams-cpp
mkdir build
cd build
cmake ..
make
make install
```

This will install in `/usr/local/include/reactive-streams/ReactiveStreams.h`

# Usage

Use via:

```c++
#include <reactive-streams/ReactiveStreams.h>
```
