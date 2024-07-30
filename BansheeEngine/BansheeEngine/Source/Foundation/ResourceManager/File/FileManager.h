#pragma once

#include <vector>
#include <fstream>

namespace Banshee
{
	class FileManager
	{
	public:
		FileManager();

		std::vector<char> ReadBinaryFile(const char* _fileName);
		std::ifstream ReadFile(const char* _filePath) const;
		const std::string& GetEngineResDirPath() const noexcept { return m_EngineResDirPath; }
		std::string GetAssetName(const std::string& _assetPath) const;

	private:
		void CreateGeneratedFolder();
		void InitializeDirPaths();

	private:
		std::string m_GeneratedDirPath;
		std::string m_EngineResDirPath;
	};
} // End of Banshee namespace