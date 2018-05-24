varying vec2 v_texcoord0;


uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;

uniform mat4 u_inverseTransposeWorldViewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_inverseProjectionMatrix;
uniform mat4 u_inverseViewMatrix;



// point light
uniform vec4 u_viewPos;
uniform vec4 u_lightPos;
uniform vec4 u_lightColor;
uniform vec4 u_lightRadius;

// directionnal light
uniform vec4 u_lightDirection;



#include "../common/gplay.sh"


struct Light 
{
    vec3 position;
    vec3 color;
    float radius;
};

struct DirLight
{
    vec3 direction;
    vec3 color;  
};





uniform mat4 u_lightSpaceMatrix;
uniform sampler2D s_shadowMap;
varying vec4 v_shadowcoord;
float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{   
    vec3 tex_coords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    tex_coords = tex_coords * 0.5 + 0.5;
    float depth = texture2D(s_shadowMap, tex_coords.xy).r;
    //float inShadow = (depth < tex_coords.z) ? 1.0 : 0.0;
    float currentDepth = tex_coords.z;
    //float bias = 0.001;
    float inShadow = (currentDepth - bias > depth) ? 1.0 : 0.0;

    return inShadow;
}












void main()
{
    // retrieve data from gbuffer
    vec3 Normal = texture2D(gNormal, v_texcoord0).rgb;
    vec3 Diffuse = texture2D(gAlbedoSpec, v_texcoord0).rgb;
    float Specular = texture2D(gAlbedoSpec, v_texcoord0).a;
    float Depth = texture2D(gDepth, v_texcoord0).r;
    
    // get world pos from depth buffer
    vec3 fragPos = worldPosFromDepth(Depth);    

    DirLight light;
    light.direction = u_lightDirection.xyz;
    light.color = u_lightColor.rgb;

    vec3 lighting = vec3(0.0);  //  = Diffuse * 0.0; // hard-coded ambient component
    vec3 viewDir  = normalize(u_viewPos.xyz - fragPos);    

    // diffuse
    vec3 lightDir = normalize(-light.direction);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.color;

    // specular
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = light.color * spec * Specular;
      





    // shadow

    //float shadow = 1.0 - ShadowCalculation(v_shadowcoord);
    //float shadow = texture2D(s_shadowMap, v_shadowcoord.xy).r;
    //float shadow = texture2D(s_shadowMap, v_texcoord0).r;
    //float shadow = 1.0 - ShadowCalculation(v_texcoord0);

    //float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);

    float cosTheta = clamp(dot(Normal, lightDir), 0.0, 1.0);
    float bias = 0.005*tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
    bias = clamp(bias, 0, 0.01);

    vec4 fragPosLightSpace = u_lightSpaceMatrix * vec4(fragPos, 1.0);
    float shadow = 1.0 - ShadowCalculation(fragPosLightSpace, bias);
    diffuse *= shadow;
    specular *= shadow;







    // result
    lighting += diffuse + specular;

    gl_FragColor = vec4(lighting, 1.0);
}

