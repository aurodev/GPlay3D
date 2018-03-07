$input a_position, a_color0
$output v_color

#include "common/common.sh"

uniform mat4 u_viewProjectionMatrix;


void main()
{
    gl_Position = u_viewProjectionMatrix * vec4(a_position, 1);
    v_color = a_color0;
}
