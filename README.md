# HG Engine
It's C++17 STL based game engine.

## Status
|Linux CMake (Azure)|Linux CMake (Travis)|Coverage|
|-------------------|--------------------|--------|
|[![Build Status](https://dev.azure.com/xelamegaxela/xelamegaxela/_apis/build/status/Megaxela.HGEngineReloaded?branchName=master)](https://dev.azure.com/xelamegaxela/xelamegaxela/_build/latest?definitionId=1?branchName=master)|[![Build Status](https://travis-ci.com/Megaxela/HGEngineReloaded.svg?branch=master)](https://travis-ci.com/Megaxela/HGEngineReloaded)|[![Coverage Status](https://coveralls.io/repos/github/Megaxela/HGEngineReloaded/badge.svg)](https://coveralls.io/github/Megaxela/HGEngineReloaded)|

## Examples

### Blitting
This example shows engine's blitting ability. It can perform pixel perfect
texture blitting.

<img src="https://github.com/Megaxela/HGEngineReloaded/blob/master/examples/Previews/Blitting.png">

### Orbital
This is sample tests - that shows engine's parent system.

<img src="https://github.com/Megaxela/HGEngineReloaded/blob/master/examples/Previews/Orbital.png">

### PBR
Example with implemented sample PBR material.

<img src="https://github.com/Megaxela/HGEngineReloaded/blob/master/examples/Previews/PBR.png">

### RenderToTexture
This example shows how to use `HG::Rendering::Base::RenderTarget` for rendering
into texture.

<img src="https://github.com/Megaxela/HGEngineReloaded/blob/master/examples/Previews/RenderToTexture.png">

### TransparentExample
This examples shows how engine treating huge amount of transparent object.

<img src="https://github.com/Megaxela/HGEngineReloaded/blob/master/examples/Previews/Transparent.png">

## Build
Project uses `cmake` based build system. Sample build process:

1. Install requirements (see below)
1. Clone repository: `git clone https://github.com/Megaxela/HGEngineReloaded`
1. Go to repository: `cd HGEngineReloaded`
1. Update submodules: `git submodule update --init`
1. Create build directory: `mkdir build`
1. Go to build directory: `cd build`
1. Configure project: `cmake -DHGRenderingOGL_USE_GLFW=On ..`
    1. If you want to build examples - add: `-DHG_BUILD_EXAMPLES=On`
    1. If you want to build tests (required gtest) - add: `-DHG_BUILD_TESTS=On`
    1. If you have special path to GLFW - you can use: `-DGLFW_LOCATION=location-of-built-glfw`
    1. If you have special path to GLEW - you can use: `-DCMAKE_PREFIX_PATH=location-of-build-glew`
1. Start building: `cmake --build . --target all`

## Libraries required in system
1. `glfw3` - Engine required 3.2.1 version. 
1. `glew` - Engine requires 2.1.0 version.
1. `googletest` - Required only if tests are built.
1. `zlib` - Engine requires this library.