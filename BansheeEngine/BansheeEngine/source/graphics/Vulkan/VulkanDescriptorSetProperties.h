#pragma once

#include "Foundation/Platform.h"

typedef struct VkBuffer_T* VkBuffer;
typedef struct VkImageView_T* VkImageView;
typedef struct VkSampler_T* VkSampler;
typedef enum VkDescriptorType VkDescriptorType;

struct DescriptorSetWriteProperties
{
	DescriptorSetWriteProperties(const uint32 _binding, const VkDescriptorType _descType, const VkBuffer& _buffer, const uint64 _bufferRance, const VkImageView& _imageView = nullptr, const VkSampler& _sampler = nullptr) :
		binding(_binding),
		descriptorType(_descType),
		buffer(_buffer),
		bufferRange(_bufferRance),
		imageView(_imageView),
		sampler(_sampler)
	{}

	uint32 binding;
	VkDescriptorType descriptorType;
	VkBuffer buffer;
	uint64 bufferRange;
	VkImageView imageView;
	VkSampler sampler;
};