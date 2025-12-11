#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <sqlite3.h>

// CONSTANTS
const int WINDOW_WIDTH{640};
const int WINDOW_HEIGHT{480};

// input task title and execute this in the Fl_Pack so tasks shows up in scroll box
Fl_Group* newTaskBox(std::string & taskTitle);

// to open FLTK GUI
int FLTKStart();
