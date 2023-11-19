#pragma once

#include <vector>
#include <fstream>

namespace Banshee
{
	class FileManager
	{
	public:
		FileManager();
		~FileManager();

		void WriteToLogFile(const char* _logData);
		std::vector<char> ReadBinaryFile(const char* _fileName);
		std::ifstream ReadFile(const char* _filePath);
		const std::string& GetEngineResDirPath() const { return m_EngineResDirPath; }

	private:
		void CreateGeneratedFolder();
		void OpenLogFile();
		void InitializeDirPaths();

	private:
		std::string m_GeneratedDirPath;
		std::string m_EngineResDirPath;
		std::ofstream m_LogFile;
	};
} // End of Banshee namespace