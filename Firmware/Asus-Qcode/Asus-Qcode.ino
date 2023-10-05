//-- Asus Qcode Card --
//
// - Firmware by: Sciguy429
//
// - This is the firmware source for the 1X Qcode card. It is very basic, there is little
// - reason to rebuild this firmware yourself.

//Debounce library
#include "Debounce.h"

//Board led
const uint8_t boardLED = 13;

//Button pin
const uint8_t buttonPin = 12;

//Button debouncer
Debounce buttonDebounce = Debounce(buttonPin);

//Total number of displays
#define MAX_DSP 2

//Mux pins
const uint8_t muxPins[] = {7, 6, 3, 4, 5, 8, 9, 2}; //A, B, C, D, E, F, G, DP

//Dsp pins
const uint8_t dspPins[] = {10, 11}; //Left to right

//Display mux outputs
volatile uint8_t muxPinsOut[][8] = {
  {1, 1, 1, 1, 1, 1, 1, 1}, //Dsp 1
  {1, 1, 1, 1, 1, 1, 1, 1}  //Dsp 2
};

//Hex numbers for display
const uint8_t hexNumbers[][7] = {
  {0, 0, 0, 0, 0, 0, 1}, //0
  {1, 0, 0, 1, 1, 1, 1}, //1
  {0, 0, 1, 0, 0, 1, 0}, //2
  {0, 0, 0, 0, 1, 1, 0}, //3
  {1, 0, 0, 1, 1, 0, 0}, //4
  {0, 1, 0, 0, 1, 0, 0}, //5
  {0, 1, 0, 0, 0, 0, 0}, //6
  {0, 0, 0, 1, 1, 1, 1}, //7
  {0, 0, 0, 0, 0, 0, 0}, //8
  {0, 0, 0, 0, 1, 0, 0}, //9
  {0, 0, 0, 1, 0, 0, 0}, //A
  {1, 1, 0, 0, 0, 0, 0}, //B
  {0, 1, 1, 0, 0, 0, 1}, //C
  {1, 0, 0, 0, 0, 1, 0}, //D
  {0, 1, 1, 0, 0, 0, 0}, //E
  {0, 1, 1, 1, 0, 0, 0}, //F
};

//Past qcodes
#define MAX_PAST_QCODES 64
uint8_t pastQCodes[MAX_PAST_QCODES];
int pastQcodeHead = 0;
int pastQcodeCount = 0;
int curQCode = 0x00;

//Interrupt vars
int lastDsp = 0;
int currentDsp = 0;

//Called ~100 times per second
SIGNAL(TIMER2_COMPA_vect) 
{
  //Advance The COMPA Register
  OCR2A += 156;

  digitalWrite(dspPins[lastDsp], LOW); //Turn off current display
  for (int i = 0; i < 8; i ++) {
    digitalWrite(muxPins[i], muxPinsOut[currentDsp][i]); //Update the mux output with the next display
  }
  digitalWrite(dspPins[currentDsp], HIGH); //Activate the next display
  lastDsp = currentDsp;
  currentDsp++;
  if (currentDsp >= MAX_DSP) {
    currentDsp = 0;
  }
}

//Sets the value of a display to a number
void muxOut(int dsp, int num, bool dp)
{
  for (int i = 0; i < 7; i ++) {
    muxPinsOut[dsp][i] = hexNumbers[num][i];
  }
  muxPinsOut[dsp][7] = (dp ? 0 : 1);
}

void setDsp(int hex, bool dp1, bool dp2)
{
  //Split up our hex
  uint8_t hexBottom = hex & 0xf;
  uint8_t hexTop = hex >> 4;

  //Set the mux pins
  muxOut(0, hexBottom, dp1);
  muxOut(1, hexTop, dp2);
}

void setup()
{
  //Board LED
	pinMode(boardLED, OUTPUT);
  digitalWrite(boardLED, LOW);

  //Button pin
  pinMode(buttonPin, INPUT_PULLUP);

  //Setup all mux pins
  for (int i = 0; i < 8; i++)
  {
    pinMode(muxPins[i], OUTPUT);
    digitalWrite(muxPins[i], HIGH); //Active LOW
  }

  //Setup the dsp pins
  for (int i = 0; i < MAX_DSP; i++)
  {
    pinMode(dspPins[i], OUTPUT);
    digitalWrite(dspPins[i], LOW); //Active HIGH
  }

  //Open serial connection to the COM_DEBUG port
  Serial.begin(115200);

  //Set starting code to 0x00
  setDsp(0x00, false, false);

  //Clear the list of past qcodes
  for (int i = 0; i < MAX_PAST_QCODES; i++)
  {
    pastQCodes[i] = 0x00;
  }

  //Setup interrupt
  // - Timer 2 COMP-A
  // - 100hz fire rate
  // - 50hz flash for each dsp
  cli();
  TCCR2A = 0;           //Init Timer2A
  TCCR2B = 0;           //Init Timer2B
  TCCR2B |= B00000111;  //Prescaler = 1024
  OCR2A = 156;          //Timer Compare2A Register
  TIMSK2 |= B00000010;
  sei();
}

void loop()
{
  //Update the button
  ButtonState buttonState = buttonDebounce.read();

  //Check our button state
  if (buttonState == ButtonState::BS_PRESSED)
  {
    //Print our saved codes
    if (pastQcodeCount > 0)
    {
      //Find starting pos
      int curPos = pastQcodeHead - pastQcodeCount;
      if (curPos < 0)
      {
        curPos = curPos + MAX_PAST_QCODES;
      }

      //Print codes
      bool dp = false;
      for (int i = 0; i < pastQcodeCount; i++)
      {
        //Set the display
        setDsp(pastQCodes[curPos], dp, !dp);
        dp = !dp;

        //Push the position forward
        curPos++;
        if (curPos >= MAX_PAST_QCODES)
        {
          curPos = 0;
        }

        //Wait
        delay(250);
      }
    }

    //Reset the display to the last code
    setDsp(curQCode, false, false);
  }

  //Wait for serial data
  if (Serial.available())
  {
    //Flash LED
    digitalWrite(boardLED, HIGH);

    //Grab our new code and update the display
    curQCode = Serial.read();
    setDsp(curQCode, false, false);

    //Save our code into the buffer
    pastQCodes[pastQcodeHead] = curQCode;
    pastQcodeHead++;
    if (pastQcodeHead >= MAX_PAST_QCODES)
    {
      pastQcodeHead = 0;
    }

    //Add up our buffer size
    if (pastQcodeCount < MAX_PAST_QCODES)
    {
      pastQcodeCount++;
    }
  }
  else
  {
    //Flash LED
    digitalWrite(boardLED, LOW);
  }
}
