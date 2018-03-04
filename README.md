# GPlay3D

GPlay3D is C ++ cross-platform game engine for creating 2D / 3D games. Based on a fork of GamePlay engine v3.0.0, goal is to continue the evolution of this version of the engine by replacing opengl with bgfx framework and integrating new features.

## Status
bgfx integration is almost finished. Currently it only works with bgfx opengl because it use old raw glsl shaders. It need a pass on shaders to use the bgfx shaderc syntax to be compatible with others bgfx renderers. 

Engine now use SDL2 by default to manage windows and inputs. Gamepad are not yet implanted.

All previous gameplay samples and demo are working but it remains to fix some path and ressources on repo.

Tested on Linux and Windows.

## Screenshots
![Imgur](https://i.imgur.com/u3arwg3.png)
![Imgur](https://i.imgur.com/JNNVlAo.jpg)
![Imgur](https://i.imgur.com/mXvz27x.jpg)
![Imgur](https://i.imgur.com/0ei9Y28.png)
