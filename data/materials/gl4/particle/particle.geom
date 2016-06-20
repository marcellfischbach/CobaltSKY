
#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 vk_MatViewInv;
uniform mat4 vk_MatProjView;


in vec4 inGeomParticleSize[];
in vec3 inGeomParticleRotation[];
in vec4 inGeomParticleTexCoord[];

out vec2 inFragTexCoord;


void main ()
{
	vec2 minS = inGeomParticleSize[0].xy;
	vec2 maxS = inGeomParticleSize[0].zw;
	
	vec2 minT = inGeomParticleTexCoord[0].xy;
	vec2 maxT = inGeomParticleTexCoord[0].zw;
	
	
	float c = cos(inGeomParticleRotation[0].x);
	float s = sin(inGeomParticleRotation[0].x);
	mat4 rot = mat4(c,   0.0, -s,  0.0,
									0.0, 1.0, 0.0, 0.0,
									s,   0.0, c,   0.0,
									0.0, 0.0, 0.0, 1.0);
									
	mat4 finalMat = rot * vk_MatViewInv;
	
	// Vertex 0
	gl_Position = vk_MatProjView * (gl_in[0].gl_Position + finalMat * vec4(minS.x, 0.0f, minS.y, 0.0));
	inFragTexCoord = vec2 (minT.x, minT.y);
	EmitVertex ();

	// Vertex 1
	gl_Position = vk_MatProjView * (gl_in[0].gl_Position + finalMat * vec4(minS.x, 0.0f, maxS.y, 0.0));
	inFragTexCoord = vec2 (minT.x, maxT.y);
	EmitVertex ();
	
	
	// Vertex 2
	gl_Position = vk_MatProjView * (gl_in[0].gl_Position + finalMat * vec4(maxS.x, 0.0f, minS.y, 0.0));
	inFragTexCoord = vec2 (maxT.x, minT.y);
	EmitVertex ();
	
	// Vertex 3
	gl_Position = vk_MatProjView * (gl_in[0].gl_Position + finalMat * vec4(maxS.x, 0.0f, maxS.y, 0.0));
	inFragTexCoord = vec2 (maxT.x, maxT.y);
	EmitVertex ();



}
