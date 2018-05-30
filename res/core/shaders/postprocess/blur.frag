
varying vec2 v_texcoord0;


uniform sampler2D image;


uniform vec4 u_horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

uniform vec4 u_direction;


vec3 blur(sampler2D image, vec2 tc)
{

    vec2 tex_offset = 1.0 / vec2(1024.0, 1024.0);    //textureSize(image, 0); // gets size of single texel
     vec3 result = texture2D(image, v_texcoord0).rgb * weight[0];

    for(int i = 1; i < 5; ++i)
         {
            result += texture2D(image, v_texcoord0 + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture2D(image, v_texcoord0 - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }

         for(int i = 1; i < 5; ++i)
         {
             result += texture2D(image, v_texcoord0 + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture2D(image, v_texcoord0 - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
    

    return result;
}


#include "../blur.glsl"


void main()
{    
   //gl_FragColor = vec4(blur(image, v_texcoord0), 1.0);

   float size = 512; //2048.0 / 10.0;
   vec2 texelSize = vec2(size, size);
   gl_FragColor = blur9(image, v_texcoord0, texelSize, u_direction.xy);
  return;


    bool horizontal = bool(u_horizontal.x);

     vec2 tex_offset = 1.0 / vec2(1024.0, 1024.0);    //textureSize(image, 0); // gets size of single texel
     vec3 result = texture2D(image, v_texcoord0).rgb * weight[0];
     if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture2D(image, v_texcoord0 + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture2D(image, v_texcoord0 - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture2D(image, v_texcoord0 + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture2D(image, v_texcoord0 - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
     }
     gl_FragColor = vec4(result, 1.0);


}