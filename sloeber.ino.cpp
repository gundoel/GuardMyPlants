#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2019-09-14 17:31:58

#include "Arduino.h"
#include "debug.h"
#include <Arduino.h>
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Timer.h>
#include "config.h"
#include "MenuData.h"
#include "Sensor.h"
#include "SoilMoistureSensor.h"
#include "WaterLevelSensor.h"
#include "Waterpump.h"
#include "Log.h"
#include "gmputil.hpp"

byte mapHexCodeToKey(unsigned long hexCode) ;
boolean isKeyDebounced() ;
byte mapASCIICodeToKey(byte asciiCode) ;
void showDefaultScreen() ;
void showStringMessage(String message, int line, int offset) ;
byte getButton() ;
char *rpad(char *dest, const char *str, char chr, unsigned char width) ;
char *lpad(char *dest, const char *str, char chr, unsigned char width) ;
char *padc(char chr, unsigned char count) ;
String getPotSizeString(Potsize potSize) ;
String getMoistureString(Moisture moisture) ;
int getNeededMoisture(Moisture moisture) ;
void togglePotSize(Potsize *potSizeVariable) ;
void toggleMoisture(Moisture *moistureVariable) ;
byte processMenuCommand(byte cmdId) ;
byte getNavAction() ;
void refreshMenuDisplay(byte refreshMode) ;
int getPotSizeMililiters(Potsize potSize) ;
int getNeededWaterMilliliters(int pot) ;
void watering(int pump, int neededMilliliters) ;
boolean isWaterOK(int neededWaterMilliliters) ;
void stopPump1() ;
void stopPump2() ;
void switchToRunMode() ;
void switchToErrorMode(String errorMessage) ;
void switchOff() ;
void setup() ;
void loop() ;

#include "GuardMyPlants.ino"


#endif
