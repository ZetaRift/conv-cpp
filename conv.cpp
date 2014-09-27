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

#define HEADER "TEMPCONVFL"
#define TEXT "Terminus"


/*

Main library, must include the following parameters for compiling: -s $(fltk-config --cxxflags --ldflags)

*/



struct data{
	char header[32];
	char text[64];
	int posin;
        int posout;

};

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
 
void quit(Fl_Widget*, void*){  // Saves the last used conversion choice integer value to conf.dat then exits
	std::fstream fd;
	data config;
	std::memset(config.header, '\0', 32);
	std::memset(config.text, '\0', 64);
	std::strcpy(config.header, HEADER);
	std::strcpy(config.text, TEXT);
	fd.open("conf.dat", std::ios::out | std::ios::binary);
        config.posin = convchoicein->value();
        config.posout = convchoiceout->value();
        fd.write((char*)&config, 128);
        fd.close();
	printf("Exited.\n");
	exit(0);

}

void sigquit(int sig){  // Saves the last used conversion choice integer value to conf.dat then exits
	printf("\n");
	printf("Caught signal %i, saving conf..\n", sig);
	std::fstream fd;
	data config;
	std::memset(config.header, '\0', 32);
	std::memset(config.text, '\0', 64);
	std::strcpy(config.header, HEADER);
	std::strcpy(config.text, TEXT);
	fd.open("conf.dat", std::ios::out | std::ios::binary);
        config.posin = convchoicein->value();
        config.posout = convchoiceout->value();
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
	}
	else {
	 puts("OK");
	 fd.read((char*)&config, 128);
	 convchoicein->value(config.posin);
	 convchoiceout->value(config.posout);
	 fd.close();
	}
}

void tempconv(Fl_Widget*, void*){

 char cstr[32];
 float n;
 float res;
 int abz;
 n = atof(inp1->value());

 if (n > 999999999) {
  out1->value("Overflow");
  
 }
 else {


  if (convchoicein->value() == 0) { // 0 

   if (n < -273.15) {
    abz = 1;
   }
   else {

    abz = 0;
    if (convchoiceout->value() == 0) { // 0 0
     res = n;
    }

    else if (convchoiceout->value() == 1) { // 0 1
     res = n * 1.8 + 32;
    }

    else if (convchoiceout->value() == 2) { // 0 2
     res = n + 273.15;
    }
   }
  }


  else if (convchoicein->value() == 1) { // 1

   if (n < -459.67) {
    abz = 1;
   }
   else {
    
    abz = 0;
    if (convchoiceout->value() == 0) { // 1 0
     res = (n - 32) * 5 / 9;
    }
    else if (convchoiceout->value() == 1) { // 1 1
     res = n;
    }
    else if (convchoiceout->value() == 2) { // 1 2
     res = (n - 32) * 5 / 9;
     res = res + 273.15;
    }
   }
  }

  else if (convchoicein->value() == 2) {

   if (n < 0) {
    abz = 1;
   }
   else {

    abz = 0;
    if (convchoiceout->value() == 0) { // 2 0
     res = n - 273.15;
    }
    else if (convchoiceout->value() == 1) { // 2 1
     res = n - 273.15;
     res = res * 1.8 + 32;
    }
    else if (convchoiceout->value() == 2) { // 2 2
     res = n;
    }
   }
  }
   sprintf(cstr, "%.2f", res); // the '%.2f' limits the output floating point to two decimal places  res = (n + 273.15) * 9 / 5;
   if (abz == 0) {

    out1->value(cstr);
   }
   else {
    out1->value("Below Absolute Zero");
   }
 }
}

int main() {
  signal(SIGINT, sigquit);  //Save when Ctrl-C is pressed
  signal(SIGTERM, sigquit);
  window2 = new Fl_Window(310,190, "Temperature converter");
  window2->callback(&quit); // Will save conf and exit when you close the window, removed the exit button.
  convchoicein = new Fl_Choice(80,30,200,20, "From");  // Conversion choices, the value of which choice is selected is an integer, for example: "C to F" is 0, "F to C" is 1, and so on..
  convchoiceout = new Fl_Choice(80,60,200,20, "To");
  Fl_Menu_Item opts[] = {
   {"Celsius"},
   {"Fahrenheit"},
   {"Kelvin"},
   {0}};
  convchoicein->menu(opts);
  convchoiceout->menu(opts);
  inp1 = new Fl_Float_Input(80,90,150,20, "Input");
  inp1->callback(&tempconv);  // Added callback for inp1, it will convert when you hit enter on the input field
  out1 = new Fl_Output(80,120,160,20, "Output");
  convbutton = new Fl_Button(110,150,70,20, "Convert");
  convbutton->type(FL_NORMAL_BUTTON);
  inp1->handle(1);
  out1->handle(1);
  out1->value("NaN");
  convbutton->callback(&tempconv);
  window2->end();
  window2->show();
  read_conf();
  Fl::run();
}
