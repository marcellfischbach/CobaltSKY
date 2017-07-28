#version 330

layout(location = 0) out vec4 cs_FragColor;

uniform sampler2D cs_Color0;

in vec2 inFragTexCoord0;

void main ()
{
	vec4 col = texture(cs_Color0, inFragTexCoord0);
	col.rgb = max(col.rgb - vec3(0.4, 0.4, 0.4), vec3(0.0, 0.0, 0.0));
	col.rgb = col.rgb * 4.0;
	col.rgb = col.rgb * col.rgb;
	col.rgb = col.rgb / 4.0;
	col.a = 0.0;
	cs_FragColor = col;
}