
#version 330

uniform vec3 cs_Distances;
uniform mat4 cs_ShadowMatsProjView[3];
uniform vec2 cs_ShadowIntensity;
uniform float cs_ShadowMapSizeInv;

uniform sampler2DArray cs_ShadowColorMap;
uniform sampler2DArrayShadow cs_ShadowMap;

float linstep(float min, float max, float v)
{
  return clamp((v - min) / (max - min), 0, 1);
}

float ReduceLightBleeding(float p_max, float Amount)
{
  // Remove the [0, Amount] tail and linearly rescale (Amount, 1].
   return linstep(Amount, 1, p_max);
}

float ChebyshevUpperBound(vec2 Moments, float t, float minVariance)
{
	// TODO: Make that variable global later.
	
  // One-tailed inequality valid if t > Moments.x
   float p = (t <= Moments.x) ? 1.0 : 0.0;
  // Compute variance.
   float Variance = Moments.y - (Moments.x*Moments.x);
  Variance = max(Variance, minVariance);
  // Compute probabilistic upper bound.
   float d = t - Moments.x;
  float p_max = Variance / (Variance + d*d);
	p_max = linstep(0.97, 0.99, p_max);
  return max(p, p_max);
	
	}
	
	

float calculate_shadow(vec4 world, vec3 cam)
{
	int layer = 0;

	float d = cam.y;
	float minVariance = 0.001;
	if (d < cs_Distances.x)
	{
		layer = 0;
		minVariance = 0.001;
	}
	else if (d < cs_Distances.y)
	{
		layer = 1;
		minVariance = 0.0008;
	}
	else if (d < cs_Distances.z)
	{
		layer = 2;
		minVariance = 0.0004;
	}
	else
	{
		return 1.0;
	}
	
	
	// transform into final depth buffer space and performce perspective division
	vec4 depthBufferSpace = cs_ShadowMatsProjView[layer] * world;
	// shift from [-1, 1] -> [0, 1]
	depthBufferSpace /= depthBufferSpace.w;
	depthBufferSpace = depthBufferSpace * 0.5 + 0.5;

	
	if (depthBufferSpace.x < 0.0 || depthBufferSpace.x >= 1.0 ||
	    depthBufferSpace.y < 0.0 || depthBufferSpace.y >= 1.0 ||
	    depthBufferSpace.z < 0.0 || depthBufferSpace.z >= 1.0)
	{
		return 1.0;
	}
	

	// VSM Shadow mapping algorithm
	vec2 Moments = texture(cs_ShadowColorMap, 
												 vec3(depthBufferSpace.xy, 
															layer)).rg;
															
	float depth = depthBufferSpace.z;
  // Compute the Chebyshev upper bound.
	float val = ChebyshevUpperBound(Moments, depth, minVariance);
	return val * cs_ShadowIntensity.x + cs_ShadowIntensity.y;
	
}
