
#version 330

layout(location = 0) out vec4 cs_FragColor;

in vec2 inFragTexCoord;
in vec3 inFragNormal;

uniform sampler2D cs_Diffuse;
uniform vec4 cs_Color;

void cs_fade_in_out_test();

void main ()
{
	cs_fade_in_out_test();
	
	cs_FragColor = vec4(1, 1, 1, 1);
}
