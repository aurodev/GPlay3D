attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord0;

attribute vec3 a_tangent;
attribute vec3 v_binormal;



uniform mat4 u_worldViewProjectionMatrix;
uniform mat4 u_worldMatrix;

//uniform mat4 u_inverseTransposeWorldViewMatrix;
//uniform mat4 u_worldMatrix;
//uniform mat4 u_viewProjectionMatrix;
//uniform mat4 u_viewMatrix;
//uniform mat4 u_projectionMatrix;


varying vec3 v_position;
varying vec3 v_normal;
varying vec2 v_texcoord0;



varying mat3 v_tbnViewSpace;
varying vec3           Tangent;                          // tangent
varying vec3           Binormal;                         // binormal

void main()
{
    vec4 worldPos = u_worldMatrix * vec4(a_position.xyz, 1.0);
    v_position = worldPos.xyz; 
    //v_texcoord0 = a_texcoord0;
    v_texcoord0 = vec2(a_texcoord0.x, 1.0 - a_texcoord0.y);
    
    mat3 normalMatrix = transpose(inverse(mat3(u_worldMatrix)));
    v_normal = normalMatrix * a_normal;

    gl_Position = u_worldViewProjectionMatrix * vec4(a_position.xyz, 1.0);




  

    mat3 NMAT = transpose(inverse(mat3(u_worldMatrix)));
    vec3 N = normalize(NMAT * a_normal);
    vec3 T = normalize(NMAT * a_tangent);
    vec3 B = cross(T,N);
    v_tbnViewSpace = (mat3(T,B,N));




    mat4 u_view = transpose(inverse(u_worldMatrix));

    vec4 normal = vec4(a_normal * 2.0 - 1.0, 0.0);
    vec3 wnormal = (u_worldMatrix * vec4(normal.xyz, 0.0) ).xyz;

    vec4 tangent = vec4(a_tangent * 2.0 - 1.0, 0.0);
    vec3 wtangent = (u_worldMatrix * vec4(tangent.xyz, 0.0) ).xyz;

    vec3 viewNormal = normalize((u_view * vec4(wnormal, 0.0) ).xyz);
    vec3 viewTangent = normalize((u_view * vec4(wtangent, 0.0) ).xyz);
    vec3 viewBitangent = cross(viewNormal, viewTangent) * tangent.w;
    v_tbnViewSpace = mat3(viewTangent, viewBitangent, viewNormal);

    v_normal = viewNormal;
    Tangent = viewTangent;
    Binormal = viewBitangent;





    Tangent = normalize(normalMatrix[0]);
    Binormal = normalize(normalMatrix[1]);

}
