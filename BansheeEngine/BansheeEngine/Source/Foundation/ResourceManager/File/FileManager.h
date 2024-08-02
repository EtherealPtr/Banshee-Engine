#pragma once

#include <vector>
#include <fstream>

namespace Banshee
{
	class FileManager
	{
	public:
		FileManager();

		std::vector<char> ReadBinaryFile(std::string_view _fileName) const;
		std::ifstream ReadFile(std::string_view _filePath) const;
		std::string GetAssetName(std::string_view _assetPath) const;

		FileManager(const FileManager&) = delete;
		FileManager(FileManager&&) = delete;
		void operator=(const FileManager&) = delete;
		void operator=(FileManager&&) = delete;
	};
} // End of Banshee namespace