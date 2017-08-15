
#version 330

uniform sampler2D cs_ShadowMap;


float calculate_shadow(vec2 texCoord)
{
		return texture(cs_ShadowMap, texCoord);
}
