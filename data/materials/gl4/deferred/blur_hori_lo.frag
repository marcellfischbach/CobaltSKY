#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Color0;

in vec2 texCoord;

void main ()
{
	vec4 r = vec4(0.0);
	float stepSize = 0.5;
	float kernelSize = 2.0;
	float num = 0.0;
	vec2 textureSizeInv = 1.0 / textureSize(cs_Color0, 0);
	for (float i=-1.0; i<=1.0; i+=stepSize)
	{
		vec2 disp = textureSizeInv * vec2 (i, 0.0) * kernelSize;
		r += texture(cs_Color0, texCoord + disp);
		
		num += 1.0;
	}
	
	cs_FragColor = r / num;
}