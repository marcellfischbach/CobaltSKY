
#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform sampler2DArray cs_Diffuse;
uniform int cs_Layer;
in vec2 texCoord;


void main ()
{
  cs_FragColor =  texture(cs_Diffuse, vec3(texCoord, cs_Layer));
}
