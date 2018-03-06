$input v_texcoord0, v_color

#include "common/common.sh"


SAMPLER2D(u_texture, 0);

#ifdef DISTANCE_FIELD
    uniform vec2 u_cutoff;
#endif


void main()
{ 
#ifdef DISTANCE_FIELD
    
    gl_FragColor = v_color;
    float distance = texture2D(u_texture, v_texcoord0).r;
    float smoothing = fwidth(distance);
    float alpha = smoothstep(0.5 - smoothing * u_cutoff.x, 0.5 + smoothing * u_cutoff.y, distance);
    gl_FragColor.a = alpha * v_color.a;
    
#else
    
    gl_FragColor = v_color;
    gl_FragColor.a = texture2D(u_texture, v_texcoord0).r * v_color.a;
    
#endif
}
