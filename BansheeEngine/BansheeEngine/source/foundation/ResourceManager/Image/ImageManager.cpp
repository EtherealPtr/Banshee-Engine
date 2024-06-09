#include "ImageManager.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Platform.h"
#include "Image.h"
#include <stb_image.h>
#include <stdexcept>

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
		image->pixels = whiteTexturePixels;
		image->imageWidth = 1;
		image->imageHeight = 1;
		image->imageSize = 4;

		image->imageIndex = static_cast<uint16>(m_Images.size());
		m_Images.emplace_back(image);
	}

	uint16 ImageManager::LoadImage(const char* _pathToImage)
	{
		const std::shared_ptr<Image> image = std::make_shared<Image>();

		int32 textureChannels = 0;
		image->pixels = stbi_load(_pathToImage, &image->imageWidth, &image->imageHeight, &textureChannels, STBI_rgb_alpha);
		image->imageSize = image->imageWidth * image->imageHeight * 4;

		if (!image->pixels)
		{
			BE_LOG(LogCategory::Error, "[RESOURCE]: Failed to load texture image %s", _pathToImage);
			throw std::runtime_error("ERROR: Failed to load texture image");
		}

		image->imageIndex = static_cast<uint16>(m_Images.size());
		m_Images.emplace_back(image);
		BE_LOG(LogCategory::Trace, "[RESOURCE]: Loaded image %s", _pathToImage);

		return image->imageIndex;
	}

	void ImageManager::UnloadImages() const
	{
		for (const auto& image : m_Images)
		{
			if (image->pixels)
			{
				stbi_image_free(image->pixels);
			}
		}

		BE_LOG(LogCategory::Trace, "[RESOURCE]: Unloaded all image resources");
	}
} // End of Banshee namespace