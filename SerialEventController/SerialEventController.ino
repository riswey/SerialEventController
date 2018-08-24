/*
 Name:		SerialEventController.ino
 Created:	8/24/2018 2:20:33 PM
 Author:	Alva
*/

#include "SerialEventController.h"

enum CMD {
	DESIREDSPEED = 34,
	START,
	STOP,
	REQUESTSPEED
};

//example Arduino state that can be set and interrogated
int pidspeed = 100;

void serialEvent() {
	while (Serial.available()) {
		char in_char = (char)Serial.read();
		//;;;;; NOTE
		//Add serial input to SerialEventController
		//Can do in loop() also
		SEController.AddCharacter(in_char);
	}
}

//;;;;; SEController calls these functions when registered character appears in serial line

//
static void setPIDSpeed(int cmd, String data) {
	pidspeed = data.toInt();
	Serial.println(String(pidspeed)); //Can send on serial output as usual. Client looks for: SHIFT_OUT + SHIFT_OUT... starting packed signature");
}

static void someOtherEvents(int cmd, String data) {
	switch (cmd) {
	case START:
		//do something
		break;
	case STOP:
		//do something
		break;
	}
}

static void requestPIDSpeed(int cmd, String data) {
	switch (cmd) {
	case REQUESTSPEED:
		//NOTE: echo the cmd code
		SEController.SendData(cmd, pidspeed);
		break;
	}
}

void setup()
{
	//;;;;; NOTE
	//Single function pattern
	SEController.RegisterEventHandler(DESIREDSPEED, setPIDSpeed);
	//Switch pattern
	SEController.RegisterEventHandler(START, someOtherEvents);
	SEController.RegisterEventHandler(STOP, someOtherEvents);
	//Request data
	SEController.RegisterEventHandler(REQUESTSPEED, requestPIDSpeed);

	Serial.begin(9600);

	//auto update client
	SEController.SendData(REQUESTSPEED, pidspeed);

}

void loop()
{

}
