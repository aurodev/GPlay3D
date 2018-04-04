$input a_position

#include "common/common.sh"

uniform mat4 u_worldViewProjectionMatrix;
uniform mat4 u_lightSpaceMatrix;
uniform mat4 u_mymodel;

void main()
{
    gl_Position = u_lightSpaceMatrix *  vec4(a_position, 1.0);
}