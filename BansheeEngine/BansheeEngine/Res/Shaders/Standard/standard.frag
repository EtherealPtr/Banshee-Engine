#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 0) in vec2 in_vertex_texCoord;
layout (location = 1) in vec3 in_vertex_normal;
layout (location = 2) flat in int in_texture_index;
layout (location = 3) in vec3 in_camera_position;
layout (location = 4) in vec3 in_fragment_position;

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

	// Ambient 
	const float ambientStrength = 0.1f;
    const vec3 ambient = ambientStrength * u_Light.color;

	// Diffuse
	const vec3 norm = normalize(in_vertex_normal);
	const vec3 lightDir = normalize(-u_Light.position);
	const float diffuseImpact = max(dot(norm, lightDir), 0.0);
	const vec3 diffuse = diffuseImpact * u_Light.color;

	// Specular
	const float specularStrength = 0.5f;
	const vec3 viewDir = normalize(in_camera_position - in_fragment_position);
	const vec3 reflectDir = reflect(-lightDir, norm);
	const float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	const vec3 specular = specularStrength * spec * u_Light.color; 

	// Combine lighting effect
	const vec3 lighting = ambient + diffuse;

	out_frag_color = baseColor * vec4(lighting, 1.0f);
}