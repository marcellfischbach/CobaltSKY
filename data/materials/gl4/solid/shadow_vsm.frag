
#version 330

layout(location = 0) out vec2 cs_FragColor;


in float inFragDepth;

void cs_fade_in_out_test();

void main ()
{
	cs_fade_in_out_test();
	
	float dx = dFdx(inFragDepth);
	float dy = dFdy(inFragDepth);
	
	cs_FragColor = vec2(abs(dx) + abs(dy), 1.0);
}
