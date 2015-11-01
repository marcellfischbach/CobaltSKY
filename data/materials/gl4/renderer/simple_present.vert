
#version 330

in vec4 vk_Position;
in vec2 vk_TexCoord0;

varying vec2 texCoord;

void main ()
{
	gl_Position = vk_Position;
	texCoord = vk_TexCoord0;
}
