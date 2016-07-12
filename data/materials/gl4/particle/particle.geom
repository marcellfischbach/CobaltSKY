
#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 vk_MatView;
uniform mat4 vk_MatProj;


in vec2 inGeomParticleSize[];
in float inGeomParticleRotation[];
in float inGeomParticleTimeToLive[];

out vec2 inFragTexCoord;


void main ()
{
	if (inGeomParticleTimeToLive[0] <= 0.0)
	{
		return;
	}
	
	vec2 minS = -inGeomParticleSize[0];
	vec2 maxS = inGeomParticleSize[0];
	
	
	
	float c = cos(inGeomParticleRotation[0]);
	float s = sin(inGeomParticleRotation[0]);
	mat4 rot = mat4(vec4(c,   0.0, s,  0.0),
									vec4(0.0, 0.0, 0.0, 0.0),
									vec4(-s,   0.0, c,   0.0),
									vec4(0.0, 0.0, 0.0, 1.0));
	
	// Vertex 0
	gl_Position = vk_MatProj * (vk_MatView * gl_in[0].gl_Position + rot * vec4(minS.x, 0.0f, minS.y, 0.0));
	inFragTexCoord = vec2 (0, 0);
	EmitVertex ();

	// Vertex 1
	gl_Position = vk_MatProj * (vk_MatView * gl_in[0].gl_Position + rot * vec4(minS.x, 0.0f, maxS.y, 0.0));
	inFragTexCoord = vec2 (0, 1);
	EmitVertex ();
	
	
	// Vertex 2
	gl_Position = vk_MatProj * (vk_MatView * gl_in[0].gl_Position + rot * vec4(maxS.x, 0.0f, minS.y, 0.0));
	inFragTexCoord = vec2 (1, 0);
	EmitVertex ();
	
	// Vertex 3
	gl_Position = vk_MatProj * (vk_MatView * gl_in[0].gl_Position + rot * vec4(maxS.x, 0.0f, maxS.y, 0.0));
	inFragTexCoord = vec2 (1, 1);
	EmitVertex ();



}
