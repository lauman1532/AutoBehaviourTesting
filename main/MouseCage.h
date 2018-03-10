// Header File for Automated Mouse Cage (MouseCage.h)
// Using Arduino Mega2560
// Vick Lau

#ifndef _MOUSECAGE_H_
#define _MOUSECAGE_H_

#include <Arduino.h>
#include <inttypes.h>
#include <avr/iom2560.h>
#include <SPI.h>


// For MCP42010 digital potentiometer
#define WRITE_POT_0 0b00010001
#define WRITE_POT_1 0b00010010
#define WRITE_BOTH_POT 0b00010011


class MouseCage
{
    private:
        uint8_t speakerSS, speaker0Pin, speaker1Pin;
        uint8_t speaker0Pin_tone, speaker1Pin_tone;
        uint8_t lickpot0Pin, lickpot1Pin;

        // low-level I/O implementation (faster than digitalWrite() as some unnecessary
        // checks are not included)
        void set_port_fast(uint8_t pin, uint8_t state); // direct I/O (low-level port name)
        void set_port(uint8_t pin, uint8_t state); // indirect I/O

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