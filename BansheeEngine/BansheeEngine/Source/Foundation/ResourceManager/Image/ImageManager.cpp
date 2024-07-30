#include "ImageManager.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Platform.h"
#include "Image.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Banshee
{
	ImageManager::ImageManager()
	{
		m_Images.reserve(1);
		CreateDefaultImage();
	}

	ImageManager::~ImageManager()
	{
		UnloadImages();
	}

	void ImageManager::CreateDefaultImage()
	{
		const std::shared_ptr<Image> image = std::make_shared<Image>();
		
		// Create a simple white texture
		unsigned char* whiteTexturePixels = (unsigned char*)stbi__malloc(4);
		whiteTexturePixels[0] = 255;
		whiteTexturePixels[1] = 255;
		whiteTexturePixels[2] = 255;
		whiteTexturePixels[3] = 255;
		image->m_Pixels = whiteTexturePixels;
		image->m_ImageWidth = 1;
		image->m_ImageHeight = 1;
		image->m_ImageSize = 4;

		image->m_ImageIndex = m_Images.size();
		m_Images.emplace_back(image);
	}

	uint16 ImageManager::LoadImage(const char* _pathToImage)
	{
		const std::shared_ptr<Image> image = std::make_shared<Image>();

		int32 textureChannels = 0;
		image->m_Pixels = stbi_load(_pathToImage, &image->m_ImageWidth, &image->m_ImageHeight, &textureChannels, STBI_rgb_alpha);
		image->m_ImageSize = image->m_ImageWidth * image->m_ImageHeight * 4;

		if (!image->m_Pixels)
		{
			BE_LOG(LogCategory::Error, "[RESOURCE]: Failed to load texture image %s", _pathToImage);
			throw std::runtime_error("ERROR: Failed to load texture image");
		}

		image->m_ImageIndex = m_Images.size();
		m_Images.emplace_back(image);
		BE_LOG(LogCategory::Trace, "[RESOURCE]: Loaded image %s", _pathToImage);

		return image->m_ImageIndex;
	}

	uint16 ImageManager::LoadImageFromMemory(const unsigned char* _bytes, const int32 _size)
	{
		const std::shared_ptr<Image> image = std::make_shared<Image>();

		int32 textureChannels = 0;
		image->m_Pixels = stbi_load_from_memory(_bytes, _size, &image->m_ImageWidth, &image->m_ImageHeight, &textureChannels, STBI_rgb_alpha);
		image->m_ImageSize = image->m_ImageWidth * image->m_ImageHeight * 4;

		if (!image->m_Pixels)
		{
			BE_LOG(LogCategory::Error, "[RESOURCE]: Failed to load texture image from memory");
			throw std::runtime_error("ERROR: Failed to load texture image from memory");
		}

		image->m_ImageIndex = m_Images.size();
		m_Images.emplace_back(image);
		BE_LOG(LogCategory::Trace, "[RESOURCE]: Loaded image from memory");

		return image->m_ImageIndex;
	}

	void ImageManager::UnloadImages() const
	{
		for (const auto& image : m_Images)
		{
			if (image->m_Pixels)
			{
				stbi_image_free(image->m_Pixels);
			}
		}

		BE_LOG(LogCategory::Trace, "[RESOURCE]: Unloaded all image resources");
	}
} // End of Banshee namespace