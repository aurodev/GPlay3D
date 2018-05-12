
varying vec2 v_texcoord0;


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;



struct Light {
    vec3 Position;
    vec3 Color;
    float Linear;
    float Quadratic;
};
const int NR_LIGHTS = 1;
//uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;


void main()
{
    // retrieve data from gbuffer
    vec3 FragPos = texture2D(gPosition, v_texcoord0).rgb;
    vec3 Normal = texture2D(gNormal, v_texcoord0).rgb;
    vec3 Albedo = texture2D(gAlbedoSpec, v_texcoord0).rgb;
    float Specular = texture2D(gAlbedoSpec, v_texcoord0).a;



    Light lights[NR_LIGHTS];
    for(int i = 0; i < NR_LIGHTS; ++i) {
    lights[i].Position = vec3(0,1,0);
    lights[i].Color = vec3(1,1,1);
    lights[i].Linear = 0.7;
    lights[i].Quadratic = 1.8;
    }


    // then calculate lighting as usual
    vec3 lighting = Albedo * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        /*// diffuse
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].Color;
        lighting += diffuse;*/

        // diffuse
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].Color;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = lights[i].Color * spec * Specular;
        // attenuation
        float distance = length(lights[i].Position - FragPos);
        float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;
    }




    gl_FragColor = vec4(lighting, 1.0);   
}