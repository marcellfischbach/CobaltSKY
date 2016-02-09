#version 330

in vec4 vk_Position;
in vec2 vk_TexCoord0;

out vec2 inFragTexCoord0;

void main ()
{
	gl_Position = vk_Position;
	inFragTexCoord0 = vk_TexCoord0;
}