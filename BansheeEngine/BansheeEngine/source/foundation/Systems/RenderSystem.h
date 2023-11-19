#pragma once

#include <vector>
#include <memory>

namespace Banshee
{
	class MeshComponent;

	class RenderSystem
	{
	public:
		static RenderSystem& Instance() noexcept;
		void AddMeshComponent(const std::shared_ptr<MeshComponent>& _component);
		const std::vector<std::shared_ptr<MeshComponent>>& GetMeshComponents() const noexcept { return m_MeshRenderers; }

	private:
		RenderSystem() noexcept = default;
		~RenderSystem() noexcept = default;

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem(RenderSystem&&) = delete;
		void operator=(const RenderSystem&) = delete;
		void operator=(RenderSystem&&) = delete;

	private:
		std::vector<std::shared_ptr<MeshComponent>> m_MeshRenderers;
	};
} // End of Banshee namespace