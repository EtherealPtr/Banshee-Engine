#include "ResourceManager.h"
#include "File/FileManager.h"
#include "Image/ImageManager.h"
#include "Foundation/Paths/PathManager.h"
#include "Foundation/ResourceManager/Image/Image.h"

namespace Banshee
{
	const ResourceManager g_ResourceManager{};

	ResourceManager::ResourceManager() :
		m_FileManager{ std::make_unique<FileManager>() },
		m_ImageManager{ std::make_unique<ImageManager>() }
	{}

	uint16 ResourceManager::LoadImageResource(std::string_view _pathToImage) const
	{
		const std::string fullPath = PathManager::GetEngineResDirPath() + _pathToImage.data();
		return m_ImageManager->LoadImage(fullPath.c_str());
	}

	uint16 ResourceManager::LoadImageFromMemory(const unsigned char* _bytes, const int32 _size) const
	{
		return m_ImageManager->LoadImageFromMemory(_bytes, _size);
	}

	std::string ResourceManager::GetAssetName(const std::string_view _assetName) const
	{
		return m_FileManager->GetAssetName(_assetName);
	}

	std::ifstream ResourceManager::ReadFile(std::string_view _filePath) const
	{
		return m_FileManager->ReadFile(_filePath.data());
	}

	std::vector<char> ResourceManager::ReadBinaryFile(std::string_view _fileName) const
	{
		return m_FileManager->ReadBinaryFile(_fileName.data());
	}

	const std::vector<std::shared_ptr<Image>>& ResourceManager::GetImages() const noexcept
	{
		return m_ImageManager->GetImages();
	}
} // End of Banshee namespace