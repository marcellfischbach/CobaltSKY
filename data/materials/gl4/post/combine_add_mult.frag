#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Color0;
uniform sampler2D cs_Color1;
uniform sampler2D cs_Color2;

in vec2 inFragTexCoord0;

void main ()
{
	cs_FragColor = (texture(cs_Color0, inFragTexCoord0) 
	             + texture(cs_Color1, inFragTexCoord0)) 
							 * texture(cs_Color2, inFragTexCoord0);
}