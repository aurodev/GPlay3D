//----------------------------------------------------------
// Attributes
//----------------------------------------------------------
$input a_position
$input a_normal
$input a_texcoord0


//----------------------------------------------------------
// Varyings
//----------------------------------------------------------
$output v_normal
$output v_texcoord0
$output v_texCoordLayer0
$output v_texCoordLayer1
$output v_texCoordLayer2



#include "common/common.sh"


#ifndef DIRECTIONAL_LIGHT_COUNT
    #define DIRECTIONAL_LIGHT_COUNT 0
#endif
#ifndef SPOT_LIGHT_COUNT
    #define SPOT_LIGHT_COUNT 0
#endif
#ifndef POINT_LIGHT_COUNT
    #define POINT_LIGHT_COUNT 0
#endif
#if (DIRECTIONAL_LIGHT_COUNT > 0) || (POINT_LIGHT_COUNT > 0) || (SPOT_LIGHT_COUNT > 0)
    #define LIGHTING
#endif




//----------------------------------------------------------
// Uniforms
//----------------------------------------------------------
uniform mat4 u_worldViewProjectionMatrix;
#if !defined(NORMAL_MAP) && defined(LIGHTING)
uniform mat4 u_normalMatrix;
#endif

#if defined(LIGHTING)

uniform mat4 u_inverseTransposeWorldViewMatrix;

#if (POINT_LIGHT_COUNT > 0) || (SPOT_LIGHT_COUNT > 0)
uniform mat4 u_worldViewMatrix;
#endif

#if (DIRECTIONAL_LIGHT_COUNT > 0)
uniform vec4 u_directionalLightDirection[DIRECTIONAL_LIGHT_COUNT];
#endif

#if (POINT_LIGHT_COUNT > 0) 
uniform vec4 u_pointLightPosition[POINT_LIGHT_COUNT];
#endif

#if (SPOT_LIGHT_COUNT > 0)
uniform vec4 u_spotLightPosition[SPOT_LIGHT_COUNT];
uniform vec4 u_spotLightDirection[SPOT_LIGHT_COUNT];
#endif

#endif



//----------------------------------------------------------
// Code
//----------------------------------------------------------
void main()
{
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * a_position;

#if defined(LIGHTING)

    #if !defined(NORMAL_MAP) 
        v_normal = normalize((u_normalMatrix * vec4(a_normal.x, a_normal.y, a_normal.z, 0)).xyz);
    #endif

#endif

    // Pass base texture coord
    v_texcoord0 = a_texcoord0;

    // Pass repeated texture coordinates for each layer
    #if LAYER_COUNT > 0
        v_texCoordLayer0 = a_texcoord0 * TEXTURE_REPEAT_0;
    #endif
    #if LAYER_COUNT > 1
        v_texCoordLayer1 = a_texcoord0 * TEXTURE_REPEAT_1;
    #endif
    #if LAYER_COUNT > 2
        v_texCoordLayer2 = a_texcoord0 * TEXTURE_REPEAT_2;
    #endif
}
