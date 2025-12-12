#include <iostream>
#include <memory>
#include <vector>
#include <unordered_set>

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

Fl_Group* GUI::taskBox(Task & t)
{
	Fl_Group* row = new Fl_Group(0, 0, 480, 40);
	row->begin();

    // make an invisible box that has id_num so that i dont need to make a struct in heap and
    // worry about freeing memory later
    // since FLTK can free its own memory and i cant
    Fl_Box* id_num = new Fl_Box(0, 0, 0, 0, nullptr);
    id_num->copy_label(std::to_string(t.id_num).c_str()); 

	Fl_Box* item = new Fl_Box(0, 0, 480, 40, "");
	item->labelsize(18);
	item->box(FL_UP_BOX);
	item->color(FL_WHITE);
	item->copy_label(t.title.c_str());

	Fl_Check_Button* checkButton = new Fl_Check_Button(445, 10, 20, 20);
    checkButton->type(FL_TOGGLE_BUTTON);
    checkButton->callback(getID, this);

	row->end();
	// row->resizable(item);
	return row;
}

void GUI::getNewTaskTitle(Fl_Widget* widget, void* userdata)
{
	// i want to eventually from the userData->value() input the value into addTask in sql.hpp.
	Fl_Input* inputData = static_cast<Fl_Input*>(widget);
    GUI* gui = static_cast<GUI*>(userdata);

    std::string newTaskTitle = inputData->value();
	std::cout << "getNewTaskTitle was called. you entered: " << newTaskTitle << '\n';

	// add to db
    gui->m_db.addTask(newTaskTitle);

	// TODO reload the list
    gui->m_taskPack->add(gui->taskBox((gui->m_db.getTaskList()).back()));
    gui->m_taskPack->redraw();
    std::cout << "redrew m_taskPack" << '\n';
}

void GUI::deleteButtonFunction(Fl_Widget* widget)
{
    std::cout << "delete button pressed" << '\n';
    // TODO this also calls the getNewTaskTitle function? idk why 
}

void GUI::getID(Fl_Widget* widget, void* userdata)
{
    std::cout << "getID called " <<'\n';
    Fl_Box* id_num_box_ptr = static_cast<Fl_Box*>(widget->parent()->child(0));
    long id_num = std::stol(std::string(id_num_box_ptr->label()));
    std::cout << "id_num: " << id_num <<'\n';

    GUI* gui = static_cast<GUI*>(userdata);
    (gui->m_ids).insert(id_num);

    std::cout << "now gui->m_ids has ids: ";
    for (long l : gui->m_ids)
    {
        std::cout << l << ', ';
    }
    std::cout << '\n';
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
	Fl_Button* addButton = new Fl_Button(0, 0, 80, 30, "add");
    // addButton->callback(GUI::deleteButtonFunction, this);

	Fl_Button* deleteButton = new Fl_Button(0, 0, 80,30, "delete");
    // deleteButton->callback(GUI::deleteButtonFunction, this);

	Fl_Button* markButton = new Fl_Button(0, 0, 80, 30, "mark");
    // markButton->callback(GUI::deleteButtonFunction, this);

	Fl_Button* sortButton = new Fl_Button(0, 0, 80, 30, "sort");
    // sortButton->callback(GUI::deleteButtonFunction, this);

	buttonsPack->end();

	Fl_Input* taskGetter = new Fl_Input(10, 10, 480, 40, "");
	taskGetter->callback(GUI::getNewTaskTitle, this);

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
