#version 330

in vec4 vk_Position;

uniform mat4 vk_MatProjViewModel;


void main ()
{
	gl_Position = vk_MatProjViewModel * vk_Position;
}
