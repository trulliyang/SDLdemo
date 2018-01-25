#version 300 es

in vec2 tc;
in vec4 frag_color;
precision highp float;

out vec4 FragColor;

void main()
{
	FragColor = frag_color;
}
