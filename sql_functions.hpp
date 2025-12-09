// chat gpt told me to add pragma once only to header files
#pragma once

#include <sqlite3.h>

// this outputs a sqlite3 ptr thing that pts to our database
sqlite3 * setupDataBase();

// this asks for input when we launch the programme
void askForInput(sqlite3 * db, bool & wannaQuit);

// this couts our tasks
void viewTasks(sqlite3 * db);

// this cin asks for a string of our task title and adds it to our todos.db
void addTask(sqlite3 * db);
bool addTask(sqlite3 * db, const char* newTaskTitle); // TODO

// this cin asks for task id number and deletes it from todos.db
void deleteTask(sqlite3 * db);

// this asks for task id number and deletes it from our todos.db
void markTask(sqlite3 * db);

