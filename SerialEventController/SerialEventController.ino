/*
 Name:		SerialEventController.ino
 Created:	8/24/2018 2:20:33 PM
 Author:	Alva
*/

#include "SerialEventController.h"

enum CMD : byte
{
	TEST = 32,
	START,
	STOP,
	SETSPEED,
	REQSPEED,
	SETP,
	SETI,
	SETD,
	REQCSV_PID      //return a comma separated list of values
};

//example Arduino state that can be set and interrogated
int pidspeed = 100;
float p = 10.1f;
float i = 20.2f;
float d = 30.3f;

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

static void setPara(int cmd, String data) {
	switch (cmd) {
	case SETSPEED:
		pidspeed = data.toInt();
		break;
	case SETP:
		p = data.toFloat();
		break;
	case SETI:
		i = data.toFloat();
		break;
	case SETD:
		d = data.toFloat();
		break;
	}
}

static void motorControl(int cmd, String data) {
	switch (cmd) {
	case START:
		//do something
		break;
	case STOP:
		//do something
		break;
	}
}

static void requestStatus(int cmd, String unused) {
	switch (cmd) {
	case REQSPEED:
		//NOTE: echo the cmd code
		SEController.SendData(cmd, pidspeed);
		break;
	case REQCSV_PID:
		String data = String(p) + "," + String(i) + "," + String(d);
		SEController.SendData(cmd, data);
		break;
	}
}

static void testFunc(int cmd, String unused) {
	String data = "State=" + String(pidspeed) + "," + String(p) + "," + String(i) + "," + String(d);
	SEController.SendData(cmd, data);
}

void setup()
{
	//;;;;; Register Handlers
	SEController.RegisterEventHandler(SETSPEED, setPara);
	SEController.RegisterEventHandler(SETP, setPara);
	SEController.RegisterEventHandler(SETI, setPara);
	SEController.RegisterEventHandler(SETD, setPara);

	SEController.RegisterEventHandler(START, motorControl);
	SEController.RegisterEventHandler(STOP, motorControl);

	SEController.RegisterEventHandler(REQSPEED, requestStatus);
	SEController.RegisterEventHandler(REQCSV_PID, requestStatus);

	SEController.RegisterEventHandler(TEST, testFunc);

	Serial.begin(9600);
}

void loop()
{

}
