
#version 330

layout(location = 0) out vec2 vk_FragColor;

in vec2 inFragTexCoord;
in vec3 inFragNormal;

uniform sampler2D vk_Diffuse;
uniform vec4 vk_Color;

void fade_in_out_test();

void main ()
{
	fade_in_out_test();
	
	float dx = dFdx(gl_FragDepth);
	float dy = dFdy(gl_FragDepth);
	
	vk_FragColor = vec2(
				gl_FragDepth, 
				gl_FragDepth*gl_FragDepth + 0.25*(dx*dx + dy*dy));
	}
