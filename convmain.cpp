#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "libconv.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>

/*

To be compiled to an executable and linked with the main library  

*/

int argc;
char **argv;

Fl_Choice *gchoice;
Fl_Window *window1;
Fl_Button *okbutton;


void quit(Fl_Widget*, void*){

 exit(0);
}
void selconv(Fl_Widget*, void*){
 window1->hide();

 if (gchoice->value() == 0) {
  Conv conv;
  conv.temprun();  //Run the temperature unit converter
 }
 else if (gchoice->value() == 1) {
  exit(0);
 }
}

int main(){
  window1 = new Fl_Window(220,110, "Select");
  window1->callback(&quit);
  okbutton = new Fl_Button(90,70,30,20, "Ok");
  okbutton->type(FL_NORMAL_BUTTON);
  gchoice = new Fl_Choice(10,30,200,20);
  Fl_Menu_Item opts[] = {
   {"Temperature Converter"},
   {"NaN"}, //This option will just exit
   {0}};
  gchoice->menu(opts);
  okbutton->callback(&selconv);
  window1->end();
  window1->show(argc, argv);
  return Fl::run();

}
