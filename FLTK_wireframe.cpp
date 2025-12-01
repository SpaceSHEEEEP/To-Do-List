#include <iostream>
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

int main()
{
	std::cout << "hello world!" << '\n';
	Fl_Window window = Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT, "HELLO FLTK!");

	Fl_Pack buttonsPack = Fl_Pack(510, 10, 110, 460);
	buttonsPack.type(Fl_Pack::VERTICAL);
	buttonsPack.spacing(10);
	buttonsPack.begin();

	// buttons on the side 
	Fl_Button addButton = Fl_Button(0, 0, 80, 30, "add");
	Fl_Button deleteButton = Fl_Button(0, 0, 80,30, "delete");	
	Fl_Button markButton = Fl_Button(0, 0, 80, 30, "mark");
	Fl_Button sortButton = Fl_Button(0, 0, 80, 30, "sort");
	buttonsPack.end();

	// Fl_Scroll(int x, int y, int w, int h, const* char label = 0)
	// this gives a scrollable box
	Fl_Scroll scroll = Fl_Scroll(10, 10, 480, 460);	
	scroll.box(FL_BORDER_BOX);
	scroll.type(Fl_Scroll::VERTICAL_ALWAYS);
	scroll.begin();
		
	// this is in scrollable box and packs boxes nicely
	Fl_Pack pack1 = Fl_Pack(0, 0, 480, 460);
	pack1.type(Fl_Pack::VERTICAL);
	pack1.spacing(1);
	pack1.begin();

	// these are boxes that are packed nicely
	Fl_Group row1 = Fl_Group(0, 0, 480, 40);
	row1.begin();
	Fl_Box item1 = Fl_Box(FL_FLAT_BOX, 0, 0, 480, 40, "buy milk"); 
	item1.labelsize(18);
	item1.box(FL_UP_BOX);
	item1.color(FL_WHITE);

	Fl_Check_Button checkButton(440, 10, 20, 20);
	row1.end();

	// this is task 2
	Fl_Box item2 = Fl_Box(FL_FLAT_BOX, 0, 0, 480, 40, "buy food"); 
	item2.labelsize(18);
	item2.box(FL_UP_BOX);
	item2.color(FL_GREEN);

	pack1.end();
	scroll.end();

	window.end();
	window.show();
	return Fl::run();
}
