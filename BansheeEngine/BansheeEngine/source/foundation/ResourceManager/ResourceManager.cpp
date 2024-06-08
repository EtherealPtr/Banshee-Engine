#include "ResourceManager.h"
#include "File/FileManager.h"
#include "Image/ImageManager.h"

namespace Banshee
{
    ResourceManager::ResourceManager() :
        m_FileManager(std::make_shared<FileManager>()),
        m_ImageManager(std::make_shared<ImageManager>())
    {}

    ResourceManager& ResourceManager::Instance()
    {
        static ResourceManager instance;
        return instance;
    }

    uint32 ResourceManager::LoadImageResource(const char* _pathToImage)
    {
        const std::string fullPath = m_FileManager->GetEngineResDirPath() + _pathToImage;
        return m_ImageManager->LoadImage(fullPath.c_str());
    }

    const char* ResourceManager::GetEngineResDirPath() noexcept
    {
        return m_FileManager->GetEngineResDirPath().c_str();
    }

    std::string ResourceManager::GetAssetName(const std::string& _assetName)
    {
        return m_FileManager->GetAssetName(_assetName);
    }
} // End of Banshee namespace