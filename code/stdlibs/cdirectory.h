#ifndef CDIRECTORY
#define CDIRECTORY

#include <vector>
#include <string>

class CDirectory
{
public:
	CDirectory(std::string path);
	~CDirectory();

	std::string GetPath();
	std::vector<std::string> GetFiles();
	std::vector<std::string> GetFiles(std::string fileExtension);
private:
	std::string _path;
};

#endif