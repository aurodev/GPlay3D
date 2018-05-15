
varying vec2 v_texcoord0;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D s_depthBuffer;

uniform mat4 u_inverseTransposeWorldViewMatrix;
uniform mat4 u_projectionMatrix;


uniform mat4 u_inverseProjectionMatrix;
uniform mat4 u_inverseViewMatrix;



struct Light {
    vec3 Position;
    vec3 Color;    
    float Linear;
    float Quadratic;
};
const int NR_LIGHTS = 1;
uniform vec4 viewPos;




float LinearizeDepth(in vec2 uv)
{
    float zNear = 0.5;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = 10.0; // TODO: Replace by the zFar  of your perspective projection
    float depth = texture2D(s_depthBuffer, uv).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}


// Linearizes a Z buffer value
float CalcLinearZ(float depth)
{
    const float zFar = 100.0;
    const float zNear = 0.1;

    // bias it from [0, 1] to [-1, 1]
    float linear = zNear / (zFar - depth * (zFar - zNear)) * zFar;

    return (linear * 2.0) - 1.0;
}

// this is supposed to get the world position from the depth buffer
vec3 WorldPosFromDepth(float depth)
{
    /*float ViewZ = CalcLinearZ(depth);

    // Get clip space
    vec4 clipSpacePosition = vec4(v_texcoord0 * 2.0 - 1.0, ViewZ, 1);

    // Clip space -> View space
    vec4 viewSpacePosition = u_inverseProjectionMatrix * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    // View space -> World space
    vec4 worldSpacePosition = u_inverseViewMatrix * viewSpacePosition;

    return worldSpacePosition.xyz;*/




    float z = depth * 2.0 - 1.0;

    // Get clip space
    vec4 clipSpacePosition = vec4(v_texcoord0 * 2.0 - 1.0, z, 1.0);

    // Clip space -> View space
    vec4 viewSpacePosition = u_inverseProjectionMatrix * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    // View space -> World space
    vec4 worldSpacePosition = u_inverseViewMatrix * viewSpacePosition;

    return worldSpacePosition.xyz;
}




void main()
{
    // retrieve data from gbuffer
    vec3 FragPos = texture2D(gPosition, v_texcoord0).rgb;
    vec3 v_normal = texture2D(gNormal, v_texcoord0).rgb;
    vec3 Diffuse = texture2D(gAlbedoSpec, v_texcoord0).rgb;
    float Specular = texture2D(gAlbedoSpec, v_texcoord0).a;


    

    // methode 1
    float depth = texture2D(s_depthBuffer, v_texcoord0).x;
    vec4 p = u_inverseProjectionMatrix * (vec4(v_texcoord0, depth, 1.0) * 2.0 - 1.0);
    vec3 viewspace_position = p.xyz / p.w;
    //FragPos = vec4(viewspace_position, 1).xyz;
    vec4 wpos = u_inverseViewMatrix * vec4(viewspace_position, 1);
    FragPos = wpos.xyz;

    // methode 2
    // get world pos from depth buffer
    /*float Depth = texture2D(s_depthBuffer, v_texcoord0).x;
    vec3 FragWorldPos = WorldPosFromDepth(Depth);
    FragPos = FragWorldPos;*/


    Light lights[NR_LIGHTS];
    for(int i = 0; i < NR_LIGHTS; ++i) {
    lights[i].Position = vec3(-2,3,1);
    lights[i].Color = vec3(1,1,1);
    lights[i].Linear = 0.07;
    lights[i].Quadratic = 0.022;
    }


     // then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos.xyz - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(v_normal, lightDir), 0.0) * Diffuse * lights[i].Color;

        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(v_normal, halfwayDir), 0.0), 16.0);
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