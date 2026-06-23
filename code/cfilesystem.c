#include "stdlibs/cfilesystem.h"
#include <filesystem>
#include <fstream>
#include <system_error>

/// Tests whether a file or directory exists at the given path.
/// @param path: The path to check.
/// @return True if something exists at path, false otherwise.
bool CFileSystem::Exists(const std::string &path)
{
	std::error_code ec;
	bool exists = std::filesystem::exists(path, ec);
	return exists && !ec;
}

/// Moves (renames) a file, overwriting the destination if it exists.
/// @note Equivalent to "mv -f source destination".
/// @param source: The path of the file to move.
/// @param destination: The path to move the file to.
/// @return True on success, false on failure.
bool CFileSystem::Move(const std::string &source, const std::string &destination)
{
	std::error_code ec;
	std::filesystem::rename(source, destination, ec);
	return !ec;
}

/// Copies a file, overwriting the destination if it exists.
/// @note Equivalent to "cp source destination".
/// @param source: The path of the file to copy.
/// @param destination: The path to copy the file to.
/// @return True on success, false on failure.
bool CFileSystem::Copy(const std::string &source, const std::string &destination)
{
	std::error_code ec;
	std::filesystem::copy_file(source, destination,
		std::filesystem::copy_options::overwrite_existing, ec);
	return !ec;
}

/// Removes a file. Removing a non-existent file is treated as success.
/// @note Equivalent to "rm path".
/// @param path: The path of the file to remove.
/// @return True on success, false on failure.
bool CFileSystem::Remove(const std::string &path)
{
	std::error_code ec;
	std::filesystem::remove(path, ec);
	return !ec;
}

/// Ensures a file exists, creating an empty one if it does not.
/// @note Equivalent to "touch path".
/// @param path: The path of the file to create or update.
/// @return True on success, false on failure.
bool CFileSystem::Touch(const std::string &path)
{
	if (CFileSystem::Exists(path))
		return true;

	std::ofstream file(path, std::ios::app);
	return file.good();
}
