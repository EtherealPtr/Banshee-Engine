#pragma once

#include "foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkRenderPass_T* VkRenderPass;
typedef struct VkImageView_T* VkImageView;
typedef struct VkFramebuffer_T* VkFramebuffer;

namespace Banshee
{
	class VulkanFramebuffer
	{
	public:
		VulkanFramebuffer() = default;
		VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const VkImageView& _imageView, const uint32 _w, const uint32 _h);
		~VulkanFramebuffer();

		VkFramebuffer Get() const { return m_Framebuffer; }

	private:
		VkFramebuffer m_Framebuffer;
		VkDevice m_Device;
	};
} // End of Banshee namespace
