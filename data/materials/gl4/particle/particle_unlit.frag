#version 330

layout(location = 0) out vec4 vk_FragColor;



in vec2 inFragTexCoord;

void main ()
{
	vk_FragColor = vec4(inFragTexCoord, 0.0, 1.0);
}

