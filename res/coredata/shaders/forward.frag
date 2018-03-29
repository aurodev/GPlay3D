$input v_texcoord0, v_color, v_normal

#include "common/bgfx_shader.sh"

SAMPLER2D(u_diffuseTexture, 0);


uniform vec4 u_ambientColor;

#define DIRECTIONAL_LIGHT_COUNT 1
uniform vec4 u_directionalLightDirection[DIRECTIONAL_LIGHT_COUNT];
uniform vec4 u_directionalLightColor[DIRECTIONAL_LIGHT_COUNT];



void main()
{

    vec3 n = v_normal;
    // transform light direction from world sapce to view space 
    //vec4 view_L_pos =  u_viewMatrix * -u_directionalLightDirection[0];
    // or
    //vec4 view_L_pos =  u_directionalLightDirection[0];
    //vec3 l = normalize(view_L_pos.xyz);
    vec3 l = normalize(u_directionalLightDirection[0].xyz);

    float intensity = max(dot(n,l), 0.0);

    vec4 color = intensity * u_directionalLightColor[0];

    color = max(color, u_ambientColor);

    gl_FragColor =  color * texture2D(u_diffuseTexture, v_texcoord0);
}