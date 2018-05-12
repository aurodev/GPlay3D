attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord0;


uniform mat4 u_worldViewProjectionMatrix;
uniform mat4 u_inverseTransposeWorldViewMatrix;
uniform mat4 u_worldMatrix;
uniform mat4 u_viewProjectionMatrix;


varying vec4 v_position;
varying vec3 v_normal;
varying vec2 v_texcoord0;

void main()
{
    gl_Position = u_worldViewProjectionMatrix * a_position;
    //gl_Position = u_viewProjectionMatrix * a_position;

    // position
    v_position = u_worldMatrix * a_position;

    // normals
    mat3 normalMatrix = mat3(u_inverseTransposeWorldViewMatrix);
    v_normal = normalMatrix * a_normal;

    // tex coords
    v_texcoord0 = a_texcoord0;
}