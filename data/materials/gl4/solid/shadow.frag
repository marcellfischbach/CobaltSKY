
#version 330

layout(location = 0) out vec4 vk_FragColor;

in vec2 inFragTexCoord;
in vec3 inFragNormal;

uniform sampler2D vk_Diffuse;
uniform vec4 vk_Color;

void vk_fade_in_out_test();

void main ()
{
	vk_fade_in_out_test();
	
	vk_FragColor = vec4(1, 1, 1, 1);
}
