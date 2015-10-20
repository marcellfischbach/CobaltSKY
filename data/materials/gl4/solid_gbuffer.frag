
#version 330

varying vec4 color;
varying vec2 texCoord;

uniform sampler2D vk_Diffuse;
uniform float vk_Mult;

void main ()
{
  gl_FragColor = texture(vk_Diffuse, texCoord * vk_Mult);
}
