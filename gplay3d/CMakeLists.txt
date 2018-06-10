# disable lua binding for now
add_definitions(-DGP_NO_LUA_BINDINGS)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(GPLAY3D_PLATFORM_SRC
            src/core/PlatformMacOSX.mm
            src/core/gplay3d-main-macosx.mm
            )
else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(GPLAY3D_PLATFORM_SRC )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


set(GPLAY3D_H
    src/ai/AIAgent.h
    src/ai/AIController.h
    src/ai/AIMessage.h
    src/ai/AIState.h
    src/ai/AIStateMachine.h
    src/animation/Animation.h
    src/animation/AnimationClip.h
    src/animation/AnimationController.h
    src/animation/AnimationTarget.h
    src/animation/AnimationValue.h
    src/audio/AudioBuffer.h
    src/audio/AudioController.h
    src/audio/AudioListener.h
    src/audio/AudioSource.h
    src/core/Base.h
    src/core/Bundle.h
    src/core/DebugNew.h
    src/core/FileSystem.h
    src/core/FileWatcher.h
    src/core/Game.h
    src/core/Logger.h
    src/core/Platform.h
    src/core/Properties.h
    src/core/Ref.h
    src/core/Singleton.h
    src/core/Stream.h
    src/core/StringHash.h
    src/core/TimeListener.h
    src/core/Variant.h
    src/events/BaseEventData.h
    src/events/EventManager.h
    src/events/EventManagerBase.h
    src/events/FastDelegate.h
    src/events/FastDelegateBind.h
    src/graphics/Camera.h
    src/graphics/Drawable.h
    src/graphics/Effect.h
    src/graphics/Font.h
    src/graphics/FrameBuffer.h
    src/graphics/HeightField.h
    src/graphics/Image.h
    src/graphics/Joint.h
    src/graphics/Light.h
    src/graphics/Material.h
    src/graphics/MaterialParameter.h
    src/graphics/Mesh.h
    src/graphics/MeshBatch.h
    src/graphics/MeshPart.h
    src/graphics/MeshSkin.h
    src/graphics/Model.h
    src/graphics/Node.h
    src/graphics/ParticleEmitter.h
    src/graphics/Pass.h
    src/graphics/RenderState.h
    src/graphics/Scene.h
    src/graphics/SceneLoader.h
    src/graphics/ScreenDisplayer.h
    src/graphics/Sprite.h
    src/graphics/SpriteBatch.h
    src/graphics/Technique.h
    src/graphics/Terrain.h
    src/graphics/TerrainPatch.h
    src/graphics/Text.h
    src/graphics/Texture.h
    src/graphics/TileSet.h
    src/graphics/VertexFormat.h
    src/graphics/View.h
    src/input/Gamepad.h
    src/input/Gesture.h
    src/input/JoystickControl.h
    src/input/Keyboard.h
    src/input/Mouse.h
    src/input/Touch.h    
    src/math/BoundingBox.h
    src/math/BoundingSphere.h
    src/math/Curve.h
    src/math/Frustum.h
    src/math/MathUtil.h
    src/math/Matrix.h
    src/math/Plane.h
    src/math/Quaternion.h
    src/math/Ray.h
    src/math/Rectangle.h
    src/math/Transform.h
    src/math/Vector2.h
    src/math/Vector3.h
    src/math/Vector4.h
    src/physics/PhysicsCharacter.h
    src/physics/PhysicsCollisionObject.h
    src/physics/PhysicsCollisionShape.h
    src/physics/PhysicsConstraint.h
    src/physics/PhysicsController.h
    src/physics/PhysicsFixedConstraint.h
    src/physics/PhysicsGenericConstraint.h
    src/physics/PhysicsGhostObject.h
    src/physics/PhysicsHingeConstraint.h
    src/physics/PhysicsRigidBody.h
    src/physics/PhysicsSocketConstraint.h
    src/physics/PhysicsSpringConstraint.h
    src/physics/PhysicsVehicle.h
    src/physics/PhysicsVehicleWheel.h
    src/renderer/BGFXGpuProgram.h
    src/renderer/BGFXImGui.h
    src/renderer/BGFXIndexBuffer.h
    src/renderer/BGFXRenderer.h
    src/renderer/BGFXTexture.h
    src/renderer/BGFXUniform.h
    src/renderer/BGFXVertexBuffer.h
    src/renderer/GeometryBuffer.h
    src/renderer/MemoryBuffer.h
    src/renderer/Renderer.h
    src/script/Script.h
    src/script/ScriptController.h
    src/script/ScriptTarget.h
    src/sparkparticles/SparkBaseRenderer.h
    src/sparkparticles/SparkGP3D.h
    src/sparkparticles/SparkParticleEmitter.h
    src/sparkparticles/SparkQuadRenderer.h
    src/sparkparticles/SparkUtility.h
    src/ui/AbsoluteLayout.h
    src/ui/Button.h
    src/ui/CheckBox.h
    src/ui/Container.h
    src/ui/Control.h
    src/ui/ControlFactory.h
    src/ui/FlowLayout.h
    src/ui/Form.h
    src/ui/ImageControl.h
    src/ui/Label.h
    src/ui/Layout.h
    src/ui/RadioButton.h
    src/ui/Slider.h
    src/ui/TextBox.h
    src/ui/Theme.h
    src/ui/ThemeStyle.h
    src/ui/VerticalLayout.h
    src/gplay3d.h
)

