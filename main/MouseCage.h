// MouseCage.h
// Library for automated home-cage system for Arduino ATmega2560
// Vick Lau

#ifndef MOUSECAGE_H
#define MOUSECAGE_H

#include <Arduino.h>
#include <inttypes.h>
#include <avr/iom2560.h>
#include <SPI.h>


// For MCP42010 digital potentiometer
#define WRITE_POT0 0b00010001
#define WRITE_POT1 0b00010010
#define WRITE_BOTH_POT 0b00010011
#define POT_SPI_SPEED 8000000 // 8 MHz max for Arduino (10 MHz max for MCP42010)

// Timer3
#define FREQ_TIMER3 80000
#define TIMER3_OCR 200 // (CPU_freq/prescaler)/desired_freq, 80 kHz (Timer 3)


class MouseCage
{
    private:
        uint8_t speakerSS, speaker0Pin, speaker1Pin;
        uint8_t lickpot0Pin, lickpot1Pin;

        void init_timer3(void);
        uint8_t generate_noise(void);
        void detect_response(void);

    public:
        MouseCage();
        MouseCage(uint8_t speakerSS_usr, uint8_t speaker0Pin_usr, uint8_t speaker1Pin_usr);
        void set_POT(uint8_t channel, uint8_t value);
        void play_noise(uint8_t channel, uint8_t volume, uint16_t duration);
        void play_tone(uint8_t channel, uint8_t volume, uint16_t duration, uint16_t freq);
        void open_door(void);
};

#endif