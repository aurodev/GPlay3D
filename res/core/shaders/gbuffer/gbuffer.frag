
varying vec3 v_position;
varying vec3 v_normal;
varying vec2 v_texcoord0;

uniform sampler2D u_diffuseTexture;
uniform sampler2D u_specularTexture;
uniform sampler2D u_normalTexture;




uniform sampler2D s_shadowMap;
varying vec4 v_shadowcoord;
float ShadowCalculation(vec4 fragPosLightSpace)
{   
    vec3 tex_coords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    tex_coords = tex_coords * 0.5 + 0.5;
    float depth = texture2D(s_shadowMap, tex_coords.xy).r;
    //float inShadow = (depth < tex_coords.z) ? 1.0 : 0.0;
    float currentDepth = tex_coords.z;
    float bias = 0.005;
    float inShadow = (currentDepth - bias > depth) ? 1.0 : 0.0;

    return inShadow;
}


void main()
{
    // store the fragment position vector in the first gbuffer texture
    //gl_FragData[0] = vec4(v_position, 1.0);

    // also store the per-fragment normals into the gbuffer
    gl_FragData[0] = vec4(normalize(v_normal), 1.0);

    // and the diffuse per-fragment color
    gl_FragData[1].rgb = texture2D(u_diffuseTexture, v_texcoord0).rgb;
    gl_FragData[1].a = texture2D(u_specularTexture, v_texcoord0).r;



	float shadow = 1.0 - ShadowCalculation(v_shadowcoord);
	gl_FragData[2].r = shadow;

	//gl_FragData[0] = texture2D(s_shadowMap, v_texcoord0);
}
