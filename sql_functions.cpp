// this is inspired bu Avey Dev https://www.youtube.com/watch?v=Pom6dXvGun0&t=187s
// this file will have sql related functions

#include <iostream> 
#include <stdio.h>
#include <sqlite3.h>
#include "sql_functions.hpp"

// hopefully wont use this when GUI done
void askForInput(sqlite3 * db, bool & wannaQuit)
{
	std::cout << "what would you like to do?" << '\n';
	std::cout << "[V]iew List, [A]dd Task, [D]elete Task, [M]ark Task as Complete, [Q]uit" << '\n';
	std::cout << "input: ";
	std::string input{""};
	while (input.length() == 0)
	{
		getline(std::cin, input);
	}

	if (input.length() != 1)
	{
		std::cout << "invalid input, try again " << "\n\n";
		askForInput(db, wannaQuit);
	}
	else
	{
		std::cout << "test: you entered " << input << "\n\n";
		int character_input = (int)input[0];
		switch (character_input)
		{
			case 'V':
			case 'v':
				viewTasks(db);
				break;
			case 'A':
			case 'a':
				addTask(db);
				break;
			case 'D':
			case 'd':
				deleteTask(db);
				break;
			case 'M':
			case 'm':
				markTask(db);
				break;
			case 'Q':
			case 'q':
				std::cout << "have a nice day!! \n\n";
				wannaQuit = true;
				break;
			default:
				std::cout << "Invalid input \n\n";
		}
	}
}

void viewTasks(sqlite3 * db)
{
	std::string sqlInput{"SELECT * FROM todos"};
	sqlite3_stmt * statement;
	
	//	sqlite3_prep_v2(ptr to database, 
	//					c string input, 
	//					max int to read c_str input, 
	//					output to statement, 
	//					ptr to unread c_str input)

	int returnCode = sqlite3_prepare_v2(db, sqlInput.c_str(), -1, &statement, nullptr);
	if (returnCode != SQLITE_OK)
	{
		std::cout << "cant prepare the statement to view tasks" << '\n';
		std::cout << "error: " << sqlite3_errmsg(db) << '\n';
		return;	
	}
	
	std::cout << "| id: |        task:        |    status:   |\n";   

	while ((returnCode = sqlite3_step(statement)) == SQLITE_ROW)
	{
		std::cout << "--------------------------------------------\n";
		
		int id = sqlite3_column_int(statement, 0);
		char * title =(char*) sqlite3_column_text(statement, 1);
		int completed = sqlite3_column_int(statement, 2);
		char * status = (char*)"incomplete";
		if (completed > 0) status = (char*)"completed";
		
		// std::cout << "| id: |     task:     |  status:  |\n";   
		// std::cout << "| " << id <<  " | " << title << " | " << status << " |\n"; 
		printf("| %-4d| %-20s| %-13s| \n", id, title, status);
	}

	sqlite3_finalize(statement);

}

void addTask(sqlite3 * db)
{
	// get task title
	std::string taskTitle{""};
	std::cout << "What's your next task?" << '\n';
	std::cout << "task: ";
	getline(std::cin, taskTitle);
	if (taskTitle.length() == 0) 
	{
		std::cout << "Invalid task name" << '\n';
		return;
	}

	// add into the todo list
	std::string sqlInput{"INSERT INTO todos (title) VALUES(?)"};

	sqlite3_stmt * statement;
	int returnCall = sqlite3_prepare_v2(db, sqlInput.c_str(), -1, &statement, nullptr);
	if (returnCall != SQLITE_OK)
	{
		std::cout << "cant prepare" << '\n';
		std::cout << "error: " << sqlite3_errmsg(db) << '\n';	
	}
	else std::cout << "prep ok! " << '\n';

	// bind params!! this is so we dont get sql injections
	sqlite3_bind_text(statement, 1, taskTitle.c_str(), -1, SQLITE_TRANSIENT);
	// sqlite3_bind_int(statement, 2, 0);

	// execute statement
	if ((returnCall = sqlite3_step(statement)) != SQLITE_DONE)
	{
		std::cout << "prepare is fine but cant add" << '\n';
		std::cout << "error: " << sqlite3_errmsg(db) << '\n';
	}
	else std::cout << "execute ok! " << '\n';

	sqlite3_finalize(statement);
}

