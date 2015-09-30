
#version 120

attribute vec4 vk_Position;
attribute vec4 vk_Color;

uniform vec2 vk_Alt;

varying vec4 color;

void main ()
{
	gl_Position = vk_Position + vec4(vk_Alt, 0.0, 0.0);
	color = vk_Color;
}
