
#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform sampler2D vk_Diffuse;
in vec2 texCoord;


void main ()
{
  vk_FragColor = texture(vk_Diffuse, texCoord);
}
