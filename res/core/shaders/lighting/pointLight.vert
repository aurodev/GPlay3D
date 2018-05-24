attribute vec4 a_position;
attribute vec2 a_texcoord0;

varying vec2 v_texcoord0;






/*varying vec4 v_shadowcoord;
uniform mat4 u_lightSpaceMatrix;
uniform mat4 u_worldMatrix;*/

void main()
{
    v_texcoord0 = a_texcoord0;
    gl_Position = vec4(a_position.xyz, 1.0);


    ///v_shadowcoord = u_lightSpaceMatrix * u_worldMatrix * a_position;
    
}