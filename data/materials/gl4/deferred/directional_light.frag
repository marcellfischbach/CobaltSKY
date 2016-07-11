
#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform mat4 vk_MatProjInv;
uniform mat4 vk_MatViewInv;

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

float calculate_shadow(vec4 world, vec3 cam);


void main ()
{
	// 
	float d = texture(vk_Depth, texCoord).r;
	if (d == 1.0)
	{
		vk_FragColor = vec4(0.7, 0.8, 1.0, 1.0);//discard;
		return;
	}
	d = d * 2.0f - 1.0;
	
	// projected paces
	vec4 proj = vec4 (xyPlane.xy, d, 1.0);
	vec4 cam = vk_MatProjInv * proj;
	cam /= cam.w;
	
	vec4 world4 = vk_MatViewInv * cam;
	vec3 world = world4.xyz;// / world4.w;
	
	vec3 normal = texture(vk_NormalLightMode, texCoord).xyz * 2.0 - 1.0;
	vec3 diffuse = texture(vk_DiffuseRoughness, texCoord).xyz;

	
	float lamb = clamp(dot (-vk_LightDirection, normal), 0.0, 1.0);
	
	
	float backLamb = clamp(dot (vk_LightDirection, normal), 0.0, 1.0) * 0.5;
	
	
	
	float shadow = calculate_shadow (world4, cam.xyz);
	
	vk_FragColor = vec4 (diffuse * vk_LightColor.rgb * vk_LightEnergy * (lamb * shadow + backLamb), 1.0);
}
