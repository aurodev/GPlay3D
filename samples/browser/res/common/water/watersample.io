vec4 v_color        : COLOR0    = vec4(1.0, 0.0, 0.0, 1.0);
vec2 v_texcoord0    : TEXCOORD0 = vec2(0.0, 0.0);

vec2 v_texcoord0 				: TEXCOORD0;
vec4 v_vertexRefractionPosition	: TEXCOORD1;
vec4 v_vertexReflectionPosition	: TEXCOORD2;
vec3 v_eyePosition				: TEXCOORD3;

vec4 a_position     : POSITION;
vec2 a_texcoord0    : TEXCOORD0;