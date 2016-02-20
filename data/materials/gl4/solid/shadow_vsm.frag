
#version 330

layout(location = 0) out vec2 vk_FragColor;


in float inFragDepth;

void vk_fade_in_out_test();

void main ()
{
	vk_fade_in_out_test();
	
	float dx = dFdx(inFragDepth);
	float dy = dFdy(inFragDepth);
	
	vk_FragColor = vec2(abs(dx) + abs(dy), 1.0);
}
