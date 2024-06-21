#pragma once

#include "Foundation/Systems/System.h"
#include "Foundation/Platform.h"
#include <memory>
#include <vector>

namespace Banshee
{
	class MeshComponent;

	class MeshSystem : public System
	{
	public:
		static MeshSystem& Instance() noexcept;
		void AddMeshComponent(const std::shared_ptr<MeshComponent>& _component);
		const std::vector<std::shared_ptr<MeshComponent>>& GetMeshComponents() const noexcept { return m_MeshRenderers; }
		std::shared_ptr<MeshComponent> GetMeshComponentById(const uint32 _meshId) const noexcept;
		virtual void Update() noexcept override {};

	protected:
		MeshSystem() noexcept = default;
		~MeshSystem() noexcept = default;

		MeshSystem(const MeshSystem&) = delete;
		MeshSystem(MeshSystem&&) = delete;
		void operator=(const MeshSystem&) = delete;
		void operator=(MeshSystem&&) = delete;

	private:
		std::vector<std::shared_ptr<MeshComponent>> m_MeshRenderers;
	};
} // End of Banshee namespace