
#version 330

uniform sampler2DArray vk_Diffuse;
uniform int vk_Layer;
in vec2 texCoord;


void main ()
{
  gl_FragColor =  texture(vk_Diffuse, vec3(texCoord, vk_Layer));
}
