
#version 330

in vec4 cs_Position;
//in vec3 cs_Normal;
//in vec2 cs_TexCoord0;

uniform mat4 cs_MatModel;


// out vec2 inGeomTexCoord;
// out vec3 inGeomNormal;


vec4 cs_bone_to_model4f(vec4 position);


void main ()
{
	vec4 position = cs_bone_to_model4f(cs_Position);
	gl_Position = cs_MatModel * position;
	//inGeomNormal = mat3(cs_MatModel) * cs_Normal;
	//inGeomTexCoord = cs_TexCoord0;
}
