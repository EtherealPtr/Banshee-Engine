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
		VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const VkImageView& _depthImageView, const uint32 _w, const uint32 _h);
		~VulkanFramebuffer();

		void RecreateFramebuffers(const uint32 _w, const uint32 _h, const std::vector<VkImageView>& _imageViews, const VkImageView& _depthImageView);
		void CreateFramebuffers(const uint32 _w, const uint32 _h, const std::vector<VkImageView>& _imageViews, const VkImageView& _depthImageView);
		void CreateFramebuffer(const uint32 _w, const uint32 _h, const VkImageView& _depthImageView);
		const std::vector<VkFramebuffer>& Get() const noexcept { return m_Framebuffers; }
		const VkFramebuffer& GetDepthFramebuffer() const noexcept { return m_DepthFramebuffer; }

		VulkanFramebuffer(const VulkanFramebuffer&) = delete;
		VulkanFramebuffer& operator=(const VulkanFramebuffer&) = delete;
		VulkanFramebuffer(VulkanFramebuffer&&) = delete;
		VulkanFramebuffer& operator=(VulkanFramebuffer&&) = delete;

	private:
		void CleanUp();

	private:
		VkDevice m_Device;
		VkRenderPass m_RenderPass;
		std::vector<VkFramebuffer> m_Framebuffers;
		VkFramebuffer m_DepthFramebuffer;
	};
} // End of Banshee namespace