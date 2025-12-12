#pragma once

#include <FL/Fl_Group.H>
#include <FL/Fl_Pack.H>

#include <sqlite3.h>
#include <vector>

#include "Database.hpp"

class GUI 
{
    Database m_db;
    const int WINDOW_WIDTH{640};
    const int WINDOW_HEIGHT{480};
	Fl_Window* m_window; 
    Fl_Pack* m_taskPack;

    std::vector<int> m_ids_selected; // when checkbox activated. maybe this or vector<bool> all false
    
    Fl_Group* taskBox(Task & t);
    static void getNewTaskTitle(Fl_Widget* widget, void* userdata); // callback

    static void deleteButtonFunction(Fl_Widget* widget, long p);

public:
    int run();
    bool drawTasks();
};
