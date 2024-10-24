#include "LightSystem.h"
#include "Foundation/Entity/Entity.h"
#include "Graphics/Components/Light/DirectionalLightComponent.h"
#include "Graphics/Components/Light/PointLightComponent.h"

namespace Banshee
{
	void LightSystem::ProcessComponents(const Entity* const _entity)
	{
		if (const auto& directionalLightComponent{ _entity->GetComponent<DirectionalLightComponent>() })
		{
			AddLightComponent(directionalLightComponent);
		}
		else if (const auto& pointLightComponent{ _entity->GetComponent<PointLightComponent>() })
		{
			AddLightComponent(pointLightComponent);
		}
	}
} // End of Banshee namespace