
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

float calculate_shadow(vec2 texCoord);

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

const float PI = 3.141569;

float chiGGX(float v)
{
    return v > 0 ? 1 : 0;
}

float GGX_Distribution(vec3 normal, vec3 halfVec, float roughness)
{
    float NdotH = dot(normal,halfVec);
    float roughness2 = roughness * roughness;
    float NdotH2 = NdotH * NdotH;
    float den = NdotH2 * roughness2 + (1 - NdotH2);
    return (chiGGX(NdotH) * roughness2) / ( PI * den * den );
}

float GGX_PartialGeometryTerm(vec3 eyeDir, vec3 normal, vec3 halfVec, float alpha)
{
    float VdotH2 = clamp(dot(eyeDir,halfVec), 0.0, 1.0);
    float chi = chiGGX( VdotH2 / clamp(dot(eyeDir,normal), 0.0, 1.0));
    VdotH2 = VdotH2 * VdotH2;
    float tan2 = ( 1 - VdotH2 ) / VdotH2;
    return (chi * 2) / ( 1 + sqrt( 1 + alpha * alpha * tan2 ) );
}

vec3 Fresnel_Schlick(vec3 eyeDir, vec3 halfVec, vec3 F0)
{
	float VdotH = dot(eyeDir, halfVec);
  return F0 + (1-F0) * pow( 1 - VdotH, 5);
}

vec3 cook_torrance(vec3 normal, vec3 lightDir, vec3 eyeDir, float NdotL, float NdotV, float roughness)
{

	// Calculate colour at normal incidence
	
	float ior = 1.0 + roughness;
	vec3 F0 = vec3(abs ((1.0 - ior) / (1.0 + ior)));
	F0 = F0 * F0;
	//F0 = lerp(F0, materialColour.rgb, metallic);
	
	
	vec3 specular = vec3(0.0);
	if(NdotL > 0.0)
	{

		// calculate intermediary values
		vec3 halfVecVector = normalize(lightDir + eyeDir);
		float NdotV = dot(normal, eyeDir);
		float NdotL = dot(normal, lightDir);
		float NdotH = dot(normal, halfVecVector);
		
		float denominator = clamp( 4 * (NdotV * clamp(dot(halfVecVector, normal), 0.0, 1.0) + 0.05), 0.0, 1.0 );
		
		denominator = clamp(4 * (NdotV * clamp(NdotH, 0.0, 1.0) + 0.5), 0.0, 1.0);
		float D = GGX_Distribution(normal, halfVecVector, roughness);
		float G = GGX_PartialGeometryTerm(eyeDir, normal, halfVecVector, roughness);
		vec3 F = Fresnel_Schlick(eyeDir, halfVecVector, F0);
		
		specular = D*G*F/denominator;
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
	
	vec3 lightDir = normalize(-cs_LightDirection);
	
	vec3 normal = texture(cs_NormalLightMode, texCoord).xyz * 2.0 - 1.0;
	//normal = normalize(normal);
	vec4 txt = texture(cs_DiffuseRoughness, texCoord);
	vec3 diffuse = txt.rgb;
	float roughness = txt.a;
	
	float NdotL = max(dot(normal, lightDir), 0.0);
	float NdotV = max(dot(normal, eyeDir), 0.0);

	float diffuse_reflection = oren_nayar(normal, lightDir, eyeDir, NdotL, NdotV, roughness);
	vec3 specular_reflection = cook_torrance(normal, lightDir, eyeDir, NdotL, NdotV, roughness);
	//diffuse_reflection = 0.0;
	

	float directToIndirect = 0.8;
	float directDiffuse_reflection = clamp(diffuse_reflection, 0.0, 1.0) * directToIndirect;
	float indirectDiffuse_reflection= clamp(diffuse_reflection, 0.0, 1.0) * (1.0 - directToIndirect);
	
	
	float shadow = calculate_shadow (texCoord);
	if (shadow < 1.0)
	{
		specular_reflection = vec3(0.0);
	}
	
	
	cs_FragColor = vec4 (diffuse * cs_LightColor.rgb * cs_LightEnergy * (directDiffuse_reflection * shadow + indirectDiffuse_reflection + specular_reflection) , 1.0);
	//cs_FragColor = vec4(specular_reflection, specular_reflection, specular_reflection, 1.0);
	//cs_FragColor = vec4(normal * 0.5 + 0.5, 1.0);
	//cs_FragColor = vec4(diffuse, 1.0);
	//cs_FragColor = vec4(shadow, shadow, shadow, 1.0);
}
