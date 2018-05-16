
varying vec4 v_position;
varying vec2 v_texcoord0;


uniform sampler2D s_albedo;
uniform sampler2D s_light;


vec3 toLinear(vec3 _rgb)
{
    return pow(abs(_rgb), vec3(2.2) );
}

vec4 toLinear(vec4 _rgba)
{
    return vec4(toLinear(_rgba.xyz), _rgba.w);
}

float toGamma(float _r)
{
    return pow(abs(_r), 1.0/2.2);
}

vec3 toGamma(vec3 _rgb)
{
    return pow(abs(_rgb), vec3(1.0/2.2) );
}

vec4 toGamma(vec4 _rgba)
{
    return vec4(toGamma(_rgba.xyz), _rgba.w);
}


void main()
{
    vec4 albedo = toLinear(texture2D(s_albedo, v_texcoord0));
    vec4 light = toLinear(texture2D(s_light, v_texcoord0));
    gl_FragColor = toGamma(albedo*light);
}
