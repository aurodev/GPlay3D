$input a_position, a_texcoord0
$output v_vertexRefractionPosition, v_vertexReflectionPosition, v_texcoord0, v_eyePosition

#include "../../shaders/common/common.sh"


uniform mat4 u_worldMatrix;
uniform mat4 u_worldViewProjectionMatrix;
uniform mat4 u_worldViewProjectionReflectionMatrix;
uniform vec4 u_cameraPosition;


void main()
{
    v_vertexRefractionPosition = u_worldViewProjectionMatrix * a_position;
    v_vertexReflectionPosition = u_worldViewProjectionReflectionMatrix * a_position;

    gl_Position = v_vertexRefractionPosition;

    v_texcoord0 = a_texcoord0;

    v_eyePosition = u_cameraPosition.xyz - (u_worldMatrix * a_position).xyz;
}