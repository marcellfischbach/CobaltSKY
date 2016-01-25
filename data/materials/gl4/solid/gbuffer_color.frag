
#version 330

layout(location = 0) out vec4 vk_DiffuseRoughness;
layout(location = 1) out vec4 vk_NormalLightMode;
layout(location = 2) out vec4 vk_EmissivMetallic;
layout(location = 3) out vec4 vk_SSSSpecular;

in vec2 texCoord;
in vec3 normal;

uniform vec2 vk_ViewportSizeInv;
uniform vec4 vk_Color;



void fade_in_out_test();

void main ()
{
	fade_in_out_test();
	
	vec3 diffuse = vk_Color.rgb;
	vk_DiffuseRoughness = vec4(diffuse, 0.0);
	vk_NormalLightMode = vec4 (normal * 0.5 + 0.5, 0.0);
	vk_EmissivMetallic = vec4(0, 0, 0, 0);
	vk_SSSSpecular = vec4 (0, 0, 0, 0);

}
