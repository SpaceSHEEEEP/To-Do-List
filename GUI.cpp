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
	Fl_Input* inputData = static_cast<Fl_Input*>(widget);
    GUI* gui = static_cast<GUI*>(userdata);
    std::string newTaskTitle = inputData->value();
	std::cout << "testFunc was called. you entered: " << newTaskTitle << '\n';

	// add to db
    gui->m_db.addTask(newTaskTitle);

	// TODO reload the list
    gui->m_taskPack->add(gui->taskBox((gui->m_db.getTaskList()).back()));
    gui->m_taskPack->redraw();
    std::cout << "redrew m_taskPack" << '\n';
}

Fl_Group* GUI::taskBox(Task & t)
{
	Fl_Group* row = new Fl_Group(0, 0, 480, 40);
	row->begin();

	Fl_Box* item = new Fl_Box(0, 0, 480, 40, "");
	item->labelsize(18);
	item->box(FL_UP_BOX);
	item->color(FL_WHITE);
	item->copy_label(t.title.c_str());

	Fl_Check_Button* checkButton = new Fl_Check_Button(445, 10, 20, 20);

	row->end();
	// row->resizable(item);
	return row;
}

int GUI::run()
{
	std::cout << "starting GUI!" << '\n'; 
	m_window = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "HELLO FLTK!");

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
	this->m_taskPack = new Fl_Pack(0, 0, 480, 460);
	this->m_taskPack->type(Fl_Pack::VERTICAL);
	this->m_taskPack->spacing(1);
	this->m_taskPack->begin();

	// these are boxes that are packed nicely
	// each group is a box of the task, and a check box
	std::vector<Fl_Group*> rows;
	// TODO instead of taskTitlesTest, i want the actual db tasks.
	// i need viewTask to give me a vector of tasks i think
    std::vector<Task> tasklist = m_db.getTaskList();
	for (Task & t : tasklist)
	{
		rows.push_back(GUI::taskBox(t));
	}

    std::cout << "drew pack of tasks" << '\n';

    this->m_taskPack->end();
	// this->m_taskPack->resizable(nullptr);

	scroll->end();
	m_window->end();
	m_window->show();

    int running = Fl::run();
    std::cout << "closing GUI" << '\n';
	return running;
}
