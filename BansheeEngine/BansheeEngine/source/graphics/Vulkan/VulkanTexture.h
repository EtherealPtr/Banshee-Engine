#pragma once

#include "Foundation/Platform.h"

typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkDevice_T* VkDevice;
typedef struct VkBuffer_T* VkBuffer;
typedef struct VkImage_T* VkImage;
typedef struct VkImageView_T* VkImageView;
typedef struct VkCommandPool_T* VkCommandPool;
typedef struct VkQueue_T* VkQueue;
typedef struct VkDeviceMemory_T* VkDeviceMemory;
typedef enum VkFormat VkFormat;

namespace Banshee
{
	class VulkanTexture
	{
	public:
		VulkanTexture(const VkDevice& _device, const VkPhysicalDevice& _gpu, const VkQueue& _graphicsQueue, const VkCommandPool& _commandPool, const char* _texturePath);
		~VulkanTexture();

		VkImageView GetTextureImageView() const { return m_TextureImageView; }

	private:
		void CreateStagingBuffer(const uint64 _size, unsigned char* _pixels);
		void CreateTextureImage(const VkBuffer& _buffer);

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;
		VkQueue m_GraphicsQueue;
		VkCommandPool m_CommandPool;
		int32 m_TextureWidth;
		int32 m_TextureHeight;
		VkImage m_TextureImage;
		VkImageView m_TextureImageView;
		VkFormat m_TextureImageFormat;
		VkDeviceMemory m_TextureImageMemory;
	};
} // End of Banshee namespace