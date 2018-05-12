attribute vec4 a_position;
attribute vec2 a_texcoord0;


uniform mat4 u_worldViewProjectionMatrix;
uniform mat4 u_inverseTransposeWorldViewMatrix;
uniform mat4 u_projectionMatrix;

varying vec2 v_texcoord0;



void main()
{
    gl_Position = u_projectionMatrix * a_position;
    v_texcoord0 = vec2(a_texcoord0.x, 1.0 - a_texcoord0.y);

    //gl_Position = u_worldViewProjectionMatrix * a_position;
    //gl_Position = vec4(a_position.xyz, 1.0);

    // tex coords
    //v_texcoord0 = a_texcoord0;
}