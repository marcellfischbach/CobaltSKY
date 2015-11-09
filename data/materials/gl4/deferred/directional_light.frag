
#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform mat4 vk_MatProjViewInv;

uniform sampler2D vk_DiffuseRoughness;
uniform sampler2D vk_NormalLightMode;
uniform sampler2D vk_EmissiveMetallic;
uniform sampler2D vk_SSSSpecular;
uniform sampler2D vk_Depth;

uniform vec4 vk_LightColor;
uniform float vk_LightEnergy; 
uniform vec3 vk_LightDirection;

in vec2 texCoord;
in vec2 xyPlane;

void main ()
{
	// 
	float d = texture(vk_Depth, texCoord).r;
	if (d == 1.0)
	{
		discard;
	}
	d = d * 2.0f - 1.0;
	
	// projected paces
	vec4 proj = vec4 (xyPlane, d, 1.0);
	vec4 world4 = vk_MatProjViewInv * proj;
	vec3 world = world4.xyz / world4.w;
	
	vec3 normal = texture(vk_NormalLightMode, texCoord).xyz * 2.0 - 1.0;
	vec3 diffuse = texture(vk_DiffuseRoughness, texCoord).xyz;

	
	float lamb = dot (-vk_LightDirection, normal);
	
	
	
	
	vk_FragColor = vec4 (diffuse * lamb * vk_LightColor.rgb * vk_LightEnergy, 1.0);
}
