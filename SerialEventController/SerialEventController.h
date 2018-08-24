
/*
* Packets look like this:
* SHIFT_OUT + SHIFT_OUT + COMMAND_CHR [+ body string] + NEW_LINE
*/

#ifndef SERIAL_EVENT_CONTROLLER
#define SERIAL_EVENT_CONTROLLER

typedef void(*serialeventhandler_t)(int chr, String data);

#define BUFFERLIMIT 200
#define MAXEVENTS 10

class SerialEventController {
	//just hardcoded for now
	String SIGNATURE = "\x0E\x0E";    //Shift Out
	const char terminal = '\n';             //\n
	String buf = "";  //remember: volatile if changed by interrupt!

	serialeventhandler_t eventfuncs[MAXEVENTS];
	String eventindex = "";   //chr at position maps to eventfuncs[]

	int HandleLine(const String line) { //must be pre-trimmed!
		if (line.length() < 3) return 1;  //header failed
		if (line.substring(0, 2) != SIGNATURE) return 1;  //Signature failed
		char chr = line[2];
		String data = line.substring(3);

		int idx = eventindex.indexOf(chr);

		eventfuncs[idx](chr, data);
		return 0;
	}

public:
	SerialEventController() {
		buf.reserve(BUFFERLIMIT);
	}

	int RegisterEventHandler(const char c, const serialeventhandler_t seh) {
		eventindex += c;
		if (eventindex.length() > MAXEVENTS) return 1;
		eventfuncs[eventindex.length() - 1] = seh;
		return 0;
	}

	bool AddCharacter(const char c) {
		if (c == terminal) {
			HandleLine(buf);
			buf = "";
		}
		else {
			buf += c;
		}
	}

	void SendData(const char c, const String data) {
		Serial.println(SIGNATURE + c + data);
	}

	void SendData(const char c, const int data) {
		Serial.println(SIGNATURE + c + String(data));
	}

	void SendData(const char c, const float data) {
		Serial.println(SIGNATURE + c + String(data));
	}

};

SerialEventController SEController;

#endif