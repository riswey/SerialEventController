
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
	const char TERMINAL = '\n';             //\n
	String buf = "";  //remember: volatile if changed by interrupt!

	serialeventhandler_t eventfuncs[MAXEVENTS];
	String eventindex = "";   //chr at position maps to eventfuncs[]

	String encodePacket(const byte cmd, const String data) {
		return SIGNATURE + (char)cmd + data + TERMINAL;
	}

	int decodePacket(const String packet, byte& cmd, String& data)
	{
		cmd = 0; data = "";
		if (packet.length() < 3) return 1;                                        //header too small (min 3)
		if (packet.substring(0, 2) != SIGNATURE) return 2;                      //packet signature failed
		//pre-trimmed
		cmd = (byte)packet[2];
		data = packet.substring(3);
		return 0;
	}

	int HandleLine(const String line) { //must be pre-trimmed!
		byte cmd;
		String data;
		decodePacket(line, cmd, data);

		int idx = eventindex.indexOf((char)cmd);

		eventfuncs[idx]((char)cmd, data);
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
		if (c == TERMINAL) {
			HandleLine(buf);
			buf = "";
		}
		else {
			buf += c;
		}
	}

	void SendData(const byte c, const String data) {
		Serial.print( encodePacket(c,data) );
	}

	void SendData(const byte c, const int data) {
		Serial.print( encodePacket(c, String(data)) );
	}

	void SendData(const byte c, const float data) {
		Serial.print( encodePacket(c, String(data)) );
	}

};

SerialEventController SEController;

#endif