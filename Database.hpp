#pragma once

#include <sqlite3.h>

class Database // TODO comment stuff
{
    sqlite3 * db;

public:
    bool addTask(std::string taskTitle);
    bool deleteTask(int id_num);
    bool markTask(int id_num);
    Database(); 
    ~Database();
};