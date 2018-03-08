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
#define SPEAKER_PIN0 11
#define SPEAKER_PIN1 12

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
	pinMode(SPEAKER_PIN0, OUTPUT);
	pinMode(SPEAKER_SS, OUTPUT);

	SPI.begin();
	//servo0.attach(9);
	//servo0.write(10);

	delay(500);
}

//=====================================
// Main loop
//=====================================

void loop(void) {
	mouseCage.play_noise(0,8,5000);
	mouseCage.play_tone(0,8,5000,500);
	delay(5000);
}

