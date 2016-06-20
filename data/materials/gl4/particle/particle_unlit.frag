#version 330

layout(location = 0) out vec4 vk_FragColor;



in vec2 inFragTexCoord;

void main ()
{
	float d = clamp(1.0 - length (inFragTexCoord - vec2(0.5, 0.5)) * 2.2, 0.0, 1.0);
	vk_FragColor = vec4(0.1, 0.1, 0.1, d * 0.2);
}

