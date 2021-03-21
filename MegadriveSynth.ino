#include <MIDI.h>
#include "Controller.h"

/*************************************************************
  MIDI CONTROLLER

  by Notes and Volts
  www.notesandvolts.com

  Version 1.2a **Arduino UNO or NANO!**

  modified by look mum no computer for use with the sega mega drive / genesis
 *************************************************************/

// Channel (Voice) buttons directly connected to arduino.
const int Midichannel1pin =  7;     
const int Midichannel2pin =  8;      
const int Midichannel3pin =  9; 
const int Midichannel4pin =  10;  
const int Midichannel5pin =  11;   
const int Midichannelomnipin =  12;   

int  Midichannel1pinstate = 0;
int  Midichannel2pinstate = 0;
int  Midichannel3pinstate = 0;
int  Midichannel4pinstate = 0;
int  Midichannel5pinstate = 0;
int  Midichannelomnipinstate = 0;

int midichannel = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

//************************************************************
//***SET THE NUMBER OF CONTROLS USED**************************
//************************************************************
//---How many buttons are connected directly to pins?---------
byte NUMBER_BUTTONS = 0;
//---How many potentiometers are connected directly to pins?--
byte NUMBER_POTS = 1;
//---How many buttons are connected to a multiplexer?---------
byte NUMBER_MUX_BUTTONS = 16;
//---How many potentiometers are connected to a multiplexer?--
byte NUMBER_MUX_POTS = 48;
//************************************************************

//***ANY MULTIPLEXERS? (74HC4067)************************************
//MUX address pins must be connected to Arduino UNO pins 2,3,4,5
//A0 = PIN2, A1 = PIN3, A2 = PIN4, A3 = PIN5
//*******************************************************************
//Mux NAME (OUTPUT PIN, , How Many Mux Pins?(8 or 16) , Is It Analog?);


Mux M1(A0, 16, true); //analog multiplexer on Arduino analog pin A0
Mux M2(A1, 16, true); //Analog multiplexer on Arduino analog pin A1
Mux M3(A2, 16, true); //Analog multiplexer on Arduino analog pin A2
Mux M4(6, 16, false); //digital multiplexer on Arduino analog pin A0
//*******************************************************************


//***DEFINE DIRECTLY CONNECTED POTENTIOMETERS************************
//Pot (Pin Number, Command, CC Control, Channel Number)
//**Command parameter is for future use**

Pot PO1(A3, 0, 75, 0);    // Frequency Modulation Level
//Pot PO2(A1, 0, 10, 1);
//Pot PO3(A2, 0, 22, 1);
//Pot PO4(A3, 0, 118, 1);
//Pot PO5(A4, 0, 30, 1);
//Pot PO6(A5, 0, 31, 1);
//*******************************************************************
//Add pots used to array below like this->  Pot *POTS[] {&PO1, &PO2, &PO3, &PO4, &PO5, &PO6};
Pot *POTS[] {&PO1};
//*******************************************************************


//***DEFINE DIRECTLY CONNECTED BUTTONS*******************************
//Button (Pin Number, Command, Note Number, Channel, Debounce Time)
//** Command parameter 0=NOTE  1=CC  2=Toggle CC **

//Button BU1(2, 0, 60, 1, 5 );
//Button BU2(3, 0, 61, 1, 5 );
//Button BU3(4, 0, 62, 1, 5 );
//Button BU4(5, 0, 63, 1, 5 );
//Button BU5(6, 0, 64, 1, 5 );
//Button BU6(7, 0, 65, 1, 5 );
//Button BU7(8, 1, 64, 1, 5 );
//Button BU8(9, 2, 64, 1, 5 );

// Pot(Mux mux, byte muxpin, byte command, byte control, byte channel)
/*Pot MPO36(
  M3,           // Mux mux,
  3,            // byte muxpin,
  0,            // byte command,
  74,           // byte control,
  0);           // byte channel
    //LFO ENABLE 
*/
Button BU13(
  13,           // Pin Number,
  0,            // Command, 
  74,           // Note Number,
  0,            // Channel, 
  5);           // Debounce Time
  //LFO ENABLE

//*******************************************************************
//Add buttons used to array below like this->  Button *BUTTONS[] {&BU1, &BU2, &BU3, &BU4, &BU5, &BU6, &BU7, &BU8};
Button *BUTTONS[] {&BU13};
//*******************************************************************


//***DEFINE BUTTONS CONNECTED TO MULTIPLEXER*************************
//Button::Button(Mux mux, byte muxpin, byte command, byte value, byte channel, byte debounce , byte ccvalue (added by sam mod))
//** Command parameter 0=NOTE  1=CC  2=Toggle CC **

// Algorithm and Preset Instrument Setting Store / Recall

