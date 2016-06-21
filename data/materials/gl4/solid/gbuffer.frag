
#version 330

layout(location = 0) out vec4 vk_DiffuseRoughness;
layout(location = 1) out vec4 vk_NormalLightMode;
layout(location = 2) out vec4 vk_EmissivMetallic;
layout(location = 3) out vec4 vk_SSSSpecular;

in vec2 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 binormal;

uniform sampler2D vk_Diffuse;
uniform sampler2D vk_Normal;
uniform vec2 vk_ViewportSizeInv;
uniform vec4 vk_Color;




void vk_fade_in_out_test();

void main ()
{
	vk_fade_in_out_test();
	vec3 t = normalize(tangent);
	vec3 b = normalize(binormal);
	vec3 n = normalize(normal);
	mat3 faceToWorldMat = mat3(t, b, n);
														 
	vec3 normalTxt = texture(vk_Normal, texCoord).rgb * 2.0 - 1.0;
	normalTxt = faceToWorldMat * normalTxt;
	
	vec3 diffuse = texture(vk_Diffuse, texCoord).rgb;
	diffuse *= vk_Color.rgb;
	vk_DiffuseRoughness = vec4(diffuse, 0.0);
	vk_NormalLightMode = vec4 (normalTxt * 0.5 + 0.5, 0.0);
	vk_EmissivMetallic = vec4(0, 0, 0, 0);
	vk_SSSSpecular = vec4 (0, 0, 0, 0);

}
