
#version 330

in vec4 vk_Position;
in vec3 vk_Normal;
in vec2 vk_TexCoord0;
in vec4 vk_BoneIndex;

uniform mat4 vk_MatProj;
uniform mat4 vk_MatView;
uniform mat4 vk_MatModel;


out vec2 texCoord;
out vec3 normal;

vec4 vk_bone_to_model4f(vec4 position);


void main ()
{
	vec4 position = vk_bone_to_model4f(vk_Position);
	gl_Position = vk_MatProj * vk_MatView * vk_MatModel * position;
	texCoord = vk_TexCoord0;
	normal = mat3(vk_MatModel) * vk_Normal;
}
