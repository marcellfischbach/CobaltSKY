
#version 330

uniform vec3 cs_Distances;
uniform mat4 cs_ShadowMatsProjView[3];
uniform vec2 cs_ShadowIntensity;
uniform float cs_ShadowMapSizeInv;

uniform sampler2DArray cs_ShadowColorMap;
uniform sampler2DArrayShadow cs_ShadowMap;


float ChebyshevUpperBound(vec2 Moments, float t)
{
	// TODO: Make that variable global later.
	float g_MinVariance = 0.01;
	
	// One-tailed inequality valid if t > Moments.x
	float p = (t <= Moments.x) ? 1.0 : 0.0;
	// Compute variance.
	float Variance = Moments.y - (Moments.x*Moments.x);
	Variance = max(Variance, g_MinVariance);
	// Compute probabilistic upper bound.
	float d = t - Moments.x ;
	float p_max = Variance / (Variance + d*d);
	
	return max(p, p_max);
}

float calculate_shadow(vec4 world, vec3 cam)
{
	int layer = 0;

	float d = cam.y;
	if (d < cs_Distances.x)
	{
		layer = 0;
	}
	else if (d < cs_Distances.y)
	{
		layer = 1;
	}
	else if (d < cs_Distances.z)
	{
		layer = 2;
	}
	else
	{
		return 1.0;
	}
	
	// transform into final depth buffer space and performce perspective division
	vec4 depthBufferSpace = cs_ShadowMatsProjView[layer] * world;
	// shift from [-1, 1] -> [0, 1]
	depthBufferSpace.xyz /= depthBufferSpace.w;
	depthBufferSpace.xy = depthBufferSpace.xy * 0.5 + 0.5;

	
	if (depthBufferSpace.x < 0.0 || depthBufferSpace.x >= 1.0 ||
	    depthBufferSpace.y < 0.0 || depthBufferSpace.y >= 1.0 ||
	    depthBufferSpace.z < 0.0 || depthBufferSpace.z >= 1.0)
	{
		return 1.0;
	}
	

	/*
	// CobaltSKY own shadow mapping algorithm
	float val = 0.0;
	float kernStep = 1.0;
	float kernSize = 0.0;
	float moment = texture(cs_ShadowColorMap, 
												 vec3(depthBufferSpace.xy, 
															layer)).r;
	if (moment > 1.0)
	{
		moment = 0.0;
	}
	
	int kerns = 0;
	for (float i=-kernSize; i<=kernSize; i+=kernStep)
	{
		for (float j=-kernSize; j<=kernSize; j+=kernStep)
		{
			vec2 kern = vec2 (i, j) * cs_ShadowMapSizeInv;
			
			
			val += texture(cs_ShadowMap, 
										 vec4(depthBufferSpace.xy + kern, 
													layer, 
			                    depthBufferSpace.z - moment));
			kerns+=1;
		}
	}
	
	val /= float(kerns);
	*/
	// VSM Shadow mapping algorithm
	vec2 Moments = texture(cs_ShadowColorMap, 
												 vec3(depthBufferSpace.xy, 
															layer)).rg;
															
	float depth = depthBufferSpace.z;
  // Compute the Chebyshev upper bound.
	float val = ChebyshevUpperBound(Moments, depth);
	
	
	return val * cs_ShadowIntensity.x + cs_ShadowIntensity.y;
}
