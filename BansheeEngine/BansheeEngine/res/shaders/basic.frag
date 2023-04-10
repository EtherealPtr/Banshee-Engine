#version 450

layout (location = 0) in vec3 out_vertex_color;
layout (location = 0) out vec4 out_frag_color;

void main()
{
	out_frag_color = vec4(out_vertex_color.rgb, 1.0f);
}