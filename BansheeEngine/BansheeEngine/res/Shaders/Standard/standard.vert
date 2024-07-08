#version 450

layout (location = 0) in vec3 in_vertex_position; 
layout (location = 1) in vec2 in_vertex_texCoord;
layout (location = 2) in vec3 in_vertex_normal;

layout (location = 0) out vec2 out_vertex_texCoord;
layout (location = 1) out vec3 out_vertex_normal;
layout (location = 2) out int out_texture_index;
layout (location = 3) out int out_texture_available;
layout (location = 4) out vec3 out_fragment_position;
layout (location = 5) out vec3 out_fragment_normal;

layout (set = 0, binding = 0) uniform ViewProjBuffer
{
	mat4 view;
	mat4 proj;
} u_ViewProj;

layout (push_constant) uniform PushConstants
{
	mat4 model;
	int textureId;
	int hasCustomTexture;
} u_PushConstants;

void main()
{
	gl_Position = u_ViewProj.proj * u_ViewProj.view * u_PushConstants.model * vec4(in_vertex_position, 1.0f);
	out_fragment_position = vec3(u_ViewProj.view * u_PushConstants.model * vec4(in_vertex_position, 1.0f));
	out_fragment_normal = mat3(transpose(inverse(u_ViewProj.view * u_PushConstants.model))) * in_vertex_normal;
	
	out_vertex_texCoord = in_vertex_texCoord;
	out_vertex_normal = in_vertex_normal;
	out_texture_available = u_PushConstants.hasCustomTexture;
	out_texture_index = u_PushConstants.textureId;
}