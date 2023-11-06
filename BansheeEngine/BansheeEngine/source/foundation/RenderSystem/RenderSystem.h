#pragma once

#include <vector>
#include <memory>

namespace Banshee
{
	class MeshRendererComponent;

	class RenderSystem
	{
	public:
		static RenderSystem& Instance();
		void AddMeshRendererComponent(const std::shared_ptr<MeshRendererComponent>& _component);
		std::vector<std::shared_ptr<MeshRendererComponent>> GetMeshComponents()
		{
			return m_MeshRenderers;
		}

	private:
		RenderSystem() {};
		~RenderSystem() {};

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem(RenderSystem&&) = delete;
		void operator=(const RenderSystem&) = delete;
		void operator=(RenderSystem&&) = delete;

	private:
		std::vector<std::shared_ptr<MeshRendererComponent>> m_MeshRenderers;
	};
} // End of Banshee namespace