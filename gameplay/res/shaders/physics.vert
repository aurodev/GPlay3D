$input a_position, a_color0
$output v_color

#include "common/common.sh"

uniform mat4 u_projectionMatrix;


void main()
{
    gl_Position = u_projectionMatrix * vec4(a_position, 1);
    v_color = a_color0;
}
