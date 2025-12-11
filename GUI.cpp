#include <iostream>
#include <memory>
#include <vector>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include "GUI.hpp"
#include "Database.hpp"

void GUI::testFunc(Fl_Widget* widget, void* userdata)
{
	// i want to eventually from the userData->value() input the value into addTask in sql.hpp.
	Fl_Input* userData = static_cast<Fl_Input*>(widget);
	std::cout << "testFunc was called. you entered: " << userData->value() << '\n';

	// add to db

	// TODO reload the list
}

Fl_Group* GUI::taskBox(std::string & taskTitle)
{
	Fl_Group* row = new Fl_Group(0, 0, 480, 40);
	row->begin();

	Fl_Box* item = new Fl_Box(0, 0, 480, 40, "");
	item->labelsize(18);
	item->box(FL_UP_BOX);
	item->color(FL_WHITE);
	item->copy_label(taskTitle.c_str());

	Fl_Check_Button* checkButton = new Fl_Check_Button(445, 10, 20, 20);

	row->end();
	// row->resizable(item);
	return row;
}

int GUI::run()
{
	std::cout << "starting GUI!" << '\n'; 
	Fl_Window* window = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "HELLO FLTK!");

	Fl_Pack* buttonsPack = new Fl_Pack(510, 10, 110, 460);
	buttonsPack->type(Fl_Pack::VERTICAL);
	buttonsPack->spacing(10);
	buttonsPack->begin();

	// buttons on the side 
	new Fl_Button(0, 0, 80, 30, "add");
	new Fl_Button(0, 0, 80,30, "delete");	
	new Fl_Button(0, 0, 80, 30, "mark");
	new Fl_Button(0, 0, 80, 30, "sort");

	buttonsPack->end();

	Fl_Input* taskGetter = new Fl_Input(10, 10, 480, 40, "");
	taskGetter->callback(GUI::testFunc, this);

	// Fl_Scroll(int x, int y, int w, int h, const* char label = 0)
	// this gives a scrollable box
	Fl_Scroll* scroll = new Fl_Scroll(10, 60, 480, 410);	
	scroll->box(FL_BORDER_BOX);
	scroll->type(Fl_Scroll::VERTICAL_ALWAYS);
	scroll->begin();
		
	// this is in scrollable box and packs boxes nicely
	Fl_Pack* pack1 = new Fl_Pack(0, 0, 480, 460);
	pack1->type(Fl_Pack::VERTICAL);
	pack1->spacing(1);
	pack1->begin();

	// these are boxes that are packed nicely
	// each group is a box of the task, and a check box
	std::vector<Fl_Group*> rows;
	// TODO instead of taskTitlesTest, i want the actual db tasks.
	// i need viewTask to give me a vector of tasks i think
    std::vector<Task> tasklist = m_db.getTaskList();
	for (Task & t : tasklist)
	{
        std::string s = t.title;
		rows.push_back(GUI::taskBox(s));
	}

	pack1->end();
	// pack1->resizable(nullptr);
	scroll->end();
	window->end();
	window->show();

    int running = Fl::run();
    std::cout << "closing GUI" << '\n';
	return running;
}


