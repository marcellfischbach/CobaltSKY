
#version 330

layout(location = 0) out vec4 cs_FragColor;

in vec2 texCoord;

uniform sampler2D cs_Diffuse;




void cs_fade_in_out_test();

void main ()
{
	
	vec3 diffuse = texture(cs_Diffuse, texCoord).rgb;
	cs_FragColor = vec4(diffuse, 1.0);
}
