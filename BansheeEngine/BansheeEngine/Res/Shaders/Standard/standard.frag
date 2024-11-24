#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 0) in vec2 in_vertex_texCoord;
layout (location = 1) in vec3 in_vertex_normal;
layout (location = 2) flat in int in_texture_index;
layout (location = 3) in vec3 in_camera_position;
layout (location = 4) in vec3 in_fragment_position;

layout (location = 0) out vec4 out_frag_color;

layout (set = 0, binding = 2) uniform texture2D textures[];
layout (set = 0, binding = 3) uniform sampler texture_sampler;

layout (set = 0, binding = 1) uniform Material 
{
    vec4 diffuseColor;
    vec4 specularColor;    // w: shininess
} u_Material;

struct LightData 
{
    vec4 positionAndType;  // xyz: position, w: type
    vec4 direction;        // xyz: direction
    vec4 color;            // xyz: color, w: color intensity
    vec4 attenuation;      // x: constant, y: linear, z: quadratic/inner cutoff, w: outer cutoff
};

layout(set = 0, binding = 4) 
readonly buffer LightBuffer 
{ 
    int totalLights; 
    LightData lights[25];
};

vec3 CalculateDirectionalLight(LightData _light, vec3 _normal, vec3 _viewDir) 
{
    const float specularIntensity = 0.1f;
    const float lightIntensity = _light.color.w;

    vec3 lightDir = normalize(-_light.direction.xyz);
    float diffImpact = max(dot(_normal, lightDir), 0.0f);
    vec3 diffuse = diffImpact * _light.color.xyz * lightIntensity;

    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0f), u_Material.specularColor.w);
    vec3 specular = specularIntensity * spec * u_Material.specularColor.xyz * lightIntensity;

    return diffuse + specular;
}

vec3 CalculatePointLight(LightData _light, vec3 _normal, vec3 _viewDir) 
{
    const float specularIntensity = 0.1f;
    const float lightIntensity = _light.color.w;

    vec3 lightDir = normalize(_light.positionAndType.xyz - in_fragment_position);
    float diffuseImpact = max(dot(_normal, lightDir), 0.0f);
    vec3 diffuse = diffuseImpact * (_light.color.xyz * lightIntensity);

    float distance = length(_light.positionAndType.xyz - in_fragment_position);
    float attenuation = 1.0f / (_light.attenuation.x + _light.attenuation.y * distance + _light.attenuation.z * (distance * distance));
    diffuse *= attenuation;

    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0f), u_Material.specularColor.w);
    vec3 specular = specularIntensity * spec * (u_Material.specularColor.xyz * lightIntensity) * attenuation;

    return diffuse + specular;
}

vec3 CalculateSpotLight(LightData _light, vec3 _normal, vec3 _viewDir) 
{
    const float specularIntensity = 0.1f;
    const float lightIntensity = _light.color.w;
    float innerCutoff = _light.attenuation.z; 
    float outerCutoff = _light.attenuation.w; 

    vec3 lightDir = normalize(_light.positionAndType.xyz - in_fragment_position);
    float diffuseImpact = max(dot(_normal, lightDir), 0.0f);
    vec3 diffuse = _light.color.xyz * diffuseImpact * lightIntensity; 

    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(_viewDir, reflectDir), 0.0f), u_Material.specularColor.w);
    vec3 specular = specularIntensity * spec * u_Material.specularColor.xyz * lightIntensity; 

    float theta = dot(lightDir, normalize(-_light.direction.xyz));
    float epsilon = innerCutoff - outerCutoff;
    float intensity = clamp((theta - outerCutoff) / epsilon, 0.0f, 1.0f);
    diffuse *= intensity;
    specular *= intensity;

    float distance = length(_light.positionAndType.xyz - in_fragment_position);
    float attenuation = 1.0f / (_light.attenuation.x + _light.attenuation.y * distance + _light.attenuation.z * (distance * distance));
    diffuse *= attenuation;
    specular *= attenuation;

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

    const float ambientStrength = 0.01f;
    vec3 ambient = ambientStrength * baseColor.xyz;

    for (int i = 0; i < totalLights; ++i) 
    {        
        LightData light = lights[i];

        if (light.positionAndType.w == 0.0f) // Directional light
        { 
            lighting += ambient + CalculateDirectionalLight(light, normal, viewDir);
        } 
        else if (light.positionAndType.w == 1.0f) // Point light
        { 
            lighting += ambient + CalculatePointLight(light, normal, viewDir);
        }
        else if (light.positionAndType.w == 2.0f) // Spotlight
        {
            lighting += ambient + CalculateSpotLight(light, normal, viewDir);
        }
    }

    vec3 resultColor = baseColor.xyz * lighting;
    out_frag_color = vec4(resultColor, 1.0f);
}
