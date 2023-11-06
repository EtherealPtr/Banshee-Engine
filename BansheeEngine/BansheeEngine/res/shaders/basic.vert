#version 450

layout (location = 0) in vec3 in_vertex_position; 
layout (location = 1) in vec3 in_vertex_color; 
layout (location = 2) in vec2 in_vertex_texCoord;

layout (location = 0) out vec3 out_vertex_color;
layout (location = 1) out vec2 out_vertex_texCoord;

void main()
{
	gl_Position = vec4(in_vertex_position, 1.0f);
	out_vertex_color = in_vertex_color;
	out_vertex_texCoord = in_vertex_texCoord;
}