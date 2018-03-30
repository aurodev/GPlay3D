$input v_texcoord0, v_color, v_normal, v_position

#include "common/bgfx_shader.sh"

SAMPLER2D(u_diffuseTexture, 0);



uniform vec3 u_cameraPosition;
uniform mat4 u_worldViewMatrix;
uniform mat4 u_worldMatrix;
uniform mat4 u_viewMatrix;



uniform vec4 u_ambientColor;


#define DIRECTIONAL_LIGHT_COUNT 1
uniform vec4 u_directionalLightDirection[DIRECTIONAL_LIGHT_COUNT];
uniform vec4 u_directionalLightColor[DIRECTIONAL_LIGHT_COUNT];

#define POINT_LIGHT_COUNT 1
uniform vec4 u_pointLightPosition[POINT_LIGHT_COUNT];
uniform vec4 u_pointLightColor[POINT_LIGHT_COUNT];

#define SPOT_LIGHT_COUNT 1
uniform vec4 u_spotLightPosition[SPOT_LIGHT_COUNT];
uniform vec4 u_spotLightDirection[SPOT_LIGHT_COUNT];
uniform vec4 u_spotLightColor[SPOT_LIGHT_COUNT];



struct Material
{
    vec3 diffuse;		// color or diffuse map
    vec3 specular;    	// color or specular map
    float shininess;
};

struct DirLight
{
    vec3 direction;    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;        
    float constant;
    float linear;
    float quadratic;    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;     
};



#define NR_POINT_LIGHTS 1


// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);



Material material;


void main()
{    

    vec3 Normal = v_normal.xyz; 
    vec3 FragPos = v_position.xyz;

   
    vec3 diffuseMap = texture2D(u_diffuseTexture, v_texcoord0).rgb;    

   

    // setup material
    material.diffuse = diffuseMap.rgb;
    material.specular = vec3(0.5, 0.5, 0.5);
    material.shininess = 32.0;




    DirLight dirLight;
    dirLight.direction = u_directionalLightDirection[0].xyz;
    dirLight.ambient = u_ambientColor.rgb;
    dirLight.diffuse = u_directionalLightColor[0].rgb;
    dirLight.specular = vec3(1.0, 1.0, 1.0);



    PointLight pointLights[1];
    pointLights[0].position = u_pointLightPosition[0].xyz;
    pointLights[0].ambient = u_ambientColor.rgb;
    pointLights[0].diffuse = u_pointLightColor[0].rgb;
    pointLights[0].specular = vec3(1.0, 1.0, 1.0);
    pointLights[0].constant = 1.0;
    pointLights[0].linear = 0.045;
    pointLights[0].quadratic = 0.0075;



    SpotLight spotLight;
    spotLight.position = u_spotLightPosition[0].xyz;
    spotLight.direction =  u_spotLightDirection[0].xyz;
    spotLight.ambient = u_ambientColor.rgb;
    spotLight.diffuse = u_spotLightColor[0].rgb;
    spotLight.specular = vec3(1.0, 1.0, 1.0);
    spotLight.constant = 1.0;
    spotLight.linear = 0.09;
    spotLight.quadratic = 0.032;
    spotLight.cutOff = 0.6;
    spotLight.outerCutOff = 0.5;




    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);
    

    vec3 result = vec3(0,0,0);

    // directional lights
    result = CalcDirLight(dirLight, norm, viewDir);

    // point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);   

    // spot lights
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  


    // gamma    
    //result = pow(result, vec3(1.0/2.2));


    gl_FragColor = vec4(result, 1.0);
}


float computeSpecular(vec3 lightDir, vec3 normal, vec3 viewDir)
{
    // Phong shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Blinn-Phong shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    return spec;
}



// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float spec = computeSpecular(lightDir, normal, viewDir);

    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}



// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float spec = computeSpecular(lightDir, normal, viewDir);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient = light.ambient *  material.diffuse;
    vec3 diffuse = light.diffuse * diff *  material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float spec = computeSpecular(lightDir, normal, viewDir);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}



