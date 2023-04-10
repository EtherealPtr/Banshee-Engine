#pragma once

#include "foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkCommandPool_T* VkCommandPool;
typedef struct VkCommandBuffer_T* VkCommandBuffer;
typedef struct VkQueue_T* VkQueue;
typedef struct VkSemaphore_T* VkSemaphore;
typedef struct VkFence_T* VkFence;

namespace Banshee
{
	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(const VkDevice& _logicalDevice, const VkCommandPool& _pool);
		~VulkanCommandBuffer();

		void Begin() const;
		void End() const;
		void Submit(const VkQueue& _queue, const VkSemaphore& _waitSem = nullptr, const VkSemaphore& _signalSem = nullptr, const VkFence& _fence = nullptr, const uint32 _waitStage = 0);
		VkCommandBuffer Get() const { return m_CommandBuffer; }

	private:
		VkDevice m_LogicalDevice;
		VkCommandPool m_CommandPool;
		VkCommandBuffer m_CommandBuffer;
	};
} // End of Banshee namespace
