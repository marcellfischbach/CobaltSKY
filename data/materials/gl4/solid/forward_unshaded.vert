
#version 330

in vec4 cs_Position;
in vec2 cs_TexCoord0;

uniform mat4 cs_MatProj;
uniform mat4 cs_MatView;
uniform mat4 cs_MatModel;


out vec2 texCoord;

void main ()
{
	gl_Position = cs_MatProj * cs_MatView * cs_MatModel * cs_Position;
	texCoord = cs_TexCoord0;
}
