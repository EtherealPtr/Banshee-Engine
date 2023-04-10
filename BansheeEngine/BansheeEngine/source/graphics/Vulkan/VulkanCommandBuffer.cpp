#include "VulkanCommandBuffer.h"
#include "foundation/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanCommandBuffer::VulkanCommandBuffer(const VkDevice& _logicalDevice, const VkCommandPool& _pool) : 
		m_LogicalDevice(_logicalDevice),
		m_CommandPool(_pool),
		m_CommandBuffer(VK_NULL_HANDLE)
	{
		VkCommandBufferAllocateInfo commandBufferAllocInfo{};
		commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocInfo.commandPool = _pool;
		commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(_logicalDevice, &commandBufferAllocInfo, &m_CommandBuffer) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::error, "Failed to allocate a command buffer");
			throw std::runtime_error("ERROR: Failed to allocate a command buffer");
		}

		BE_LOG(LogCategory::info, "Vulkan command buffer allocated successfully");
	}
	
	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		vkFreeCommandBuffers(m_LogicalDevice, m_CommandPool, 1, &m_CommandBuffer);
		m_CommandBuffer = VK_NULL_HANDLE;
		BE_LOG(LogCategory::trace, "Vulkan command buffer freed")
	}
	
	void VulkanCommandBuffer::Begin() const
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);
	}
	
	void VulkanCommandBuffer::End() const
	{
		vkEndCommandBuffer(m_CommandBuffer);
	}
	
	void VulkanCommandBuffer::Submit(const VkQueue& _queue, const VkSemaphore& _waitSem, const VkSemaphore& _signalSem, const VkFence& _fence, const uint32 _waitStage)
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = _waitSem ? 1 : 0;
		submitInfo.pWaitSemaphores = &_waitSem;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;
		submitInfo.signalSemaphoreCount = _signalSem ? 1 : 0;
		submitInfo.pSignalSemaphores = &_signalSem;
		submitInfo.pWaitDstStageMask = &_waitStage;

		if (vkQueueSubmit(_queue, 1, &submitInfo, _fence) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::warning, "Failed to submit command buffer to queue");
		}
	}
} // End of Banshee namespace