set(GPLAY3D_CPP
    src/ai/AIAgent.cpp
    src/ai/AIController.cpp
    src/ai/AIMessage.cpp
    src/ai/AIState.cpp
    src/ai/AIStateMachine.cpp
    src/animation/Animation.cpp
    src/animation/AnimationClip.cpp
    src/animation/AnimationController.cpp
    src/animation/AnimationTarget.cpp
    src/animation/AnimationValue.cpp
    src/audio/AudioBuffer.cpp
    src/audio/AudioController.cpp
    src/audio/AudioListener.cpp
    src/audio/AudioSource.cpp
    src/core/Bundle.cpp
    src/core/DebugNew.cpp
    src/core/FileSystem.cpp
    src/core/FileWatcher.cpp
    src/core/Game.cpp   
    src/core/Logger.cpp
    src/core/Platform.cpp
    src/core/PlatformSDL2.cpp
    src/core/Properties.cpp
    src/core/Ref.cpp
    src/events/EventManager.cpp
    src/events/EventManagerBase.cpp
    src/graphics/Camera.cpp
    src/graphics/Drawable.cpp
    src/graphics/Effect.cpp
    src/graphics/Font.cpp
    src/graphics/FrameBuffer.cpp
    src/graphics/HeightField.cpp
    src/graphics/Image.cpp
    src/graphics/Joint.cpp
    src/graphics/Light.cpp
    src/graphics/Material.cpp
    src/graphics/MaterialParameter.cpp
    src/graphics/Mesh.cpp
    src/graphics/MeshBatch.cpp
    src/graphics/MeshPart.cpp
    src/graphics/MeshSkin.cpp
    src/graphics/Model.cpp
    src/graphics/Node.cpp
    src/graphics/ParticleEmitter.cpp
    src/graphics/Pass.cpp
    src/graphics/RenderState.cpp
    src/graphics/Scene.cpp
    src/graphics/SceneLoader.cpp
    src/graphics/ScreenDisplayer.cpp
    src/graphics/Sprite.cpp
    src/graphics/SpriteBatch.cpp
    src/graphics/Technique.cpp
    src/graphics/Terrain.cpp
    src/graphics/TerrainPatch.cpp
    src/graphics/Text.cpp
    src/graphics/Texture.cpp
    src/graphics/TileSet.cpp
    src/graphics/VertexFormat.cpp
    src/graphics/View.cpp
    src/input/Gamepad.cpp
    src/input/JoystickControl.cpp    
    src/math/BoundingBox.cpp
    src/math/BoundingSphere.cpp
    src/math/Curve.cpp
    src/math/Frustum.cpp
    src/math/MathUtil.cpp
    src/math/Matrix.cpp
    src/math/Plane.cpp
    src/math/Quaternion.cpp
    src/math/Ray.cpp
    src/math/Rectangle.cpp
    src/math/Transform.cpp
    src/math/Vector2.cpp
    src/math/Vector3.cpp
    src/math/Vector4.cpp
    src/physics/PhysicsCharacter.cpp
    src/physics/PhysicsCollisionObject.cpp
    src/physics/PhysicsCollisionShape.cpp
    src/physics/PhysicsConstraint.cpp
    src/physics/PhysicsController.cpp
    src/physics/PhysicsFixedConstraint.cpp
    src/physics/PhysicsGenericConstraint.cpp
    src/physics/PhysicsGhostObject.cpp
    src/physics/PhysicsHingeConstraint.cpp
    src/physics/PhysicsRigidBody.cpp
    src/physics/PhysicsSocketConstraint.cpp
    src/physics/PhysicsSpringConstraint.cpp
    src/physics/PhysicsVehicle.cpp
    src/physics/PhysicsVehicleWheel.cpp
    src/renderer/BGFXGpuProgram.cpp
    src/renderer/BGFXImGui.cpp
    src/renderer/BGFXIndexBuffer.cpp
    src/renderer/BGFXRenderer.cpp
    src/renderer/BGFXTexture.cpp
    src/renderer/BGFXUniform.cpp
    src/renderer/BGFXVertexBuffer.cpp
    src/renderer/GeometryBuffer.cpp
    src/renderer/MemoryBuffer.cpp
    src/script/Script.cpp
    src/script/ScriptController.cpp
    src/script/ScriptTarget.cpp
    src/sparkparticles/SparkBaseRenderer.cpp
    src/sparkparticles/SparkParticleEmitter.cpp
    src/sparkparticles/SparkQuadRenderer.cpp
    src/sparkparticles/SparkUtility.cpp
    src/ui/AbsoluteLayout.cpp
    src/ui/Button.cpp
    src/ui/CheckBox.cpp
    src/ui/Container.cpp
    src/ui/Control.cpp
    src/ui/ControlFactory.cpp
    src/ui/FlowLayout.cpp
    src/ui/Form.cpp
    src/ui/ImageControl.cpp
    src/ui/Label.cpp
    src/ui/Layout.cpp
    src/ui/RadioButton.cpp
    src/ui/Slider.cpp
    src/ui/TextBox.cpp
    src/ui/Theme.cpp
    src/ui/ThemeStyle.cpp
    src/ui/VerticalLayout.cpp
)



