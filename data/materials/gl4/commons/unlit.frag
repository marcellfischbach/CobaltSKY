#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform vec4 vk_Color;

void main ()
{
	vk_FragColor = vk_Color;
}

