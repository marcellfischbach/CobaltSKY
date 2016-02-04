
#version 330

in vec4 vk_Position;
//in vec3 vk_Normal;
//in vec2 vk_TexCoord0;

uniform mat4 vk_MatModel;


// out vec2 inGeomTexCoord;
// out vec3 inGeomNormal;

void main ()
{
	gl_Position = vk_MatModel * vk_Position;
	//inGeomNormal = mat3(vk_MatModel) * vk_Normal;
	//inGeomTexCoord = vk_TexCoord0;
}
