#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Color0;

in vec2 inFragTexCoord0;

void main ()
{
	vec4 col = texture(cs_Color0, inFragTexCoord0);
	if ((col.r + col.g + col.b) < (3.0 * 0.9))
	{
		col = vec4(0.0, 0.0, 0.0, 0.0);
	}
	else
	{
		col.rgb *= 1.0;
		col.a = 0.0;
	}
	cs_FragColor = col;
}