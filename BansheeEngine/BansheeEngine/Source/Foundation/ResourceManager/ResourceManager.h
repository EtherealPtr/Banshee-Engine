#pragma once

#include "Foundation/Platform.h"
#include <memory>
#include <string>
#include <fstream>
#include <vector>

namespace Banshee
{
	class FileManager;
	class ImageManager;
	struct Image;

	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager() = default;

		uint16 LoadImageResource(std::string_view _pathToImage) const;
		uint16 LoadImageFromMemory(const unsigned char* _bytes, const int32 _size) const;
		std::string GetAssetName(std::string_view _assetName) const;
		std::ifstream ReadFile(std::string_view _filePath) const;
		std::vector<char> ReadBinaryFile(std::string_view _fileName) const;
		const std::vector<std::shared_ptr<Image>>& GetImages() const noexcept;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

	private:
		std::unique_ptr<FileManager> m_FileManager;
		std::unique_ptr<ImageManager> m_ImageManager;
	};

	extern const ResourceManager g_ResourceManager; // Defined in ResourceManager.cpp
} // End of Banshee namespace