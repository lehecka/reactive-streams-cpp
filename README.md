# reactive-streams-cpp

Reactive Streams Specification and Virtual Classes for C++

NOTE: This is a temporary location for this project. Intent is to move to https://github.com/reactive-streams/reactive-streams-cpp

# Dependencies

After first checkout, initialize and update submodules:

```
# inside root ./reactive-streams-cpp
git submodule init
git submodule update --recursive
```

# Build & Install

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
