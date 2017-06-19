
#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_DiffuseRoughness;
uniform sampler2D cs_NormalLightMode;
uniform sampler2D cs_EmissiveMetallic;
uniform sampler2D cs_SSSSpecular;
uniform sampler2D cs_Depth;

in vec2 texCoord;

void main ()
{
	float d = texture(cs_Depth, texCoord).r;
	vec3 diffuse = texture(cs_DiffuseRoughness, texCoord).rgb;
	
	cs_FragColor = vec4 (d, d, d, 1.0);
	cs_FragColor = vec4 (diffuse, 1.0);
}
