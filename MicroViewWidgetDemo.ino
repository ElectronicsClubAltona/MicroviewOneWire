
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2 // for microview veroboard

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
/*
	MicroView Arduino Library
	Copyright (C) 2014 GeekAmmo

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <MicroView.h>

// The Arduino build process doesn't create a prototype for the spin function,
// (probably because it has a function pointer as a parameter)
// so it's added here.
void spin(int16_t lowVal, int16_t highVal, int16_t stepSize,
          unsigned long stepDelay, void (*drawFunction)(int16_t val));

MicroViewWidget *widget1, *widget2;

int16_t prevVal; // previous widget value

void setup() {
  uView.begin();
  uView.clear(PAGE);
  // start serial port
  //1  Serial.begin(9600);
  //1  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
}

void loop() {
  int val;
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  //1Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  //1Serial.println("DONE");

  //1Serial.print("Temperature is: ");
  //1Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?
  // You can have more than one IC on the same bus.
  // 0 refers to the first IC on the wire

  uView.clear(PAGE);
  //  uView.setFontType(1);
  uView.print((float)sensors.getTempCByIndex(0), 2);
  uView.print(" \n");
  uView.display();
  //  uView.setFontType(0);
  delay(5000);

}

// Function to update widget1
void update1widget(int16_t val) {
  widget1->setValue(val);
}

// Function to update widget1 and widget2
void update2widgets(int16_t val) {
  widget1->setValue(val);
  widget2->setValue(val);
}

// Update function for Demo 8
void customSlider0(int16_t val) {
  widget1->setValue(val);
  uView.setCursor(widget1->getX() + 34, widget1->getY() + 1);
  uView.print("0x");
  if (val < 0x10) { // add leading 0 if necessary. Only 2 digits supported.
    uView.print('0');
  }
  uView.print(val, HEX);
}

// Update function for Demo 9
void customSlider1(int16_t val) {
  widget1->setValue(val);
  uint8_t offsetY = widget1->getY() - 10;
  uint8_t offsetX = widget1->getX() + 14;
  uView.setCursor(offsetX, offsetY);
  uView.print("      "); // erase the previous value in case it's longer
  // calculate the offset to centre the value
  offsetX += ((widget1->getMaxValLen() - widget1->getValLen()) * 3);
  uView.setCursor(offsetX, offsetY);
  uView.print(val);
}

// Update function for Demo 10
void customSlider2(int16_t val) {
  uView.setCursor(widget1->getX() + 1, widget1->getY() + 24);
  widget1->drawNumValue(val);
  // calculate to reverse the pointer direction
  widget1->setValue((int16_t) ((int32_t) widget1->getMaxValue() +
                               (int32_t) widget1->getMinValue() -
                               (int32_t) val));
}

// Update function for Demo 11
void customSlider3(int16_t val) {
  int16_t maxVal = widget1->getMaxValue();
  uint16_t range = (uint16_t) (maxVal - widget1->getMinValue());
  uint8_t offsetX = widget1->getX() + 9;

  // erase previous value.
  // pointer position is calculated the same way as the widget code.
  uint8_t offsetY = (float)(uint16_t)(maxVal - prevVal) / (float)range * 40;
  uView.setCursor(offsetX, offsetY);
  uView.print("  "); // This is being lazy. Should calculate width for value.

  // draw new value
  offsetY = (float)(uint16_t)(maxVal - val) / (float)range * 40;
  uView.setCursor(offsetX, offsetY);
  widget1->drawNumValue(val);

  widget1->setValue(val);
}

// Update function for Demo 12
void customGauge0(int16_t val) {
  widget1->setValue(val);

  uView.setCursor(widget1->getX() - 17, widget1->getY() + 19);
  uView.setFontType(1);
  // add leading space if necessary, to right justify.
  // only 2 digit (plus decimal) numbers are supported.
  if (val < 100) {
    uView.print(' ');
  }
  uView.print((float)val / 10, 1);
  uView.setFontType(0);
}

// Update function for Demo 13
void customGauge1(int16_t val) {
  widget1->setValue(val);
  uView.setCursor(widget1->getX() - 2, widget1->getY() + 9);
  uView.print((char)(val + 'A' - 1));
}

// Clear the screen buffer and draw the demo number in the corner
void demoNumber(int num) {
  uView.clear(PAGE);
  uView.setCursor(0, 0);
  uView.print(num);
  uView.print(":");
}

// Spin up, then down, through the values.
//
// For each value, call the update function and display the new screen.
void spin(int16_t lowVal, int16_t highVal, int16_t stepSize,
          unsigned long stepDelay, void (*drawFunction)(int16_t val)) {
  drawFunction(lowVal);
  uView.display();
  prevVal = lowVal;
  delay(1500);

  for (int16_t i = lowVal + stepSize; i <= highVal; i += stepSize) {
    drawFunction(i);
    uView.display();
    prevVal = i;
    delay(stepDelay);
    if ((i == 0) && (lowVal != 0)) { // pause briefly for a value of 0
      delay(750);
    }
  }

  delay(1500);

  for (int16_t i = highVal; i >= lowVal; i -= stepSize) {
    drawFunction(i);
    uView.display();
    prevVal = i;
    delay(stepDelay);
    if ((i == 0) && (lowVal != 0)) { // pause briefly for a value of 0
      delay(750);
    }
  }

  delay(1500);
}
