
#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=9) out;


uniform mat4 cs_ShadowMapMatProjView[3];
in vec2 inGeomTexCoord[];
in vec3 inGeomNormal[];

out vec2 inFragTexCoord;
out vec3 inFragNormal;

void main ()
{
	// render first layer
	gl_Layer = 0;
	
	gl_Position = cs_ShadowMapMatProjView[0] * gl_in[0].gl_Position;
	inFragTexCoord = inGeomTexCoord[0];
	inFragNormal = inGeomNormal[0];
	EmitVertex ();
	gl_Position = cs_ShadowMapMatProjView[0] * gl_in[1].gl_Position;
	inFragTexCoord = inGeomTexCoord[1];
	inFragNormal = inGeomNormal[1];
	EmitVertex ();
	gl_Position = cs_ShadowMapMatProjView[0] * gl_in[2].gl_Position;
	inFragTexCoord = inGeomTexCoord[2];
	inFragNormal = inGeomNormal[2];
	EmitVertex ();
	
	EndPrimitive ();
	
	// render second layer
	gl_Layer = 1;
	
	gl_Position = cs_ShadowMapMatProjView[1] * gl_in[0].gl_Position;
	inFragTexCoord = inGeomTexCoord[0];
	inFragNormal = inGeomNormal[0];
	EmitVertex ();
	gl_Position = cs_ShadowMapMatProjView[1] * gl_in[1].gl_Position;
	inFragTexCoord = inGeomTexCoord[1];
	inFragNormal = inGeomNormal[1];
	EmitVertex ();
	gl_Position = cs_ShadowMapMatProjView[1] * gl_in[2].gl_Position;
	inFragTexCoord = inGeomTexCoord[2];
	inFragNormal = inGeomNormal[2];
	EmitVertex ();
	
	EndPrimitive ();

	// render first layer
	gl_Layer = 2;
	
	gl_Position = cs_ShadowMapMatProjView[2] * gl_in[0].gl_Position;
	inFragTexCoord = inGeomTexCoord[0];
	inFragNormal = inGeomNormal[0];
	EmitVertex ();
	gl_Position = cs_ShadowMapMatProjView[2] * gl_in[1].gl_Position;
	inFragTexCoord = inGeomTexCoord[1];
	inFragNormal = inGeomNormal[1];
	EmitVertex ();
	gl_Position = cs_ShadowMapMatProjView[2] * gl_in[2].gl_Position;
	inFragTexCoord = inGeomTexCoord[2];
	inFragNormal = inGeomNormal[2];
	EmitVertex ();
	
	EndPrimitive ();

	
	
}
