// Automated Mouse Cage Program
// Vick Lau

#include <SPI.h>
#include <RFIDuino.h>
#include <Servo.h>
#include "MouseCage.h"

//=====================================
// Constants
//=====================================

// Pin assignments
#define SPEAKER_SS 23
#define SPEAKER0_PIN 11
#define SPEAKER1_PIN 12

#define LICKPOT0_PIN 18
#define LICKPOT1_PIN 19

//=====================================
// Variables
//=====================================

//RFIDuino antenna0(1.2);
//uint8_t tagData[5];
//uint32_t tagID = 0;

MouseCage mouseCage(23,11,12);
//Servo servo0;

//String inputBuffer = "";

//bool isTimeOut = false;

//=====================================
// Global function prototypes
//=====================================


//=====================================
// Setup
//=====================================

void setup(void) {
	pinMode(SPEAKER0_PIN, OUTPUT);
	pinMode(SPEAKER1_PIN, OUTPUT);
	pinMode(SPEAKER_SS, OUTPUT);

	Serial.begin(9600);
	SPI.begin();
	//servo0.attach(9);
	//servo0.write(10);

	delay(500);
}

//=====================================
// Main loop
//=====================================

void loop(void) {
	mouseCage.play_noise(0,10,2000);
	mouseCage.play_tone(0,10,2000,500);
	delay(2000);
	mouseCage.play_noise(0,255,2000);
	mouseCage.play_tone(0,255,2000,500);
	delay(2000);
}
