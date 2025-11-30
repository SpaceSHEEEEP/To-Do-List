#pragma once

#include <sqlite3.h>

sqlite3 * setupDataBase();
void askForInput(sqlite3 * db, bool & wannaQuit);
void viewTasks(sqlite3 * db);
void addTask(sqlite3 * db);
void deleteTask(sqlite3 * db);
void markTask(sqlite3 * db);

