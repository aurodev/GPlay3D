# GPlay3D

GPlay3D is C ++ cross-platform game engine for creating 2D / 3D games. Based on a fork of GamePlay engine v3.0.0, goal is to continue the evolution of this version of the engine by replacing opengl with bgfx framework and integrating new features.

## Status
- Actually only Qt projects are up to date to build engine and samples, cmake will follow soon.
- bgfx integration is almost finished. Currently it only works with bgfx opengl because it use old raw glsl shaders. It need a pass on shaders to use the bgfx shaderc syntax to be compatible with others bgfx renderers. 
- Engine now use SDL2 by default to manage windows and inputs. Gamepad are not yet implanted.
- Lua is temporarily disabled to speed up the compilation.
- All previous gameplay samples and demo are now working.
- Tested on Linux and Windows.

## Screenshots
![Imgur](https://i.imgur.com/u3arwg3.png)
![Imgur](https://i.imgur.com/JNNVlAo.jpg)
![Imgur](https://i.imgur.com/mXvz27x.jpg)
![Imgur](https://i.imgur.com/0ei9Y28.png)
