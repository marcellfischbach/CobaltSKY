
#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=9) out;


uniform mat4 cs_ShadowMapMatProjView[3];

//in vec2 inGeomTexCoord[];
//in vec3 inGeomNormal[];

//out vec2 inFragTexCoord;
//out vec3 inFragNormal;
out float inFragDepth;

void main ()
{
	for (int i=0; i<3; ++i)
	{
		gl_Layer = i;
		gl_Position = cs_ShadowMapMatProjView[i] * gl_in[0].gl_Position;
		inFragDepth = gl_Position.z / gl_Position.w;
		EmitVertex ();
		
		gl_Position = cs_ShadowMapMatProjView[i] * gl_in[1].gl_Position;
		inFragDepth = gl_Position.z / gl_Position.w;
		EmitVertex ();
		
		gl_Position = cs_ShadowMapMatProjView[i] * gl_in[2].gl_Position;
		inFragDepth = gl_Position.z / gl_Position.w;
		EmitVertex ();
		
		EndPrimitive ();
		
	}
	
}
