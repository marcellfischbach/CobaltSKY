
#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec2 inGeomTexCoord[];
out vec2 inFragTexCoord;
out float inFragLayer;


void main ()
{
	gl_Layer = 0;

	inFragLayer = 0;

	gl_Position = gl_in[0].gl_Position;
	inFragTexCoord = inGeomTexCoord[0];
	EmitVertex ();
	
	gl_Position = gl_in[1].gl_Position;
	inFragTexCoord = inGeomTexCoord[1];
	EmitVertex ();
	
	gl_Position = gl_in[2].gl_Position;
	inFragTexCoord = inGeomTexCoord[2];
	EmitVertex ();
	
	EndPrimitive ();
	
	
	gl_Layer = 1;

	inFragLayer = 1;

	gl_Position = gl_in[0].gl_Position;
	inFragTexCoord = inGeomTexCoord[0];
	EmitVertex ();
	
	gl_Position = gl_in[1].gl_Position;
	inFragTexCoord = inGeomTexCoord[1];
	EmitVertex ();
	
	gl_Position = gl_in[2].gl_Position;
	inFragTexCoord = inGeomTexCoord[2];
	EmitVertex ();
	
	EndPrimitive ();

	gl_Layer = 2;
	
	inFragLayer = 2;
	
	gl_Position = gl_in[0].gl_Position;
	inFragTexCoord = inGeomTexCoord[0];
	EmitVertex ();
	
	gl_Position = gl_in[1].gl_Position;
	inFragTexCoord = inGeomTexCoord[1];
	EmitVertex ();
	
	gl_Position = gl_in[2].gl_Position;
	inFragTexCoord = inGeomTexCoord[2];
	EmitVertex ();
	
	EndPrimitive ();
}