Button MBU1( M4, 0,  1, 14, 0, 5 , 8);  // Algorithm 1
Button MBU2( M4, 1,  1, 14, 0, 5 , 24); // Algorithm 2
Button MBU3( M4, 2,  1, 14, 0, 5 , 40); // Algorithm 3
Button MBU4( M4, 3,  1, 14, 0, 5 , 56); // Algorithm 4
Button MBU5( M4, 4,  1, 14, 0, 5 , 72); // Algorithm 5
Button MBU6( M4, 5,  1, 14, 0, 5 , 88); // Algorithm 6
Button MBU7( M4, 6,  1, 14, 0, 5 , 104);// Algorithm 7
Button MBU8( M4, 7,  1, 14, 0, 5 , 119);// Algorithm 8
Button MBU9( M4, 8,  1,  6, 0, 5 , 0);  // Store  1
Button MBU10(M4, 9,  1,  9, 0, 5 , 0);  // Recall 1
Button MBU11(M4, 10, 1,  6, 0, 5 , 25); // Store  2
Button MBU12(M4, 11, 1,  9, 0, 5 , 25); // Recall 2
Button MBU13(M4, 12, 1,  6, 0, 5 , 60); // Store  3
Button MBU14(M4, 13, 1,  9, 0, 5 , 60); // Recall 3
Button MBU15(M4, 14, 1,  6, 0, 5 , 100);// Store  4
Button MBU16(M4, 15, 1,  9, 0, 5 , 100);// Recall 4
//*******************************************************************
////Add multiplexed buttons used to array below like this->  Button *MUXBUTTONS[] {&MBU1, &MBU2, &MBU3, &MBU4, &MBU5, &MBU6.....};
Button *MUXBUTTONS[] {
  &MBU1, &MBU2, &MBU3, &MBU4, &MBU5, &MBU6, &MBU7, &MBU8, 
  &MBU9, &MBU10, &MBU11, &MBU12, &MBU13, &MBU14, &MBU15, &MBU16
};

//*******************************************************************


//***DEFINE POTENTIOMETERS CONNECTED TO MULTIPLEXER*******************
//Pot::Pot(Mux mux, byte muxpin, byte command, byte control, byte channel)
//**Command parameter is for future use**

Pot MPO1(M1, 0, 0, 23, 0); //multiple op4
Pot MPO2(M1, 1, 0, 22, 0); //multiple op3
Pot MPO3(M1, 2, 0, 21, 0); //multiple op2
Pot MPO4(M1, 3, 0, 20, 0); //multiple op1
Pot MPO5(M1, 4, 0, 72, 0); //amplitude modulation op4
Pot MPO6(M1, 5, 0, 73, 0); //amplitude modulation op3
Pot MPO7(M1, 6, 0, 71, 0); //amplitude modulation op2
Pot MPO8(M1, 7, 0, 70, 0); //amplitude modulation op1
Pot MPO9(M1, 8, 0, 62, 0);  //release op4
Pot MPO10(M1, 9, 0, 61, 0); //release op3
Pot MPO11(M1, 10, 0, 60, 0);//release op2
Pot MPO12(M1, 11, 0, 59, 0);//release op1
Pot MPO13(M1, 12, 0, 58, 0);//sustain op4
Pot MPO14(M1, 13, 0, 57, 0);//sustain op3
Pot MPO15(M1, 14, 0, 56, 0);//sustain op2
Pot MPO16(M1, 15, 0, 55, 0);//sustain op1
Pot MPO17(M2, 0, 0, 54, 0); //second decay op4
Pot MPO18(M2, 1, 0, 53, 0); //second decay op3
Pot MPO19(M2, 2, 0, 52, 0); //second decay op2
Pot MPO20(M2, 3, 0, 51, 0); //second decay op1
Pot MPO21(M2, 4, 0, 50, 0); //first decay op4
Pot MPO22(M2, 5, 0, 49, 0); //first decay op3
Pot MPO23(M2, 6, 0, 48, 0); //first decay op2
Pot MPO24(M2, 7, 0, 47, 0); //first decay op1
Pot MPO25(M2, 8, 0, 46, 0); //attack op4
Pot MPO26(M2, 9, 0, 45, 0); //attack op3
Pot MPO27(M2, 10, 0, 44, 0);//attack op2
Pot MPO28(M2, 11, 0, 43, 0);//attack op1
Pot MPO29(M2, 12, 0, 42, 0);//rate scaling op4f
Pot MPO30(M2, 13, 0, 41, 0);//rate scaling op3
Pot MPO31(M2, 14, 0, 40, 0);//rate scaling op2
Pot MPO32(M2, 15, 0, 39, 0); //rate scaling op1
Pot MPO37(M3, 4, 0, 27, 0); //detune op4
Pot MPO38(M3, 5, 0, 26, 0); //detune op3
Pot MPO39(M3, 6, 0, 25, 0); //detune op2
Pot MPO40(M3, 7, 0, 24, 0); //detune op1
Pot MPO41(M3, 8, 0, 19, 0); //level op4
Pot MPO42(M3, 9, 0, 17, 0); //level op2
Pot MPO43(M3, 10, 0, 16, 0); //level op1
Pot MPO44(M3, 11, 0, 18, 0); //level op3
Pot MPO45(M3, 12, 0, 93, 0); //SSE-EG ON + SETTING OP4
Pot MPO46(M3, 13, 0, 92, 0); //SSE-EG ON + SETTING OP3
Pot MPO47(M3, 14, 0, 91, 0);//SSE-EG ON + SETTING OP2
Pot MPO48(M3, 15, 0, 90, 0);//SSE-EG ON + SETTING OP1
Pot MPO33(M3, 0, 0, 15, 0); //FM FEEDBACK 
Pot MPO34(M3, 1, 0, 1, 0);  //LFO SPEED
Pot MPO35(M3, 2, 0, 76, 0);  //AMPLITUDE MODULATION
// Pot MPO36(M3, 3, 0, 74, 0);  //LFO ENABLE


