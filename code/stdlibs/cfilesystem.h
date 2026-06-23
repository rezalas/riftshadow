#ifndef CFILESYSTEM
#define CFILESYSTEM

#include <string>

class CFileSystem
{
public:
	static bool Exists(const std::string &path);
	static bool Move(const std::string &source, const std::string &destination);
	static bool Copy(const std::string &source, const std::string &destination);
	static bool Remove(const std::string &path);
	static bool Touch(const std::string &path);
};

#endif
