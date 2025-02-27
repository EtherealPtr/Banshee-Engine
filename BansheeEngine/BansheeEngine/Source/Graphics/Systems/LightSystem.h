#pragma once

#include <vector>
#include <optional>
#include "Graphics/Components/Light/LightComponent.h"
#include "Graphics/Components/Light/DirectionalLightComponent.h"

namespace Banshee
{
    class Entity;

    class LightSystem
    {
    public:
        LightSystem() noexcept = default;
        ~LightSystem() noexcept = default;

        void ProcessComponents(const Entity* const _entity);
        std::vector<LightComponent>& GetLightComponents() noexcept { return m_LightComponents; }
        const std::optional<DirectionalLightComponent>& GetDirectionalLight() const noexcept { return m_DirectionalLight; }

        LightSystem(const LightSystem&) = delete;
        LightSystem& operator=(const LightSystem&) = delete;
        LightSystem(LightSystem&&) = delete;
        LightSystem& operator=(LightSystem&&) = delete;

    private:
        void AddLightComponent(const LightComponent& _lightComponent);

    private:
        std::vector<LightComponent> m_LightComponents;
        std::optional<DirectionalLightComponent> m_DirectionalLight;
    };
} // End of namespace