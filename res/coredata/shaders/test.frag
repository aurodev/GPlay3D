
varying vec4 v_position;
varying vec3 v_normal;
varying vec2 v_texcoord0;

uniform sampler2D s_diffuse;
uniform sampler2D s_specular;

void main()
{
    gl_FragData[0] = v_position;

    gl_FragData[1] = normalize(vec4(v_normal, 1));

    gl_FragData[2].rgb = texture2D(s_diffuse, v_texcoord0).rgb;
    gl_FragData[2].a = texture2D(s_specular, v_texcoord0).a;
}