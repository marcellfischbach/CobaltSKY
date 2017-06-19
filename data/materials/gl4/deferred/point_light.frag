
#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform mat4 cs_MatProjViewInv;

uniform sampler2D cs_DiffuseRoughness;
uniform sampler2D cs_NormalLightMode;
uniform sampler2D cs_EmissiveMetallic;
uniform sampler2D cs_SSSSpecular;
uniform sampler2D cs_Depth;

uniform vec4 cs_LightColor;
uniform float cs_LightEnergy; 
uniform vec3 cs_LightPosition;
uniform float cs_LightRadius;

in vec2 texCoord;
in vec2 xyPlane;

float calculate_shadow(vec3 world);


void main ()
{
	// 
	float d = texture(cs_Depth, texCoord).r;
	if (d == 1.0)
	{
		discard;
	}
	d = d * 2.0f - 1.0;
	
	// projected paces
	vec4 proj = vec4 (xyPlane, d, 1.0);
	vec4 world4 = cs_MatProjViewInv * proj;
	vec3 world = world4.xyz / world4.w;
	
	vec3 normal = texture(cs_NormalLightMode, texCoord).xyz * 2.0 - 1.0;
	vec3 diffuse = texture(cs_DiffuseRoughness, texCoord).xyz;


	vec3 lightDir = cs_LightPosition - world;
	float lightDistance = length(lightDir);
	lightDir /= lightDistance;
	
	float lamb = dot (lightDir, normal);
	float lightIntensity = clamp (1.0f - lightDistance / cs_LightRadius, 0.0, 1.0);
	
	
	
	cs_FragColor = vec4 (diffuse * cs_LightColor.rgb * lamb * lightIntensity * cs_LightEnergy, 1.0);
	
	cs_FragColor *= calculate_shadow (world);
}
