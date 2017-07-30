
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

float oren_nayar(vec3 normal, vec3 lightDir, vec3 eyeDir, float NdotL, float NdotV, float roughness)
{
		// calculate intermediary values

	float angleVN = acos(NdotV);
	float angleLN = acos(NdotL);
	
	float alpha = max(angleVN, angleLN);
	float beta = min(angleVN, angleLN);
	float gamma = dot(eyeDir - normal * NdotV, lightDir - normal * NdotL);
	
	float roughnessSquared = roughness * roughness;
	
	// calculate A and B
	float A = 1.0 - 0.5 * (roughnessSquared / (roughnessSquared + 0.57));

	float B = 0.45 * (roughnessSquared / (roughnessSquared + 0.09));

	float C = sin(alpha) * tan(beta);
	
	// put it all together
	float L1 = max(0.0, NdotL) * (A + B * max(0.0, gamma) * C);
	return L1;
}

float cook_torrance(vec3 normal, vec3 lightDir, vec3 eyeDir, float NdotL, float NdotV, float roughness)
{
	float F0 = 0.6; // fresnel reflectance at normal incidence

	float specular = 0.0;
	if(NdotL > 0.0)
	{

		// calculate intermediary values
		vec3 halfVector = normalize(lightDir + eyeDir);
		float NdotH = max(dot(normal, halfVector), 0.0); 
		float NdotV = max(dot(normal, eyeDir), 0.0); // note: this could also be NdotL, which is the same value
		float VdotH = max(dot(eyeDir, halfVector), 0.0);
		float mSquared = roughness * roughness;

		// geometric attenuation
		float NH2 = 2.0 * NdotH;
		float g1 = (NH2 * NdotV) / VdotH;
		float g2 = (NH2 * NdotL) / VdotH;
		float geoAtt = min(1.0, min(g1, g2));

		// roughness (or: microfacet distribution function)
		// beckmann distribution function
		float r1 = 1.0 / ( 3.14 * mSquared * pow(NdotH, 4.0));
		float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
		float roughnessExp = r1 * exp(r2);

		// fresnel
		// Schlick approximation
		float fresnel = pow(1.0 - NdotV, 5.0);
		fresnel *= (1.0 - F0);
		fresnel += F0;

		specular = (fresnel * geoAtt * roughnessExp) / (NdotV * NdotL * 3.14);
	}
	return specular;
}

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
	vec4 proj = vec4 (xyPlane.xy, d, 1.0);
	vec4 cam = cs_MatProjInv * proj;
	cam /= cam.w;
	
	vec4 world4 = cs_MatViewInv * cam;
	vec3 world = world4.xyz;// / world4.w;
	
	vec3 eye = (cs_MatViewInv * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 eyeDir = normalize (eye - world);
	
	vec3 lightDir = -cs_LightDirection;
	
	vec3 normal = texture(cs_NormalLightMode, texCoord).xyz * 2.0 - 1.0;
	normal = normalize(normal);
	vec4 txt = texture(cs_DiffuseRoughness, texCoord);
	vec3 diffuse = txt.rgb;
	float roughness = txt.a;
	
	float NdotL = max(dot(normal, lightDir), 0.0);
	float NdotV = max(dot(normal, eyeDir), 0.0);

	float diffuse_reflection = oren_nayar(normal, lightDir, eyeDir, NdotL, NdotV, roughness);
	float specular_reflection = cook_torrance(normal, lightDir, eyeDir, NdotL, NdotV, roughness);
	
	

	float directToIndirect = 1.0;
	float directDiffuse_reflection = clamp(diffuse_reflection, 0.0, 1.0) * directToIndirect;
	float indirectDiffuse_reflection= ((diffuse_reflection + 1.0) / 2.0) * (1.0 - directToIndirect);
	
	
	float shadow = calculate_shadow (world4, cam.xyz);
	if (shadow < 1.0)
	{
		specular_reflection = 0.0;
	}
	
	
	cs_FragColor = vec4 (diffuse * cs_LightColor.rgb * cs_LightEnergy * (directDiffuse_reflection * shadow + indirectDiffuse_reflection + specular_reflection) , 1.0);
	//cs_FragColor = vec4(specular_reflection, specular_reflection, specular_reflection, 1.0);

}
