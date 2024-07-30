#include "FileManager.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/DLLConfig.h"
#include <filesystem>

namespace Banshee
{
	FileManager::FileManager() : 
		m_GeneratedDirPath(""),
		m_EngineResDirPath("")
	{
		InitializeDirPaths();
		CreateGeneratedFolder();
	}

	void FileManager::InitializeDirPaths()
	{
		std::filesystem::path currentFilePath(__FILE__);
		std::filesystem::path currentFileDirectory = currentFilePath.parent_path();

		// Traverse the directory tree upwards until we find the "Res" folder
		for (std::filesystem::path directory = currentFileDirectory; !directory.empty(); directory = directory.parent_path()) 
		{
			for (const auto& entry : std::filesystem::directory_iterator(directory)) 
			{
				if (entry.is_directory() && entry.path().filename() == "Res")
				{
					m_GeneratedDirPath = entry.path().parent_path().generic_string() + '/';
					m_EngineResDirPath = entry.path().generic_string() + '/';
					return;
				}
			}
		}
		
		BE_LOG(LogCategory::Warning, "[FILEMANAGER]: Failed to initialize paths");
	}

	void FileManager::CreateGeneratedFolder()
	{
		m_GeneratedDirPath += "Banshee_Generated/";

		if (std::filesystem::exists(m_GeneratedDirPath))
		{
			return;
		}

		std::filesystem::create_directory(m_GeneratedDirPath);
	}

	std::vector<char> FileManager::ReadBinaryFile(const char* _fileName)
	{
		std::ifstream inputFile(m_EngineResDirPath + _fileName, std::ios::binary | std::ios::ate);
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
		std::ifstream file(m_EngineResDirPath + _filePath);

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