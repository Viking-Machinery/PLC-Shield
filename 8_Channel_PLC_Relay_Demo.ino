// DN22D08 Demonstration Code
// 24V PLC Shield for Arduino Nano
// Written by: James Hussey
// Viking Machinery
// www.vikingmachinery.co.nz
// Revision 1.40
// December 2022

/*

  Functional Description

  Pressing the 4x onboard push buttons changes the tube display numbers.
  e.g. press button 1, the tube displays 1111. Press button 2, the tube displays 2222. So on.

  Triggering Inputs 1-8 will activate a the corresponding relay number.
  The relay will delay 0.5 seconds before turning off when the input is released


  ---Segment Display Format----
  --A--
  F---B
  --G--
  E---C
  --D--
   __  __   __  __
  |__||__| |__||__|
  |__||__| |__||__|


  -------------------------------*/

#include <FlexiTimer2.h> // ------------------------Make sure to include the Flexi Timer library in your Arduino IDE set up!!
#define uchar unsigned char
#define uint  unsigned int

// -----------------------------------------------Do not modify this section! Everything in here stays constant for any application.

int display_dat;
int key_value;

const int led = 13;     //Onboard LED

int first_run = 0;     // Stores first run data

//Pin connected to latch of Digital Tube Module
int latchPin = A3;
//Pin connected to clock of Digital Tube Module
int clockPin = A4;
//Pin connected to data of Digital Tube Module
int dataPin = A5;
//Pin connected to 595_OE of Digital Tube Module
int OE_595 = A2;

const int K1 = 12;  // Assigned push button inputs to pins
const int K2 = 10;  // Leave these alone!
const int K3 = 8;
const int K4 = A0;

const int INPUT1 = 2;  // Assigned to input terminals
const int INPUT2 = 3;  // Leave these alone!
const int INPUT3 = 4;
const int INPUT4 = 5;
const int INPUT5 = 6;
const int INPUT6 = 7;
const int INPUT7 = 9;
const int INPUT8 = 11;

const int RS485_RD = A1; // Assigns the RS485 coms pin. Once again, leave this alone!


/*       NO.:0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22 23 24 25 26 27 28
  /*Character :0,1,2,3,4,5,6,7,8,9,A, b, C, c, d, E, F, H, h, L, n, N, o, P, r, t, U, -,  ,*/
uchar  TUBE_SEG[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x58, 0x5e, 0x79, 0x71, 0x76, 0x74, 0x38, 0x54, 0x37, 0x5c, 0x73, 0x50, 0x78, 0x3e, 0x40, 0x00}; //Common anode Digital Tube Character Range.

uchar TUBE_NUM[8] = {0xfe, 0xff, 0xfd, 0xff, 0xfb, 0xff, 0xf7, 0xff,}; //Tube bit number

uchar dat_buf[8];

uchar dat;          // dat : Data to be displayed
uchar com_num;      // com_num :  Digital Tube Common
uchar relay_port;   //relay_port : Relay port


///////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------Set Up Code Start-------------------------------------------


void setup() {

  // This section sets up the pin conditions. You dont need to make any changes here ever.

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(OE_595, OUTPUT);

  pinMode(K1, INPUT_PULLUP);
  pinMode(K2, INPUT_PULLUP);
  pinMode(K3, INPUT_PULLUP);
  pinMode(K4, INPUT_PULLUP);

  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(INPUT5, INPUT);
  pinMode(INPUT6, INPUT);
  pinMode(INPUT7, INPUT);
  pinMode(INPUT8, INPUT);

  FlexiTimer2::set(2, 1.0 / 1000, TubeDisplay4Bit); // call every 2ms "ticks"
  FlexiTimer2::start();

  digitalWrite(INPUT1, HIGH);
  digitalWrite(INPUT2, HIGH);
  digitalWrite(INPUT3, HIGH);
  digitalWrite(INPUT4, HIGH);
  digitalWrite(INPUT5, HIGH);
  digitalWrite(INPUT6, HIGH);
  digitalWrite(INPUT7, HIGH);
  digitalWrite(INPUT8, HIGH);

  digitalWrite(K1, HIGH);
  digitalWrite(K2, HIGH);
  digitalWrite(K3, HIGH);
  digitalWrite(K4, HIGH);

  key_value = 0000;  // Initial 8 Bit Display

  first_run = 1;     // Stores first run data

  digitalWrite(OE_595, LOW);// Enable 74HC595
}


//---------------------------------------Set Up Code End-------------------------------------------

///////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------Main Code Start-------------------------------------------

// You can subsitute your won code here at will

