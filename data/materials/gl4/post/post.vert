#version 330

in vec4 cs_Position;
in vec2 cs_TexCoord0;

out vec2 inFragTexCoord0;
out vec2 inFragXYPlane;

void main ()
{
	gl_Position = cs_Position;
	inFragTexCoord0 = cs_TexCoord0;
	inFragXYPlane = cs_Position.xy;
}