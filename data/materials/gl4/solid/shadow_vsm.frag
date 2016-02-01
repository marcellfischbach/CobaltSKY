
#version 330

layout(location = 0) out vec2 vk_FragColor;

in float inFragDepth;

void fade_in_out_test();

void main ()
{
	fade_in_out_test();
	
	float dx = dFdx(inFragDepth);
	float dy = dFdy(inFragDepth);
	
	vk_FragColor = vec2(
				inFragDepth, 
				inFragDepth*inFragDepth + 0.25*(dx*dx + dy*dy)
	);
	
}