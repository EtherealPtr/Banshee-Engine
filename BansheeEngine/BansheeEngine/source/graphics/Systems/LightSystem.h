#pragma once

#include <memory>
#include <vector>
#include "Graphics/Components/Light/LightComponent.h"

namespace Banshee
{
    class LightSystem
    {
    public:
        static LightSystem& Instance() noexcept
        {
            static LightSystem instance;
            return instance;
        }

        void SetLightComponents(const std::vector<std::shared_ptr<LightComponent>>& _lightComponents) { m_LightComponents = _lightComponents; }
        const std::vector<std::shared_ptr<LightComponent>>& GetLightComponents() const noexcept { return m_LightComponents; }

        LightSystem(const LightSystem&) = delete;
        LightSystem(LightSystem&&) = delete;
        LightSystem& operator=(const LightSystem&) = delete;
        LightSystem& operator=(LightSystem&&) = delete;

    protected:
        LightSystem() noexcept = default;
        ~LightSystem() noexcept = default;

    private:
        std::vector<std::shared_ptr<LightComponent>> m_LightComponents;
    };
} // End of Banshee namespace