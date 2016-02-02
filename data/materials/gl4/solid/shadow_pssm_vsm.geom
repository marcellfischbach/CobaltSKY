
#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=9) out;


uniform mat4 vk_ShadowMapMatView[3];
uniform mat4 vk_ShadowMapMatProj[3];
uniform vec2 vk_ShadowMapNearFar[3];

in vec2 inGeomTexCoord[];
in vec3 inGeomNormal[];

out vec2 inFragTexCoord;
out vec3 inFragNormal;
out float inFragDepth;

void main ()
{
	for (int i=0; i<3; ++i)
	{
		float invDepthScale = 1.0 / (vk_ShadowMapNearFar[i].y - vk_ShadowMapNearFar[i].x);
		gl_Layer = i;
		vec4 viewPos = vk_ShadowMapMatView[i] * gl_in[0].gl_Position;
		gl_Position = vk_ShadowMapMatProj[i] * viewPos;
		inFragDepth = (viewPos.y - vk_ShadowMapNearFar[i].x) * invDepthScale;
		inFragTexCoord = inGeomTexCoord[0];
		inFragNormal = inGeomNormal[0];
		EmitVertex ();
		
		viewPos = vk_ShadowMapMatView[i] * gl_in[1].gl_Position;
		gl_Position = vk_ShadowMapMatProj[i] * viewPos;
		inFragDepth = (viewPos.y - vk_ShadowMapNearFar[i].x) * invDepthScale;
		inFragTexCoord = inGeomTexCoord[1];
		inFragNormal = inGeomNormal[1];
		EmitVertex ();
		
		viewPos = vk_ShadowMapMatView[i] * gl_in[2].gl_Position;
		gl_Position = vk_ShadowMapMatProj[i] * viewPos;
		inFragDepth = (viewPos.y - vk_ShadowMapNearFar[i].x) * invDepthScale;
		inFragTexCoord = inGeomTexCoord[2];
		inFragNormal = inGeomNormal[2];
		EmitVertex ();
		
		EndPrimitive ();
		
	}
	/*
	// render first layer
	gl_Layer = 0;
	vec4 viewPos;
	viewPos = vk_ShadowMapMatView[0] * gl_in[0].gl_Position;
	gl_Position = vk_ShadowMapMatProj[0] * viewPos;
	inFragDepth = viewPos.y;
	inFragTexCoord = inGeomTexCoord[0];
	inFragNormal = inGeomNormal[0];
	EmitVertex ();
	viewPos = vk_ShadowMapMatView[0] * gl_in[1].gl_Position;
	gl_Position = vk_ShadowMapMatProj[0] * viewPos;
	inFragDepth = viewPos.y;
	inFragTexCoord = inGeomTexCoord[1];
	inFragNormal = inGeomNormal[1];
	EmitVertex ();
	viewPos = vk_ShadowMapMatView[0] * gl_in[2].gl_Position;
	gl_Position = vk_ShadowMapMatProj[0] * viewPos;
	inFragDepth = viewPos.y;
	inFragTexCoord = inGeomTexCoord[2];
	inFragNormal = inGeomNormal[2];
	EmitVertex ();
	
	EndPrimitive ();
	
	// render second layer
	gl_Layer = 1;
	
	viewPos = vk_ShadowMapMatView[1] * gl_in[0].gl_Position;
	gl_Position = vk_ShadowMapMatProj[1] * viewPos;
	inFragDepth = viewPos.y;
	inFragTexCoord = inGeomTexCoord[0];
	inFragNormal = inGeomNormal[0];
	EmitVertex ();
	viewPos = vk_ShadowMapMatView[1] * gl_in[1].gl_Position;
	gl_Position = vk_ShadowMapMatProj[1] * viewPos;
	inFragDepth = viewPos.y;
	inFragTexCoord = inGeomTexCoord[1];
	inFragNormal = inGeomNormal[1];
	EmitVertex ();
	viewPos = vk_ShadowMapMatView[1] * gl_in[2].gl_Position;
	gl_Position = vk_ShadowMapMatProj[1] * viewPos;
	inFragDepth = viewPos.y;
	inFragTexCoord = inGeomTexCoord[2];
	inFragNormal = inGeomNormal[2];
	EmitVertex ();
	
	EndPrimitive ();

	// render first layer
	gl_Layer = 2;
	
	gl_Position = vk_ShadowMapMatProjView[2] * gl_in[0].gl_Position;
	inFragTexCoord = inGeomTexCoord[0];
	inFragNormal = inGeomNormal[0];
	EmitVertex ();
	gl_Position = vk_ShadowMapMatProjView[2] * gl_in[1].gl_Position;
	inFragTexCoord = inGeomTexCoord[1];
	inFragNormal = inGeomNormal[1];
	EmitVertex ();
	gl_Position = vk_ShadowMapMatProjView[2] * gl_in[2].gl_Position;
	inFragTexCoord = inGeomTexCoord[2];
	inFragNormal = inGeomNormal[2];
	EmitVertex ();
	
	EndPrimitive ();
	*/
	
	
}
