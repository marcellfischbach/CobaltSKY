
#version 330

layout(location = 0) out vec4 vk_Color;

in vec2 inFragTexCoord;
in vec3 inFragNormal;

uniform sampler2D vk_Diffuse;

void main ()
{
	vk_Color = texture(vk_Diffuse, inFragTexCoord);
	//vk_Color = vec4 (inFragNormal * 0.5 + 0.5, 1.0);
}
