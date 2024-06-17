#pragma once

#include "Foundation/Platform.h"
#include <glm/glm.hpp>

namespace Banshee
{
	class Material
	{
    public:
        Material(glm::vec3 _diffuse = glm::vec3(1.0f), glm::vec3 _specular = glm::vec3(1.0f), float _shininess = 1.0f) noexcept :
            m_DiffuseColor(_diffuse),
            m_SpecularColor(_specular),
            m_Shininess(_shininess)
        {}

        void SetDiffuseColor(const glm::vec3& _color) noexcept { m_DiffuseColor = _color; }
        void SetSpecularColor(const glm::vec3& _color) noexcept { m_SpecularColor = _color; }
        void SetShininess(const float _value) noexcept { m_Shininess = _value; }
        
        const glm::vec3& GetDiffuseColor() const noexcept { return m_DiffuseColor; }
        const glm::vec3& GetSpecularColor() const noexcept { return m_SpecularColor; }
        float GetShininess() const noexcept { return m_Shininess; }

    private:
        glm::vec3 m_DiffuseColor;
        glm::vec3 m_SpecularColor;
        float m_Shininess;
	};
} // End of Banshee namespace
