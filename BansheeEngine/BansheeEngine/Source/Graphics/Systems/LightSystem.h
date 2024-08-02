#pragma once

#include <memory>
#include <vector>

namespace Banshee
{
    class LightComponent;

    class LightSystem
    {
    public:
        LightSystem() noexcept = default;
        ~LightSystem() noexcept = default;

        void SetLightComponents(const std::vector<std::shared_ptr<LightComponent>>& _lightComponents) { m_LightComponents = _lightComponents; }
        const std::vector<std::shared_ptr<LightComponent>>& GetLightComponents() const noexcept { return m_LightComponents; }

        LightSystem(const LightSystem&) = delete;
        LightSystem(LightSystem&&) = delete;
        LightSystem& operator=(const LightSystem&) = delete;
        LightSystem& operator=(LightSystem&&) = delete;

    private:
        std::vector<std::shared_ptr<LightComponent>> m_LightComponents;
    };
} // End of Banshee namespace