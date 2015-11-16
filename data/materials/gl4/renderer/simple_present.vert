
#version 330

in vec4 vk_Position;
in vec2 vk_TexCoord0;

uniform vec2 vk_LeftBottom;
uniform vec2 vk_Delta;

out vec2 texCoord;

void main ()
{
	gl_Position = vec4(vk_LeftBottom + vk_Position.xy * vk_Delta, 0, 1);
	texCoord = vk_TexCoord0;
}
