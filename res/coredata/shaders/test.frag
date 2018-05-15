
varying vec3 v_position;
varying vec3 v_normal;
varying vec2 v_texcoord0;

uniform sampler2D s_diffuse;
uniform sampler2D s_specular;


void main()
{
    // store the fragment position vector in the first gbuffer texture
    gl_FragData[0] = vec4(v_position, 1.0);

    // also store the per-fragment normals into the gbuffer
    gl_FragData[1] = vec4(normalize(v_normal), 1.0);

    // and the diffuse per-fragment color
    gl_FragData[2].rgb = texture2D(s_diffuse, v_texcoord0).rgb;
    gl_FragData[2].a = texture2D(s_specular, v_texcoord0).r;
}