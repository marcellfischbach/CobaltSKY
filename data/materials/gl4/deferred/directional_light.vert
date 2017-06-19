
#version 330

in vec4 cs_Position;
in vec2 cs_TexCoord0;

out vec2 texCoord;
out vec2 xyPlane;

void main ()
{
	gl_Position = cs_Position;
	texCoord = cs_TexCoord0;
	xyPlane = cs_Position.xy;
}