void loop() {



  while (1) {

    //----------------------------------- 24V inouts & relay outpust section --------------
    if (digitalRead(INPUT1) == LOW) {
      bitSet(relay_port, 0);
      delay(500);
    }
    else {
      (bitClear(relay_port, 0));
    }

    if (digitalRead(INPUT2) == LOW) {
      bitSet(relay_port, 1);
      delay(500);
    }
    else {
      (bitClear(relay_port, 1));
    }

    if (digitalRead(INPUT3) == LOW) {
      bitSet(relay_port, 2);
      delay(500);
    }
    else {
      (bitClear(relay_port, 2));
    }

    if (digitalRead(INPUT4) == LOW) {
      bitSet(relay_port, 3);
      delay(500);
    }
    else {
      (bitClear(relay_port, 3));
    }

    if (digitalRead(INPUT5) == LOW) {
      bitSet(relay_port, 4);
      delay(500);
    }
    else {
      (bitClear(relay_port, 4));
    }

    if (digitalRead(INPUT6) == LOW) {
      bitSet(relay_port, 5);
      delay(500);
    }
    else {
      (bitClear(relay_port, 5));
    }

    if (digitalRead(INPUT7) == LOW) {
      bitSet(relay_port, 6);
      delay(500);
    }
    else {
      (bitClear(relay_port, 6));
    }

    if (digitalRead(INPUT8) == LOW) {
      bitSet(relay_port, 7);
      delay(500);
    }
    else {
      (bitClear(relay_port, 7));
    }

    //------------------------------------Push buttons and 8 bit display section ----------------
    if     (digitalRead(K1) == LOW) {
      key_value = 0;
    }
    else if (digitalRead(K2) == LOW) {
      key_value = 1;
    }
    else if (digitalRead(K3) == LOW) {
      key_value = 2;
    }
    else if (digitalRead(K4) == LOW) {
      key_value = 3;
    }
    else {
      key_value = 4;
    }


  }
}

//---------------------------------------Main Code End-------------------------------------------

///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------Tube Display Sub Routine Start-----------------------------------

// This section is the sub routine for tube display functions


void TubeDisplayOneBit()
{
  uchar tube_dat, bit_num;
  uchar display_l, display_h, relay_dat;

  tube_dat = TUBE_SEG[dat]; //Common Cathode Digital Tube,bit negated
  bit_num = TUBE_NUM[com_num];

  display_l  = ((tube_dat & 0x10) >> 3); //Q4   <-D1 -3
  display_l |= ((bit_num & 0x01) << 2); //DIGI0<-D2 +2
  display_l |= ((tube_dat & 0x08) >> 0); //Q3   <-D3 0
  display_l |= ((tube_dat & 0x01) << 4); //Q0   <-D4 -4
  display_l |= ((tube_dat & 0x80) >> 2); //Q7   <-D5 -2
  display_l |= ((tube_dat & 0x20) << 1); //Q5   <-D6 1
  display_l |= ((tube_dat & 0x04) << 5); //Q2   <-D7 5

  display_h  = ((bit_num & 0x02) >> 0); //DIGI1<-D1 0
  display_h |= ((bit_num & 0x04) >> 0); //DIGI2<-D2 0
  display_h |= ((tube_dat  & 0x40) >> 3); //Q6   <-D3 -3
  display_h |= ((tube_dat  & 0x02) << 3); //Q1   <-D4 3
  display_h |= ((bit_num & 0x08) << 2); //DIGI3<-D5 2

  relay_dat = relay_port;


  //ground latchPin and hold low for as long as you are transmitting
  digitalWrite(latchPin, LOW);

  shiftOut(dataPin, clockPin, MSBFIRST, display_h);
  shiftOut(dataPin, clockPin, MSBFIRST, display_l);
  shiftOut(dataPin, clockPin, MSBFIRST, relay_dat);

  //return the latch pin high to signal chip that it
  //no longer needs to listen for information
  digitalWrite(latchPin, HIGH);
}

//-----------------------------Tube Display Onebit Sub Routine End---------------------------------

///////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------Tube Display 4bit Sub Routine Start--------------------------------

void TubeDisplay4Bit(void)
{
  if (com_num < 7) com_num ++; else com_num = 0;
  dat = dat_buf[com_num];
  TubeDisplayOneBit();


  if (key_value == 0) display_dat = 1111;
  else if (key_value == 1) display_dat = 2222;
  else if (key_value == 2) display_dat = 3333;
  else if (key_value == 3) display_dat = 4444;
  else if (key_value > 3) display_dat = 0000;


  dat_buf[0] = display_dat / 1000;
  display_dat = display_dat % 1000;
  dat_buf[2] = display_dat / 100;
  display_dat = display_dat % 100;
  dat_buf[4] = display_dat / 10;
  dat_buf[6] = display_dat % 10;

}

//------------------------------Tube Display 4bit Sub Routine End--------------------------------

/////////////////////////////////////////////////////////////////////////////////////////////////
