#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkDevice_T* VkDevice;
typedef struct VkBuffer_T* VkBuffer;
typedef struct VkCommandPool_T* VkCommandPool;
typedef struct VkQueue_T* VkQueue;
typedef struct VkImage_T* VkImage;
typedef struct VkImageView_T* VkImageView;
typedef struct VkDeviceMemory_T* VkDeviceMemory;
typedef enum VkFormat VkFormat;

namespace Banshee
{
	struct VulkanImage
	{
		VulkanImage(const VkImage& _image, const VkImageView _imageView, const VkDeviceMemory& _imageMemory) :
			image(_image),
			imageView(_imageView),
			imageMemory(_imageMemory)
		{}

		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;
	};

	class VulkanTextureManager
	{
	public:
		VulkanTextureManager(const VkDevice& _device, const VkPhysicalDevice& _gpu, const VkQueue& _graphicsQueue, const VkCommandPool& _commandPool);
		~VulkanTextureManager();

		std::vector<VkImageView> GetTextureImageViews() const;
		void UploadTextures();

	private:
		void CreateStagingBuffer(const uint64 _sizeOfBuffer, const unsigned char* _pixels, const uint32 _imgW, const uint32 _imgH);
		void CreateTextureImage(const VkBuffer& _buffer, const uint32 _imgW, const uint32 _imgH);

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;
		VkQueue m_GraphicsQueue;
		VkCommandPool m_CommandPool;
		VkFormat m_TextureImageFormat;
		std::vector<VulkanImage> m_TextureImages;
	};
} // End of Banshee namespace