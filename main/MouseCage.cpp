/*
    MouseCage.cpp
    Library for automated home-cage system for Arduino ATmega2560
    Vick Lau
*/

#include "MouseCage.h"

volatile uint8_t timer3OutputPin;
volatile uint32_t noiseCount = 0;
volatile uint8_t door0RemainTime = 0;
volatile uint8_t door1RemainTime = 0;
volatile Servo door0Control, door1Control;

static volatile uint32_t lfsr = 0x0302UL;
volatile uint32_t lfsrMask = (1UL<<31)|(1UL<<27)|(1UL<<26)|(1UL<<1);

/*
    set_port():

    low-level I/O implementation (faster than digitalWrite() as some unnecessary
    checks for PWM and timer are not included, use digitalWrite() if you are unsure).

    There are still overhead when using set_port() in exchange for readability and
    portability.
*/
static inline void set_port(uint8_t pin, uint8_t state);
static void init_timer3(timer_isr_t isr);
static void disable_timer3_isr(timer_isr_t isr);


//=====================================
// Static functions
//=====================================

static inline void set_port(const uint8_t pin, const uint8_t state)
{
    const uint8_t port = digitalPinToPort(pin);
    const uint8_t bitMask = digitalPinToBitMask(pin);
    uint8_t* reg = portOutputRegister(port);
    
    const uint8_t oldSREG = SREG; // save previous state register value
    cli(); // disable global interrupts to grant atomicity
    if (state == HIGH)	
        *reg |= bitMask; // bit math, set pin to high in the Port Output Register
    else
        *reg &= ~bitMask; // bit math, set pin to low in the Port Output Register
    SREG = oldSREG; // restore state register (I bit), hence enable global interrputs
}

static void init_timer3(const timer_isr_t isr)
{
    const uint8_t oldSREG = SREG;
    cli();
    TCCR3A = 0; // clear Timer/Counter Compare Register 3A
    TCNT3 = 0; // clear Timer/Counter3
    TCCR3B = (1U << WGM32)|(1U << CS30); // enable CTC mode on Timer3
    if (isr == isrA)
    {
        OCR3A = TIMER3_OCR_A; // set Output Compare Register 3A to TIMER3_OCR
        TIMSK3 = (1U << OCIE3A); // enable Timer3, Output Compare A Match Interrupt
    }
    else if (isr == isrB)
    {
        OCR3B = TIMER3_OCR_B; // set Output Compare Register 3B to TIMER3_OCR
        TIMSK3 = (1U << OCIE3B); // enable Timer3, Output Compare B Match Interrupt
    }
    SREG = oldSREG;
}

static void disable_timer3_isr(const timer_isr_t isr)
{
    const uint8_t oldSREG = SREG;
    cli();
    if (isr == isrA)
    {
        TIMSK3 = (0U << OCIE3A); // disable Timer 3, Output Compare A Match Interrupt
        // TCCR3B = 0; // set Timer3 to normal mode (no PWM for pin 2,3,5)
        OCR3A = 0; // set Output Compare Register 3A back to 0
    }
    else if (isr == isrB)
    {
        TIMSK3 = (0U << OCIE3B);
        OCR3B = 0;
    }
    SREG = oldSREG;
}

ISR(TIMER3_COMPA_vect) // Timer3, Output Compare A Match interrupt vector
{
    if (noiseCount > 0)
    {
        if (lfsr & 1)
        {
            lfsr = (lfsr >> 1) ^ lfsrMask;
            set_port(timer3OutputPin, HIGH); 
        }
        else
        {
            lfsr >>= 1;
            set_port(timer3OutputPin, LOW);
        }
        noiseCount--;
    }
    else
    {
        disable_timer3_isr(isrA);
        set_port(timer3OutputPin, LOW);
    }
}

ISR(TIMER3_COMPB_vect) // Timer3, Output Compare B Match interrupt vector
{
    if (door0RemainTime > 0)
    {
        door0RemainTime--;
    }
    else
    {
        closeDoor = true;
        disable_timer3_isr(isrB);
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
    lickpot0Pin = 18;
    lickpot1Pin = 19;
    door0Pin = 9;
    // door1Pin = 
}

MouseCage::MouseCage(uint8_t speakerSS_usr, uint8_t speaker0Pin_usr, uint8_t speaker1Pin_usr)
{
    speakerSS = speakerSS_usr;
    speaker0Pin = speaker0Pin_usr;
    speaker1Pin = speaker1Pin_usr;
    // lickpot0Pin = 18;
    // lickpot1Pin = 19;
    // door0Pin = 9;
    // door1Pin =
}

void MouseCage::init(void)
{
    DDRA = (1U<<DDA2)|(1U<<DDA1)|(1U<<DDA0); // same as setting PORTA 1,2,3 to OUTPUT
    PORTA = (0U<<PA2)|(0U<<PA1)|(1U<<PA0); // set SS to high initially to avoid SPI transaction
    antenna0.init();

    // Check door
    door0.attach(door0Pin);
    door0.write(180);
    delay(1000);
    door0.write(0);

    Serial.begin(9600);
    SPI.begin();

    delay(500);
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
    if (duration > 0)
    {
        set_POT(channel, volume); // set potentiometer for speaker
        noiseCount = FREQ_TIMER3_A * duration/1000; // freq * duration in sec
        if (channel == 0)
        {
            timer3OutputPin = speaker0Pin;
            init_timer3(isrA);
        }
        else if (channel == 1)
        {
            timer3OutputPin = speaker1Pin;
            init_timer3(isrA);
        }
    }
}

void MouseCage::play_tone(const uint8_t channel, const uint8_t volume, const uint16_t duration, const uint16_t freq)
{
    if (duration > 0)
    {
        set_POT(channel, volume);
        if (channel == 0)
            tone(speaker0Pin, freq, duration);
        else if (channel == 1)
            tone(speaker1Pin, freq, duration);
    }
}

void MouseCage::open_door(const uint8_t door)
{
    if (door == 0)
        door0.write(180);
    else
        door1.write(180);
}

void MouseCage::close_door(const uint8_t door)
{
    if (door == 0)
        door0.write(0);
    else
        door1.write(0);
}

bool MouseCage::detect_tag(void)
{
    uint8_t tagBuffer[5] = {0};
    uint16_t curMilis, prevMillis, elaspedMillis = 0;

    curMilis = millis();
    prevMillis = curMilis;
    while(elaspedMillis < 1000)
    {
        if(antenna0.scanForTag(tagBuffer))
        {
            Serial.write(startMarker);
            Serial.write(tagBuffer, 5);
            Serial.write(endMarker);
            Serial.println();
            return true;
            break;
        }
        curMilis = millis();
        elaspedMillis = curMilis - prevMillis;
    }
}

//=====================================
// MouseCage private functions
//=====================================

void MouseCage::door_control(uint8_t door)
{
    open_door(door);
    if (door == 0)
    {
        door0RemainTime = FREQ_TIMER3_B * DOOR_DURATION/1000;
        init_timer3(isrB);
    }
    else
    {
        door1RemainTime = FREQ_TIMER3_B * DOOR_DURATION/1000;
        init_timer3(isrC);
    }
}

void MouseCage::enter_testing(void)
{
    
    close_door(0);
    detect_tag();
    open_door(1);
}

void MouseCage::detect_response(void)
{

}

