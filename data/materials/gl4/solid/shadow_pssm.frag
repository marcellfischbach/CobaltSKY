
#version 330

layout(location = 0) out vec4 vk_FragColor;

in vec2 inFragTexCoord;
in vec3 inFragNormal;

uniform sampler2D vk_Diffuse;
uniform vec4 vk_Color;

void main ()
{
	vk_FragColor = texture(vk_Diffuse, inFragTexCoord);
	vk_FragColor *= vk_Color;
	//vk_FragColor = vec4 (inFragNormal * 0.5 + 0.5, 1.0);
}
