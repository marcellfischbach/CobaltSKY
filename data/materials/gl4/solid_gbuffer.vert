
#version 330

in vec4 vk_Position;
in vec4 vk_Color;
in vec2 vk_TexCoord0;

uniform mat4 vk_MatView;
uniform mat4 vk_MatModel;
uniform mat4 vk_MatProjViewModel;


varying vec4 color;
varying vec2 texCoord;

void main ()
{
	gl_Position = vk_MatProjViewModel * vk_Position;
	color = vk_Color;
	texCoord = vk_TexCoord0;
}
