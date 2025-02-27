#pragma once

#include "Foundation/Platform.h"
#include "Foundation/ResourceManager/Image/Image.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>

namespace Banshee
{
	class ImageManager
	{
	public:
		ImageManager();
		~ImageManager();

		const std::vector<Image>& GetImages() const noexcept { return m_Images; }
		uint16 LoadImage(std::string_view _pathToImage) const;
		uint16 LoadImageFromMemory(const unsigned char* _bytes, const int32 _size) const;
		void UnloadImages() const;

		ImageManager(const ImageManager&) = delete;
		ImageManager(ImageManager&&) = delete;
		void operator=(const ImageManager&) = delete;
		void operator=(ImageManager&&) = delete;

	private:
		void CreateDefaultImage();

	private:
		std::vector<Image> m_Images;
		using OnImageLoaded = std::function<void(const Image&)>;
		OnImageLoaded m_OnImageLoaded;
	};
} // End of namespace