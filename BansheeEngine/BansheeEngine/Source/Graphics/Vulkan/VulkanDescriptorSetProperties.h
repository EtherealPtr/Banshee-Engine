#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkBuffer_T* VkBuffer;
typedef struct VkImageView_T* VkImageView;
typedef struct VkSampler_T* VkSampler;
typedef enum VkDescriptorType VkDescriptorType;

struct DescriptorSetWriteBufferProperties
{
	DescriptorSetWriteBufferProperties(const uint32 _binding, const VkDescriptorType _descType, const VkBuffer& _buffer, const uint64 _bufferRance) noexcept :
		binding(_binding),
		descriptorType(_descType),
		buffer(_buffer),
		bufferRange(_bufferRance)
	{}

	uint32 binding;
	VkDescriptorType descriptorType;
	VkBuffer buffer;
	uint64 bufferRange;
};

struct DescriptorSetWriteTextureProperties
{
	DescriptorSetWriteTextureProperties(const uint32 _binding, const VkDescriptorType _descType, const std::vector<VkImageView>& _imageViews, const VkSampler& _sampler) :
		binding(_binding),
		descriptorType(_descType),
		imageViews(_imageViews),
		sampler(_sampler)
	{}

	uint32 binding;
	VkDescriptorType descriptorType;
	std::vector<VkImageView> imageViews;
	VkSampler sampler;
};