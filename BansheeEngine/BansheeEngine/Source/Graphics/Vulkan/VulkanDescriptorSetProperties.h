#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkBuffer_T* VkBuffer;
typedef struct VkImageView_T* VkImageView;
typedef struct VkSampler_T* VkSampler;
typedef enum VkDescriptorType VkDescriptorType;

namespace Banshee
{
	struct DescriptorSetWriteBufferProperties
	{
		DescriptorSetWriteBufferProperties(const uint32 _binding, const VkDescriptorType _descType, const VkBuffer& _buffer, const uint64 _bufferRance) noexcept :
			m_Binding(_binding),
			m_DescriptorType(_descType),
			m_Buffer(_buffer),
			m_BufferRange(_bufferRance)
		{}

		uint32 m_Binding;
		VkDescriptorType m_DescriptorType;
		VkBuffer m_Buffer;
		uint64 m_BufferRange;
	};

	struct DescriptorSetWriteTextureProperties
	{
		DescriptorSetWriteTextureProperties(const uint32 _binding, const VkDescriptorType _descType, const std::vector<VkImageView>& _imageViews, const VkSampler& _sampler) :
			m_Binding(_binding),
			m_DescriptorType(_descType),
			m_ImageViews(_imageViews),
			m_Sampler(_sampler)
		{}

		uint32 m_Binding;
		VkDescriptorType m_DescriptorType;
		std::vector<VkImageView> m_ImageViews;
		VkSampler m_Sampler;
	};
} // End of Banshee namespace