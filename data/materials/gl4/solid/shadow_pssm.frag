
#version 330

layout(location = 0) out vec4 vk_Color;

in vec2 inFragTexCoord;

uniform sampler2D vk_Diffuse;

void main ()
{
	vk_Color = texture(vk_Diffuse, texCoord).rgb;
}
