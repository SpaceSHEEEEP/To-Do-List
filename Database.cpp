#include <iostream> 
#include <stdio.h>
#include <sqlite3.h>
#include "Database.hpp"

bool Database::addTask(std::string taskTitle)
{
    if (taskTitle.length() == 0) 
    {
        std::cout << "Invalid task name" << '\n';
        return false;
    }

    // add into the todo list
    std::string sqlInput{"INSERT INTO todos (title) VALUES(?)"};

    sqlite3_stmt * statement;
    int returnCall = sqlite3_prepare_v2(db, sqlInput.c_str(), -1, &statement, nullptr);
    if (returnCall != SQLITE_OK)
    {
        std::cout << "cant prepare" << '\n';
        std::cout << "error: " << sqlite3_errmsg(db) << '\n';
        return false;	
    }
    else std::cout << "prep ok! " << '\n';

    // bind params!! this is so we dont get sql injections
    sqlite3_bind_text(statement, 1, taskTitle.c_str(), -1, SQLITE_TRANSIENT);

    // execute statement
    if ((returnCall = sqlite3_step(statement)) != SQLITE_DONE)
    {
        std::cout << "prepare is fine but cant add" << '\n';
        std::cout << "error: " << sqlite3_errmsg(db) << '\n';
        return false;
    }
    else std::cout << "execute ok! " << '\n';

    sqlite3_finalize(statement);
    return true;
}

bool Database::deleteTask(int id_num)
{
    std::string sqlInput{"DELETE FROM todos WHERE id = ?;"};
    sqlite3_stmt * statement;
    int returnCode = sqlite3_prepare_v2(db, sqlInput.c_str(), -1, &statement, nullptr);
    if (returnCode != SQLITE_OK)
    {
        std::cout << "cant prepare the statement to delete task" << '\n';
        std::cout << "error: " << sqlite3_errmsg(db) << '\n';
        return false;
    } 
    else std::cout << "prep ok! " << '\n';
    
    returnCode = sqlite3_bind_int(statement, 1, id_num);

    if ((returnCode = sqlite3_step(statement)) != SQLITE_DONE)
    {
        std::cout << "can prep but cant execute " << 'n';
        std::cout << "error: " << sqlite3_errmsg(db) << '\n';
        return false;
    }
    else std::cout << "execute ok! " << '\n';

    sqlite3_finalize(statement);
    return true;
}

bool Database::markTask(int id_num)
{
    // find out if this is completed(1) or incomplete(0)
    std::string sqlInput{"SELECT completed FROM todos WHERE id = ?;"};
    sqlite3_stmt * statement;
    int returnCode = sqlite3_prepare_v2(db, sqlInput.c_str(), -1, &statement, nullptr);
    if (returnCode != SQLITE_OK) 
    {
        std::cout << "cant prepare" << '\n';
        std::cout << "error: " << sqlite3_errmsg(db) << '\n';
        return false;
    }
    else std::cout << "prep ok! " << '\n';

    sqlite3_bind_int(statement, 1, id_num);

    returnCode = sqlite3_step(statement);
    if (returnCode != SQLITE_ROW)
    {
        std::cout << "error cant read status " << '\n';
        sqlite3_finalize(statement);
        return false;
    }
    
    bool completed{false};
    if (sqlite3_column_int(statement, 2) != 0) completed = true;

    // change completed to incomplete or vice versa TODO!!!
    sqlInput = "UPDATE todos SET completed = 1 WHERE id = ?;";
    returnCode = sqlite3_prepare_v2(db, sqlInput.c_str(), -1, &statement, nullptr);
    if (returnCode != SQLITE_OK) 
    {
        std::cout << "cant prepare" << '\n';
        std::cout << "error: " << sqlite3_errmsg(db) << '\n';
        return false;
    }
    else std::cout << "prep ok! " << '\n';

    sqlite3_bind_int(statement, 1, id_num);

    returnCode = sqlite3_step(statement);
    if (returnCode != SQLITE_DONE)
    {
        std::cout << "can prep but cant complete task" << '\n';
        std::cout << "error: " << sqlite3_errmsg(db) << '\n';
        return false;
    }
    else std::cout << "execute ok!" << '\n';

    sqlite3_finalize(statement);
    return true;
}

// should add throw exeptions since c++ constructor cant return true or false
Database::Database() 
{
    sqlite3 * DB;
    int returnCode{sqlite3_open("todos.db", &DB)};
    if (returnCode != SQLITE_OK)
    {
        std::cout << "error with setupDataBase method" << '\n';
    }
    db = DB;
}

Database::~Database()
{
    std::cout << "closing db " << '\n';
    sqlite3_close(db);
}