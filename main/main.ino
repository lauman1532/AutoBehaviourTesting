// main.ino
// Automated Mouse Cage Program for Arduino ATmega2560
// Vick Lau

#include "MouseCage.h"
#include <SPI.h>

//=====================================
// Constants
//=====================================

// Pin assignments (check datasheet for port names)
#define SPEAKER_SS 22 // POT's Slave Select pin, controlling volume of audio, pin 22 (PA0)
#define SPEAKER0_PIN 23 // pin 23 (PA1)
#define SPEAKER1_PIN 24 // pin 24 (PA2)

#define LICKPOT0_PIN 18 // INT
#define LICKPOT1_PIN 19

#define SERVO0_PIN 9

//
#define HABITUATION 1

//=====================================
// Variables
//=====================================

//uint8_t tagData[5];
//uint32_t tagID = 0;

MouseCage mouseCage;

//bool isTimeOut = false;

//=====================================
// Global function prototypes
//=====================================


//=====================================
// Setup
//=====================================

void setup(void)
{
    /*
    DDRA = (1U<<DDA2)|(1U<<DDA1)|(1U<<DDA0); // same as setting PORTA 1,2,3 to OUTPUT
    PORTA = (0U<<PA2)|(0U<<PA1)|(1U<<PA0); // set SS to high initially to avoid SPI      transaction

    Serial.begin(9600);
    SPI.begin();

    delay(500);
    */
    mouseCage.init();
}

//=====================================
// Main loop
//=====================================

void loop(void)
{
    if (HABITUATION == true)
    {
        mouseCage.open_door(0);
        mouseCage.open_door(1);
        mouseCage.play_tone(0, 10, 800, 80);
        delay(1000);
        mouseCage.play_tone(1, 10, 800, 80);
    }
    else
    {
        //mouseCage.play_noise(1, 10, 1000);
        mouseCage.play_tone(1, 10, 1000, 100);
    }

}
