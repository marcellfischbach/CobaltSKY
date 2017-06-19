#version 330

in vec4 cs_Position;

uniform mat4 cs_MatProjViewModel;


void main ()
{
	gl_Position = cs_MatProjViewModel * cs_Position;
}