//*******************************************************************
//Add multiplexed pots used to array below like this->  Pot *MUXPOTS[] {&MPO1, &MPO2, &MPO3, &MPO4, &MPO5, &MPO6.....};
Pot *MUXPOTS[] {
  &MPO1, &MPO2, &MPO3, &MPO4, &MPO5, &MPO6, &MPO7, &MPO8, 
  &MPO9, &MPO10, &MPO11, &MPO12, &MPO13, &MPO14, &MPO15, &MPO16, 
  &MPO17, &MPO18, &MPO19, &MPO20, &MPO21, &MPO22, &MPO23, &MPO24, 
  &MPO25, &MPO26, &MPO27, &MPO28, &MPO29, &MPO30, &MPO31, &MPO32,
  &MPO33, &MPO34, &MPO35, /*&MPO36,*/ &MPO37, &MPO38, &MPO39, &MPO40, 
  &MPO41, &MPO42, &MPO43, &MPO44, &MPO45, &MPO46, &MPO47, &MPO48,
};
//*******************************************************************




int midiByte = 0 ;
void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);

 
pinMode(Midichannel1pin, INPUT);
pinMode(Midichannel2pin, INPUT);
pinMode(Midichannel3pin, INPUT);
pinMode(Midichannel4pin, INPUT);
pinMode(Midichannel5pin, INPUT);
pinMode(Midichannelomnipin, INPUT);

}


void loop() {

  //  if (MIDI.read())
  {
//    MIDI.send(MIDI.getType(),
///              MIDI.getData1(),
//              MIDI.getData2(),
//              MIDI.getChannel());
  }
if (Serial.available() > 0) {
    midiByte = Serial.read() ;
    Serial.write(midiByte) ;
  }


  if (NUMBER_BUTTONS != 0) updateButtons();
  if (NUMBER_POTS != 0) updatePots();
  if (NUMBER_MUX_BUTTONS != 0) updateMuxButtons();
  if (NUMBER_MUX_POTS != 0) updateMuxPots();

 Midichannel1pinstate =    digitalRead(Midichannel1pin);
 Midichannel2pinstate =    digitalRead(Midichannel2pin);
 Midichannel3pinstate =    digitalRead(Midichannel3pin);
 Midichannel4pinstate =    digitalRead(Midichannel4pin);
 Midichannel5pinstate =    digitalRead(Midichannel5pin);
 Midichannelomnipinstate = digitalRead(Midichannelomnipin);

      if ( Midichannel1pinstate == HIGH) {midichannel = 1;}
 else if ( Midichannel2pinstate == HIGH) {midichannel = 2;}
 else if ( Midichannel3pinstate == HIGH) {midichannel = 3;}
 else if ( Midichannel4pinstate == HIGH) {midichannel = 4;}
 else if ( Midichannel5pinstate == HIGH) {midichannel = 5;}

}

