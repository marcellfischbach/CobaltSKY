
#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform sampler2D vk_DiffuseRoughness;
uniform sampler2D vk_NormalLightMode;
uniform sampler2D vk_EmissiveMetallic;
uniform sampler2D vk_SSSSpecular;
uniform sampler2D vk_Depth;

in vec2 texCoord;

void main ()
{
	float d = texture(vk_Depth, texCoord).r;
	vec3 diffuse = texture(vk_DiffuseRoughness, texCoord).rgb;
	
	vk_FragColor = vec4 (d, d, d, 1.0);
	vk_FragColor = vec4 (diffuse, 1.0);
}
