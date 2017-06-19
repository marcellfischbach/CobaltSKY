
#version 330

in vec4 cs_Position;
in vec3 cs_Normal;
in vec3 cs_Tangent;
in vec3 cs_BiNormal;
in vec2 cs_TexCoord0;

uniform mat4 cs_MatProj;
uniform mat4 cs_MatView;
uniform mat4 cs_MatModel;


out vec2 texCoord;
out vec3 normal;
out vec3 tangent;
out vec3 binormal;

void main ()
{
	gl_Position = cs_MatProj * cs_MatView * cs_MatModel * cs_Position;
	texCoord = cs_TexCoord0;
	
	mat3 matModel3 = mat3(cs_MatModel);
	normal = matModel3 * cs_Normal;
	tangent = matModel3 * cs_Tangent;
	binormal = matModel3 * cs_BiNormal;
}
