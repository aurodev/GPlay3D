vec3 v_color                    : COLOR0 = vec3(1.0, 0.0, 0.0);
vec2 v_texcoord1                : TEXCOORD1 = vec2(0.0, 0.0);
vec3 v_normalVector                   : NORMAL = vec3(0.0, 0.0, 0.0);  
vec4 v_positionWorldViewSpace   : TEXCOORD2 = vec4(0.0, 0.0, 0.0, 0.0);
vec3 v_cameraDirection          : TEXCOORD3 = vec3(0.0, 0.0, 0.0);
float v_clipDistance            : TEXCOORD4 = float(0.0);


vec4 a_position     : POSITION;
vec3 a_color0       : COLOR0;
vec3 a_normal       : NORMAL;
vec4 a_blendWeights : BLENDWEIGHT;
vec4 a_blendIndices : BLENDINDICES;
vec2 a_texcoord1    : TEXCOORD1;


vec4 i_data0    : TEXCOORD7;
vec4 i_data1    : TEXCOORD6;
vec4 i_data2    : TEXCOORD5;
vec4 i_data3    : TEXCOORD4;
vec4 i_data4    : TEXCOORD3;
vec4 i_data5    : TEXCOORD2;
vec4 i_data6    : TEXCOORD1;
vec4 i_data7    : TEXCOORD0;