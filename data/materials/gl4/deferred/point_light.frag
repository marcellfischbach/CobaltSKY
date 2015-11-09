
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
uniform vec3 vk_LightPosition;
uniform float vk_LightRadius;

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


	vec3 lightDir = vk_LightPosition - world;
	float lightDistance = length(lightDir);
	lightDir /= lightDistance;
	
	float lamb = dot (lightDir, normal);
	float lightIntensity = clamp (1.0f - lightDistance / vk_LightRadius, 0.0, 1.0);
	
	
	
	vk_FragColor = vec4 (diffuse * vk_LightColor.rgb * lamb * lightIntensity * vk_LightEnergy, 1.0);
}
