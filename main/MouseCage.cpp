// Automated Mouse Cage (MouseCage.cpp)
// Vick Lau

#include "MouseCage.h"

MouseCage::MouseCage() {
	speakerSS = 23;
	speakerPin0 = 11;
	speakerPin1 = 12;
}

MouseCage::MouseCage(uint8_t speakerSS_usr, uint8_t speakerPin0_usr, 
	uint8_t speakerPin1_usr) {
	speakerSS = speakerSS_usr;
	speakerPin0 = speakerPin0_usr;
	speakerPin1 = speakerPin1_usr;
}

void MouseCage::set_DPOT(uint8_t channel, uint8_t value) {
	//SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
	digitalWrite(speakerSS, LOW); // set SS to low for fetching data
	if (channel == 0)
		SPI.transfer(WRITE_POT_0); // digital POT A
	else if (channel == 1)
		SPI.transfer(WRITE_POT_1); // digital POT B
	else
		SPI.transfer(WRITE_BOTH_POT); // default to both POTs
	SPI.transfer(value); // 0 - 8 are fine
	digitalWrite(speakerSS, HIGH); // set SS back to high
	//SPI.endTransaction();
}

void MouseCage::play_noise(uint8_t channel, uint8_t volume, uint16_t duration) {
	uint32_t curMillis = 0;
	if (channel >= 0 && channel <= 2) {
		set_DPOT(channel, volume);
		if (channel == 0) {
			curMillis = millis();
			while(millis() - curMillis < duration)
				digitalWrite(speakerPin0, generate_noise());
		}
		else if (channel == 1) {
			curMillis = millis();
			while(millis() - curMillis < duration)
				digitalWrite(speakerPin1, generate_noise());
		}
		else {
			curMillis = millis();
			while(millis() - curMillis < duration) {
				digitalWrite(speakerPin0, generate_noise());
				digitalWrite(speakerPin1, generate_noise());
			}
		}
		digitalWrite(speakerPin0, LOW);
		digitalWrite(speakerPin1, LOW);
	}
}

void MouseCage::play_tone(uint8_t channel, uint8_t volume, uint16_t duration,
	uint16_t freq) {
	if (channel >= 0 && channel <= 2) {
		set_DPOT(channel, volume);
		if (channel == 0)
			tone(speakerPin0, freq, duration);
		else if (channel == 1)
			tone(speakerPin1, freq, duration);
		else {
			tone(speakerPin0, freq, duration);
			tone(speakerPin1, freq, duration);
		}
	}
}

void MouseCage::open_door() {

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