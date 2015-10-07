
#version 120

attribute vec4 vk_Position;
attribute vec4 vk_Color;

uniform mat4 vk_MatView;
uniform mat4 vk_MatModel;
uniform mat4 vk_MatProjViewModel;


varying vec4 color;

void main ()
{
	gl_Position = vk_MatProjViewModel * vk_Position;
	color = vk_Color;
}
