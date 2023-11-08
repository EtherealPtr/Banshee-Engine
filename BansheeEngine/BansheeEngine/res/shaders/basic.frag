#version 450

layout (location = 0) in vec3 in_vertex_color;
layout (location = 1) in vec2 in_vertex_texCoord;

layout (location = 0) out vec4 out_frag_color;

layout (binding = 2) uniform sampler2D texture_sampler;

void main()
{
    vec4 texColor = texture(texture_sampler, in_vertex_texCoord);
    out_frag_color = vec4(in_vertex_color, 1.0f) * texColor;
}