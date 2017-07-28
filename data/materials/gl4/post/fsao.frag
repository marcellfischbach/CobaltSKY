#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Depth;
uniform sampler2D cs_Normal;
uniform vec2 cs_DepthSizeInv;

uniform mat4 cs_MatProjInv;
uniform mat4 cs_MatViewInv;
uniform mat4 cs_MatProjViewInv;


in vec2 inFragTexCoord0;
in vec2 inFragXYPlane;

void main ()
{
	float d = texture(cs_Depth, inFragTexCoord0).r;
	if (d == 1.0)
	{
		//cs_FragColor = vec4(0, 0, 0, 1);
		cs_FragColor = vec4(1, 1, 1, 1);
		return;
	}
	
	vec3 normal = texture(cs_Normal,  inFragTexCoord0).xyz * 2.0 - 1.0;
	
	d = d * 2.0f - 1.0;
	
	// projected paces
	vec4 proj = vec4 (inFragXYPlane.xy, d, 1.0);
	proj = cs_MatProjInv * proj;
	proj /= proj.w;
	float depth = proj.y;
	vec3 worldRef = (cs_MatViewInv * proj).xyz;
	
	float stepSize = 1.0;
	float kernelSize = 10.0;
	float num = 0.0;
	float f = 0.0;
	vec3 val = vec3(0, 0, 0);
	float i = 0.0;
	float j = 0.0;
	
	float near = 0.0;
	float far = 15.0;
	
	float	normDepth = (depth - near) / (far - near);
	kernelSize = kernelSize - normDepth * kernelSize;
	if (kernelSize > 0.0)
	{
		
		float maxLength = 1.0;
		for (float i=-1.0; i<=1.0; i+=stepSize)
		{
			
			for (float j=-1.0; j<=1.0; j+= stepSize)
			{
				if (i==0.0 && j==0.0)
				{
					continue;
				}
				vec2 disp = cs_DepthSizeInv * vec2 (i, j) * kernelSize;
				vec2 dispProj = 2.0 * disp;
				
				float dRef = texture(cs_Depth,  inFragTexCoord0 + disp).r;
				if (dRef != 1.0)
				{
				
					dRef = dRef * 2.0f - 1.0;
					
					// projected paces
					proj = vec4 (inFragXYPlane.xy + dispProj, dRef, 1.0);
					vec4 worldProj = cs_MatProjViewInv * proj;
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
	}
	else
	{
		kernelSize = 0.0;
		f = 1.0;
	}
	cs_FragColor = vec4(f, f, f, 1);
	//kernelSize /= 10.0;
	//cs_FragColor = vec4(kernelSize, kernelSize, kernelSize, 1.0);
}