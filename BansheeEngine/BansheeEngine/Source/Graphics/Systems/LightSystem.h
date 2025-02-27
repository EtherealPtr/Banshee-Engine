#pragma once

#include <memory>
#include <vector>

namespace Banshee
{
    class Entity;
    class LightComponent;

    class LightSystem
    {
    public:
        LightSystem() noexcept = default;
        ~LightSystem() noexcept = default;

        void ProcessComponents(const Entity* const _entity);
        const std::vector<std::shared_ptr<LightComponent>>& GetLightComponents() const noexcept { return m_LightComponents; }

        LightSystem(const LightSystem&) = delete;
        LightSystem& operator=(const LightSystem&) = delete;
        LightSystem(LightSystem&&) = delete;
        LightSystem& operator=(LightSystem&&) = delete;

    private:
        void AddLightComponent(const std::shared_ptr<LightComponent>& _lightComponent) { m_LightComponents.emplace_back(_lightComponent); }

    private:
        std::vector<std::shared_ptr<LightComponent>> m_LightComponents;
    };
} // End of namespace