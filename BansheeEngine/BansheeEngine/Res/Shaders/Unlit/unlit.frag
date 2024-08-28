#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 0) in vec2 in_vertex_texCoord;
layout (location = 1) flat in int in_texture_index;

layout (location = 0) out vec4 out_frag_color;

layout (binding = 2) uniform texture2D textures[];
layout (binding = 3) uniform sampler texture_sampler;

layout (set = 0, binding = 1) uniform Material
{
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
} u_Material;

layout (set = 0, binding = 4) uniform LightUBO
{
	vec3 position;
	float padding1;
	vec3 color;
	float padding2;
} u_Light;

void main()
{
 	vec4 baseColor = vec4(u_Material.diffuseColor, 1.0f);

	if (in_texture_index > 0)
	{
		vec4 texColor = texture(sampler2D(textures[in_texture_index], texture_sampler), in_vertex_texCoord);
		baseColor = texColor * vec4(u_Material.diffuseColor, 1.0);
	}

	out_frag_color = baseColor;
}