bool addTask(sqlite3 * db, const char* newTaskTitle)
{
	if (sizeof(newTaskTitle) == 0) 
	{
		std::cout << "Invalid task name" << '\n';
		return false;
	}
	else
	{
		std::cout << "title length non zero TEST" << '\n';
		return true;
	}

	// add into the todo list
	std::string sqlInput{"INSERT INTO todos (title) VALUES(?)"};

	sqlite3_stmt * statement;
	int returnCall = sqlite3_prepare_v2(db, sqlInput.c_str(), -1, &statement, nullptr);
	if (returnCall != SQLITE_OK)
	{
		std::cout << "cant prepare" << '\n';
		std::cout << "error: " << sqlite3_errmsg(db) << '\n';	
	}
	else std::cout << "prep ok! " << '\n';

	// bind params!! this is so we dont get sql injections
	sqlite3_bind_text(statement, 1, newTaskTitle, -1, SQLITE_TRANSIENT);
	// sqlite3_bind_int(statement, 2, 0);

	// execute statement
	if ((returnCall = sqlite3_step(statement)) != SQLITE_DONE)
	{
		std::cout << "prepare is fine but cant add" << '\n';
		std::cout << "error: " << sqlite3_errmsg(db) << '\n';
	}
	else std::cout << "execute ok! " << '\n';

	sqlite3_finalize(statement);
}


void deleteTask(sqlite3 * db)
{
	int id_num{0};
	std::cout << "What's the id number of the task you want deleted?" << '\n';
	std::cout << "delete this: ";
	std::cin >> id_num;

	std::string sqlInput{"DELETE FROM todos WHERE id = ?;"};
	sqlite3_stmt * statement;
	int returnCode = sqlite3_prepare_v2(db, sqlInput.c_str(), -1, &statement, nullptr);
	if (returnCode != SQLITE_OK)
	{
		std::cout << "cant prepare the statement to delete task" << '\n';
		std::cout << "error: " << sqlite3_errmsg(db) << '\n';
	} 
	else std::cout << "prep ok! " << '\n';
	
	returnCode = sqlite3_bind_int(statement, 1, id_num);

	if ((returnCode = sqlite3_step(statement)) != SQLITE_DONE)
	{
		std::cout << "can prep but cant execute " << 'n';
		std::cout << "error: " << sqlite3_errmsg(db) << '\n';
	}
	else std::cout << "execute ok! " << '\n';

	sqlite3_finalize(statement);
}

void markTask(sqlite3 * db)
{
	int id_num{0};
	std::cout << "What's the id number of the task you want marked as completed/incomplete?" << '\n';
	std::cout << "mark this: ";
	std::cin >> id_num;

	// find out if this is completed(1) or incomplete(0)
	std::string sqlInput{"SELECT completed FROM todos WHERE id = ?;"};
	sqlite3_stmt * statement;
	int returnCode = sqlite3_prepare_v2(db, sqlInput.c_str(), -1, &statement, nullptr);
	if (returnCode != SQLITE_OK) 
	{
		std::cout << "cant prepare" << '\n';
		std::cout << "error: " << sqlite3_errmsg(db) << '\n';
	}
	else std::cout << "prep ok! " << '\n';

	sqlite3_bind_int(statement, 1, id_num);

	returnCode = sqlite3_step(statement);
	if (returnCode != SQLITE_ROW)
	{
		std::cout << "error cant read status " << '\n';
		sqlite3_finalize(statement);
		return;
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
	}
	else std::cout << "prep ok! " << '\n';

	sqlite3_bind_int(statement, 1, id_num);

	returnCode = sqlite3_step(statement);
	if (returnCode != SQLITE_DONE)
	{
		std::cout << "can prep but cant complete task" << '\n';
		std::cout << "error: " << sqlite3_errmsg(db) << '\n';
	}
	else std::cout << "execute ok!" << '\n';

	sqlite3_finalize(statement);
}

sqlite3 * setupDataBase()
{
	sqlite3 * db;
	int returnCode{sqlite3_open("todos.db", &db)};
	if (returnCode != SQLITE_OK)
	{
		std::cout << "error with setupDataBase function" << '\n';
		return NULL;
	}
	return db;
}



