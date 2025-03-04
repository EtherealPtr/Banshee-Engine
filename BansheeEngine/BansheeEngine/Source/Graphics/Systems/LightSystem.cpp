#include "LightSystem.h"
#include "Foundation/Entity/Entity.h"
#include "Graphics/Components/Light/LightComponent.h"

namespace Banshee
{
	void LightSystem::ProcessComponents(const Entity* const _entity)
	{
        if (const auto lightComponent = _entity->GetComponent<LightComponent>())
        {
            AddLightComponent(*lightComponent);
        }
	}

    void LightSystem::AddLightComponent(const LightComponent& _lightComponent)
    {
        m_LightComponents.emplace_back(_lightComponent);

        if (!_lightComponent.IsDirectionalLight())
        {
            return;
        }

        if (!m_DirectionalLight.has_value())
        {
            m_DirectionalLight = static_cast<const DirectionalLightComponent&>(_lightComponent);
        }
    }
} // End of namespace