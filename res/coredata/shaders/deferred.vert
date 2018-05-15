attribute vec4 a_position;
attribute vec2 a_texcoord0;


uniform mat4 u_worldViewProjectionMatrix;
uniform mat4 u_inverseTransposeWorldViewMatrix;
uniform mat4 u_projectionMatrix;

varying vec2 v_texcoord0;

void main()
{

    v_texcoord0 = a_texcoord0;
    gl_Position = vec4(a_position.xyz, 1.0);
}