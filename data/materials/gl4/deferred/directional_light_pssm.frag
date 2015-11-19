
#version 330

uniform vec3 vk_Distances;
uniform mat4 vk_ShadowMats[3];
uniform float vk_MapBias;
uniform vec2 vk_ShadowIntensity;

uniform sampler2DArrayShadow vk_ShadowMap;

float calc(vec3 ref, int layer)
{
	return texture(vk_ShadowMap, vec4 (ref.xy, layer, ref.z * vk_MapBias));
}

float calculate_shadow(vec4 world, vec3 cam)
{
	int layer = 0;
	float d = -cam.z;
	if (d < vk_Distances.x)
	{
		layer = 0;
	}
	else if (d < vk_Distances.y)
	{
		layer = 1;
	}
	else if (d < vk_Distances.z)
	{
		layer = 2;
	}
	else
	{
		return 1.0;
	}
	
	// transform into final depth buffer space and performce perspective division
	vec4 depthBufferSpace = vk_ShadowMats[layer] * world;
	depthBufferSpace /= depthBufferSpace.w;

	// shift from [-1, 1] -> [0, 1]
	depthBufferSpace = depthBufferSpace * 0.5 + 0.5;

	
	if (depthBufferSpace.x < 0.0 || depthBufferSpace.x >= 1.0 ||
	    depthBufferSpace.y < 0.0 || depthBufferSpace.y >= 1.0 ||
	    depthBufferSpace.z < 0.0 || depthBufferSpace.z >= 1.0)
	{
		return 1.0;
	}
	
	
	return calc(depthBufferSpace.xyz, layer) * vk_ShadowIntensity.x + vk_ShadowIntensity.y;
}
