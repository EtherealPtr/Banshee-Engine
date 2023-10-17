#include "VulkanCommandBuffer.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanCommandBuffer::VulkanCommandBuffer(const VkDevice& _logicalDevice, const VkCommandPool& _pool, const uint16_t _count) :
		m_LogicalDevice(_logicalDevice),
		m_CommandPool(_pool),
		m_CommandBuffers(_count, VK_NULL_HANDLE)
	{
		VkCommandBufferAllocateInfo commandBufferAllocInfo{};
		commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocInfo.commandPool = _pool;
		commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocInfo.commandBufferCount = _count;

		if (vkAllocateCommandBuffers(_logicalDevice, &commandBufferAllocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to allocate command buffers");
		}
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		vkFreeCommandBuffers(m_LogicalDevice, m_CommandPool, static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
	}

	void VulkanCommandBuffer::Begin(const uint16_t _bufferIndex) const
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		vkBeginCommandBuffer(m_CommandBuffers[_bufferIndex], &beginInfo);
	}

	void VulkanCommandBuffer::End(const uint16_t _bufferIndex) const
	{
		vkEndCommandBuffer(m_CommandBuffers[_bufferIndex]);
	}

	void VulkanCommandBuffer::Submit(const uint16_t _bufferIndex, const VkQueue& _queue, const VkSemaphore& _waitSem, const VkSemaphore& _signalSem, const VkFence& _fence, const uint32_t _waitStage)
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = _waitSem ? 1 : 0;
		submitInfo.pWaitSemaphores = &_waitSem;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[_bufferIndex];
		submitInfo.signalSemaphoreCount = _signalSem ? 1 : 0;
		submitInfo.pSignalSemaphores = &_signalSem;
		submitInfo.pWaitDstStageMask = &_waitStage;

		if (vkQueueSubmit(_queue, 1, &submitInfo, _fence) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to submit command buffer to queue");
		}
	}
}
