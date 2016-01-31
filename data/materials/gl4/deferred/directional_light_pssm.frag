
#version 330

uniform vec3 vk_Distances;
uniform mat4 vk_ShadowMats[3];
uniform float vk_MapBias;
uniform vec2 vk_ShadowIntensity;

uniform sampler2DArray vk_ShadowColorMap;
uniform sampler2DArrayShadow vk_ShadowMap;

float ChebyshevUpperBound(vec2 Moments, float t)
{
	// One-tailed inequality valid if t > Moments.x
	float p = 0.0;
	if (t <= Moments.x)  p = 1.0;
	
	// Compute variance.
	float Variance = Moments.y - (Moments.x*Moments.x);
	Variance = max(Variance, 0.0001);
	// Compute probabilistic upper bound.
	float d = t - Moments.x;
	float p_max = Variance / (Variance + d*d);
	return max(p, p_max);
}


float calculate_shadow(vec4 world, vec3 cam)
{
	int layer = 0;

	float d = cam.y;
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

	// shift from [-1, 1] -> [0, 1]
	depthBufferSpace = depthBufferSpace * 0.5 + 0.5;

	
	if (depthBufferSpace.x < 0.0 || depthBufferSpace.x >= 1.0 ||
	    depthBufferSpace.y < 0.0 || depthBufferSpace.y >= 1.0 ||
	    depthBufferSpace.z < 0.0 || depthBufferSpace.z >= 1.0)
	{
		return 1.0;
	}
	
	//float intensity = texture(vk_ShadowMap, vec4 (depthBufferSpace.xy, layer, depthBufferSpace.z * vk_MapBias));
	
	//return intensity * vk_ShadowIntensity.x + vk_ShadowIntensity.y;
	
	vec2 val2 = texture(vk_ShadowColorMap, vec3(depthBufferSpace.xy, layer)).rg;
	return ChebyshevUpperBound(val2, depthBufferSpace.z);
}
