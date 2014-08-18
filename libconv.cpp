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
#include "libconv.h"

#define HEADER "TEMPCONVFL"
#define TEXT "Terminus"


/*

Main library, must include the following parameters for compiling: -s $(fltk-config --cxxflags --ldflags)

*/

int argc;
char **argv;



struct data{
	char header[32];
	char text[64];
	int pos;

};

Fl_Choice *convchoice;
Fl_Window *window1;
Fl_Float_Input *inp1;
Fl_Output *out1;
Fl_Button *convbutton;

using namespace std;
 
void quit(Fl_Widget*, void*){  // Saves the last used conversion choice integer value to conf.dat then exits
	std::fstream fd;
	data config;
	std::memset(config.header, '\0', 32);
	std::memset(config.text, '\0', 64);
	std::strcpy(config.header, HEADER);
	std::strcpy(config.text, TEXT);
	fd.open("conf.dat", std::ios::out | std::ios::binary);
        config.pos = convchoice->value();
        fd.write((char*)&config, 128);
        fd.close();
	printf("Exited.\n");
	exit(0);

}

void read_conf(){
	std::fstream fd;
	data config;
	printf("checking if conf.dat exists...");
	fd.open("conf.dat", std::ios::in | std::ios::binary);
	if (fd == NULL) {
	 puts("no");
	 convchoice->value(0);
	}
	else {
	 puts("OK");
	 fd.read((char*)&config, 128);
	 convchoice->value(config.pos);
	 fd.close();
	}
}


void tempconv(Fl_Widget*, void*){

 char cstr[32];
 float n;
 float res;

 if (convchoice->value() == 0) {  //C to F
  n = atof(inp1->value());
  res = n * 1.8 + 32;
 }

 else if (convchoice->value() == 1) { //F to C
  n = atof(inp1->value());
  res = (n - 32) * 5 / 9;

 }

 else if (convchoice->value() == 2) { //K to C
  n = atof(inp1->value());
  res = n - 273.15;
 }

 else if (convchoice->value() == 3) { //C to K
  n = atof(inp1->value());
  res = n + 273.15;
 }

 else if (convchoice->value() == 4) { //F to R
  n = atof(inp1->value());
  res = n + 459.67;
 }

 else if (convchoice->value() == 5) { //C to R
  n = atof(inp1->value());
  res = (n + 273.15) * 9 / 5;
 }

 sprintf(cstr, "%.2f", res); // the '%.2f' limits the output floating point to two decimal places
 out1->value(cstr);

}



void Conv::temprun() {
  window1 = new Fl_Window(310,160, "Temperature converter");
  window1->callback(&quit); // Will save conf and exit when you close the window, removed the exit button.
  convchoice = new Fl_Choice(80,30,200,20);  // Conversion choices, the value of which choice is selected is an integer, for example: "C to F" is 0, "F to C" is 1, and so on..
  Fl_Menu_Item opts[] = {
   {"Celsius to Fahrenheit"},
   {"Fahrenheit to Celsius"},
   {"Kelvin to Celsius"},
   {"Celsius to Kelvin"},
   {"Fahrenheit to Rankine"},
   {"Celsius to Rankine"},
   {0}};
  convchoice->menu(opts);
  inp1 = new Fl_Float_Input(80,60,150,20, "Input");
  inp1->callback(&tempconv);  // Added callback for inp1, it will convert when you hit enter on the input field
  out1 = new Fl_Output(80,90,150,20, "Output");
  convbutton = new Fl_Button(110,120,70,20, "Convert");
  convbutton->type(FL_NORMAL_BUTTON);
  inp1->handle(1);
  out1->handle(1);
  out1->value("NaN");
  convbutton->callback(&tempconv);
  window1->end();
  window1->show(argc, argv);
  read_conf();
  Fl::run();
}
