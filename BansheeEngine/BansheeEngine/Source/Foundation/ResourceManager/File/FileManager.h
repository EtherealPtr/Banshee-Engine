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
		std::string GetAssetName(const std::string& _assetPath) const;

		FileManager(const FileManager&) = delete;
		FileManager(FileManager&&) = delete;
		void operator=(const FileManager&) = delete;
		void operator=(FileManager&&) = delete;
	};
} // End of Banshee namespace