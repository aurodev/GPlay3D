
varying vec3 v_position;
varying vec3 v_normal;
varying vec2 v_texcoord0;

uniform sampler2D u_diffuseTexture;
uniform sampler2D u_specularTexture;
uniform sampler2D u_normalTexture;


varying mat3 v_tbnViewSpace;
varying vec3           Tangent;                          // tangent
varying vec3           Binormal;                         // binormal

void main()
{
    // store the fragment position vector in the first gbuffer texture
    //gl_FragData[0] = vec4(v_position, 1.0);

    // also store the per-fragment normals into the gbuffer
    gl_FragData[0] = vec4(normalize(v_normal), 1.0);

    // and the diffuse per-fragment color
    gl_FragData[1].rgb = texture2D(u_diffuseTexture, v_texcoord0).rgb;
    gl_FragData[1].a = texture2D(u_specularTexture, v_texcoord0).r;



    // bump map
    vec3 normalMap = texture2D(u_normalTexture, v_texcoord0).rgb * 2.0 - 1.0;
    normalMap = v_tbnViewSpace * normalMap;
    gl_FragData[2] = vec4(normalMap, 1.0);

}
