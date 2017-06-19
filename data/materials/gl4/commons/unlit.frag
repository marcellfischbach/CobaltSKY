#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform vec4 cs_Color;

void main ()
{
	cs_FragColor = cs_Color;
}

