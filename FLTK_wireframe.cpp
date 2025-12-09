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

// CONSTANTS
const int WINDOW_WIDTH{640};
const int WINDOW_HEIGHT{480};

Fl_Group* newTaskBox(std::string & taskTitle)
{
	Fl_Group* row = new Fl_Group(0, 0, 480, 40);
	row->begin();

	Fl_Box* item1 = new Fl_Box(FL_FLAT_BOX, 5, 5, 430, 30, taskTitle.c_str()); 
	item1->labelsize(18);
	item1->box(FL_UP_BOX);
	item1->color(FL_WHITE);
	// item1->copy_label(taskTitle.c_str());

	Fl_Check_Button* checkButton = new Fl_Check_Button(445, 10, 20, 20);

	row->end();
	// row->resizable(item1);
	return row; 
}

static std::vector<std::string> taskTitlesTest = {"buy chicken", "buy fish", "buy coffee"};

int main()
{

	std::cout << "hello world!" << '\n';
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
