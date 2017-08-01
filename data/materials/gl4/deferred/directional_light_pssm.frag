
#version 330

uniform vec3 cs_Distances;
uniform mat4 cs_ShadowMatsProjView[3];
uniform vec2 cs_ShadowIntensity;
uniform float cs_ShadowMapSizeInv;

uniform sampler2DArray cs_ShadowColorMap;
uniform sampler2DArrayShadow cs_ShadowMap;


float calculate_shadow(vec4 world, vec3 cam)
{
	int layer = 0;

	float d = cam.y;
	float varianceMult = 1.0;
	if (d < cs_Distances.x)
	{
		layer = 0;
		varianceMult = 1.0;
	}
	else if (d < cs_Distances.y)
	{
		layer = 1;
		varianceMult = 2.0;
	}
	else if (d < cs_Distances.z)
	{
		layer = 2;
		varianceMult = 4.0;
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
	
	float stepRange = 1.0;
	float stepSize = 1.0;
	float kernelSize = 0.001;
	

	float variance = texture(cs_ShadowColorMap, 
												 vec3(depthBufferSpace.xy, 
															layer)).r * 2.0;

	
	float shadow = 0.0;
	float amount = 0.0;
	for (float y=-stepRange; y<=stepRange; y+=stepSize)
	{
		float devY = y * kernelSize;
		for (float x=-stepRange; x<=stepRange; x+=stepSize)
		{
			float devX = x * kernelSize;
			
			float dev = x*x + y*y;
			
		
			
			float impact = 1.0 - dev / 2.0;
			shadow += texture(cs_ShadowMap, vec4(depthBufferSpace.xy + vec2(devX, devY), layer, depthBufferSpace.z - variance * varianceMult)) * impact;
			amount += impact;
		}
	}
	
	shadow /= amount;

	return shadow * cs_ShadowIntensity.x + cs_ShadowIntensity.y;
	
}
