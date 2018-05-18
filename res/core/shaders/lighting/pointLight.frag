
varying vec2 v_texcoord0;

//uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;

uniform mat4 u_inverseTransposeWorldViewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_inverseProjectionMatrix;
uniform mat4 u_inverseViewMatrix;
uniform vec4 u_viewPos;
uniform vec4 u_lightPos;
uniform vec4 u_lightColor;
uniform vec4 u_lightRadius;



struct Light 
{
    vec3 Position;
    vec3 Color;    
    float Linear;
    float Quadratic;
};



float LinearizeDepth(in vec2 uv)
{
    float zNear = 0.5;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = 10.0; // TODO: Replace by the zFar  of your perspective projection
    float depth = texture2D(gDepth, uv).x;
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
    //vec3 FragPos = texture2D(gPosition, v_texcoord0).rgb;
    vec3 v_normal = texture2D(gNormal, v_texcoord0).rgb;
    vec3 Diffuse = texture2D(gAlbedoSpec, v_texcoord0).rgb;
    float Specular = texture2D(gAlbedoSpec, v_texcoord0).a;


    

    // methode 1
    float depth = texture2D(gDepth, v_texcoord0).x;
    vec4 p = u_inverseProjectionMatrix * (vec4(v_texcoord0, depth, 1.0) * 2.0 - 1.0);
    vec3 viewspace_position = p.xyz / p.w;
    //FragPos = vec4(viewspace_position, 1).xyz;
    vec4 wpos = u_inverseViewMatrix * vec4(viewspace_position, 1);
    vec3 FragPos = wpos.xyz;

    // methode 2
    // get world pos from depth buffer
    /*float Depth = texture2D(gDepth, v_texcoord0).x;
    vec3 FragWorldPos = WorldPosFromDepth(Depth);
    FragPos = FragWorldPos;*/

    
    Light light;
    light.Position = u_lightPos.xyz;
    light.Color = u_lightColor.rgb;
    light.Linear = 0.045;
    light.Quadratic = 0.0075;


     // then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.01; // hard-coded ambient component
    vec3 viewDir  = normalize(u_viewPos.xyz - FragPos);
    

        // diffuse
        vec3 lightDir = normalize(light.Position - FragPos);
        vec3 diffuse = max(dot(v_normal, lightDir), 0.0) * Diffuse * light.Color;

        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(v_normal, halfwayDir), 0.0), 16.0);
        vec3 specular = light.Color * spec * Specular;




        // attenuation
        //float distance = length(light.Position - FragPos);
        //float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
        
        //float attenuation = clamp(1.0 - dot(lightDir, lightDir), 0.0, 1.0);
        
        //vec3 distance = (light.Position - FragPos) / u_lightRadius.x;
        //float attenuation = 1.0f - dot(distance, distance);

        float attenuation = smoothstep(u_lightRadius.x, 0, length(light.Position - FragPos));

      

        // result
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;        
    

    gl_FragColor = vec4(lighting, 1.0);
}
