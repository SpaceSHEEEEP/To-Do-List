#include <iostream> 
#include <type_traits>
#include <stdio.h>
#include <sqlite3.h>
#include "Database.hpp"

// sqlInput = "INSERT INTO todos (title) VALUES(?)" for example
template<typename T> 
bool Database::executeSQL(std::string SQLstatement, T & bindThis) 
{
    sqlite3_stmt * statement;
    int returnCall = sqlite3_prepare_v2(m_db, SQLstatement.c_str(), -1, &statement, nullptr);
    if (returnCall != SQLITE_OK)
    {
        std::cout << "cant prepare" << '\n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
        return false;	
    }
    else std::cout << "prep ok! " << '\n';

    // bind params!! this is so we dont get sql injections
    if constexpr (std::is_same_v<T, std::string>) 
    {
        sqlite3_bind_text(statement, 1, bindThis.c_str(), -1, SQLITE_TRANSIENT);
    }
    else if constexpr (std::is_same_v<T, long>) 
    {
        sqlite3_bind_int(statement, 1, bindThis);
    }
    else 
    {
        std::cout << "you entered weird variable" << '\n';
        return false;
    }

    // execute statement
    if ((returnCall = sqlite3_step(statement)) != SQLITE_DONE)
    {
        std::cout << "prepare is fine but cant step" << '\n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
        return false;
    }
    else std::cout << "execute ok! " << '\n';

    sqlite3_finalize(statement);

    // refresh m_tasks
    refreshTasks();

    return true;
}

bool Database::addTask(std::string & taskTitle)
{
    // wanna remove whitespace from front 
    if (taskTitle[0] == ' ') 
    {
        int startingIndex{0};
        while (taskTitle[startingIndex] == ' ') startingIndex++;
        taskTitle = taskTitle.substr(startingIndex);
    }
    // remove whitespace from back
    if (taskTitle[taskTitle.size()-1] == ' ')
    {
        int lastIndex = taskTitle.size() - 1;
        while (taskTitle[lastIndex] == ' ') lastIndex--;
        taskTitle = taskTitle.substr(0, lastIndex + 1);
    }
    std::cout << "add function test. taskTitle: " << taskTitle << '\n';
    // if (taskTitle.length() == 0) // dont need this because fltk does it for me

    // add into the todo list
    std::string sqlInput{"INSERT INTO todos (title) VALUES(?)"};
    executeSQL("INSERT INTO todos (title) VALUES(?)", taskTitle);

    return true;
}

bool Database::deleteTask(long id_num)
{
    std::string sqlInput{"DELETE FROM todos WHERE id = ?;"};
    executeSQL("DELETE FROM todos WHERE id = ?;", id_num);

    return true;
}

bool Database::markTask(long id_num) // now can only set to complete. fix this
{
    // find out if this is completed(1) or incomplete(0)
    std::string sqlInput{"SELECT completed FROM todos WHERE id = ?;"};
    executeSQL("UPDATE todos SET completed = (CASE WHEN completed == 0 THEN 1 ELSE 0  END) WHERE id = ?;", id_num);

    return true;
}

std::vector<Task> Database::refreshTasks()
{
    // return m_tasks;
    // instead of adding tasks and deleting tasks from m_tasks after every add, or delete or mark,
    // ill just clear m_tasks and add the tasks again
    // it wont be nice if the to do list has 1000s of tasks but this is a small project
    m_tasks.clear();

	std::string sqlInput{"SELECT * FROM todos"};
	sqlite3_stmt * statement;
	
	//	sqlite3_prep_v2(ptr to database, 
	//					c string input, 
	//					max int to read c_str input, -1 for all
	//					output to statement, 
	//					ptr to unread c_str input)

	int returnCode = sqlite3_prepare_v2(m_db, sqlInput.c_str(), -1, &statement, nullptr);
	if (returnCode != SQLITE_OK)
	{
		std::cout << "cant prepare the statement to view tasks" << '\n';
		std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
		return m_tasks;	
	}

	while ((returnCode = sqlite3_step(statement)) == SQLITE_ROW)
	{
		int id = sqlite3_column_int(statement, 0);
		std::string title = (char*)sqlite3_column_text(statement, 1);
		int status = sqlite3_column_int(statement, 2);

        m_tasks.push_back(Task(id, title, status));
	}

	sqlite3_finalize(statement);

    return m_tasks;    
}

std::vector<Task> Database::getTaskList()
{
    return m_tasks;
}

// should add throw exeptions since c++ constructor cant return true or false
Database::Database() 
{
    // open the database
    std::cout << "opening m_db" << '\n';
    sqlite3 * DB;
    int returnCode{sqlite3_open("todos.db", &DB)};
    if (returnCode != SQLITE_OK)
    {
        std::cout << "error with setupDataBase method" << '\n';
        // TODO if error, maybe file dont exist, then create file
    }
    m_db = DB;

    // add values to m_tasks
    refreshTasks();
}

Database::~Database()
{
    std::cout << "closing m_db " << '\n';
    sqlite3_close(m_db);
}