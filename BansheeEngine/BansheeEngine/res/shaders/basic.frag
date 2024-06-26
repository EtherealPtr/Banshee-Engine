#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 0) in vec3 in_vertex_color;
layout (location = 1) in vec2 in_vertex_texCoord;
layout (location = 2) in vec3 in_vertex_normal;
layout (location = 3) flat in int in_texture_index;
layout (location = 4) flat in int in_texture_available;
layout (location = 5) in vec3 in_fragment_position;

layout (location = 0) out vec4 out_frag_color;

layout (binding = 2) uniform texture2D textures[];
layout (binding = 3) uniform sampler texture_sampler;

layout (set = 0, binding = 4) uniform LightUBO
{
	vec3 lightLocation;
	vec3 lightColor;
} u_Light;

void main()
{
	const float ambientStrength = 0.1f;
	const vec3 lightColor = u_Light.lightColor;
    const vec3 ambient = ambientStrength * lightColor;

	if (in_texture_available == 1)
	{
		vec4 texColor = texture(sampler2D(textures[in_texture_index], texture_sampler), in_vertex_texCoord);
		out_frag_color = texColor * vec4(in_vertex_color, 1.0);
	}
    else
	{
		out_frag_color = vec4(in_vertex_color, 1.0f);
	}

	vec3 lightLocation = normalize(u_Light.lightLocation);
	vec3 norm = normalize(in_vertex_normal);
	vec3 lightDir = normalize(lightLocation - in_fragment_position);

	float diffuseImpact = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseImpact * u_Light.lightColor;
	out_frag_color *= vec4(ambient + diffuse, 1.0f);
}