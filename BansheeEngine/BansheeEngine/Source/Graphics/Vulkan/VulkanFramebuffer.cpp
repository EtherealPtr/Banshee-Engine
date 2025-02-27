#include "VulkanFramebuffer.h"
#include "Foundation/Logging/Logger.h"
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace Banshee
{
    VulkanFramebuffer::VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const std::vector<VkImageView>& _attachments, uint32 _w, uint32 _h) :
        m_Device{ _logicalDevice },
        m_RenderPass{ _renderPass },
        m_Framebuffer{ VK_NULL_HANDLE }
    {
        CreateFramebuffer(_w, _h, _attachments);
    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {
        CleanUp();
    }

    void VulkanFramebuffer::RecreateFramebuffer(uint32 _w, uint32 _h, const std::vector<VkImageView>& _attachments)
    {
        CleanUp();
        CreateFramebuffer(_w, _h, _attachments);
    }

    void VulkanFramebuffer::CreateFramebuffer(uint32 _w, uint32 _h, const std::vector<VkImageView>& _attachments)
    {
        BE_LOG(LogCategory::Trace, "[FRAMEBUFFER]: Creating framebuffer");

        VkFramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = m_RenderPass;
        framebufferCreateInfo.attachmentCount = static_cast<uint32>(_attachments.size());
        framebufferCreateInfo.pAttachments = _attachments.data();
        framebufferCreateInfo.width = _w;
        framebufferCreateInfo.height = _h;
        framebufferCreateInfo.layers = 1;

        if (vkCreateFramebuffer(m_Device, &framebufferCreateInfo, nullptr, &m_Framebuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR: Failed to create framebuffer");
        }

        BE_LOG(LogCategory::Info, "[FRAMEBUFFER]: Created framebuffer");
    }

    void VulkanFramebuffer::CleanUp()
    {
        if (m_Framebuffer != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(m_Device, m_Framebuffer, nullptr);
            m_Framebuffer = VK_NULL_HANDLE;
        }
    }
} // End of namespace