$input v_texcoord0, v_color

#include "common/common.sh"

///////////////////////////////////////////////////////////
// Uniforms
//@@uniform sampler2D u_texture;
SAMPLER2D(u_texture, 0);

///////////////////////////////////////////////////////////
// Varyings
//@@varying vec2 v_texCoord;
//@@varying vec4 v_color;


void main()
{
    gl_FragColor = v_color * texture2D(u_texture, v_texcoord0);
}