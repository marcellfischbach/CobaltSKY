
#version 330

uniform vec3 vk_Distances;
uniform mat4 vk_ShadowMatsProj[3];
uniform mat4 vk_ShadowMatsView[3];
uniform vec2 vk_ShadowProjNearFar[3];
uniform float vk_MapBias;
uniform vec2 vk_ShadowIntensity;

uniform sampler2DArray vk_ShadowColorMap;
uniform sampler2DArrayShadow vk_ShadowMap;


float linstep(float min, float max, float v)  
{ 
  return clamp((v - min) / (max - min), 0.0, 1.0);  
}  

float ReduceLightBleeding(float p_max, float Amount)  
{  
  // Remove the [0, Amount] tail and linearly rescale (Amount, 1].  
   return linstep(Amount, 1, p_max);  
} 

float ChebyshevUpperBound(vec2 moments, float compare)
{
	/*
	// One-tailed inequality valid if t > Moments.x
	float p = 0.0;
	if (compare <= moments.x) p = 1.0;
	
	// Compute variance.
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance, 0.00001);
	// Compute probabilistic upper bound.
	float d = compare - moments.x;
	float p_max = variance / (variance + d*d);
	p_max = max(p, p_max);
	p_max = ReduceLightBleeding(p_max, 0.25);
	
	return p_max;
	*/
	
	float p = smoothstep(compare-0.005, compare, moments.x);
	float variance = max(moments.y - moments.x*moments.x, -0.001);
	float d = compare - moments.x;
	float p_max = linstep(0.2, 1.0, variance / (variance + d*d));
	return clamp(max(p, p_max), 0.0, 1.0);
	
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
	vec4 shadowCamSpace = vk_ShadowMatsView[layer] * world;
	vec4 depthBufferSpace = vk_ShadowMatsProj[layer] * shadowCamSpace;

	// shift from [-1, 1] -> [0, 1]
	depthBufferSpace = depthBufferSpace * 0.5 + 0.5;

	
	if (depthBufferSpace.x < 0.0 || depthBufferSpace.x >= 1.0 ||
	    depthBufferSpace.y < 0.0 || depthBufferSpace.y >= 1.0 ||
	    depthBufferSpace.z < 0.0 || depthBufferSpace.z >= 1.0)
	{
		return 1.0;
	}
	
	vec2 nearFar = vk_ShadowProjNearFar[layer];
	
	float nd = (shadowCamSpace.y - nearFar.x) / (nearFar.y - nearFar.x);
	
	vec2 moments = texture(vk_ShadowColorMap, vec3(depthBufferSpace.xy, layer)).rg;
	return ChebyshevUpperBound(moments, nd);
}
