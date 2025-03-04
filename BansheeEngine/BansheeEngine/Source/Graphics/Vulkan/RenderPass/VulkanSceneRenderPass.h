#pragma once

#include "VulkanRenderPass.h"

namespace Banshee
{
	class VulkanSceneRenderPass : public VulkanRenderPass
	{
	public:
		VulkanSceneRenderPass(const VkDevice& _device, const VkFormat _colorFormat, const VkFormat _depthFormat);

	protected:
		virtual void CreateRenderPass() override;

	private:
		VkFormat m_ColorFormat;
		VkFormat m_DepthFormat;
	};
} // End of namespace