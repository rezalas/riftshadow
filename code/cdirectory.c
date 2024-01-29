#include "stdlibs/cdirectory.h"
#include <filesystem>
#include <algorithm>

/// Represents a directory in a file system.
/// @param path: The path used to gather directory information.
CDirectory::CDirectory(std::string path)
{
	_path = path;
}

/// Destructor
CDirectory::~CDirectory()
{
}

/// Retrieves the path used to gather directory information .
/// @return The path of the directory used by this object.
std::string CDirectory::GetPath()
{
	return _path;
}

/// Retrieves a list of all files from the directory.
/// @return A list of files from the directory.
std::vector<std::string> CDirectory::GetFiles()
{
	std::vector<std::string> files;
	for (const auto & entry : std::filesystem::directory_iterator(_path))
	{
		if(entry.is_regular_file())
			files.push_back(entry.path());
	}

	std::sort(files.begin(), files.end());
	return files;
}

/// Retrieves a list of files from the directory with a specific extension.
/// @param fileExtension: The extension used to filter the resulting file list.
/// @return A list of files with the given fileExtension from the directory.
std::vector<std::string> CDirectory::GetFiles(std::string fileExtension)
{
	std::vector<std::string> files;
	for (const auto & entry : std::filesystem::directory_iterator(_path))
	{
		if(entry.is_regular_file() && entry.path().extension() == fileExtension)
			files.push_back(entry.path());
	}

	std::sort(files.begin(), files.end());
	return files;
}
