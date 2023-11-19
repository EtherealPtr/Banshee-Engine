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
} // End of Banshee namespace