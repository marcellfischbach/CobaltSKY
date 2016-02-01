#version 330

layout(location = 0) out vec2 vk_Moments;

uniform sampler2DArray vk_ShadowBuffer;
uniform float vk_ShadowBufferSizeInv;

in vec2 inFragTexCoord;
in float inFragLayer;


void main ()
{
	
	vec2 disp = vec2 (vk_ShadowBufferSizeInv, 0.0);
	
	vec2 val = texture(vk_ShadowBuffer, vec3 (inFragTexCoord, inFragLayer)).rg * 0.6;
	val += texture(vk_ShadowBuffer, vec3 (inFragTexCoord + disp, inFragLayer)).rg * 0.2;
	val += texture(vk_ShadowBuffer, vec3 (inFragTexCoord - disp, inFragLayer)).rg * 0.2;

	vk_Moments = val;
																									
}

