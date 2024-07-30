#include "FileManager.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/DLLConfig.h"
#include "Foundation/Paths/PathManager.h"
#include <filesystem>

namespace Banshee
{
	FileManager::FileManager() 
	{
		if (!std::filesystem::exists(PathManager::GetGeneratedDirPath()))
		{
			std::filesystem::create_directory(PathManager::GetGeneratedDirPath());
		}
	}

	std::vector<char> FileManager::ReadBinaryFile(const char* _fileName)
	{
		std::ifstream inputFile(PathManager::GetEngineResDirPath() + _fileName, std::ios::binary | std::ios::ate);
		if (!inputFile.is_open())
		{
			BE_LOG(LogCategory::Warning, "[FILEMANAGER]: Failed to read binary file: %s", _fileName);
		}

		const size_t fileSize = static_cast<size_t>(inputFile.tellg());
		std::vector<char> buffer(fileSize);

		inputFile.seekg(0);
		inputFile.read(buffer.data(), fileSize);
		inputFile.close();

		return buffer;
	}
	
	std::ifstream FileManager::ReadFile(const char* _filePath) const
	{
		const std::string fullPath = PathManager::GetEngineResDirPath() + _filePath;
		std::ifstream file(fullPath);

		if (!file.is_open())
		{
			BE_LOG(LogCategory::Error, "[FILEMANAGER]: Failed to open file: %s", _filePath);
		}

		return file;
	}

	std::string FileManager::GetAssetName(const std::string& _assetPath) const
	{
		std::filesystem::path path(_assetPath);
		return path.filename().string();
	}
} // End of Banshee namespace