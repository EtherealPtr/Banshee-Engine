#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkRenderPass_T* VkRenderPass;
typedef enum VkFormat VkFormat;

namespace Banshee
{
	class VulkanRenderPass
	{
	public:
		VulkanRenderPass(const VkDevice& _device, const VkFormat _colorFormat, const VkFormat _depthFormat);
		VulkanRenderPass(const VkDevice& _device, const VkFormat _depthFormat);
		~VulkanRenderPass();

		const VkRenderPass& Get() const noexcept { return m_RenderPass; }

		VulkanRenderPass(const VulkanRenderPass&) = delete;
		VulkanRenderPass& operator=(const VulkanRenderPass&) = delete;
		VulkanRenderPass(VulkanRenderPass&&) = delete;
		VulkanRenderPass& operator=(VulkanRenderPass&&) = delete;

	private:
		VkDevice m_Device;
		VkRenderPass m_RenderPass;
	};
} // End of namespace