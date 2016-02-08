#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform sampler2D vk_Color0;
uniform vec2 vk_Color0SizeInv;

in vec inFragTexCoord0;

void main ()
{
	vec4 r = texture(vk_Color0, inFragTexCoord0);
	r += texture(vk_Color0, inFragTexCoord0 - vec2(0.0, vk_Color0SizeInv.y));
	r += texture(vk_Color0, inFragTexCoord0 + vec2(0.0, vk_Color0SizeInv.y));
	
	
	vk_FragColor = r / 3.0;
}