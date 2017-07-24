
#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform mat4 cs_MatProjInv;
uniform mat4 cs_MatViewInv;

uniform sampler2D cs_DiffuseRoughness;
uniform sampler2D cs_NormalLightMode;
uniform sampler2D cs_EmissiveMetallic;
uniform sampler2D cs_SSSSpecular;
uniform sampler2D cs_Depth;

uniform vec4 cs_LightColor;
uniform float cs_LightEnergy; 
uniform vec3 cs_LightDirection;

in vec2 texCoord;
in vec2 xyPlane;

float calculate_shadow(vec4 world, vec3 cam);


void main ()
{
	// 
	float d = texture(cs_Depth, texCoord).r;
	if (d == 1.0)
	{
cs_FragColor = vec4(0.7, 0.8, 1.0, 1.0);//discard;
		return;	}
	d = d * 2.0f - 1.0;
	
	// projected paces
	vec4 proj = vec4 (xyPlane.xy, d, 1.0);
	vec4 cam = cs_MatProjInv * proj;
	cam /= cam.w;
	
	vec4 world4 = cs_MatViewInv * cam;
	vec3 world = world4.xyz;// / world4.w;
	
	vec3 normal = texture(cs_NormalLightMode, texCoord).xyz * 2.0 - 1.0;
	vec3 diffuse = texture(cs_DiffuseRoughness, texCoord).xyz;

	float directToIndirect = 0.5;
	float lamb = dot (-cs_LightDirection, normal);
	float directLamb = clamp(lamb, 0.0, 1.0) * directToIndirect;
	float indirectLamb = ((lamb + 1.0) / 2.0) * (1.0 - directToIndirect);
	
	
	float shadow = calculate_shadow (world4, cam.xyz);
	
	cs_FragColor = vec4 (diffuse * cs_LightColor.rgb * cs_LightEnergy * (directLamb * shadow + indirectLamb) , 1.0);
}
