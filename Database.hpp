#pragma once

#include <vector>
#include <sqlite3.h>
#include "Task.hpp"

class Database // TODO comment stuff
{
    sqlite3 * m_db;
    std::vector<Task> m_tasks;

public:
    template<typename T>
    bool executeSQL(std::string SQLstatement, T & bindThis);
    bool addTask(std::string & taskTitle);
    bool deleteTask(long id_num);
    bool markTask(long id_num);
    std::vector<Task> refreshTasks();
    std::vector<Task> getTaskList();
    Database(); 
    ~Database();
};