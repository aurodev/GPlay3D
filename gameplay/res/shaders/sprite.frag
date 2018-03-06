$input v_texcoord0, v_color

#include "common/common.sh"

SAMPLER2D(u_texture, 0);

void main()
{
    gl_FragColor = v_color * texture2D(u_texture, v_texcoord0);
}