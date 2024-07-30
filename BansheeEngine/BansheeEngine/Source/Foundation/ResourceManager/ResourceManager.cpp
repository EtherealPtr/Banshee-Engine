#include "ResourceManager.h"
#include "File/FileManager.h"
#include "Image/ImageManager.h"
#include "Foundation/Paths/PathManager.h"

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

    uint16 ResourceManager::LoadImageResource(const char* _pathToImage)
    {
        const std::string fullPath = PathManager::GetEngineResDirPath() + _pathToImage;
        return m_ImageManager->LoadImage(fullPath.c_str());
    }

    std::string ResourceManager::GetAssetName(const std::string& _assetName)
    {
        return m_FileManager->GetAssetName(_assetName);
    }
} // End of Banshee namespace