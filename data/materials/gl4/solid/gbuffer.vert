
#version 330

in vec4 vk_Position;
in vec3 vk_Normal;
in vec2 vk_TexCoord0;

uniform mat4 vk_MatView;
uniform mat4 vk_MatModel;
uniform mat4 vk_MatProjViewModel;


out vec2 texCoord;
out vec3 normal;

void main ()
{
	gl_Position = vk_MatProjViewModel * vk_Position;
	texCoord = vk_TexCoord0;
	normal = mat3(vk_MatModel) * vk_Normal;
}
