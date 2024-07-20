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
		std::ifstream ReadFile(const char* _filePath) const;
		const std::string& GetEngineResDirPath() const noexcept { return m_EngineResDirPath; }
		std::string GetAssetName(const std::string& _assetPath) const;

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