// Header File for Automated Mouse Cage (MouseCage.h)
// Vick Lau

#ifndef MOUSECAGE_H
#define MOUSECAGE_H

#include <Arduino.h>
#include <SPI.h>
#include <inttypes.h>

// For MCP42010 digital potentiometer
#define WRITE_POT_0 0b00010001
#define WRITE_POT_1 0b00010010
#define WRITE_BOTH_POT 0b00010011


class MouseCage {
    private:
        uint8_t speakerSS, speaker0Pin, speaker1Pin;
        uint8_t lickpot0Pin, lickpot1Pin;

        uint8_t generate_noise(void);
        void detect_response(void);
    public:
        MouseCage();
        MouseCage(uint8_t speakerSS_usr, uint8_t speaker0Pin_usr, uint8_t speaker1Pin_usr);
        void set_POT(uint8_t channel, uint8_t value);
        void play_noise(uint8_t channel, uint8_t volume, uint16_t duration);
        void play_tone(uint8_t channel, uint8_t volume, uint16_t duration,uint16_t freq);
        void open_door(void);
};

#endif