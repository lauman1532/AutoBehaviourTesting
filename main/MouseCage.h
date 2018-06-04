/* 
    MouseCage.h
    Library for automated home-cage system for Arduino ATmega2560
    Vick Lau
*/

#ifndef MOUSECAGE_H
#define MOUSECAGE_H

#include <Arduino.h>
#include <inttypes.h>
#include <avr/iom2560.h>
#include <SPI.h>
#include "Servo.h" // modified version of the original Servo.h, uses timer1 only
#include "RFIDuino.h" // modified version, added init(), excluded unecessary functions, 
                      // refined timeout 

typedef enum { isrA, isrB, isrC } timer_isr_t;

// For MCP42010 digital potentiometer
#define WRITE_POT0 0b00010001
#define WRITE_POT1 0b00010010
#define WRITE_BOTH_POT 0b00010011
#define POT_SPI_SPEED 8000000 // 8 MHz max for Arduino (10 MHz max for MCP42010)

// Timer3 settings for play_noise()
// 16 MHz CPU, prescalar = 1 for TIMER 3
#define FREQ_TIMER3_A 80000
#define TIMER3_OCR_A 200 // (CPU_freq/prescaler)/desired_freq, 80 kHz (Timer3A)

// Timer3 settings for RFIDUINO LEDs
#define FREQ_TIMER3_B 300
#define TIMER3_OCR_B 53333 // (CPU_freq/prescaler)/desired_freq, 300 Hz (Timer3B)

// A2M communication
#define A2M_START_MARKER '>' // Arduino to Matlab packet marker
#define A2M_TIMEOUT 8000 // 8 seconds until timeout


class MouseCage
{
    private:
        uint8_t speakerSS, speaker0Pin, speaker1Pin;
        uint8_t lickpot0Pin, lickpot1Pin;
        uint8_t door0Pin, door1Pin;
        Servo door0, door1;
        RFIDuino antenna0;

        // uint8_t generate_noise(void);
        void door_control(uint8_t door); 
        bool detect_tag(void);
        uint32_t tag_conversion(uint8_t tagBuffer[5]);
        // void enter_testing(void);
        void detect_response(void);

    public:
        MouseCage();
        MouseCage(uint8_t speakerSS_usr, uint8_t speaker0Pin_usr, 
            uint8_t speaker1Pin_usr);
        void init(void);
        void set_POT(uint8_t channel, uint8_t value);
        void play_noise(uint8_t channel, uint8_t volume, uint16_t duration);
        void play_tone(uint8_t channel, uint8_t volume, uint16_t duration, uint16_t freq);
        void open_door(uint8_t door);
        void close_door(uint8_t door);
        void enter_testing(void);
};

#endif