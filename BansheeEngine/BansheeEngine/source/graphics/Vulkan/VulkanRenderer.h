#pragma once

#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanVertexBuffer.h"
#include "../MeshComponent.h"

namespace Banshee
{
	class Window;

	class VulkanRenderer
	{
	public:
		VulkanRenderer(const Window* _window);
		~VulkanRenderer();

		void DrawFrame();

	private:
		void CreateFramebuffers();
		void CreateCommandBuffers();
		void RecordRenderCommands(const uint8 _imgIndex);

	private:
		VulkanInstance m_VkInstance;
		VulkanSurface m_VkSurface;
		VulkanDevice m_VkDevice;
		VulkanSwapchain m_VkSwapchain;
		VulkanRenderPass m_VkRenderPass;
		MeshComponent m_Mesh;
		VulkanVertexBuffer m_VertexBuffer;
		VulkanGraphicsPipeline m_VkGraphicsPipeline;
		VulkanCommandPool m_VkCommandPool;
		VulkanSemaphore m_VkSemaphores;
		VulkanFence m_VkInFlightFences;
		std::vector<VulkanCommandBuffer> m_VkCommandBuffers;
		std::vector<VulkanFramebuffer> m_VkFramebuffers;
		uint8 m_CurrentFrameIndex;
	};
} // End of Banshee namespace
