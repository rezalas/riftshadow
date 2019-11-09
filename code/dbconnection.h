#ifndef DBCONNECTION_H
    #define DBCONNECTION_H
    
    #include <string>

class DbConnection
{
public:
    std::string Db;
    std::string Host;
    std::string User;
    std::string Pwd;
    unsigned int Port = 0;
};

#endif /* DBCONNECTION_H */