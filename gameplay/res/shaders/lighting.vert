#if defined(BUMPED)
void applyLight(vec4 position, mat3 tangentSpaceTransformMatrix)
{
    #if (defined(SPECULAR) || (POINT_LIGHT_COUNT > 0) || (SPOT_LIGHT_COUNT > 0))
        vec4 positionWorldViewSpace = u_worldViewMatrix * position;
    #endif
    
    #if defined(SPECULAR)
        // Compute camera direction and transform it to tangent space.
        v_cameraDirection = tangentSpaceTransformMatrix * (u_cameraPosition.xyz - positionWorldViewSpace.xyz);
    #endif
}
#else
void applyLight(vec4 position)
{
    #if defined(SPECULAR) || (POINT_LIGHT_COUNT > 0) || (SPOT_LIGHT_COUNT > 0)
	   vec4 positionWorldViewSpace = u_worldViewMatrix * position;
    #endif

    #if defined(SPECULAR)  
	   v_cameraDirection = u_cameraPosition.xyz - positionWorldViewSpace.xyz;
    #endif
}
#endif
