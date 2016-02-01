
#version 330

uniform vec3 vk_Distances;
uniform mat4 vk_ShadowMatsProj[3];
uniform mat4 vk_ShadowMatsView[3];
uniform float vk_MapBias;
uniform vec2 vk_ShadowIntensity;

uniform sampler2DArray vk_ShadowColorMap;
uniform sampler2DArrayShadow vk_ShadowMap;


float linstep(float min, float max, float v)  
{ 
  return clamp((v - min) / (max - min), 0, 1);  
}  

float ReduceLightBleeding(float p_max, float Amount)  
{  
  // Remove the [0, Amount] tail and linearly rescale (Amount, 1].  
   return linstep(Amount, 1, p_max);  
} 

float ChebyshevUpperBound(vec2 Moments, float t)
{
	// One-tailed inequality valid if t > Moments.x
	float p = 0.0;
	if (t <= Moments.x) p = 1.0;
	
	// Compute variance.
	float Variance = Moments.y - (Moments.x*Moments.x);
	Variance = max(Variance, 0.01);
	// Compute probabilistic upper bound.
	float d = t - Moments.x;
	float p_max = Variance / (Variance + d*d);
	p_max = max(p, p_max);
	p_max = ReduceLightBleeding(p_max, 0.25);
	
	return p_max;
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
	
	vec2 moments = texture(vk_ShadowColorMap, vec3(depthBufferSpace.xy, layer)).rg;
	return ChebyshevUpperBound(moments, shadowCamSpace.y+0.02);
}
