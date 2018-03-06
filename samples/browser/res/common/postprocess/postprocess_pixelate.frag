$input v_texcoord0

#include "../../shaders/common/common.sh"

SAMPLER2D(u_texture, 0);


float discretize(float f, float d)
{
    return floor(f*d + 0.5)/d;
}

vec2 discretize(vec2 v, float d)
{
    return vec2(discretize(v.x, d), discretize(v.y, d));
}

void main()
{
    vec2 texCoord = discretize(v_texcoord0, 128.0);
    gl_FragColor = texture2D(u_texture, texCoord);
}

 
 


