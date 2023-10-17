#pragma once

#include <cstdint>

typedef struct VkDevice_T* VkDevice;
typedef struct VkRenderPass_T* VkRenderPass;

namespace Banshee
{
	class VulkanRenderPass
	{
	public:
		VulkanRenderPass(const VkDevice& _device, const uint32_t _colorFormat, const uint32_t _depthFormat);
		~VulkanRenderPass();

		VkRenderPass Get() const { return m_RenderPass; }

	private:
		VkDevice m_Device;
		VkRenderPass m_RenderPass;
	};
}
