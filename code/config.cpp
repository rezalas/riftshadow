#include "config.h"
#include <iostream>
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/rapidjson.h"
#include "include/rapidjson/filereadstream.h"
#include "include/rapidjson/filewritestream.h"

void Config::Load()
{
    FILE* fp = fopen(_FilePath,"r");
    if (fp == NULL)
        return;
    char buffer[65536];
    rapidjson::FileReadStream inputStream(fp,buffer, sizeof(buffer));
    Settings.ParseStream(inputStream);
    fclose(fp);
    loaded = true;
}

std::string Config::GetValue(const char* key)
{
    if(Settings.HasMember(key))
    {
        if(Settings[key].IsString())
        {
            return Settings[key].GetString();
        }
        else
        {
            return std::to_string(Settings[key].GetInt());
        }
        
    }

    return nullptr;
}


DbConnection Config::GetDbConnection(const char* connection)
{
    auto conn = Settings["DbConnections"][connection].GetObject();
    DbConnection result = DbConnection();
    result.Db = std::string(conn["Db"].GetString());
    result.Host = std::string(conn["Host"].GetString());
    result.Pwd = std::string(conn["Pwd"].GetString());
    result.User = std::string(conn["User"].GetString());
    if(conn.HasMember("Port"))
    {
        result.Port = conn["Port"].GetUint();
    }
    return result;   
}

Config::Config(const char* file)
{
    _FilePath = file;
    loaded = false;
    Load();
}

bool Config::isLoaded()
{
    return loaded;
}
