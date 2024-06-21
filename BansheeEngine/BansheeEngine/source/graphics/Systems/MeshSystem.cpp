#include "MeshSystem.h"
#include "Graphics/Components/MeshComponent.h"
#include "Foundation/Logging/Logger.h"
#include <algorithm>

namespace Banshee
{
	MeshSystem& Banshee::MeshSystem::Instance() noexcept
	{
		static MeshSystem instance;
		return instance;
	}

	void MeshSystem::AddMeshComponent(const std::shared_ptr<MeshComponent>& _component)
	{
		_component->RegisterSystem(this);
		m_MeshRenderers.emplace_back(_component);
	}

	std::shared_ptr<MeshComponent> MeshSystem::GetMeshComponentById(const uint32 _meshId) const noexcept
	{
		for (const auto& meshComponent : m_MeshRenderers)
		{
			if (meshComponent->GetMeshId() == _meshId)
			{
				return meshComponent;
			}
		}
		return nullptr;
	}
} // End of Banshee namespace