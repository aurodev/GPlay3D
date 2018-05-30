
varying vec4 v_position;
varying vec2 v_texcoord0;


uniform sampler2D s_albedo;
uniform sampler2D s_light;
uniform sampler2D s_bloom;


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






vec4 blur(sampler2D image, vec2 tc)
{
    vec4 sum = vec4(0.0);   
    
    //the amount to blur, i.e. how far off center to sample from 
    //1.0 -> blur by one pixel
    //2.0 -> blur by two pixels, etc.
    float blur = 1.0 / 1024.0; //radius/resolution; 
    
    //the direction of our blur
    //(1.0, 0.0) -> x-axis blur
    //(0.0, 1.0) -> y-axis blur
    float hstep = 0.5; //dir.x;
    float vstep = 0.5; //dir.y;
    
    //apply blurring, using a 9-tap filter with predefined gaussian weights
    
    sum += texture2D(image, vec2(tc.x - 4.0*blur*hstep, tc.y - 4.0*blur*vstep)) * 0.0162162162;
    sum += texture2D(image, vec2(tc.x - 3.0*blur*hstep, tc.y - 3.0*blur*vstep)) * 0.0540540541;
    sum += texture2D(image, vec2(tc.x - 2.0*blur*hstep, tc.y - 2.0*blur*vstep)) * 0.1216216216;
    sum += texture2D(image, vec2(tc.x - 1.0*blur*hstep, tc.y - 1.0*blur*vstep)) * 0.1945945946;
    
    sum += texture2D(image, vec2(tc.x, tc.y)) * 0.2270270270;
    
    sum += texture2D(image, vec2(tc.x + 1.0*blur*hstep, tc.y + 1.0*blur*vstep)) * 0.1945945946;
    sum += texture2D(image, vec2(tc.x + 2.0*blur*hstep, tc.y + 2.0*blur*vstep)) * 0.1216216216;
    sum += texture2D(image, vec2(tc.x + 3.0*blur*hstep, tc.y + 3.0*blur*vstep)) * 0.0540540541;
    sum += texture2D(image, vec2(tc.x + 4.0*blur*hstep, tc.y + 4.0*blur*vstep)) * 0.0162162162;

    return sum;
}


vec4 bluury(sampler2D image, vec2 uv)
{
    vec4 sum = vec4(0.0);
    float LENGTH = 0.001;
 
   // blur in y (vertical) take nine samples, with the distance LENGTH between them
   sum += texture2D(image, vec2(uv.x - 4.0 * LENGTH, uv.y)) * 0.05;
   sum += texture2D(image, vec2(uv.x - 3.0 * LENGTH, uv.y)) * 0.09;
   sum += texture2D(image, vec2(uv.x - 2.0 * LENGTH, uv.y)) * 0.12;
   sum += texture2D(image, vec2(uv.x -       LENGTH, uv.y)) * 0.15;
   sum += texture2D(image, vec2(uv.x, uv.y)) * 0.16;
   sum += texture2D(image, vec2(uv.x +       LENGTH, uv.y)) * 0.15;
   sum += texture2D(image, vec2(uv.x + 2.0 * LENGTH, uv.y)) * 0.12;
   sum += texture2D(image, vec2(uv.x + 3.0 * LENGTH, uv.y)) * 0.09;
   sum += texture2D(image, vec2(uv.x + 4.0 * LENGTH, uv.y)) * 0.05;
 
   return sum;
}


#include "../blur.glsl"


void main()
{
    /*vec4 albedo = toLinear(texture2D(s_albedo, v_texcoord0));
    vec4 light = toLinear(texture2D(s_light, v_texcoord0));
    gl_FragColor = toGamma(albedo*light);
    return;*/






    vec4 albedo = (texture2D(s_albedo, v_texcoord0));
    vec4 light = (texture2D(s_light, v_texcoord0));
    vec4 bloom = texture2D(s_bloom, v_texcoord0);
    //vec4 bloom = blur(s_bloom, v_texcoord0);


    vec3 hdrColor = vec3(albedo*light).rgb;
    const float gamma = 2.2;
    float exposure = 1.0;



    /*float size = 1024.0 / 2.0;
   vec2 texelSize = vec2(size, size);
    vec4 bluuur = (blur9(s_bloom, v_texcoord0, texelSize, vec2(0, 1.0)));
    hdrColor += vec3(bluuur.rgb);*/
    //
    hdrColor += vec3(bloom.rgb);

    //vec4 bluuur = bluury(s_bloom, v_texcoord0);
    //hdrColor += vec3(bluuur.rgb);    
    


    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    // Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

    gl_FragColor = vec4(mapped, 1.0);

}
