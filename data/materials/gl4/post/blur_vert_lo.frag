#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Color0;
uniform vec2 cs_Color0SizeInv;

in vec2 inFragTexCoord0;

void main ()
{
	vec4 r = vec4(0.0);
	float stepSize = 0.05;
	float kernelSize = 20.0;
	float num = 0.0;
	for (float i=-1.0; i<=1.0; i+=stepSize)
	{
		vec2 disp = cs_Color0SizeInv * vec2 (0.0, i) * kernelSize;
		float impact = 1.0 - i*i;
		r += texture(cs_Color0, inFragTexCoord0 + disp) * impact;
		
		num += impact;
	}
	
	cs_FragColor = r / num;
}