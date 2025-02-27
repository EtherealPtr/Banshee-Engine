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
		VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const std::vector<VkImageView>& _attachments, uint32 _w, uint32 _h);
		~VulkanFramebuffer();

		void RecreateFramebuffer(uint32 _w, uint32 _h, const std::vector<VkImageView>& _attachments);
		const VkFramebuffer& GetFramebuffer() const noexcept { return m_Framebuffer; }

		VulkanFramebuffer(const VulkanFramebuffer&) = delete;
		VulkanFramebuffer& operator=(const VulkanFramebuffer&) = delete;
		VulkanFramebuffer(VulkanFramebuffer&&) = delete;
		VulkanFramebuffer& operator=(VulkanFramebuffer&&) = delete;

	private:
		void CreateFramebuffer(uint32 _w, uint32 _h, const std::vector<VkImageView>& _attachments);
		void CleanUp();

	private:
		VkDevice m_Device;
		VkRenderPass m_RenderPass;
		VkFramebuffer m_Framebuffer;
	};
} // End of namespace