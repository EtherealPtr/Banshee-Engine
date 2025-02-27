#include "LightSystem.h"
#include "Foundation/Entity/Entity.h"
#include "Graphics/Components/Light/LightComponent.h"

namespace Banshee
{
	void LightSystem::ProcessComponents(const Entity* const _entity)
	{
        if (auto lightComponent{ _entity->GetComponent<LightComponent>() })
        {
            AddLightComponent(lightComponent);
        }
	}
} // End of namespace