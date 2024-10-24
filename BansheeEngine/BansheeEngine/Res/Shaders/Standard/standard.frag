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
    vec4 diffuseColor;
    vec4 specularColor; // w component contains shininess
} u_Material;

struct LightData 
{
    vec4 typeAndPosition;  // x: type, yzw: position
    vec4 direction;        // xyz: direction
    vec4 color;            // xyz: color
    vec4 attenuation;      // x: constant, y: linear, z: quadratic
};

layout(set = 0, binding = 4) readonly buffer LightBuffer 
{
    LightData lights[];
};

vec3 CalculateDirectionalLight(LightData _light, vec3 _normal, vec3 _viewDir) 
{
    vec3 lightDir = normalize(-_light.direction.xyz);
    float diff = max(dot(_normal, lightDir), 0.0f);
    vec3 diffuse = diff * _light.color.xyz;

    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0f), u_Material.specularColor.w);
    vec3 specular = 0.5f * spec * u_Material.specularColor.xyz;

    return diffuse + specular;
}

vec3 CalculatePointLight(LightData _light, vec3 _normal, vec3 _viewDir) 
{
    vec3 lightDir = normalize(_light.typeAndPosition.yzw - in_fragment_position);
    float diff = max(dot(_normal, lightDir), 0.0f);

    float distance = length(_light.typeAndPosition.yzw - in_fragment_position);
    float attenuation = 1.0f / (_light.attenuation.x + _light.attenuation.y * distance + _light.attenuation.z * (distance * distance));

    vec3 diffuse = diff * _light.color.xyz;
    diffuse *= attenuation;

    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0f),  u_Material.specularColor.w);
    vec3 specular = 0.5f * spec * u_Material.specularColor.xyz;

    return diffuse + specular;
}

void main() 
{
    vec4 baseColor = u_Material.diffuseColor;

    if (in_texture_index > 0) 
    {
        vec4 texColor = texture(sampler2D(textures[in_texture_index], texture_sampler), in_vertex_texCoord);
        baseColor = texColor * u_Material.diffuseColor;
    }

    vec3 normal = normalize(in_vertex_normal);
    vec3 viewDir = normalize(in_camera_position - in_fragment_position);
    vec3 lighting = vec3(0.0f);

    const float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * vec3(1.0f);

    for (int i = 0; i < 2; ++i) 
    {
        LightData light = lights[i];

        if (light.typeAndPosition.x == 0.0f) // Directional light
        { 
            lighting += CalculateDirectionalLight(light, normal, viewDir);
        } 
        else if (light.typeAndPosition.x == 1.0f) // Point light
        { 
            lighting += CalculatePointLight(light, normal, viewDir);
        }
    }

    lighting += ambient;
    out_frag_color = baseColor * vec4(lighting, 1.0f);
}
