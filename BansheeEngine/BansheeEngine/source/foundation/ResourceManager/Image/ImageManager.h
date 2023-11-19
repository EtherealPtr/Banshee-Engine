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
		ImageManager() = default;
		~ImageManager();

		const std::vector<std::shared_ptr<Image>>& GetImages() const noexcept { return m_Images; }
		uint32 LoadImage(const char* _pathToImage);
		void UnloadImages() const;

	private:
		std::vector<std::shared_ptr<Image>> m_Images;
	};
} // End of Banshee namespace