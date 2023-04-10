#pragma once

#include "Subsystem.h"
#include <vector>
#include <fstream>

namespace Banshee
{
	class FileManager : public Subsystem
	{
	public:
		DECLARE_BANSHEE_SYSTEM(FileManager);

		void WriteToLogFile(const char* _logData);
		std::vector<char> ReadBinaryFile(const char* _fileName);
		std::ifstream ReadFile(const char* _filePath);

	protected:
		virtual void Initialize() override;
		virtual void Shutdown() override;

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