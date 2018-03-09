// Automated Mouse Cage (MouseCage.cpp)
// Vick Lau

#include "MouseCage.h"

MouseCage::MouseCage() {
	speakerSS = 23;
	speaker0Pin = 11;
	speaker1Pin = 12;
}

MouseCage::MouseCage(uint8_t speakerSS_usr, uint8_t speaker0Pin_usr, uint8_t speaker1Pin_usr) {
	speakerSS = speakerSS_usr;
	speaker0Pin = speaker0Pin_usr;
	speaker1Pin = speaker1Pin_usr;
}

void MouseCage::set_POT(uint8_t channel, uint8_t value) {
	SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
	digitalWrite(speakerSS, LOW); // set SS to low for fetching data
	delayMicroseconds(1);
	if (channel == 0)
		SPI.transfer(WRITE_POT_0); // digital POT 0
	else if (channel == 1)
		SPI.transfer(WRITE_POT_1); // digital POT 0
	else
		SPI.transfer(WRITE_BOTH_POT); // default to both POTs
	SPI.transfer(value); // 0-255
	digitalWrite(speakerSS, HIGH); // set SS back to high
	SPI.endTransaction();
}

void MouseCage::play_noise(uint8_t channel, uint8_t volume, uint16_t duration) {
	uint32_t curMillis = 0;
	
	set_POT(channel, volume);
	if (channel == 0) {
		curMillis = millis();
		while(millis() - curMillis < duration)
			digitalWrite(speaker0Pin, generate_noise());
	}
	else if (channel == 1) {
		curMillis = millis();
		while(millis() - curMillis < duration)
			digitalWrite(speaker1Pin, generate_noise());
	}
	else {
		curMillis = millis();
		while(millis() - curMillis < duration) {
			digitalWrite(speaker0Pin, generate_noise());
			digitalWrite(speaker1Pin, generate_noise());
		}
	}
	digitalWrite(speaker0Pin, LOW);
	digitalWrite(speaker1Pin, LOW);
}

void MouseCage::play_tone(uint8_t channel, uint8_t volume, uint16_t duration, uint16_t freq) {
	set_POT(channel, volume);
	if (channel == 0)
		tone(speaker0Pin, freq, duration);
	else if (channel == 1)
		tone(speaker1Pin, freq, duration);
	else {
		tone(speaker0Pin, freq, duration);
		tone(speaker1Pin, freq, duration);
	}
}

void MouseCage::open_door(void) {

}

void MouseCage::detect_response(void) {

}

//=====================================
// Linear Feedback Shift Register for White Noise Generation

uint8_t MouseCage::generate_noise(void) {
	static uint32_t lfsr = 0x0302UL;
	uint32_t lfsrMask = (1UL<<31|1UL<<27|1UL<<26|1UL<<1);

	if (lfsr & 1) {
		lfsr = (lfsr >> 1) ^ lfsrMask;
		return 1;
	}
	else {
		lfsr >>= 1;
		return 0;
	}
}