// Automated Mouse Cage (MouseCage.cpp)
// Using Arduino Mega2560
// Vick Lau

#include "MouseCage.h"

MouseCage::MouseCage()
{
	speakerSS = digitalPinToPort(22);
	speaker0Pin = digitalPinToPort(23);
	speaker1Pin = digitalPinToPort(24);

	speaker0Pin_tone = 23; // use only for tone() function
	speaker1Pin_tone = 24; // use only for tone() function
}

MouseCage::MouseCage(uint8_t speakerSS_usr, uint8_t speaker0Pin_usr, uint8_t speaker1Pin_usr)
{
	speakerSS = digitalPinToPort(speakerSS_usr);
	speaker0Pin = digitalPinToPort(speaker0Pin_usr);
	speaker1Pin = digitalPinToPort(speaker1Pin_usr);

	speaker0Pin_tone = speaker0Pin_usr;
	speaker1Pin_tone = speaker1Pin_usr;
}

void MouseCage::set_POT(const uint8_t channel, const uint8_t value)
{
	SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
	PORTA &= ~_BV(speakerSS); // set SS to low for fetching data
	//digitalWrite(speakerSS, LOW); 
	delayMicroseconds(1);
	if (channel == 0)
		SPI.transfer(WRITE_POT_0); // digital POT 0
	else if (channel == 1)
		SPI.transfer(WRITE_POT_1); // digital POT 0
	else
		SPI.transfer(WRITE_BOTH_POT); // default to both POTs
	SPI.transfer(value); // 0-255
	PORTA |= _BV(speakerSS);// set SS back to high
	//digitalWrite(speakerSS, HIGH);
	//SPI.endTransaction();
}

void MouseCage::play_noise(const uint8_t channel, const uint8_t volume, const uint16_t duration)
{
	uint32_t curMillis = 0;
	
	set_POT(channel, volume);
	if (channel == 0)
	{
		curMillis = millis();
		while(millis() - curMillis < duration)
			set_port_fast(speaker0Pin, generate_noise());
			//digitalWrite(speaker0Pin, generate_noise());
	}
	else if (channel == 1)
	{
		curMillis = millis();
		while(millis() - curMillis < duration)
			set_port_fast(speaker1Pin, generate_noise());
			//digitalWrite(speaker1Pin, generate_noise());
	}
	else {
		curMillis = millis();
		while(millis() - curMillis < duration)
		{
			set_port_fast(speaker0Pin, generate_noise());
			set_port_fast(speaker1Pin, generate_noise());
		}
	}
	digitalWrite(speaker0Pin, LOW);
	digitalWrite(speaker1Pin, LOW);
}

void MouseCage::play_tone(const uint8_t channel, const uint8_t volume, const uint16_t duration, const uint16_t freq)
{
	set_POT(channel, volume);
	if (channel == 0)
		tone(speaker0Pin_tone, freq, duration);
	else if (channel == 1)
		tone(speaker1Pin_tone, freq, duration);
	else {
		tone(speaker0Pin_tone, freq, duration);
		tone(speaker1Pin_tone, freq, duration);
	}
}

void MouseCage::open_door(void)
{

}

void MouseCage::detect_response(void)
{

}

//=====================================
// Private functions

// Linear Feedback Shift Register for White Noise Generation
uint8_t MouseCage::generate_noise(void) 
{
	static uint32_t lfsr = 0x0302UL;
	uint32_t lfsrMask = (1UL<<31|1UL<<27|1UL<<26|1UL<<1);

	if (lfsr & 1)
	{
		lfsr = (lfsr >> 1) ^ lfsrMask;
		return 1;
	}
	else
	{
		lfsr >>= 1;
		return 0;
	}
}

void MouseCage::set_port_fast(const uint8_t pin, const uint8_t state)
{
	uint8_t* reg = portOutputRegister(pin);
	if (state == HIGH)
		*reg |= _BV(pin);
	else
		*reg &= ~_BV(pin);
}

void MouseCage::set_port(const uint8_t pin, const uint8_t state)
{
	uint8_t* reg = portOutputRegister(digitalPinToPort(pin));
	if (state == HIGH)
		*reg |= digitalPinToBitMask(pin);
	else
		*reg &= digitalPinToBitMask(pin);
}