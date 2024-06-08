#include "ImageManager.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Platform.h"
#include "Image.h"
#include <stb_image.h>
#include <stdexcept>

namespace Banshee
{
	ImageManager::~ImageManager()
	{
		UnloadImages();
	}

	uint32 ImageManager::LoadImage(const char* _pathToImage)
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

		image->imageIndex = static_cast<uint32>(m_Images.size());
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