//*****************************************************************
void updateButtons() {

  // Cycle through Button array
  for (int i = 0; i < NUMBER_BUTTONS; i = i + 1) {
    byte message = BUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      switch (BUTTONS[i]->Bcommand) {
        case 0: //Note
          MIDI.sendNoteOn(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
          break;
        case 1: //CC
          MIDI.sendControlChange(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
          break;
        case 2: //Toggle
          if (BUTTONS[i]->Btoggle == 0) {
            MIDI.sendControlChange(BUTTONS[i]->Bvalue, 127, BUTTONS[i]->Bchannel);
            BUTTONS[i]->Btoggle = 1;
          }
          else if (BUTTONS[i]->Btoggle == 1) {
            MIDI.sendControlChange(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
            BUTTONS[i]->Btoggle = 0;
          }
          break;
      }
    }

    //  Button is not pressed
    if (message == 1) {
      switch (BUTTONS[i]->Bcommand) {
        case 0:
          MIDI.sendNoteOff(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
          break;
        case 1:
          MIDI.sendControlChange(BUTTONS[i]->Bvalue, 0, BUTTONS[i]->Bchannel);
          break;
      }
    }
  }
}
//*******************************************************************
void updateMuxButtons() {

  // Cycle through Mux Button array
  for (int i = 0; i < NUMBER_MUX_BUTTONS; i = i + 1) {

    MUXBUTTONS[i]->muxUpdate();
    byte message = MUXBUTTONS[i]->getValue();

    //  Button is pressed
    if (message == 0) {
      switch (MUXBUTTONS[i]->Bcommand) {
        case 0: //Note
          MIDI.sendNoteOn(MUXBUTTONS[i]->Bvalue, 127, MUXBUTTONS[i]->Bchannel);
          break;
        case 1: //CC
          // single channel
          if (Midichannelomnipinstate == LOW) {
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue,  MUXBUTTONS[i]->Bccvalue, MUXBUTTONS[i]->Bchannel  + midichannel);
          }
          // multi channel
          if (Midichannelomnipinstate == HIGH) {
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue,  MUXBUTTONS[i]->Bccvalue, MUXBUTTONS[i]->Bchannel  + 1);
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue,  MUXBUTTONS[i]->Bccvalue, MUXBUTTONS[i]->Bchannel  + 2);
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue,  MUXBUTTONS[i]->Bccvalue, MUXBUTTONS[i]->Bchannel  + 3);
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue,  MUXBUTTONS[i]->Bccvalue, MUXBUTTONS[i]->Bchannel  + 4);
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue,  MUXBUTTONS[i]->Bccvalue, MUXBUTTONS[i]->Bchannel  + 5);
          }          
          break;
        case 2: //Toggle
          if (MUXBUTTONS[i]->Btoggle == 0) {
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 127, MUXBUTTONS[i]->Bchannel);
            MUXBUTTONS[i]->Btoggle = 1;
          }
          else if (MUXBUTTONS[i]->Btoggle == 1) {
            MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 0, MUXBUTTONS[i]->Bchannel);
            MUXBUTTONS[i]->Btoggle = 0;
          }
          break;
      }
    }
    //  Button is not pressed
    if (message == 1) {
      switch (MUXBUTTONS[i]->Bcommand) {
        case 0:
          MIDI.sendNoteOff(MUXBUTTONS[i]->Bvalue, 0, MUXBUTTONS[i]->Bchannel);
          break;
        case 1:
          //MIDI.sendControlChange(MUXBUTTONS[i]->Bvalue, 0, MUXBUTTONS[i]->Bchannel); this has been removed by sam to stop the cc returning to 0 on release of the button as it confuses the megadrive
          break;
      }
    }
  }
}
//***********************************************************************
void updatePots() {
  for (int i = 0; i < NUMBER_POTS; i = i + 1) {
    byte potmessage = POTS[i]->getValue();

    // single channel operation
    if (potmessage != 255 && Midichannelomnipinstate == LOW) {
      MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel + midichannel);  //midichannel is the offset added to change channels
    }

    // multi channel operation
    if (potmessage != 255 && Midichannelomnipinstate == HIGH) {
      MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel + 1);
      MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel + 2);
      MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel + 3);
      MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel + 4);
      MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel + 5);
    }
    
  }
}
//***********************************************************************
void updateMuxPots() {
  for (int i = 0; i < NUMBER_MUX_POTS; i = i + 1) {
    MUXPOTS[i]->muxUpdate();
    byte potmessage = MUXPOTS[i]->getValue();

    // single channel operation
    if (potmessage != 255 && Midichannelomnipinstate == LOW) {
      MIDI.sendControlChange(MUXPOTS[i]->Pcontrol, potmessage, MUXPOTS[i]->Pchannel + midichannel);
    }

    // multi channel operation
    if (potmessage != 255 && Midichannelomnipinstate == HIGH) {
      MIDI.sendControlChange(MUXPOTS[i]->Pcontrol, potmessage, MUXPOTS[i]->Pchannel + 1);
      MIDI.sendControlChange(MUXPOTS[i]->Pcontrol, potmessage, MUXPOTS[i]->Pchannel + 2);
      MIDI.sendControlChange(MUXPOTS[i]->Pcontrol, potmessage, MUXPOTS[i]->Pchannel + 3);
      MIDI.sendControlChange(MUXPOTS[i]->Pcontrol, potmessage, MUXPOTS[i]->Pchannel + 4);
      MIDI.sendControlChange(MUXPOTS[i]->Pcontrol, potmessage, MUXPOTS[i]->Pchannel + 5);
    }
    
  }
}
