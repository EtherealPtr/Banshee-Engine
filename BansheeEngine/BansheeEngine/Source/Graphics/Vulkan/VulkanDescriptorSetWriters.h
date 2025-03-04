#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkBuffer_T* VkBuffer;
typedef struct VkImageView_T* VkImageView;
typedef struct VkSampler_T* VkSampler;
typedef enum VkDescriptorType VkDescriptorType;

namespace Banshee
{
	struct VulkanDescriptorSetBufferWriter
	{
		VulkanDescriptorSetBufferWriter() noexcept :
			m_Binding{ 0 },
			m_DescriptorType{},
			m_Buffer{},
			m_BufferRange{ 0 }
		{}

		void Initialize(const uint32 _binding, const VkDescriptorType _descType) noexcept
		{
			m_Binding = _binding;
			m_DescriptorType = _descType;
		}

		void SetBuffer(const VkBuffer& _buffer, const uint64 _bufferRange) noexcept
		{
			m_Buffer = _buffer;
			m_BufferRange = _bufferRange;
		}

		uint32 m_Binding;
		VkDescriptorType m_DescriptorType;
		VkBuffer m_Buffer;
		uint64 m_BufferRange;
	};

	struct VulkanDescriptorSetTextureWriter
	{
		VulkanDescriptorSetTextureWriter() noexcept :
			m_Binding{ 0 },
			m_DescriptorType{},
			m_ImageViews{},
			m_Sampler{}
		{}

		void Initialize(const uint32 _binding, const VkDescriptorType _descType) noexcept
		{
			m_Binding = _binding;
			m_DescriptorType = _descType;
		}

		void SetImageView(const std::vector<VkImageView>& _imageViews) noexcept
		{
			m_ImageViews = _imageViews;
		}

		void SetSampler(const VkSampler& _sampler) noexcept
		{
			m_Sampler = _sampler;
		}

		uint32 m_Binding;
		VkDescriptorType m_DescriptorType;
		std::vector<VkImageView> m_ImageViews;
		VkSampler m_Sampler;
	};
} // End of namespace