
#version 330

in vec4 vk_Position;
in vec2 vk_TexCoord0;

out vec2 texCoord;
out vec2 xyPlane;

void main ()
{
	gl_Position = vk_Position;
	texCoord = vk_TexCoord0;
	xyPlane = vk_Position.xy;
}
