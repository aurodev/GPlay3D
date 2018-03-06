//----------------------------------------------------------
// Attributes
//----------------------------------------------------------
$input a_position
$input a_normal
$input a_tangent
$input a_bitangent
$input a_texcoord0
$input a_texcoord1
$input i_data0
$input i_data1
$input i_data2
$input i_data3

//----------------------------------------------------------
// Varyings
//----------------------------------------------------------
$output v_texcoord0
$output v_texcoord1
$output v_normalVector
$output v_tangent
$output v_bitangent
$output v_normal
$output v_positionWorldViewSpace
$output v_cameraDirection
$output v_clipDistance



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
#if defined(SKINNING)
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];
#endif

#if defined(LIGHTING)
uniform mat4 u_inverseTransposeWorldViewMatrix;

#if defined(SPECULAR) || (POINT_LIGHT_COUNT > 0) || (SPOT_LIGHT_COUNT > 0)
uniform mat4 u_worldViewMatrix;
#endif

#if defined(BUMPED) && (DIRECTIONAL_LIGHT_COUNT > 0)
uniform vec4 u_directionalLightDirection[DIRECTIONAL_LIGHT_COUNT];
#endif

#if (POINT_LIGHT_COUNT > 0)
uniform vec4 u_pointLightPosition[POINT_LIGHT_COUNT];
#endif

#if (SPOT_LIGHT_COUNT > 0) 
uniform vec4 u_spotLightPosition[SPOT_LIGHT_COUNT];
#if defined(BUMPED)
uniform vec4 u_spotLightDirection[SPOT_LIGHT_COUNT];
#endif
#endif

#if defined(SPECULAR)
uniform vec4 u_cameraPosition;
#endif

#endif

#if defined(TEXTURE_REPEAT)
uniform vec4 u_textureRepeat;
#endif

#if defined(TEXTURE_OFFSET)
uniform vec4 u_textureOffset;
#endif

#if defined(CLIP_PLANE)
uniform mat4 u_worldMatrix;
uniform vec4 u_clipPlane;
#endif



#if defined(SKINNING)
    #include "skinning.vert"
#else
    #include "skinning-none.vert" 
#endif


//----------------------------------------------------------
// Code
//----------------------------------------------------------
void main()
{
    vec4 position = getPosition();

#if defined (INSTANCED)
    mat4 model;
    model[0] = i_data0;
    model[1] = i_data1;
    model[2] = i_data2;
    model[3] = i_data3;
    vec4 worldPos = model * position;
    gl_Position = u_worldViewProjectionMatrix * worldPos;
#else
    gl_Position = u_worldViewProjectionMatrix * position;
#endif

    #if defined(LIGHTING)
        vec3 normal = getNormal();
        // Transform the normal, tangent and binormals to view space.
        mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
        v_normal = normalize(inverseTransposeWorldViewMatrix * normal);    

        #if defined(SPECULAR) || (POINT_LIGHT_COUNT > 0) || (SPOT_LIGHT_COUNT > 0)
    	   v_positionWorldViewSpace = u_worldViewMatrix * position;
        #endif

        #if defined(BUMPED)    
            vec3 tangent = getTangent();
            vec3 binormal = getBinormal();
            v_tangent  = normalize(inverseTransposeWorldViewMatrix * tangent);
            v_bitangent = normalize(inverseTransposeWorldViewMatrix * binormal);
            
             mat3 tangentSpaceTransformMatrix = mat3(
                normalize(v_tangent),
                normalize(v_bitangent),
                normalize(v_normal)
                );   

            //mat3 tangentSpaceTransformMatrix = mat3(tangentVector.x, binormalVector.x, normalVector.x, tangentVector.y, binormalVector.y, normalVector.y, tangentVector.z, binormalVector.z, normalVector.z);
            //@@applyLight(position, tangentSpaceTransformMatrix);
            //v_tangentSpaceTransformMatrix = tangentSpaceTransformMatrix;
        #else    
            //v_normalVector = normalVector;
            //@@applyLight(position);
            //v_normal = normalVector;
        #endif

        #if defined(SPECULAR)
            #if defined(BUMPED)
                v_cameraDirection = tangentSpaceTransformMatrix * (u_cameraPosition.xyz - v_positionWorldViewSpace.xyz);
            #else
                v_cameraDirection = u_cameraPosition.xyz - v_positionWorldViewSpace.xyz;
            #endif
        #endif

    #endif    
    
    v_texcoord0 = vec2(a_texcoord0.x, 1.0 - a_texcoord0.y);
    
    #if defined(TEXTURE_REPEAT)
        v_texcoord0 *= u_textureRepeat.xy;
    #endif
    
    #if defined(TEXTURE_OFFSET)
        v_texcoord0 += u_textureOffset.xy;
    #endif
    
    #if defined(LIGHTMAP)
        v_texcoord1 = a_texcoord1;
    #endif
    
    #if defined(CLIP_PLANE)
        v_clipDistance = dot(u_worldMatrix * position, u_clipPlane);
    #endif
}
