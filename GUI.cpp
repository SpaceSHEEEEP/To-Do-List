// a lot of copy and pasting, fix this TODO

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_set>
#include <algorithm>

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

Fl_Group* GUI::createTaskBox(Task & t)
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
	item->copy_label(t.title.c_str());
	if (t.completed == 0) item->color(fl_rgb_color(253, 255, 182));
    else item->color(fl_rgb_color(228, 241, 238));

	Fl_Check_Button* checkButton = new Fl_Check_Button(445, 10, 20, 20);
    checkButton->type(FL_TOGGLE_BUTTON);
    checkButton->callback(updateIDs, this);

	row->end();
	// row->resizable(item);
	return row;
}

void GUI::refreshScroll()
{
    m_taskPack->clear();
    for (Task & t : m_Database.getTaskList())
    {
        if (t.completed == 0) m_taskPack->add(createTaskBox(t));
    }    
    for (Task & t : m_Database.getTaskList())
    {
        if (t.completed == 1) m_taskPack->add(createTaskBox(t));
    }  
    m_taskPack->redraw();
    m_taskPack->parent()->redraw();
}

void GUI::getNewTaskTitle(Fl_Widget* widget, void* userdata)
{
	// i want to eventually from the userData->value() input the value into addTask in sql.hpp.
	Fl_Input* inputData = static_cast<Fl_Input*>(widget);
    GUI* gui = static_cast<GUI*>(userdata);

    std::string newTaskTitle = inputData->value();
	std::cout << "getNewTaskTitle was called. you entered: " << newTaskTitle << '\n';
    inputData->value("");

	// add to db
    gui->m_Database.addTask(newTaskTitle);

	// TODO reload the list
    gui->m_taskPack->add(gui->createTaskBox((gui->m_Database.getTaskList()).back()));
    gui->m_taskPack->redraw();
    gui->m_taskPack->parent()->redraw();
    std::cout << "redrew m_taskPack" << '\n';
}

void GUI::addCallBack(Fl_Widget* widget, void* userdata)
{
    // widget is the add button, userdata is Fl_Input
    // need to find gui
    std::cout << "addCallBack ran" << '\n'; return;
    GUI* gui = static_cast<GUI*>(userdata);
    std::string newTaskTitle = gui->m_textBox->value();
    
    if (gui->m_Database.addTask(newTaskTitle)) std::cout << "added: " << newTaskTitle << '\n';
    else std::cout << "failed to add: " << newTaskTitle << '\n';

    gui->m_ids.clear();
    
    // redraw taskBoxes
    gui->refreshScroll();
}

void GUI::deleteCallBack(Fl_Widget* widget, void* userdata)
{
    std::cout << "delete button pressed" << '\n';
    // TODO this also calls the getNewTaskTitle function? idk why 

    GUI* gui = static_cast<GUI*>(userdata);
    for (long l : gui->m_ids)
    {
        if (gui->m_Database.deleteTask(l)) std::cout << "deleted " << l << '\n';
        else std::cout << "failed to delete " << l << '\n';
    }
    (gui->m_ids).clear();
    
    // redraw taskBoxes
    gui->refreshScroll();
}

void GUI::markCallBack(Fl_Widget* widget, void* userdata)
{
    std::cout << "mark button pressed" << '\n';
    // TODO this also calls the getNewTaskTitle function? idk why 

    GUI* gui = static_cast<GUI*>(userdata);
    for (long id_num : gui->m_ids)
    {
        if (gui->m_Database.markTask(id_num)) std::cout << "marked " << id_num << '\n';
        else std::cout << "failed to mark " << id_num << '\n';
    }
    (gui->m_ids).clear();
    
    // redraw taskBoxes
    gui->refreshScroll();
}

void GUI::sortCallBack(Fl_Widget* widget, void* userdata)
{
    GUI* gui = static_cast<GUI*>(userdata);
    gui->refreshScroll();
}

void GUI::updateIDs(Fl_Widget* widget, void* userdata)
{
    Fl_Box* id_num_box_ptr = static_cast<Fl_Box*>(widget->parent()->child(0));
    GUI* gui = static_cast<GUI*>(userdata);
    Fl_Check_Button* checkButton = static_cast<Fl_Check_Button*>(widget);
    long id_num = std::stol(std::string(id_num_box_ptr->label()));

    if (static_cast<int>(checkButton->value()) == 1) gui->m_ids.insert(id_num);
    else (gui->m_ids).erase(id_num);
}

int GUI::run()
{
	std::cout << "starting GUI!" << '\n'; 
    int running = Fl::run();
    std::cout << "closing GUI" << '\n';
	return running;
}

GUI::GUI()
{
	m_window = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "TO-DO-LIST!");
	m_textBox = new Fl_Input(10, 10, 480, 40, "");
	m_textBox->callback(GUI::getNewTaskTitle, this);

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
    this->refreshScroll();

	scroll->end();

	Fl_Pack* buttonsPack = new Fl_Pack(510, 10, 110, 460);
	buttonsPack->type(Fl_Pack::VERTICAL);
	buttonsPack->spacing(10);
	buttonsPack->begin();

	// buttons on the side 
	Fl_Button* addButton = new Fl_Button(0, 0, 80, 30, "add");
    addButton->callback(GUI::addCallBack, m_textBox);

	Fl_Button* deleteButton = new Fl_Button(0, 0, 80,30, "delete");
    deleteButton->callback(GUI::deleteCallBack, this);

	Fl_Button* markButton = new Fl_Button(0, 0, 80, 30, "mark");
    markButton->callback(GUI::markCallBack, this);

	Fl_Button* sortButton = new Fl_Button(0, 0, 80, 30, "sort");
    sortButton->callback(GUI::sortCallBack, this);

	buttonsPack->end();

	m_window->end();
	m_window->show();
}
