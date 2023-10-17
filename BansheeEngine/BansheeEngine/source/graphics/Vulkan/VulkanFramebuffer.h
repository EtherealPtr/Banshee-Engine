#pragma once

#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkRenderPass_T* VkRenderPass;
typedef struct VkImageView_T* VkImageView;
typedef struct VkFramebuffer_T* VkFramebuffer;

namespace Banshee
{
	class VulkanFramebuffer
	{
	public:
		VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const std::vector<VkImageView>& _swapImageViews, const VkImageView& _depthImageView, const uint32_t _w, const uint32_t _h);
		~VulkanFramebuffer();

		std::vector<VkFramebuffer> Get() const { return m_Framebuffers; }

	private:
		VkDevice m_Device;
		std::vector<VkFramebuffer> m_Framebuffers;
	};
}
