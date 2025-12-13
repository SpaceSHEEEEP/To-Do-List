#pragma once

#include <vector>
#include <unordered_set>

#include <FL/Fl_Input.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Pack.H>

#include <sqlite3.h>

#include "Database.hpp"

class GUI 
{
    Database m_db;
    const int WINDOW_WIDTH{640};
    const int WINDOW_HEIGHT{480};
	Fl_Window* m_window; 
    Fl_Pack* m_taskPack;
    Fl_Input* m_taskGetter;

    std::unordered_set<long> m_ids; // when checkbox activated. maybe this or vector<bool> all false
    
    Fl_Group* taskBox(Task & t);
    static void addCallBack(Fl_Widget* widget, void* userdata);
    static void getNewTaskTitle(Fl_Widget* widget, void* userdata); // callback
    static void deleteTasks(Fl_Widget* widget, void* userdata);
    static void markTasks(Fl_Widget* widget, void* userdata);
    static void updateIDs(Fl_Widget* widget, void* userdata);

public:
    int run();
    bool drawTasks();
};
