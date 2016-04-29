
#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform sampler2DArray vk_Diffuse;
uniform int vk_Layer;
in vec2 texCoord;


void main ()
{
  vk_FragColor =  texture(vk_Diffuse, vec3(texCoord, vk_Layer));
}
