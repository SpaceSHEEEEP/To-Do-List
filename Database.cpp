#include <iostream> 
#include <stdio.h>
#include <sqlite3.h>
#include "Database.hpp"

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

    sqlite3_stmt * statement;
    int returnCall = sqlite3_prepare_v2(m_db, sqlInput.c_str(), -1, &statement, nullptr);
    if (returnCall != SQLITE_OK)
    {
        std::cout << "cant prepare" << '\n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
        return false;	
    }
    else std::cout << "prep ok! " << '\n';

    // bind params!! this is so we dont get sql injections
    sqlite3_bind_text(statement, 1, taskTitle.c_str(), -1, SQLITE_TRANSIENT);

    // execute statement
    if ((returnCall = sqlite3_step(statement)) != SQLITE_DONE)
    {
        std::cout << "prepare is fine but cant add" << '\n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
        return false;
    }
    else std::cout << "execute ok! " << '\n';

    // TODO add new task to m_tasks vector
    sqlInput = "SELECT MAX(id) FROM todos;";
    returnCall = sqlite3_prepare_v2(m_db, sqlInput.c_str(), -1, &statement, nullptr);
    if (returnCall != SQLITE_OK)
    {
        std::cout << "cant prepare" << '\n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
        return false;	
    }
    else std::cout << "prep ok! " << '\n';

    // execute statement
    if ((returnCall = sqlite3_step(statement)) != SQLITE_ROW)
    {
        std::cout << "prepare is fine but execute to find max id" << '\n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
        return false;
    }
    else std::cout << "execute ok! " << '\n';
    int maxID = sqlite3_column_int(statement, 0);
    m_tasks.push_back(Task(maxID, taskTitle, 0));

    sqlite3_finalize(statement);
    return true;
}

bool Database::deleteTask(long id_num)
{
    std::string sqlInput{"DELETE FROM todos WHERE id = ?;"};
    sqlite3_stmt * statement;
    int returnCode = sqlite3_prepare_v2(m_db, sqlInput.c_str(), -1, &statement, nullptr);
    if (returnCode != SQLITE_OK)
    {
        std::cout << "cant prepare the statement to delete task" << '\n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
        return false;
    } 
    else std::cout << "prep ok! " << '\n';
    
    returnCode = sqlite3_bind_int(statement, 1, id_num);

    if ((returnCode = sqlite3_step(statement)) != SQLITE_DONE)
    {
        std::cout << "can prep but cant execute " << 'n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
        return false;
    }
    else std::cout << "execute ok! " << '\n';

    // TODO delete new task to m_tasks vector
    for (int i = 0; i < m_tasks.size(); i++)
    {
        if (m_tasks[i].id_num == id_num)
        {
            m_tasks.erase(m_tasks.begin() + i);
            break;
        }
    }

    sqlite3_finalize(statement);
    return true;
}

bool Database::markTask(long id_num) // now can only set to complete. fix this
{
    // find out if this is completed(1) or incomplete(0)
    std::string sqlInput{"SELECT completed FROM todos WHERE id = ?;"};
    sqlite3_stmt * statement;
    int returnCode = sqlite3_prepare_v2(m_db, sqlInput.c_str(), -1, &statement, nullptr);
    if (returnCode != SQLITE_OK) 
    {
        std::cout << "cant prepare" << '\n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
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
    returnCode = sqlite3_prepare_v2(m_db, sqlInput.c_str(), -1, &statement, nullptr);
    if (returnCode != SQLITE_OK) 
    {
        std::cout << "cant prepare" << '\n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
        return false;
    }
    else std::cout << "prep ok! " << '\n';

    sqlite3_bind_int(statement, 1, id_num);

    returnCode = sqlite3_step(statement);
    if (returnCode != SQLITE_DONE)
    {
        std::cout << "can prep but cant complete task" << '\n';
        std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
        return false;
    }
    else std::cout << "execute ok!" << '\n';

    sqlite3_finalize(statement);

    // Update m_tasks
    for (int i = 0; i < m_tasks.size(); i++)
    {
        if (m_tasks[i].id_num == id_num)
        {
            m_tasks[i].completed++;
            break;
        }
    }

    return true;
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
	std::string sqlInput{"SELECT * FROM todos"};
	sqlite3_stmt * statement;
	
	//	sqlite3_prep_v2(ptr to database, 
	//					c string input, 
	//					max int to read c_str input, -1 for all
	//					output to statement, 
	//					ptr to unread c_str input)

	returnCode = sqlite3_prepare_v2(m_db, sqlInput.c_str(), -1, &statement, nullptr);
	if (returnCode != SQLITE_OK)
	{
		std::cout << "cant prepare the statement to view tasks" << '\n';
		std::cout << "error: " << sqlite3_errmsg(m_db) << '\n';
		return;	
	}

	while ((returnCode = sqlite3_step(statement)) == SQLITE_ROW)
	{
		int id = sqlite3_column_int(statement, 0);
		std::string title = (char*)sqlite3_column_text(statement, 1);
		int status = sqlite3_column_int(statement, 2);

        m_tasks.push_back(Task(id, title, status));
	}

	sqlite3_finalize(statement);

}

Database::~Database()
{
    std::cout << "closing m_db " << '\n';
    sqlite3_close(m_db);
}