// MouseCage.cpp
// Library for automated home-cage system for Arduino ATmega2560
// Vick Lau

#include "MouseCage.h"

volatile uint8_t timer3OutputPin;
volatile uint32_t noiseCount = 0;

static volatile uint32_t lfsr = 0x0302UL;
volatile uint32_t lfsrMask = (1UL<<31|1UL<<27|1UL<<26|1UL<<1);

// low-level I/O implementation (faster than digitalWrite() as some unnecessary
// checks for PWM or timer are not included, use digitalWrite() if you are unsure)
// There are still overhead when using set_port() in exchange for portability.
static inline void set_port(uint8_t pin, uint8_t state);
static void disable_timer3(void);


//=====================================
// Static functions
//=====================================

static inline void set_port(const uint8_t pin, const uint8_t state)
{
	const uint8_t port = digitalPinToPort(pin);
	const uint8_t bitMask = digitalPinToBitMask(pin);
	uint8_t* reg = portOutputRegister(port);
	
	const uint8_t sreg = SREG;
	cli();
	if (state == HIGH)	
		*reg |= bitMask;	
	else
		*reg &= ~bitMask;
	SREG = sreg;
}

static void disable_timer3(void)
{
	const uint8_t sreg = SREG;
	cli();
	TIMSK3 = (0U << OCIE3A); // disable Timer 3, Output Compare A Match Interrupt
	TCCR3B = 0; // set Timer 3 to normal mode (no PWM for pin 2,3,5)
	OCR3A = 0; // set Output Compare Register 3A back to 0
	SREG = sreg;
}

ISR(TIMER3_COMPA_vect) // Timer 3, Output Compare A Match interrupt vector
{
	if (noiseCount > 0)
	{
		if (lfsr & 1)
		{
			lfsr = (lfsr >> 1) ^ lfsrMask;
			set_port(timer3OutputPin, HIGH); 
			//*portOutputRegister(timer3OutputPin) |= (1U << timer3OutputPin);
		}
		else
		{
			lfsr >>= 1;
			set_port(timer3OutputPin, LOW);
			//*portOutputRegister(timer3OutputPin) &= ~(1U << timer3OutputPin);
		}
		noiseCount--;
	}
	else
	{
		disable_timer3();
		set_port(timer3OutputPin, LOW);
		//*portOutputRegister(timer3OutputPin) &= ~(1U << timer3OutputPin);
	}
}

//=====================================
// End of static functions
//=====================================

MouseCage::MouseCage()
{
	speakerSS = 22;
	speaker0Pin = 23;
	speaker1Pin = 24;
}

MouseCage::MouseCage(uint8_t speakerSS_usr, uint8_t speaker0Pin_usr, uint8_t speaker1Pin_usr)
{
	speakerSS = speakerSS_usr;
	speaker0Pin = speaker0Pin_usr;
	speaker1Pin = speaker1Pin_usr;
}

void MouseCage::set_POT(const uint8_t channel, const uint8_t value)
{
	SPI.beginTransaction(SPISettings(POT_SPI_SPEED, MSBFIRST, SPI_MODE0));
	set_port(speakerSS, LOW); // set SS to low for fetching data
	if (channel == 0)
		SPI.transfer(WRITE_POT0); // digital POT 0
	else if (channel == 1)
		SPI.transfer(WRITE_POT1); // digital POT 1
	else
		SPI.transfer(WRITE_BOTH_POT); // default to both POTs
	SPI.transfer(value); // 0-255;
	set_port(speakerSS, HIGH);
	SPI.endTransaction();
}

// Timer interrupt based (Timer 3) white noise generation instead of using millis()
// Uses Galois linear feedback shift register technique
// Limitation: only one speaker will work at a time at the moment
void MouseCage::play_noise(const uint8_t channel, const uint8_t volume, const uint16_t duration)
{
	set_POT(channel, volume); // set potentiometer for speaker
	if (duration > 0)
	{
		noiseCount = FREQ_TIMER3 * duration/1000; // freq * duration in sec
		if (channel == 0)
		{
			timer3OutputPin = speaker0Pin;
			init_timer3();
		}
		else if (channel == 1)
		{
			timer3OutputPin = speaker1Pin;
			init_timer3();
		}
	}
}

void MouseCage::play_tone(const uint8_t channel, const uint8_t volume, const uint16_t duration, const uint16_t freq)
{
	set_POT(channel, volume);
	if (channel == 0)
		tone(speaker0Pin, freq, duration);
	else if (channel == 1)
		tone(speaker1Pin, freq, duration);
}

void MouseCage::open_door(void)
{

}

void MouseCage::detect_response(void)
{

}

//=====================================
// MouseCage private functions
//=====================================

void MouseCage::init_timer3(void)
{
	const uint8_t sreg = SREG;
	cli(); // disable global interrupts
	TCCR3A = 0;
	TCCR3B = 0;
	TCCR3B = (1U << WGM32)|(1U << CS30); // enable CTC mode on Timer 3
	OCR3A = TIMER3_OCR; // set Output Compare Register 3A to TIMER3_OCR
	TIMSK3 = (1U << OCIE3A); // enable	Timer 3, Output Compare A Match Interrupt
	SREG = sreg;
}