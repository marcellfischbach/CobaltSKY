#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform sampler2D vk_Depth;
uniform sampler2D vk_Normal;
uniform vec2 vk_DepthSizeInv;

uniform mat4 vk_MatProjInv;
uniform mat4 vk_MatViewInv;
uniform mat4 vk_MatProjViewInv;


in vec2 inFragTexCoord0;
in vec2 inFragXYPlane;

void main ()
{
	float d = texture(vk_Depth, inFragTexCoord0).r;
	if (d == 1.0)
	{
		vk_FragColor = vec4(0, 0, 0, 1);
		return;
	}
	
	vec3 normal = texture(vk_Normal,  inFragTexCoord0).xyz * 2.0 - 1.0;
	
	d = d * 2.0f - 1.0;
	
	// projected paces
	vec4 proj = vec4 (inFragXYPlane.xy, d, 1.0);
	proj = vk_MatProjInv * proj;
	proj /= proj.w;
	float depth = proj.y;
	vec3 worldRef = (vk_MatViewInv * proj).xyz;
	
	float stepSize = 1.0;
	float kernelSize = 20.0;
	float num = 0.0;
	float f = 0.0;
	vec3 val = vec3(0, 0, 0);
	float i = 0.0;
	float j = 0.0;
	
	float near = 0.0;
	float far = 100.0;
	
	float	normDepth = (depth - near) / (far - near);
	kernelSize = kernelSize - normDepth * kernelSize;
	
	float maxLength = 1.0;
	for (float i=-1.0; i<=1.0; i+=stepSize)
	{
		
		for (float j=-1.0; j<=1.0; j+= stepSize)
		{
			if (i==0.0 && j==0.0)
			{
				continue;
			}
			vec2 disp = vk_DepthSizeInv * vec2 (i, j) * kernelSize;
			vec2 dispProj = 2.0 * disp;
			
			float dRef = texture(vk_Depth,  inFragTexCoord0 + disp).r;
			if (dRef != 1.0)
			{
			
				dRef = dRef * 2.0f - 1.0;
				
				// projected paces
				proj = vec4 (inFragXYPlane.xy + dispProj, dRef, 1.0);
				vec4 worldProj = vk_MatProjViewInv * proj;
				vec3 world = worldProj.xyz / worldProj.w;
				
				vec3 diff = world - worldRef;
				float len = clamp(length(diff), 0.0, maxLength) / maxLength;
				diff = normalize(diff);
				
				float distFact = smoothstep(1.0, 0.0, len);
				float l = distFact * dot(diff, normal) * 2.0;
				f += l;
				num += 1.0;
			}
		}
	}	

	f /= num;
	f = 1.0 - f;
	vk_FragColor = vec4(f, f, f, 1);
}