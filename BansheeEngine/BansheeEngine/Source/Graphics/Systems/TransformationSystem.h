#pragma once

#include "Foundation/Entity/Entity.h"
#include "Graphics/Components/Transform/TransformComponent.h" 
#include <unordered_map>
#include <memory>

namespace Banshee
{
	class TransformationSystem
	{
	public:
		void ProcessComponents(const Entity* const _entity);
		std::shared_ptr<TransformComponent> GetTransformComponent(const uint32 _entityId) const;
		glm::mat4 GetEntityModelMatrix(const uint32 _entityId) const;

	private:
		std::unordered_map<uint32, std::shared_ptr<TransformComponent>> m_EntityTransformMap;
	};
} // End of namespace