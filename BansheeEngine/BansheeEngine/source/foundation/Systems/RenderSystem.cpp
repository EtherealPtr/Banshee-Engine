#include "RenderSystem.h"
#include "../Components/MeshRendererComponent.h"

namespace Banshee
{
	RenderSystem& Banshee::RenderSystem::Instance()
	{
		static RenderSystem s_RenderSystem;
		return s_RenderSystem;
	}

	void RenderSystem::AddMeshRendererComponent(const std::shared_ptr<MeshRendererComponent>& _component)
	{
		m_MeshRenderers.emplace_back(_component);
	}

} // End of Banshee namespace