#pragma once

#include <vector>
#include <sqlite3.h>
#include "Task.hpp"

class Database // TODO comment stuff
{
    sqlite3 * m_db;
    std::vector<Task> m_tasks;

public:
    bool addTask(std::string & taskTitle);
    bool deleteTask(int id_num);
    bool markTask(int id_num);
    std::vector<Task> getTaskList();
    Database(); 
    ~Database();
};