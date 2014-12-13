#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Output.H>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <csignal>


/*

Main library, must include the following parameters for compiling: -s $(fltk-config --cxxflags --ldflags)

*/

Fl_Choice *convchoicein;
Fl_Choice *convchoiceout;
Fl_Window *window1;
Fl_Window *window2;
Fl_Choice *gchoice;
Fl_Float_Input *inp1;
Fl_Output *out1;
Fl_Button *convbutton;
Fl_Button *okbutton;

using namespace std;

void quit(Fl_Widget*, void*){
	printf("Exited.\n");
	exit(0);

}

void sigquit(int sig){  // Who dosen't press Ctrl-C?
	printf("\n");
	printf("Caught signal %i\n", sig);
	printf("Exited.\n");
	exit(0);

}

void velconv(Fl_Widget*, void*){

 char cstr[32];
 float n;
 float res;
 int abz;
 n = atof(inp1->value());

 if (convchoicein->value() == 0) {  // MPH to X
 	if (convchoiceout->value() == 0) { //MPH to MPH
 		out1->value(inp1->value());
 	}
 	else if (convchoiceout->value() == 1) { // MPH to kM/H
		res = n * 1.60934;
	}
	else if (convchoiceout->value() == 2) { //MPH to Knot
		res = n * 0.868976;
	}
 }
 else if (convchoicein->value() == 1) { // kM/H to X
 	if (convchoiceout->value() == 0) { // kM/H to MPH
		res = n * 0.621371;
	}
	else if (convchoiceout->value() == 1) { // kM/H to.. It's the same.
		out1->value(inp1->value());
	}
	else if (convchoiceout->value() == 2) { // kM/H to Knot
		res = n * 0.539957;
	}
 }
 else if (convchoicein->value() == 2) { // Knot to X
 	if (convchoiceout->value() == 0) { // Knot to MPH
        res = n * 1.15078;
    }
    else if (convchoiceout->value() == 1) { // Knot to kM/H
        res = n * 1.852;
    }
    else if (convchoiceout->value() == 2) {
        out1->value(inp1->value());
    }
 }

 sprintf(cstr, "%.2f", res);
 out1->value(cstr);
}

int main() {
  signal(SIGINT, sigquit);  //Save when Ctrl-C is pressed
  signal(SIGTERM, sigquit); //And save when signal 15 is caught
  window2 = new Fl_Window(310,190, "Velocity converter");
  window2->callback(&quit);
  convchoicein = new Fl_Choice(80,30,200,20, "From");
  convchoiceout = new Fl_Choice(80,60,200,20, "To");
  Fl_Menu_Item opts[] = { // Common units for now
   {"MPH"},
   {"kM/H"},
   {"Knot"},
   {0}};
  convchoicein->menu(opts);
  convchoiceout->menu(opts);
  inp1 = new Fl_Float_Input(80,90,150,20, "Input");
  inp1->callback(&velconv);  // Added callback for inp1, it will convert when you hit enter on the input field
  out1 = new Fl_Output(80,150,160,20, "Output");
  convbutton = new Fl_Button(110,120,70,20, "Convert");
  convbutton->type(FL_NORMAL_BUTTON);
  inp1->handle(1);
  out1->handle(1);
  out1->value("NaN");
  convbutton->callback(&velconv);
  window2->end();
  window2->show();
  Fl::run();
}
