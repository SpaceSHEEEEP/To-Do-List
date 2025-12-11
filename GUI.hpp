#pragma once

#include <sqlite3.h>
#include <vector>

class GUI 
{
    const int WINDOW_WIDTH{640};
    const int WINDOW_HEIGHT{480};
    std::vector<std::string> taskTitlesTest = {"buy chicken", "buy fish", "buy coffee", "eat fried chicken", "drink water", "visit friend", "fry chicken", "find a job", "get married", "get money", "grind for honor", "get rich", "drive car", "make to do list", "learn vim"};
    Fl_Group* taskBox(std::string & taskTitle);
    static void testFunc(Fl_Widget* widget, void* userdata); // callback

public:
    int run();
    bool refresh();
};
