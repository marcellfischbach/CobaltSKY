
#version 330

layout(location = 0) out vec4 cs_DiffuseRoughness;
layout(location = 1) out vec4 cs_NormalLightMode;
layout(location = 2) out vec4 cs_EmissivMetallic;
layout(location = 3) out vec4 cs_SSSSpecular;

in vec2 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 binormal;

uniform sampler2D cs_Diffuse;
uniform sampler2D cs_Normal;
uniform vec2 cs_ViewportSizeInv;
uniform vec4 cs_Color;




void cs_fade_in_out_test();

void main ()
{
	cs_fade_in_out_test();
	vec3 t = normalize(tangent);
	vec3 b = normalize(binormal);
	vec3 n = normalize(normal);
	mat3 faceToWorldMat = mat3(t, b, n);
														 
	vec3 normalTxt = texture(cs_Normal, texCoord).rgb * 2.0 - 1.0;
	normalTxt = faceToWorldMat * normalTxt;
	
	vec3 diffuse = texture(cs_Diffuse, texCoord).rgb;
	diffuse *= cs_Color.rgb;
	cs_DiffuseRoughness = vec4(diffuse, 0.0);
	cs_NormalLightMode = vec4 (normalTxt * 0.5 + 0.5, 0.0);
	cs_EmissivMetallic = vec4(0, 0, 0, 0);
	cs_SSSSpecular = vec4 (0, 0, 0, 0);

}
