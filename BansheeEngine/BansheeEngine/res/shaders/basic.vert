#version 450

layout (location = 0) in vec3 in_vertex_position; 
layout (location = 1) in vec3 in_vertex_color; 
layout (location = 0) out vec3 out_vertex_color;

void main()
{
	gl_Position = vec4(in_vertex_position, 1.0f);
	out_vertex_color = in_vertex_color;
}