#pragma once

#include "VulkanRenderPass.h"

namespace Banshee
{
	class VulkanShadowRenderPass : public VulkanRenderPass
	{
	public:
		VulkanShadowRenderPass(const VkDevice& _device, const VkFormat _depthFormat);

	protected:
		virtual void CreateRenderPass() override;

	private:
		VkFormat m_DepthFormat;
	};
} // End of namespace