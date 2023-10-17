#pragma once

#include <cstdint>

typedef struct VkBuffer_T* VkBuffer;
typedef enum VkDescriptorType VkDescriptorType;

struct DescriptorSetWriteProperties
{
	DescriptorSetWriteProperties(const uint32_t _binding, const VkDescriptorType _descType, const VkBuffer& _buffer, const uint64_t _bufferRance) :
		binding(_binding),
		descriptorType(_descType),
		buffer(_buffer),
		bufferRange(_bufferRance)
	{}

	uint32_t binding;
	VkDescriptorType descriptorType;
	VkBuffer buffer;
	uint64_t bufferRange;
};