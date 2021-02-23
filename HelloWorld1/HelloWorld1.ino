#include <MicroView.h>

/*
* select Arduino Uno for use with the uView OLED
 * 13 July - 2015
 Tuesday 23 February  14:54:42 AEDT 2021   revived on OPTI 745 XFCE Xubuntu
 */
void setup() {
  uView.begin();                  // start MicroView
  uView.clear(PAGE);              // clear page
  //  uView.print("Hello\nWorld\n");	// display HelloWorld
  uView.println();
  uView.print("     ");
  uView.print(6);
  uView.display();
}

void loop () {
  for (int v=0; v<30; v++){ 
    //   uView.begin();                  // start MicroView
    //  uView.clear(PAGE);              // clear page
    //  uView.println();  
    uView.print(".");
    uView.print(v);
    delay(1000);     
    uView.display();
  }
  uView.begin();                  // start MicroView
  uView.clear(PAGE);              // clear page
}












