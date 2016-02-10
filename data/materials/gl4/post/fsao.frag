#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform sampler2D vk_Color;
uniform sampler2D vk_Depth;
uniform sampler2D vk_Normal;
uniform vec2 vk_ColorSizeInv;

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
	
	vec4 color= texture(vk_Color, inFragTexCoord0);
	vec3 normal = texture(vk_Normal,  inFragTexCoord0).xyz * 2.0 - 1.0;
	
	d = d * 2.0f - 1.0;
	
	// projected paces
	vec4 proj = vec4 (inFragXYPlane.xy, d, 1.0);
	vec4 world4 = vk_MatProjViewInv * proj;
	world4 /= world4.w;
	vec3 world = world4.xyz;
	
	float stepSize = 1.0;
	float kernelSize = 10.0;
	float num = 0.0;
	float f = 0.0;
	vec3 val = vec3(0, 0, 0);
	float i = 1.0;
	//for (float i=-1.0; i<=1.0; i+=stepSize)
	{
		
		float j = 1.0;
		//for (float j=-1.0; j<=1.0; j+= stepSize)
		{
			vec2 disp = vk_ColorSizeInv * vec2 (i, j) * kernelSize;
			
			float dRef = texture(vk_Depth,  inFragTexCoord0 + disp).r;
			if (dRef != 1.0)
			{
			
				dRef = dRef * 2.0f - 1.0;
				
				// projected paces
				proj = vec4 (inFragXYPlane.xy + 2.0 * disp, dRef, 1.0);
				world4 = vk_MatProjViewInv * proj;
				world4 /= world4.w;
				vec3 worldRef = world4.xyz;
				
				vec3 diff = worldRef - world;
				float len = length(diff) * 100000.0;
				
				diff = normalize(diff);
				val = val + vec3(len);
				
				num += 1.0;
			}
			else
			{
				val = vec3(1, 1, 0);
				num = 1.0;
			}
			
		}
	}	

	vk_FragColor = vec4(val , 1);
}