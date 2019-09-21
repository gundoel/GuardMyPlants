#ifndef _GMPMenu_
#define _GMPMenu_
#include "MenuManager.h"
#include <avr/pgmspace.h>

/*
Generated using LCD Menu Builder at http://lcd-menu-bulder.cohesivecomputing.co.uk/.
*/

enum GMPMenuCommandId
{
  menuCommandBack = 0,
  menuCommandSettings,
  menuCommandPot1Size_Selection,
  menuCommandPot1Moisture_Selection,
  menuCommandPot2Size_Selection,
  menuCommandPot2Moisture_Selection,
  menuCommandTest,
  menuCommandWaterlevel,
  menuCommandPot1Moisture,
  menuCommandPot2Moisture,
  menuCommandPot1Pump,
  menuCommandPot2Pump,
  menuCommandReset
};

PROGMEM const char GMPMenu_back[] = "Zurueck";
PROGMEM const char GMPMenu_exit[] = "Menue verlass.";

PROGMEM const char GMPMenu_1_1[] = "Groesse Topf 1";
PROGMEM const char GMPMenu_1_2[] = "Feucht. Topf 1";
PROGMEM const char GMPMenu_1_3[] = "Groesse Topf 2";
PROGMEM const char GMPMenu_1_4[] = "Feucht. Topf 2";
PROGMEM const MenuItem GMPMenu_List_1[] = {{menuCommandPot1Size_Selection, GMPMenu_1_1}, {menuCommandPot1Moisture_Selection, GMPMenu_1_2}, {menuCommandPot2Size_Selection, GMPMenu_1_3}, {menuCommandPot2Moisture_Selection, GMPMenu_1_4}, {menuCommandBack, GMPMenu_back}};

PROGMEM const char GMPMenu_2_1[] = "Wasserstand";
PROGMEM const char GMPMenu_2_2[] = "Feucht. Topf 1";
PROGMEM const char GMPMenu_2_3[] = "Feucht. Topf 2";
PROGMEM const char GMPMenu_2_4[] = "Pumpe Topf 1";
PROGMEM const char GMPMenu_2_5[] = "Pumpe Topf 2";
PROGMEM const MenuItem GMPMenu_List_2[] = {{menuCommandWaterlevel, GMPMenu_2_1}, {menuCommandPot1Moisture, GMPMenu_2_2}, {menuCommandPot2Moisture, GMPMenu_2_3}, {menuCommandPot1Pump, GMPMenu_2_4}, {menuCommandPot2Pump, GMPMenu_2_5}, {menuCommandBack, GMPMenu_back}};

PROGMEM const char GMPMenu_1[] = "Einstellungen";
PROGMEM const char GMPMenu_2[] = "Test";
PROGMEM const char GMPMenu_3[] = "Einst. zuruecks";
PROGMEM const MenuItem GMPMenu_Root[] = {{menuCommandSettings, GMPMenu_1, GMPMenu_List_1, menuCount(GMPMenu_List_1)}, {menuCommandTest, GMPMenu_2, GMPMenu_List_2, menuCount(GMPMenu_List_2)}, {menuCommandReset, GMPMenu_3}, {menuCommandBack, GMPMenu_exit}};

/*
case menuCommandPot1Size_Selection :
	break;
case menuCommandPot1Moisture_Selection :
	break;
case menuCommandPot2Size_Selection :
	break;
case menuCommandPot2Moisture_Selection :
	break;
case menuCommandWaterlevel :
	break;
case menuCommandPot1Moisture :
	break;
case menuCommandPot2Moisture :
	break;
case menuCommandPot1Pump :
	break;
case menuCommandPot2Pump :
	break;
case menuCommandReset :
	break;
*/

/*
<?xml version="1.0"?>
<RootMenu xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
	<!-- Attributes BackText and ExitText removed, since not needed for GMP-Menu. Menu Builder creates constant nonetheless.
		Remove constants GMPMenu_back and GMP_exit from MenuData.h after Building Menu -->
	<Config IdPrefix="menuCommand" VarPrefix="GMPMenu" UseNumbering="false"
	IncludeNumberHierarchy="false" MaxNameLen="15" MenuBackFirstItem="false" AvrProgMem="true" BackText="Zurueck" ExitText="Menue verlass."/>
	<MenuItems>
		<Item Id="Settings" Name="Einstellungen">
			<MenuItems>
				<!-- _Selection suffix to mark menu as selection menu -->
				<Item Id="Pot1Size_Selection" Name="Groesse Topf 1"/>
				<Item Id="Pot1Moisture_Selection" Name="Feucht. Topf 1"/>
				<Item Id="Pot2Size_Selection" Name="Groesse Topf 2"/>
				<Item Id="Pot2Moisture_Selection" Name="Feucht. Topf 2"/>
			</MenuItems>
		</Item>
		<Item Id="Test" Name="Test">
			<MenuItems>
				<Item Id="Waterlevel" Name="Wasserstand"/>
				<Item Id="Pot1Moisture" Name="Feucht. Topf 1"/>
				<Item Id="Pot2Moisture" Name="Feucht. Topf 2"/>
				<Item Id="Pot1Pump" Name="Pumpe Topf 1"/>
				<Item Id="Pot2Pump" Name="Pumpe Topf 2"/>
			</MenuItems>
		</Item>
		<Item Id="Reset" Name="Einst. zuruecks."/>
	</MenuItems>
</RootMenu>
*/
#endif
