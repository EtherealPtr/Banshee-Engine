#pragma once

#include "LightData.h"

namespace Banshee
{
    class Light
    {
    public:
        Light(const glm::vec3& _location, const glm::vec3& _color = glm::vec3(1.0f)) noexcept :
            m_LightData(_location, _color)
        {}

        const glm::vec3& GetLocation() const noexcept { return m_LightData.location; }
        const glm::vec3& GetColor() const noexcept { return m_LightData.color; }

    private:
        LightData m_LightData;
    };
} // End of Banshee namespace
