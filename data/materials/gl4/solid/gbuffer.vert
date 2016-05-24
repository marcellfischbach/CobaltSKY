
#version 330

in vec4 vk_Position;
in vec3 vk_Normal;
in vec3 vk_Tangent;
in vec3 vk_BiNormal;
in vec2 vk_TexCoord0;

uniform mat4 vk_MatProj;
uniform mat4 vk_MatView;
uniform mat4 vk_MatModel;


out vec2 texCoord;
out vec3 normal;
out vec3 tangent;
out vec3 binormal;

void main ()
{
	gl_Position = vk_MatProj * vk_MatView * vk_MatModel * vk_Position;
	texCoord = vk_TexCoord0;
	
	mat3 matModel3 = mat3(vk_MatModel);
	normal = matModel3 * vk_Normal;
	tangent = matModel3 * vk_Tangent;
	binormal = matModel3 * vk_BiNormal;
}
