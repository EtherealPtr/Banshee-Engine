#pragma once

#include "Foundation/Platform.h"
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
		VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const std::vector<VkImageView>& _swapImageViews, const VkImageView& _depthImageView, const uint32 _w, const uint32 _h);
		~VulkanFramebuffer();

		const std::vector<VkFramebuffer>& Get() const noexcept { return m_Framebuffers; }

	private:
		VkDevice m_Device;
		std::vector<VkFramebuffer> m_Framebuffers;
	};
} // End of Banshee namespace