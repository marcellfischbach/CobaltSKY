
#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform mat4 cs_MatProjInv;
uniform mat4 cs_MatViewInv;

uniform sampler2D cs_Depth;

uniform vec3 cs_Distances;
uniform mat4 cs_ShadowMatsProjView[3];

uniform sampler2DArray cs_ShadowColorMap;
uniform sampler2DArrayShadow cs_ShadowMap;

in vec2 texCoord;
in vec2 xyPlane;


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

	
	
	float shadow = texture(cs_ShadowMap, vec4(depthBufferSpace.xy, layer, depthBufferSpace.z - variance * varianceMult));

	
	return mix(0.0, 1.0, shadow);
}



void main ()
{
	float d = texture(cs_Depth, texCoord).r;
	if (d == 1.0)
	{
		discard;
	}
	d = d * 2.0f - 1.0;
	
	// projected paces
	vec4 proj = vec4 (xyPlane.xy, d, 1.0);
	vec4 cam = cs_MatProjInv * proj;
	cam /= cam.w;
	
	vec4 world4 = cs_MatViewInv * cam;
	
	
	float shadow = calculate_shadow (world4, cam.xyz);
	cs_FragColor = vec4(shadow, shadow, shadow, 1.0);
}

