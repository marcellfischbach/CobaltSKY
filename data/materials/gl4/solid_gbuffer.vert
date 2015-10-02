
#version 120

attribute vec4 vk_Position;
attribute vec4 vk_Color;

uniform float vk_AltX;
uniform float vk_AltY;

varying vec4 color;

void main ()
{
	gl_Position = vk_Position + vec4(vk_AltX, vk_AltY, 0.0, 0.0);
	color = vk_Color;
}
