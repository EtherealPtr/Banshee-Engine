#pragma once

#include "Foundation/Platform.h"
#include <memory>
#include <string>

namespace Banshee
{
	class FileManager;
	class ImageManager;

	class ResourceManager
	{
	public:
		static ResourceManager& Instance();

		uint32 LoadImageResource(const char* _pathToImage);
		const std::shared_ptr<FileManager>& GetFileManager() const noexcept { return m_FileManager; }
		const std::shared_ptr<ImageManager>& GetImageManager() const noexcept { return m_ImageManager; }
		const char* GetEngineResDirPath() noexcept;
		std::string GetAssetName(const std::string& _assetName);

	private:
		ResourceManager();
		~ResourceManager() = default;

	private:
		std::shared_ptr<FileManager> m_FileManager;
		std::shared_ptr<ImageManager> m_ImageManager;
	};
} // End of Banshee namespace