#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2019-08-29 22:17:18

#include "Arduino.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <AnalogMatrixKeypad.h>
#include "MenuData.h"
#include "Sensor.h"
#include "SoilMoistureSensor.h"
#include "WaterLevelSensor.h"
#include "Waterpump.h"
#include "Log.h"
#include "config.h"
#include "gmputil.hpp"

void setup() ;
void showDefaultScreen() ;
void showStateMessage(String message) ;
byte getButton() ;
char* rpad(char *dest, const char *str, char chr, unsigned char width) ;
char* lpad(char *dest, const char *str, char chr, unsigned char width) ;
char* padc(char chr, unsigned char count) ;
String getPotSizeString(int potSize) ;
String getMoistureString(int moisture) ;
void togglePotSize(Potsize *potSizeVariable) ;
void toggleMoisture(Moisture *moistureVariable) ;
byte toggleSelection(byte cmdId) ;
byte processMenuCommand(byte cmdId) ;
byte getNavAction() ;
void refreshMenuDisplay(byte refreshMode) ;
int getPotSizeMililiters(Potsize potSize) ;
double getPumpTimeMilliSeconds(int neededWaterMilliliters) ;
boolean waterPlant(int pot) ;
void loop() ;

#include "GuardMyPlants.ino"


#endif
