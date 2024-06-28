#version 450

layout (location = 0) in vec3 in_vertex_position; 
layout (location = 1) in vec2 in_vertex_texCoord;
layout (location = 2) in vec3 in_vertex_normal;

layout (location = 0) out vec3 out_vertex_color;
layout (location = 1) out vec2 out_vertex_texCoord;
layout (location = 3) out int out_texture_index;
layout (location = 4) out int out_texture_available;

layout (set = 0, binding = 0) uniform ViewProjBuffer
{
	mat4 view;
	mat4 proj;
} u_ViewProj;

layout (set = 0, binding = 1) uniform Material
{
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
} u_Material;

layout (push_constant) uniform PushConstants
{
	mat4 model;
	int textureId;
	int hasCustomTexture;
} u_PushConstants;

void main()
{
	gl_Position = u_ViewProj.proj * u_ViewProj.view * u_PushConstants.model * vec4(in_vertex_position, 1.0f);
	
	out_vertex_texCoord = in_vertex_texCoord;
	out_vertex_color = u_Material.diffuseColor;
	out_texture_index = u_PushConstants.textureId;
	out_texture_available = u_PushConstants.hasCustomTexture;
}