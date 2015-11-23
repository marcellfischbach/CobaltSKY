
#version 330

uniform mat4 vk_ShadowMats[6];
uniform float vk_MapBias;
uniform vec2 vk_ShadowIntensity;
uniform vec3 vk_LightPosition;

uniform sampler2DArrayShadow vk_ShadowMap;


vec4 calculate_shadow(vec3 world)
{
	int layer = 0;
	
	vec3 dir = world - vk_LightPosition;
	vec3 d = abs(dir);
	if (d.x > d.y && d.x > d.z)
	{
		if (dir.x > 0)
		{
			layer = 0;
		}
		else
		{
			layer = 1;
		}
	}
	else if (d.y > d.x && d.y > d.z)
	{
		if (dir.y > 0)
		{
			layer = 2;
		}
		else
		{
			layer = 3;
		}
	}
	else
	{
		if (dir.z > 0)
		{
			layer = 4;
		}
		else
		{
			layer = 5;
		}
	}
	
	
	
	

	// transform into final depth buffer space and performce perspective division
	vec4 depthBufferSpace = vk_ShadowMats[layer] * vec4(world, 1.0);
	depthBufferSpace.xyz /= depthBufferSpace.w;

	// shift from [-1, 1] -> [0, 1]
	depthBufferSpace = depthBufferSpace * 0.5 + 0.5;

	
	if (depthBufferSpace.x < 0.0 || depthBufferSpace.x >= 1.0 ||
	    depthBufferSpace.y < 0.0 || depthBufferSpace.y >= 1.0 ||
	    depthBufferSpace.z < 0.0 || depthBufferSpace.z >= 1.0)
	{
		return vec4(1.0, 0.0, 1.0, 1.0);
	}
	
	float f = texture(vk_ShadowMap, vec4 (depthBufferSpace.xy, layer, depthBufferSpace.z * vk_MapBias));
	return vec4 (f, f, f, 1.0);
}
