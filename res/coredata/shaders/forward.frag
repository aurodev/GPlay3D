$input v_texcoord0, v_color, v_normal, v_position

#include "common/bgfx_shader.sh"

SAMPLER2D(u_diffuseTexture, 0);


uniform vec4 u_ambientColor;

#define DIRECTIONAL_LIGHT_COUNT 1
uniform vec4 u_directionalLightDirection[DIRECTIONAL_LIGHT_COUNT];
uniform vec4 u_directionalLightColor[DIRECTIONAL_LIGHT_COUNT];


uniform vec3 u_cameraPosition;
uniform mat4 u_worldViewMatrix;






void main()
{
	vec4 texColor = texture2D(u_diffuseTexture, v_texcoord0);

    vec3 ambient = u_ambientColor.rgb;


    vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(u_directionalLightDirection[0].xyz - v_position.xyz);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_directionalLightColor[0].rgb;


	vec3 result = (ambient + diffuse) * texColor.rgb;

    gl_FragColor = vec4(result, 1.0);
}

