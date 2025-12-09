// this is inspired bu Avey Dev https://www.youtube.com/watch?v=Pom6dXvGun0&t=187s

#include <iostream> 
#include "sql_functions.hpp"
#include "FLTK_wireframe.hpp"

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

	bool wannaQuit{false};
	while (!wannaQuit)
	{
		askForInput(db, wannaQuit);
		std::cout << "=============================================\n\n";
	}

	sqlite3_close(db);

	FLTKStart();

	return 0;
}


