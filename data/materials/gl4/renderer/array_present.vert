
#version 330

in vec4 cs_Position;
in vec2 cs_TexCoord0;

uniform vec2 cs_LeftBottom;
uniform vec2 cs_Delta;

out vec2 texCoord;

void main ()
{
	gl_Position = vec4(cs_LeftBottom + cs_Position.xy * cs_Delta, cs_Position.zw);
	texCoord = cs_TexCoord0;
}
