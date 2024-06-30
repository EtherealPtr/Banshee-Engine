#pragma once

#include "Foundation/Observer/Observer.h"
#include "Foundation/DLLConfig.h"
#include "LightData.h"
#include "Foundation/Components/Component.h"

namespace Banshee
{
    class LightComponent : public Component, public Observer
    {
    public:
        BANSHEE_ENGINE LightComponent(const glm::vec3& _color) noexcept :
            m_LightData{ glm::vec3(0.0f), _color },
            m_NeedsUpdate(true)
        {}

        void SetColor(const glm::vec3& _color) noexcept { m_LightData.color = _color; }
        const glm::vec3& GetColor() const noexcept { return m_LightData.color; }
        bool NeedsUpdate() const noexcept { return m_NeedsUpdate; }
        void ResetNeedsUpdate() noexcept { m_NeedsUpdate = false; }
        virtual void OnNotify() noexcept override;

    private:
        LightData m_LightData;
        bool m_NeedsUpdate;
    };
} // End of Banshee namespace
