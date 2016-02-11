#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform sampler2D vk_Color0;
uniform sampler2D vk_Color1;

in vec2 inFragTexCoord0;

void main ()
{
	vk_FragColor = texture(vk_Color0, inFragTexCoord0) 
	             * texture(vk_Color1, inFragTexCoord0);
}