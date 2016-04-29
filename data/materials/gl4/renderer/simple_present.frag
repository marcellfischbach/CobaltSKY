
#version 330

layout(location = 0) out vec4 vk_FragColor;

uniform sampler2D vk_Diffuse;
in vec2 texCoord;


void main ()
{
  vk_FragColor = vec4 (texCoord, 0, 0) + texture(vk_Diffuse, texCoord);
}
