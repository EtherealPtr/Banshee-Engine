#pragma once

#include "Foundation/Platform.h"
#include <vector>
#include <memory>

namespace Banshee
{
	struct Image;

	class ImageManager
	{
	public:
		ImageManager();
		~ImageManager();

		const std::vector<std::shared_ptr<Image>>& GetImages() const noexcept { return m_Images; }
		uint16 LoadImage(const char* _pathToImage);
		uint16 LoadImageFromMemory(const unsigned char* _bytes, const int _size);
		void UnloadImages() const;

	private:
		void CreateDefaultImage();

	private:
		std::vector<std::shared_ptr<Image>> m_Images;
	};
} // End of Banshee namespace