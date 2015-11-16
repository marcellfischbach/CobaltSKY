
#version 330

uniform sampler2D vk_Diffuse;
in vec2 texCoord;


void main ()
{
  gl_FragColor =  texture(vk_Diffuse, texCoord);
}
