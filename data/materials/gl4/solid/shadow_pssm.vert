
#version 330

in vec4 vk_Position;
in vec2 vk_TexCoord0;

uniform mat4 vk_MatModel;


out vec2 inGeomTexCoord;

void main ()
{
	gl_Position = vk_MatModel * vk_Position;
	inGeomTexCoord = vk_TexCoord0;
}
