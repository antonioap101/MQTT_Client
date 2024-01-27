// utilityFunctions.h
#ifndef OLED_SCREEN_H
#define OLED_SCREEN_H

#include <Arduino.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define SCREEN_I2C_ADDRESS 0x3D

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled;

void setupOledScreen();

void sensorStartScreen();

void printOnOLEDScreen(String title, String arg, int resultStringSize, const uint8_t* font);

#endif // UTILITY_FUNCTIONS_H
