#include "FileManager.h"
#include "Logger.h"
#include "DLLConfig.h"
#include <filesystem>

namespace Banshee
{
	FileManager::FileManager() : 
		m_GeneratedDirPath(""),
		m_EngineResDirPath(""),
		m_LogFile{}
	{
		Initialize();
	}

	FileManager::~FileManager()
	{
		Shutdown();
	}

	FileManager& FileManager::Instance()
	{
		static FileManager s_FileManager{};
		return s_FileManager;
	}

	void FileManager::Initialize()
	{
		InitializeDirPaths();
		CreateGeneratedFolder();
		OpenLogFile();
	}

	void FileManager::Shutdown()
	{
		m_LogFile.close();
	}

	void FileManager::InitializeDirPaths()
	{
		const std::filesystem::path currentWorkDir = std::filesystem::current_path();

		// Traverse the directory tree downwards until we find the target folder
		for (const auto& entry : std::filesystem::recursive_directory_iterator(currentWorkDir))
		{
			if (entry.is_directory() && entry.path().filename() == "res")
			{
				m_GeneratedDirPath = entry.path().parent_path().generic_string() + '/';
				m_EngineResDirPath = entry.path().generic_string() + '/';
				return;
			}
		}

		BE_LOG(LogCategory::warning, "Failed to initialize paths");
	}

	void FileManager::CreateGeneratedFolder()
	{
		m_GeneratedDirPath += "banshee_generated/";

		if (std::filesystem::exists(m_GeneratedDirPath))
		{
			return;
		}

		BE_LOG(LogCategory::info, "Created banshee generated directory");
		std::filesystem::create_directory(m_GeneratedDirPath);
	}

	void FileManager::OpenLogFile()
	{
		m_LogFile.open(m_GeneratedDirPath + "/logs.txt", std::ios::out);
	}

	void FileManager::WriteToLogFile(const char* _logData)
	{
		m_LogFile << _logData << '\n';
	}

	std::vector<char> FileManager::ReadBinaryFile(const char* _fileName)
	{
		std::ifstream inputFile(m_EngineResDirPath + _fileName, std::ios::binary | std::ios::ate);

		if (!inputFile.is_open())
		{
			BE_LOG(LogCategory::warning, "Failed to read binary file: %s", _fileName);
		}

		const size_t fileSize = static_cast<size_t>(inputFile.tellg());
		std::vector<char> buffer(fileSize);

		inputFile.seekg(0);
		inputFile.read(buffer.data(), fileSize);
		inputFile.close();

		return buffer;
	}
	
	std::ifstream FileManager::ReadFile(const char* _filePath)
	{
		std::ifstream file(m_EngineResDirPath + _filePath);

		if (!file.is_open())
		{
			BE_LOG(LogCategory::error, "Failed to open INI file: %s", _filePath);
		}

		return file;
	}
} // End of Banshee namespace
