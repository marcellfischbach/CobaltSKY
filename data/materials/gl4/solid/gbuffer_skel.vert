
#version 330

in vec4 cs_Position;
in vec3 cs_Normal;
in vec2 cs_TexCoord0;
in vec4 cs_BoneIndex;

uniform mat4 cs_MatProj;
uniform mat4 cs_MatView;
uniform mat4 cs_MatModel;


out vec2 texCoord;
out vec3 normal;

vec4 cs_bone_to_model4f(vec4 position);


void main ()
{
	vec4 position = cs_bone_to_model4f(cs_Position);
	gl_Position = cs_MatProj * cs_MatView * cs_MatModel * position;
	texCoord = cs_TexCoord0;
	normal = mat3(cs_MatModel) * cs_Normal;
}
