$input a_position, a_normal, a_texcoord0, a_tangent, a_bitangent
$output v_color, v_texcoord0, v_normal, v_position, v_tbnViewSpace

#include "common/bgfx_shader.sh"



uniform mat4 u_worldViewProjectionMatrix;
uniform mat4 u_inverseTransposeWorldViewMatrix;
uniform mat4 u_worldMatrix;
uniform mat4 u_worldViewMatrix;
uniform mat4 u_viewMatrix;



#define DIRECTIONAL_LIGHT_COUNT 1
uniform vec4 u_directionalLightDirection[DIRECTIONAL_LIGHT_COUNT];


#define BUMP_MAP


void main()
{
    vec4 position = u_worldViewProjectionMatrix * a_position;
    v_texcoord0 = vec2(a_texcoord0.x, 1.0 - a_texcoord0.y);

    mat3 normalMatrix = mat3(u_inverseTransposeWorldViewMatrix);
    v_normal = normalMatrix * a_normal;
    

	v_position = u_worldViewMatrix * a_position;
	

#if defined(BUMP_MAP)
	mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix);
	vec3 N = normalize(inverseTransposeWorldViewMatrix * a_normal);
	vec3 T = normalize(inverseTransposeWorldViewMatrix * a_tangent);
    vec3 B = cross(T,N);
    v_tbnViewSpace = mat3(T,B,N);
#endif

    gl_Position = position;
}