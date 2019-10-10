# HG Engine
It's C++17 STL based game engine.

## Status
|Linux CMake (Azure)|Linux CMake (Travis)|Coverage|
|-------------------|--------------------|--------|
|[![Build Status](https://dev.azure.com/xelamegaxela/xelamegaxela/_apis/build/status/Megaxela.HGEngineReloaded?branchName=master)](https://dev.azure.com/xelamegaxela/xelamegaxela/_build/latest?definitionId=1?branchName=master)|[![Build Status](https://travis-ci.com/Megaxela/HGEngineReloaded.svg?branch=master)](https://travis-ci.com/Megaxela/HGEngineReloaded)|[![Coverage Status](https://coveralls.io/repos/github/Megaxela/HGEngineReloaded/badge.svg)](https://coveralls.io/github/Megaxela/HGEngineReloaded)|

## Abilities

### Supported OS
* :heavy_check_mark: Linux
* :heavy_check_mark: Windows
* :x: MacOS (coming soon)

### Supported Rendering Backends
* :heavy_check_mark: OpenGL 3.3+ (requires [GLEW 2.1.0](https://github.com/nigels-com/glew))
* :x: Vulkan (coming soon)
* :x: DirectX (not planned)

### Supported Physics Backends
* :heavy_check_mark: [PlayRho](https://github.com/louis-langholtz/PlayRho) (work in progress)
* :x: [Bullet](https://github.com/bulletphysics/bullet3) (coming soon)

### External tools
* :heavy_check_mark: [Tiled](https://github.com/bjorn/tiled) (work in progress)

### Features
* Simplified ECS system (closer to Unity 3D)
* Behaviours coder friendly property system
* Support of dynamic graphics API change (theoretical)
* Asynchronous resource loading
* User friendly thread pool for user parallel actions
* Benchmarking system
* Manual resource control (with leak detect) via HG::Core::ResourceCache
* Integrated support of ingame statistics
* Flexible resource access system
* Integrated engine and ingame logging system
* Rendering to texture system
* Blit to texture system
* [ImGui](https://github.com/ocornut/imgui) support
* Independent input system (mouse, keyboard, gamepads)
* Assets package tool (engine support is in progress)
* Models loading with [Assimp](https://github.com/assimp/assimp)
* Bitmaps loading with [STB image](https://github.com/nothings/stb)

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
This examples shows how engine treating huge amount of transparent object. (pretty bad now)

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
    1. See next section with configure arguments...
1. Start building: `cmake --build . --target all`

### Engine configure arguments
 |      Variable Name      |Possible Values|Default|Description                                      |
 |-------------------------|---------------|-------|-------------------------------------------------|
 | `HG_BUILD_EXAMPLES`     |   `On`/`Off`  | `Off` | Build provided examples or not                  |
 | `HG_BUILD_TESTS`        |   `On`/`Off`  | `Off` | Enables tests discover and assets configuration |
 | `HG_TEST_COVERAGE`      |   `On`/`Off`  | `Off` | Enables compile options for coverage check      |
 | `HG_BUILD_WARNINGS`     |   `On`/`Off`  | `Off` | Enables huge amount of warnings for build       |
 | `HG_BUILD_Core_TESTS`   |   `On`/`Off`  | `Off` | Enables Core module tests                       |
 | `HG_BUILD_Tools_TESTS`  |   `On`/`Off`  | `Off` | Enables Tools module tests                      |
 | `HG_BUILD_Utils_TESTS`  |   `On`/`Off`  | `Off` | Enables Utils module tests                      |
 | `HG_BUILD_ALL_TESTS`    |   `On`/`Off`  | `Off` | Enables all modules tests                       |
 | `HG_STD_STATIC_LINKAGE` |   `On`/`Off`  | `Off` | Enables static C++ STD library linkage          |

## Libraries required in system
1. `glew` - Engine requires 2.1.0 version if OpenGL support required.
1. `googletest` - Required only if tests are built.
1. `zlib` - Engine requires this library.

## Libraries provided by submodules/inplace
1. Submodules:
    1. [ALogger](https://github.com/Megaxela/ALogger) - simple logging library.
    1. [ByteArray](https://github.com/Megaxela/ByteArray) - bytearray helper implementation.
    1. [RingBuffer](https://github.com/Megaxela/ringbuffer) - stl like ringbuffer implementation.
    1. [assimp](https://github.com/assimp/assimp) - library for 3d models loading.
    1. [glfw](https://github.com/glfw/glfw) - glfw library for crossplatform system routine handling.
    1. [glm](https://github.com/g-truc/glm) - gl mathematics library.
    1. [imgui](https://github.com/ocornut/imgui) - immediate mode graphical user interface.
    1. [json](https://github.com/nlohmann/json) - nlohmann's json library.
1. Inplace:
    1. [PlayRho](https://github.com/louis-langholtz/PlayRho) - real-time 2d physics engine.
    1. [gl](https://github.com/acdemiralp/gl) - c++11 gl objects wrapping library.
    1. [rapidxml](http://rapidxml.sourceforge.net/) - xml loading library.
    1. [stb](https://github.com/nothings/stb) - engine uses only stb image loading implementation.

## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

Library is licensed under the [MIT License](https://opensource.org/licenses/MIT) 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.