#include "RenderSystem.h"
#include "../Components/MeshComponent.h"

namespace Banshee
{
	RenderSystem& Banshee::RenderSystem::Instance()
	{
		static RenderSystem s_RenderSystem;
		return s_RenderSystem;
	}

	void RenderSystem::AddMeshComponent(const std::shared_ptr<MeshComponent>& _component)
	{
		m_MeshRenderers.emplace_back(_component);
	}

} // End of Banshee namespace