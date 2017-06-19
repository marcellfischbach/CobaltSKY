
#version 330

in vec4 cs_Position;
in vec3 cs_Normal;
in vec2 cs_TexCoord0;

uniform mat4 cs_MatModel;


out vec2 inGeomTexCoord;
out vec3 inGeomNormal;

void main ()
{
	gl_Position = cs_MatModel * cs_Position;
	inGeomNormal = mat3(cs_MatModel) * cs_Normal;
	inGeomTexCoord = cs_TexCoord0;
}
