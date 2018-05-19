
//-----------------------------------------------------------------------------------
// Linearizes a Z buffer value
//-----------------------------------------------------------------------------------
float LinearizeDepth(float depth, float zNear, float zFar)
{    
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

//-----------------------------------------------------------------------------------
// Get the world position from the depth buffer
//-----------------------------------------------------------------------------------
vec3 worldPosFromDepth(float depth)
{
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
