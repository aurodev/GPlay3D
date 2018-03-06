$input v_texcoord0

#include "../../shaders/common/common.sh"

SAMPLER2D(u_texture, 0);

void main()
{
    // Sample the diffuse texture for base color
    float gray = dot(texture2D(u_texture, v_texcoord0).rgb, vec3(0.299, 0.587, 0.114));
    
    gl_FragColor = vec4(gray, gray, gray, 1.0);
}
