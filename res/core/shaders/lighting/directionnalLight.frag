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
      

    // result
    lighting += diffuse + specular;

    gl_FragColor = vec4(lighting, 1.0);
}

