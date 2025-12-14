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
    const int WINDOW_WIDTH{640};
    const int WINDOW_HEIGHT{480};
    
    std::unordered_set<long> m_ids; 

    Database m_db;
	Fl_Window* m_window; 
    Fl_Pack* m_taskPack;
    Fl_Input* m_textBox;

    Fl_Group* createTaskBox(Task & t);
    void refreshScroll();
    static void getNewTaskTitle(Fl_Widget* widget, void* userdata); // callback
    static void addCallBack(Fl_Widget* widget, void* userdata);
    static void deleteCallBack(Fl_Widget* widget, void* userdata);
    static void markCallBack(Fl_Widget* widget, void* userdata);
    static void updateIDs(Fl_Widget* widget, void* userdata);

public:
    int run();
    bool drawTasks();
};
