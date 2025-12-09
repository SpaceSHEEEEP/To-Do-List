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

#include "FLTK_wireframe.hpp"

// CONSTANTS
const int WINDOW_WIDTH{640};
const int WINDOW_HEIGHT{480};

static std::vector<std::string> taskTitlesTest = {"buy chicken", "buy fish", "buy coffee", "eat fried chicken", "drink water", "visit friend", "fry chicken", "find a job", "get married", "get money", "grind for honor", "get rich", "drive car", "make to do list", "learn vim"};

Fl_Group* newTaskBox(std::string taskTitle)
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

int FLTKStart()
{

	std::cout << "hello world!" << '\n'; // TODO FIX 640
	Fl_Window* window = new Fl_Window(640, WINDOW_HEIGHT, "HELLO FLTK!");

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

	// Fl_Scroll(int x, int y, int w, int h, const* char label = 0)
	// this gives a scrollable box
	Fl_Scroll* scroll = new Fl_Scroll(10, 10, 480, 460);	
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
	for (std::string & s : taskTitlesTest)
	{
		std::cout << s << '\n';
		rows.push_back(newTaskBox(s));
	}

	pack1->end();
	// pack1->resizable(nullptr);
	scroll->end();

	window->end();
	window->show();
	return Fl::run();
}
