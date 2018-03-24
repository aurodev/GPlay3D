#ifndef BASE_H_
#define BASE_H_

// C/C++
#include <new>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cwchar>
#include <cwctype>
#include <cctype>
#include <cmath>
#include <cstdarg>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <set>
#include <stack>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <limits>
#include <functional>
#include <bitset>
#include <typeinfo>
#include <thread>
#include <mutex>
#include <chrono>
#include "Logger.h"

#ifdef __ANDROID__
    #define GP_PLATFORM_ANDROID		1
#elif WIN32
    #define GP_PLATFORM_WINDOWS		1
#elif __linux__
    #define GP_PLATFORM_LINUX		1
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        #define GP_PLATFORM_IOS		1
    #elif TARGET_OS_MAC
        #define GP_PLATFORM_MACOS	1
    #endif
#endif


// Common
#ifndef NULL
#define NULL     0
#endif

namespace gameplay
{
/**
 * Print logging (implemented per platform).
 * @script{ignore}
 */
extern void print(const char* format, ...);

// Define a platform-independent case-insensitive ASCII string comparison function.
extern int strcmpnocase(const char* s1, const char* s2);
}

// Current function macro.
#ifdef WIN32
#define __current__func__ __FUNCTION__
#else
#define __current__func__ __func__
#endif

// Assert macros.
#ifdef _DEBUG
#define GP_ASSERT(expression) assert(expression)
#else
#define GP_ASSERT(expression)
#endif

#if defined(WIN32) && defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK()
#endif

// Error macro.
#ifdef GP_ERRORS_AS_WARNINGS
#define GP_ERROR GP_WARN
#else
#define GP_ERROR(...) do \
    { \
        gameplay::Logger::log(gameplay::Logger::LEVEL_ERROR, "%s -- ", __current__func__); \
        gameplay::Logger::log(gameplay::Logger::LEVEL_ERROR, __VA_ARGS__); \
        gameplay::Logger::log(gameplay::Logger::LEVEL_ERROR, "\n"); \
        DEBUG_BREAK(); \
        assert(0); \
        std::exit(-1); \
    } while (0)
#endif

// Warning macro.
#define GP_WARN(...) do \
    { \
        gameplay::Logger::log(gameplay::Logger::LEVEL_WARN, "%s -- ", __current__func__); \
        gameplay::Logger::log(gameplay::Logger::LEVEL_WARN, __VA_ARGS__); \
        gameplay::Logger::log(gameplay::Logger::LEVEL_WARN, "\n"); \
    } while (0)

// Info macro.
#define GP_INFO(...) do \
    { \
        gameplay::Logger::log(gameplay::Logger::LEVEL_INFO, "%s -- ", __current__func__); \
        gameplay::Logger::log(gameplay::Logger::LEVEL_INFO, __VA_ARGS__); \
        gameplay::Logger::log(gameplay::Logger::LEVEL_INFO, "\n"); \
    } while (0)

#if defined(WIN32)
    #pragma warning( disable : 4005 )
    #pragma warning( disable : 4172 )
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4267 )
    #pragma warning( disable : 4311 )
    #pragma warning( disable : 4316 )
    #pragma warning( disable : 4390 )
    #pragma warning( disable : 4800 )
    #pragma warning( disable : 4996 )
#endif

// Bullet Physics
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#define BV(v) (btVector3((v).x, (v).y, (v).z))
#define BQ(q) (btQuaternion((q).x, (q).y, (q).z, (q).w))

// Debug new for memory leak detection
#include "DebugNew.h"

// Object deletion macro
#define SAFE_DELETE(x) \
    { \
        delete x; \
        x = NULL; \
    }

// Array deletion macro
#define SAFE_DELETE_ARRAY(x) \
    { \
        delete[] x; \
        x = NULL; \
    }

// Ref cleanup macro
#define SAFE_RELEASE(x) \
    if (x) \
    { \
        (x)->release(); \
        x = NULL; \
    }

