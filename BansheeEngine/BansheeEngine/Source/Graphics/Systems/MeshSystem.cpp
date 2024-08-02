#include "MeshSystem.h"
#include "Graphics/Components/MeshComponent.h"

namespace Banshee
{
	void MeshSystem::SetMeshComponents(const std::vector<std::shared_ptr<MeshComponent>>& _meshComponents)
	{
		m_MeshRenderers = _meshComponents;
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