set(GPLAY3D_LUA
)

include_directories(
    src
    ../external-deps/include
)


MESSAGE("INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}")

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
    include_directories(${GTK2_INCLUDE_DIRS})
    add_definitions(${GTK2_CFLAGS_OTHER})
    add_definitions(-D__linux__)
	add_definitions(-lstdc++)
	set(GPLAY3D_MAIN src/core/gplay3d-main-linux.cpp)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(GPLAY3D_MAIN src/core/gplay3d-main-windows.cpp)	
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Windows")


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(-std=c++11 -stdlib=libc++)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_definitions(-std=c++11)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	include_directories(../external-deps/include/compat/msvc)
endif()

add_library(gplay3d STATIC
    ${GPLAY3D_H}
    ${GPLAY3D_CPP}
    ${GPLAY3D_MAIN}
    ${GPLAY3D_LUA}
)

set_target_properties(gplay3d PROPERTIES
    OUTPUT_NAME "gplay3d"
    CLEAN_DIRECT_OUTPUT 1
    VERSION ${GPLAY3D_VERSION}
)

#source_group(lua FILES ${GPLAY3D_LUA})
#source_group(res FILES ${GPLAY3D_RES} ${GPLAY3D_RES} ${GPLAY3D_RES_SHADERS} ${GPLAY3D_RES_UI})
source_group(src FILES ${GPLAY3D_H} ${GPLAY3D_CPP} ${GPLAY3D_MAIN})

# copy res directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../res DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
