#pragma once

#include "Foundation/Platform.h"
#include "Graphics/Vulkan/VulkanVertexBufferManager.h"
#include <memory>

typedef struct VkCommandPool_T* VkCommandPool;

namespace Banshee
{
	class VulkanDevice;
	class VulkanVertexBuffer;
	class MeshData;
	class Entity;

	class MeshSystem
	{
	public:
		MeshSystem(const VulkanDevice& _device, const VkCommandPool& _commandPool);
		~MeshSystem() noexcept = default;

		void ProcessComponents(const Entity* const _entity);
		const std::vector<MeshData>& GetAllSubMeshes();
		const std::vector<MeshData>& GetSubMeshes(const uint32 _bufferId) const;
		VulkanVertexBuffer* GetVertexBuffer(const uint32 _bufferId);

		MeshSystem(const MeshSystem&) = delete;
		MeshSystem& operator=(const MeshSystem&) = delete;
		MeshSystem(MeshSystem&&) = delete;
		MeshSystem& operator=(MeshSystem&&) = delete;

	private:
		void UpdateSubMeshCache();
		void AddMeshes(std::vector<MeshData>& _meshes);
		void AddMesh(MeshData& _mesh);

	private:
		VulkanVertexBufferManager m_VertexBufferManager;
		std::vector<MeshData> m_CachedSubMeshes;
		std::unordered_map<uint32, std::vector<MeshData>> m_VertexBufferIdToSubMeshes;
		bool m_IsCacheDirty;
		uint32 m_TotalMeshCount;
	};
} // End of namespace