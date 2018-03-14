# GPlay3D
GPlay3D is C++ cross-platform game engine for creating 2D / 3D games.
GPlay3D is a fork of the [GamePlay engine](http://www.gameplay3d.io/) v3.0 and based on the [BGFX](https://github.com/bkaradzic/bgfx) graphics api.

Goal of this fork is to continue the evolution of this engine version by integrating new functionalities and modern rendering techniques.


## Status
- Actually only Qt projects are up to date to build engine and samples, cmake will follow soon.
- bgfx integration is almost finished. Currently it only works with bgfx opengl because it use old raw glsl shaders. It need a pass on shaders to use the bgfx shaderc syntax to be compatible with others bgfx renderers. 
- Engine now use SDL2 by default to manage windows and inputs. Gamepad are not yet implanted.
- Lua is temporarily disabled to speed up the compilation.
- All previous gameplay samples and demo are now working.
- Tested on Linux and Windows.


## Screenshots
<img src="https://i.imgur.com/u3arwg3.png" width="50%" height="%"><img src="https://i.imgur.com/JNNVlAo.jpg" width="50%" height="%">
<img src="https://i.imgur.com/0ei9Y28.png" width="50%" height="%"><img src="https://i.imgur.com/mXvz27x.jpg" width="50%" height="%">
<img src="https://i.imgur.com/nRpTNIm.jpg" width="50%" height="%"><img src="https://i.imgur.com/SDIgTkt.png" width="50%" height="%">
