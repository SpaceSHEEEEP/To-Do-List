#include <iostream> 
#include <stdio.h>
#include <sqlite3.h>

// NOTES
// todos.db id=1 and id=4 has no complete value. find a way so its automatically = 0

sqlite3 * setupDataBase();
void askForInput(sqlite3 * db);
void viewTasks(sqlite3 * db);
void addTask(sqlite3 * db);
void deleteTask(sqlite3 * db);
void completeTask(sqlite3 * db);

int main()
{
	std::cout << "hello world!" << '\n';

	sqlite3 * db = setupDataBase();
	if (db == NULL)
	{
		std::cout << "cant open database sprry" << '\n';
		return 1;
	}
	
	std::cout << "\n\n";
	std::cout << "!!!! TERMINAL TODO LIST !!!!" << '\n';
	
	while (true)
	{
		askForInput(db);
		std::cout << "=============================================\n\n";
	}

	sqlite3_close(db);
	return 0;
}

void askForInput(sqlite3 * db)
{
	std::cout << "what would you like to do?" << '\n';
	std::cout << "[V]iew List, [A]dd Task, [D]elete Task, [M]ark Task as Complete" << '\n';
	std::cout << "input: ";
	std::string input{""};
	getline(std::cin, input);

	if (input.length() != 1)
	{
		std::cout << "invalid input, try again " << "\n\n";
		askForInput(db);
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
				completeTask(db);
				break;
			case 'Q':
			case 'q':
				std::cout << "have a nice day!! \n\n";
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

	// add into the todo list

}

void deleteTask(sqlite3 * db)
{
	int id_num{0};
	std::cout << "What's the id number of the task you want deleted?" << '\n';
	std::cout << "delete this: ";
	std::cin >> id_num;

	std::string sqlInput{"DELETE FROM todos WHERE id=" + std::to_string(id_num) + ";"};
	sqlite3_stmt * statement;
	int returnCode = sqlite3_prepare_v2(db, sqlInput.c_str(), -1, &statement, nullptr);
	if ((returnCode = sqlite3_step(statement)) != SQLITE_OK)
	{
		std::cout << "cant prepare the statement to delete task" << '\n';
		std::cout << "error: " << sqlite3_errmsg(db) << '\n';
		return;	
	}
	
	sqlite3_finalize(statement);
}



void completeTask(sqlite3 * db)
{

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



