$input a_position, a_normal, a_texcoord0
$output v_color, v_texcoord0, v_normal, v_position

#include "common/bgfx_shader.sh"



uniform mat4 u_worldViewProjectionMatrix;
uniform mat4 u_inverseTransposeWorldViewMatrix;
uniform mat4 u_worldMatrix;
uniform mat4 u_worldViewMatrix;



#define DIRECTIONAL_LIGHT_COUNT 1
uniform vec4 u_directionalLightDirection[DIRECTIONAL_LIGHT_COUNT];





void main()
{
    vec4 position = u_worldViewProjectionMatrix * a_position;
    v_texcoord0 = vec2(a_texcoord0.x, 1.0 - a_texcoord0.y);

    mat3 normalMatrix = mat3(u_inverseTransposeWorldViewMatrix);
    v_normal = normalMatrix * a_normal;
    

	v_position = u_worldMatrix * a_position;


    gl_Position = position;
}