

uniform sampler2DArray cs_BinaryGradient;
uniform vec2 cs_BinaryGradientToScreen;
uniform int cs_FadeInOutValue;


void cs_fade_in_out_test()
{
	if (cs_FadeInOutValue == 0)
	{
		discard;
	}
	else if (cs_FadeInOutValue < 17)
	{
		vec2 gradientCoord = gl_FragCoord.xy / 8.0;
		
		vec4 gr = texture(cs_BinaryGradient, vec3(gradientCoord, cs_FadeInOutValue));
		if (gr.r < 0.5)
		{
			discard;
		}
	}
}