#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include "include/rapidjson/document.h"
#include "dbconnection.h"

class Config 
{
private: 
	rapidjson::Document Settings;
	const char* _FilePath;
	bool loaded;
	void Load();
public:
	Config(const char* file = "../config.json");
	std::string GetValue(const char* key);
	DbConnection GetDbConnection(const char* key);
	bool isLoaded();
};

#endif /* CONFIG_H */
