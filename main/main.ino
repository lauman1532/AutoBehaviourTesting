// Automated Mouse Cage Program for Arduino Mega2560
// Vick Lau

#include <SPI.h>
#include <RFIDuino.h>
#include <Servo.h>
#include "MouseCage.h"

//=====================================
// Constants
//=====================================

// Pin assignments (check datasheet for low-level port name)
#define SPEAKER_SS 22 // POT's Slave Select (SCK) for 2 speakers, pin 22 (PA0)
#define SPEAKER0_PIN 23 // pin 23 (PA1)
#define SPEAKER1_PIN 24 // pin 24 (PA2)

#define LICKPOT0_PIN 18
#define LICKPOT1_PIN 19

//=====================================
// Variables
//=====================================

//RFIDuino antenna0(1.2);
//uint8_t tagData[5];
//uint32_t tagID = 0;

MouseCage mouseCage(SPEAKER_SS, SPEAKER0_PIN, SPEAKER1_PIN);
//Servo servo0;

//String inputBuffer = "";

//bool isTimeOut = false;

//=====================================
// Global function prototypes
//=====================================


//=====================================
// Setup
//=====================================

void setup(void)
{
	/* pinMode(SPEAKER0_PIN, OUTPUT);
	pinMode(SPEAKER1_PIN, OUTPUT);
	pinMode(SPEAKER_SS, OUTPUT); */
	DDRA = B00000011; // same as setting PORTA 1,2,3 to OUTPUT
	
	Serial.begin(9600);
	SPI.begin();
	//servo0.attach(9);
	//servo0.write(10);

	delay(500);
}

//=====================================
// Main loop
//=====================================

void loop(void)
{
	mouseCage.play_noise(0,0,2000);
	mouseCage.play_tone(0,0,2000,500);
	delay(2000);
	mouseCage.play_noise(0,1,2000);
	mouseCage.play_tone(0,1,2000,500);
	delay(2000);
}
