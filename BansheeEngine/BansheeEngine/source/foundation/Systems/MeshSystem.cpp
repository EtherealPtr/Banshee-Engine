#include "MeshSystem.h"
#include "Foundation/Components/MeshComponent.h"
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

	void MeshSystem::Update()
	{
		BE_LOG(LogCategory::Trace, "Updating mesh component");
		SortByLayer();
	}

	void MeshSystem::SortByLayer()
	{
		std::sort(m_MeshRenderers.begin(), m_MeshRenderers.end(), [](const std::shared_ptr<MeshComponent>& _c1, const std::shared_ptr<MeshComponent>& _c2)
		{
			return std::tie(_c1->GetSortLayer().layerId, _c1->GetSortLayer().priority) <
				   std::tie(_c2->GetSortLayer().layerId, _c2->GetSortLayer().priority);
		});
	}
} // End of Banshee namespace