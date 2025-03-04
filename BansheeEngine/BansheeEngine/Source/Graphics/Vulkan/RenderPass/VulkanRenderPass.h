#pragma once

typedef struct VkDevice_T* VkDevice;
typedef struct VkRenderPass_T* VkRenderPass;
typedef enum VkFormat VkFormat;

namespace Banshee
{
	class VulkanRenderPass
	{
	public:
		VulkanRenderPass(const VkDevice& _device) noexcept;
		virtual ~VulkanRenderPass() noexcept { CleanUp(); }

		VulkanRenderPass(const VulkanRenderPass&) = delete;
		VulkanRenderPass& operator=(const VulkanRenderPass&) = delete;
		VulkanRenderPass(VulkanRenderPass&&) = delete;
		VulkanRenderPass& operator=(VulkanRenderPass&&) = delete;

		void CleanUp() noexcept;
		const VkRenderPass& Get() const noexcept { return m_RenderPass; }

	protected:
		virtual void CreateRenderPass() = 0;

	protected:
		VkDevice m_Device;
		VkRenderPass m_RenderPass;
	};
} // End of namespace