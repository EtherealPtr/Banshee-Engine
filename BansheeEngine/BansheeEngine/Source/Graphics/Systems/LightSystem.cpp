#include "LightSystem.h"
#include "Foundation/Entity/Entity.h"
#include "Graphics/Components/Light/DirectionalLightComponent.h"

namespace Banshee
{
	void LightSystem::ProcessComponents(const std::shared_ptr<Entity>& _entity)
	{
		if (const auto& lightComponent{ _entity->GetComponent<DirectionalLightComponent>() })
		{
			AddLightComponent(lightComponent);
		}
	}
} // End of Banshee namespace