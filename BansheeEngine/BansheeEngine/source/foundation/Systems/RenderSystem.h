#pragma once

#include <vector>
#include <memory>

namespace Banshee
{
	class MeshComponent;

	class RenderSystem
	{
	public:
		static RenderSystem& Instance();
		void AddMeshComponent(const std::shared_ptr<MeshComponent>& _component);
		std::vector<std::shared_ptr<MeshComponent>> GetMeshComponents()
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
		std::vector<std::shared_ptr<MeshComponent>> m_MeshRenderers;
	};
} // End of Banshee namespace