

uniform sampler2DArray vk_BinaryGradient;
uniform vec2 vk_BinaryGradientToScreen;
uniform int vk_FadeInOutValue;


void vk_fade_in_out_test()
{
	if (vk_FadeInOutValue == 0)
	{
		discard;
	}
	else if (vk_FadeInOutValue < 17)
	{
		vec2 gradientCoord = gl_FragCoord.xy / 8.0;
		
		vec4 gr = texture(vk_BinaryGradient, vec3(gradientCoord, vk_FadeInOutValue));
		if (gr.r < 0.5)
		{
			discard;
		}
	}
}