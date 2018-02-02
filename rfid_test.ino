#include <RFIDuino.h>
#include <Servo.h>

//===========================
// Constants
//===========================

//const uint16_t msgInterval = 5000;
const char startMarker = '<';
const char endMarker = '>';

const uint16_t connectionTimeOut = 10000;
const uint16_t experimentTimeOut = 60000;

//===========================
// Variables
//===========================

RFIDuino antenna0(1.2);
byte tagData[5];
uint32_t tagID = 0;

Servo servo0;

String inputBuffer = "";

uint32_t curMillis, prevMillis, elaspedMillis;
bool isTimeOut = false;

void wait_for_matlab();
void send_mouseID();
int8_t get_response_matlab();
void check_timeout(uint8_t timeOutType);

//===========================

void setup() 
{
	Serial.begin(9600);

	servo0.attach(9);
	servo0.write(10);

	delay(500);
	Serial.println("Arduino is ready");

	wait_for_matlab();
}

//===========================

void loop() 
{
	if(!isTimeOut)
	{
		if(antenna0.scanForTag(tagData))
		{
			send_mouseID();
			switch(get_response_matlab())
			{
				case -1: // If there is a connection timeout
					break;

				case 1: // Entry
					entrySeq();
					break;

				case 2: // Exit
					exitSeq();
					break;

				default:
					break;
			}
		}
	}
	else
	{
		timeout();
	}
}

//===========================
// Function Definitions
//===========================

void wait_for_matlab()
{
	//curMillis = millis();
	//prevMillis = curMillis;
	while(true)
	{
		/*curMillis = millis();
		elaspedMillis = curMillis - prevMillis;
		if(elaspedMillis >= msgInterval)
		{
			Serial.println("Waiting for Matlab");
			prevMillis += msgInterval;
		}*/
		Serial.println("Waiting for Matlab");
		if(Serial.available() > 0)
		{
			char x = Serial.read();

			if(x == '!')
			{
				inputBuffer = Serial.readStringUntil('\n');
				if(inputBuffer == "MATLAB_RDY")
				{
					inputBuffer = "";
					isTimeOut = false;
					Serial.println("Matlab is ready");
					break;
				}
			}
		}
	}
}

//===========================

void send_mouseID()
{
	Serial.write(startMarker);
	Serial.write(tagData, 5);
	Serial.write(endMarker);
	Serial.println();
}

//===========================

int8_t get_response_matlab() 
{
	curMillis = millis();
	prevMillis = curMillis;

	while(true)
	{
		curMillis = millis();
		elaspedMillis = curMillis - prevMillis;
		check_timeout(1);

		if(!isTimeOut)
		{
			if(Serial.available() > 0)
			{
				char x = Serial.read();

				if(x == '!')
				{
					inputBuffer = Serial.readStringUntil('\n');
					if(inputBuffer == "MATLAB_RDY")
					{
						inputBuffer = "";
						return 0;
						break;
					}
					else if(inputBuffer == "ENTRY")
					{
						inputBuffer = "";
						return 1;
						break;
					}
					else if(inputBuffer == "EXIT")
					{
						inputBuffer = "";
						return 2;
						break;
					}
				}
			}
		}
		else
		{
			return -1;
			break;
		}
	}
}

//===========================

void check_timeout(uint8_t timeOutType)
{
	switch(timeOutType)
	{
		case 1: // Connection timeout
		{
			if(elaspedMillis > connectionTimeOut)
			{
				isTimeOut = true;
				break;
			}
		}

		case 2: // Experiment timeout
			break;

		default:
			break;
	}
}

//===========================

void timeout()
{
	Serial.println(F("ERROR: CONNECTION TIMEOUT!"));
	delay(2000);

	servo0.write(10);

	wait_for_matlab();
}

//===========================

void entrySeq()
{
	servo0.write(170);
}

//==========================

void exitSeq()
{
	servo0.write(10);
}