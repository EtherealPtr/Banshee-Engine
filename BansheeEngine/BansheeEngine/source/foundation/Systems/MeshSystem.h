#pragma once

#include "System.h"
#include <vector>
#include <memory>

namespace Banshee
{
	class MeshComponent;

	class MeshSystem : public System
	{
	public:
		static MeshSystem& Instance() noexcept;
		void AddMeshComponent(const std::shared_ptr<MeshComponent>& _component);
		const std::vector<std::shared_ptr<MeshComponent>>& GetMeshComponents() const noexcept { return m_MeshRenderers; }
		virtual void Update() override;

	protected:
		MeshSystem() noexcept = default;
		~MeshSystem() noexcept = default;

		MeshSystem(const MeshSystem&) = delete;
		MeshSystem(MeshSystem&&) = delete;
		void operator=(const MeshSystem&) = delete;
		void operator=(MeshSystem&&) = delete;

		void SortByLayer();

	private:
		std::vector<std::shared_ptr<MeshComponent>> m_MeshRenderers;
	};
} // End of Banshee namespace