// Math
#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)
#define MATH_RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f)      // Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.
#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f
#define MATH_E                      2.71828182845904523536f
#define MATH_LOG10E                 0.4342944819032518f
#define MATH_LOG2E                  1.442695040888963387f
#define MATH_PI                     3.14159265358979323846f
#define MATH_PIOVER2                1.57079632679489661923f
#define MATH_PIOVER4                0.785398163397448309616f
#define MATH_PIX2                   6.28318530717958647693f
#define MATH_EPSILON                0.000001f
#define MATH_CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))
#ifndef M_1_PI
#define M_1_PI                      0.31830988618379067154
#endif

// NOMINMAX makes sure that windef.h doesn't add macros min and max
#ifdef WIN32
    #define NOMINMAX
#endif

// Audio (OpenAL)
#ifdef __ANDROID__
    #include <AL/al.h>
    #include <AL/alc.h>
    #define AL_ALEXT_PROTOTYPES
    #include <AL/alext.h>
#elif WIN32
    #define AL_LIBTYPE_STATIC
    #include <AL/al.h>
    #include <AL/alc.h>
#elif __linux__
    #include <AL/al.h>
    #include <AL/alc.h>
#elif __APPLE__
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#endif

// Compressed Media
#include <vorbis/vorbisfile.h>

// Image
#include <png.h>

// Scripting
using std::va_list;
#include <lua/lua.hpp>

#define WINDOW_VSYNC        1

// Graphics (OpenGL)
#ifdef __ANDROID__
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
    extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
    extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
    extern PFNGLISVERTEXARRAYOESPROC glIsVertexArray;
    extern PFNGLMAPBUFFEROESPROC glMapBuffer;
    extern PFNGLUNMAPBUFFEROESPROC glUnmapBuffer;
    #define GL_WRITE_ONLY GL_WRITE_ONLY_OES
    #define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
    #define glClearDepth glClearDepthf
    #define OPENGL_ES
    #define GP_USE_VAO
#elif WIN32
        #define WIN32_LEAN_AND_MEAN
        #define GLEW_STATIC
        #include <GL/glew.h>
        #define GP_USE_VAO
        #include <bgfx/bgfx.h>
#elif __linux__
        #include <bgfx/bgfx.h>
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
        #define glBindVertexArray glBindVertexArrayOES
        #define glDeleteVertexArrays glDeleteVertexArraysOES
        #define glGenVertexArrays glGenVertexArraysOES
        #define glIsVertexArray glIsVertexArrayOES
        #define glMapBuffer glMapBufferOES
        #define glUnmapBuffer glUnmapBufferOES
        #define GL_WRITE_ONLY GL_WRITE_ONLY_OES
        #define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
        #define glClearDepth glClearDepthf
        #define OPENGL_ES
        #define GP_USE_VAO
    #elif TARGET_OS_MAC
        #include <OpenGL/gl.h>
        #include <OpenGL/glext.h>
        #define glBindVertexArray glBindVertexArrayAPPLE
        #define glDeleteVertexArrays glDeleteVertexArraysAPPLE
        #define glGenVertexArrays glGenVertexArraysAPPLE
        #define glIsVertexArray glIsVertexArrayAPPLE
        #define GP_USE_VAO
    #else
        #error "Unsupported Apple Device"
    #endif
#endif

// Hardware buffer
namespace gameplay
{

/** Gamepad handle */
#ifdef __ANDROID__
typedef unsigned int GamepadHandle;
#else
typedef unsigned long GamepadHandle;
#endif
}


/**
 * Executes the specified AL code and checks the AL error afterwards
 * to ensure it succeeded.
 *
 * The AL_LAST_ERROR macro can be used afterwards to check whether a AL error was
 * encountered executing the specified code.
 */
#define AL_CHECK( al_code ) do \
    { \
        while (alGetError() != AL_NO_ERROR) ; \
        al_code; \
        __al_error_code = alGetError(); \
        if (__al_error_code != AL_NO_ERROR) \
        { \
            GP_ERROR(#al_code ": %d", (int)__al_error_code); \
        } \
    } while(0)

/** Global variable to hold AL errors
 * @script{ignore} */
extern ALenum __al_error_code;

/**
 * Accesses the most recently set global AL error.
 */
#define AL_LAST_ERROR() __al_error_code

#endif
