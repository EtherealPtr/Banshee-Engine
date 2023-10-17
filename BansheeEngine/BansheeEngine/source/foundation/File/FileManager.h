#pragma once

#include <vector>
#include <fstream>

namespace Banshee
{
	class FileManager
	{
	public:
		static FileManager& Instance();

		void WriteToLogFile(const char* _logData);
		std::vector<char> ReadBinaryFile(const char* _fileName);
		std::ifstream ReadFile(const char* _filePath);

	private:
		FileManager();
		~FileManager();

		FileManager(const FileManager&) = delete;
		FileManager(FileManager&&) = delete;
		void operator=(const FileManager&) = delete;
		void operator=(FileManager&&) = delete;

		void CreateGeneratedFolder();
		void OpenLogFile();
		void InitializeDirPaths();

	private:
		std::string m_GeneratedDirPath;
		std::string m_EngineResDirPath;
		std::ofstream m_LogFile;
	};
} // End of Banshee namespace