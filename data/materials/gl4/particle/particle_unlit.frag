#version 330

layout(location = 0) out vec4 cs_FragColor;



in vec2 inFragTexCoord;
in vec3 inFragTextPage;

void main ()
{
	float d = clamp(1.0 - length (inFragTexCoord - vec2(0.5, 0.5)) * 2.2, 0.0, 1.0);
	cs_FragColor = vec4(0.2, 0.3, 0.4, d * 0.2);
	//float f = inFragTextPage.z * (inFragTextPage.x / 10.0) + (1.0 - inFragTextPae
	//float f = mix (inFragTextPage.x / 4.0, inFragTextPage.y / 4.0, inFragTextPage.z);
	
	//cs_FragColor = vec4(f, f, f, 1);
}

