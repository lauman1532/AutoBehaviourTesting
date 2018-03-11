// main.ino
// Automated Mouse Cage Program for Arduino ATmega2560
// Vick Lau

#include <SPI.h>
#include <RFIDuino.h>
#include "lib/Servo.h" // modified version of the original Servo.h, uses timer1 only
#include "MouseCage.h"

//=====================================
// Constants
//=====================================

// Pin assignments (check datasheet for low-level port name)
#define SPEAKER_SS 22 // POT's Slave Select pin, controlling volume of audio, pin 22 (PA0)
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
    DDRA = (1U<<DDA2)|(1U<<DDA1)|(1U<<DDA0); // same as setting PORTA 1,2,3 to OUTPUT
    PORTA |= (1U << digitalPinToPort(SPEAKER_SS)); // set SS to high initially

    Serial.begin(9600);
    SPI.begin();

    delay(500);
}

//=====================================
// Main loop
//=====================================

void loop(void)
{
    mouseCage.play_noise(0,0,1000);
    //mouseCage.play_tone(0,0,1000,50);
    delay(3000);
    mouseCage.play_noise(0,100,1000);
    //mouseCage.play_tone(0,60,1000,50);
    delay(4000);
}
