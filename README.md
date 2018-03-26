# GPlay3D
GPlay3D is C++ cross-platform game engine for creating 2D/3D games.

GPlay3D is a fork of the [GamePlay engine](http://www.gameplay3d.io/) v3.0 and based on the [BGFX](https://github.com/bkaradzic/bgfx) graphics framework. Goal of this fork is to continue evolution of this engine by integrating new features and modern rendering techniques.


## Status
- bgfx integration is completed however it currently only works with bgfx opengl because it use the old glsl shaders. It needs a pass on shaders to use the bgfx shaderc syntax to be compatible with others bgfx renderers. 
- Engine now use SDL2 by default to manage windows and inputs. Gamepad are not yet implanted.
- Lua is temporarily disabled to speed up the compilation.
- All previous samples and demo are now working with the new renderer.
- Tested on Linux and Windows.


## Building
* [CMake (Linux)](https://github.com/fredakilla/GPlay3D/wiki/CMake-Linux-Setup)
* [CMake (Windows)](https://github.com/fredakilla/GPlay3D/wiki/CMake-Windows-Setup)
* [Qt Creator (Linux, Windows, MacOS)](https://github.com/fredakilla/GPlay3D/wiki/QtCreator-Setup)

See [wiki](https://github.com/fredakilla/GPlay3D/wiki) for more detailed build instructions.


## Screenshots
<img src="https://i.imgur.com/u3arwg3.png" width="50%" height="%"><img src="https://i.imgur.com/JNNVlAo.jpg" width="50%" height="%">
<img src="https://i.imgur.com/0ei9Y28.png" width="50%" height="%"><img src="https://i.imgur.com/mXvz27x.jpg" width="50%" height="%">
<img src="https://i.imgur.com/nRpTNIm.jpg" width="50%" height="%"><img src="https://i.imgur.com/SDIgTkt.png" width="50%" height="%">


## Features
- BGFX based rendering system.
- Scene graph system with support for lights, cameras, audio, physics, and drawables.
- Declarative scene, animation, particles and material bindings.
- Material system with built-in shader library (forward rendering).
- Post-processing.
- Physics using Bullet.
- Particle effects with built-in particle system or SPARK engine system.
- Height map based terrains with multiple surface layers and LOD.
- Easy-to-use sprite, tileset and text rendering.
- Declarative UI system supporing 2D/3D theme-able user interfaces.
- Nice, customizable and complete built-in 2D GUI, with buttons, lists, edit boxes, layout...
- ImGui GUI support.
- File watcher system for hot reloading (shaders, scripts...)
- Fully extensible animation system with skeletal character animation.
- Complete 3D audio system with WAV and OGG support.
- AI agent, state machine and messaging.
- Event messaging system.
- Full vector math library with 2D/3D math and visibility culling.
- Mouse, keyboard, touch, gestures and gamepad support.
- Lua script bindings and user binding generator tool.
- Binary encoding tool for creating optimized asset bundles for TTF fonts and 3D FBX assets.
- Documented using doxygen.
- Image supported format (DDS, KTX, PVR, JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
- 3D model supported format (FBX)

## Todo
- Add assimp library to support new 3D file formats
- Deferred rendering.
- Shadow mapping.
- Editor.