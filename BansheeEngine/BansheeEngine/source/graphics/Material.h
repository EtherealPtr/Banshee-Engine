#pragma once

#include "Foundation/Platform.h"
#include <glm/glm.hpp>

namespace Banshee
{
	class Material
	{
    public:
        Material(glm::vec3 _diffuse = glm::vec3(1.0f), glm::vec3 _specular = glm::vec3(1.0f), float _shininess = 32.0f) noexcept :
            m_DiffuseColor(glm::vec4(_diffuse, 1.0f)),
            m_SpecularColor(glm::vec4(_specular, 1.0f)),
            m_Shininess(_shininess)
        {}

        void SetDiffuseColor(const glm::vec3& _color) noexcept { m_DiffuseColor = glm::vec4(_color, 1.0f); }
        void SetSpecularColor(const glm::vec3& _color) noexcept { m_SpecularColor = glm::vec4(_color, 1.0f); }
        void SetShininess(const float _value) noexcept { m_Shininess = _value; }
        
        const glm::vec4& GetDiffuseColor() const noexcept { return m_DiffuseColor; }
        const glm::vec4& GetSpecularColor() const noexcept { return m_SpecularColor; }
        float GetShininess() const noexcept { return m_Shininess; }

    private:
        glm::vec4 m_DiffuseColor;
        glm::vec4 m_SpecularColor;
        float m_Shininess;
	};
} // End of Banshee namespace
