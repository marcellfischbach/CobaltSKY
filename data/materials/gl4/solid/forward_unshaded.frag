
#version 330

layout(location = 0) out vec4 vk_FragColor;

in vec2 texCoord;

uniform sampler2D vk_Diffuse;




void vk_fade_in_out_test();

void main ()
{
	
	vec3 diffuse = texture(vk_Diffuse, texCoord).rgb;
	vk_FragColor = vec4(diffuse, 1.0);
}
