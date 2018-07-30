# HG Engine
It's C++17 STL based game engine.

## Libraries
Engine contains following libraries:
* Rendering
    * Base
        * `imgui` - project contains files of this library, because it was modified. 
    * OpenGL
        * `gl` - project contains files of this library, because it was modified.
    * Physics
        * `PlayRho` - modern 2D physics library. Included from submodules.
* Utils
    * `ALogger` - logging library, included from submodules.
    * `ByteArray` - byte array interface system. Included from submodules.
    * `glm` - math library, included from submodules.
    * `rapidxml` - project contains files of this library, because it was modified (`CMakeLists` added).
    * `stb` - project contains files of this library, cause it's very fragmented.
    
### Libraries required in system
1. `googletest` - Engine searches this library with `find_package(gtest)`. Required only if tests are built.
1. `zlib` - Engine searches this library with `find_package(zlib)`.