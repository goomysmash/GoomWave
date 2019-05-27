/*
Copyright 2019 Jonah Eskin

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
This is the GoomWave project, an attempt at a hackable responsive LED GameCube controller framework, based on 2 different projects. NintendoSpy and ShineWave. 
This project aims to be an improved version of ShineWave with more versatility and understandability.
Currently this project is designed for Arduino Nano and DotStar LEDs, although other LED types and microcontrollers may be added in the future

Resources:
GameCube controller data protocol: http://www.int03.co.uk/crema/hardware/gamecube/gc-control.html
NintendoSpy project: https://github.com/jaburns/NintendoSpy
ShineWave project: https://github.com/Serisium/Shinewave
This helpful video on Switch Case State Machines: https://youtu.be/v8KXa5uRavg?list=PLH_Belnbfmpi3nsjzXG1jGfH4rfW11P48
 */
#include <FastLED.h>    //Publicly available library for sending data to LEDs

//Declarations for reading the GameCube Data                        
#define dataStringLength              89                                                                      //25 for GC data + 64 for controller data
#define readPin(controllerDataPin)    ((PIND&(1<<(controllerDataPin)))>>controllerDataPin)                    //returns a 1 or 0 based off if pin is high or low
bool rawData[dataStringLength];                                                                               //rawData is an array of booleans which represent a 1 or 0 in the serial string
const uint8_t controllerDataPin = 7;                                                                          //Pin going to the arduino.
#define MICROSECOND_NOPS "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"   //Assembly for 1 microsecond using clock pulses on a 16MHz processor. Needs to be changed for other clock speed processors.

//LED declarations
#define NUM_LEDS 5    // How many leds in your strip?
//All color declarations are made for Dotstars. Neopixels would need to be changed to GBR format.
#define Black     0, 0, 0     //Colors are RedBlueGreen (RBG) format for DotStars.
#define Red       255, 0, 0
#define Blue      0, 255, 0
#define Green     0, 0, 255
#define White     255, 255, 255
#define Purple    255, 255, 0
CRGB leds[NUM_LEDS]; // Define the array of leds
uint8_t gHue = 0; //number that dictates how far along the idle animation we are

//origin poll declarations for whether to calibrate the controller or not
bool started = false;
uint8_t originPollNumber = 0;

//Variable declarations for buttons and mode states (b = true/1 means b is pressed, b = false/0 means b is not pressed, etc.)
bool a; 
bool b;
bool x;
bool y;
bool dDown;
bool r; //r hard press
bool l; //l hard press
bool z;
bool s; //start button
bool rumble; //whether we're getting the signal from the GameCube to rumble

//declarations for the analog parts of the data
uint8_t xA=0;   //Analog stick x axis
uint8_t yA=0;   //Analog stick y axis
uint8_t xC=0;   //C stick x axis
uint8_t yC=0;   //C stick y axis
uint8_t L=0;    //L analog
uint8_t R=0;    //R analog
signed int xAorigin = 0; //Origin offsets to be calculated for Melee
signed int yAorigin = 0; 
signed int xCorigin = 0; 
signed int yCorigin = 0; 
signed int Lorigin = 0; 
signed int Rorigin = 0;

//Post-calibrated variables for analog sections
float xAresult = 0;
float yAresult = 0;
float xCresult = 0;
float yCresult = 0;
float Lresult = 0;
float Rresult = 0;

//after more math to convert them to very close to melee values
float hA = 0; //hA = hypotenuse of analog stick
float hC = 0;
float maxhAnew = 0; //Hypotenuse max'd to 1.0
float maxhCnew = 0;
float xAnew = 0; //variable from 0-1.0 for calculating LED states, x axis Analog stick
float yAnew = 0; //y axis Analog stick
float xCnew = 0;  //x axis C stick
float yCnew = 0; //y axis C stick
float Lnew = 0; //L analog
float Rnew = 0; //R analog

//Variables for the previous read of button presses
bool dDownPrev; //previous state of dpad down so that you can switch modes
bool aPrev; //Previous state of buttons for state machine animations
bool bPrev;
bool xPrev; 
bool yPrev;
bool lPrev;
bool rPrev;
bool zPrev;
bool sPrev;

//Mode variables
byte modeCount = 0; //To switch between modes
byte mode = 0; //Which mode we're in

//State machine variable declarations
const unsigned long buttonDelay = 10; //How long the button will flash bright before holding its color

//b button press variables
int state_b = 0; //for the state machine
unsigned long time1b = 0; //Timer
unsigned long time0b = 0; //Value at start of timer
#define bFlashColor 240,255,240 //Change the numbers (0-255) in this line for different flash colors
#define bHoldColor  100,255,100 //Change the numbers (0-255) in this line for different hold colors

//a button press variables
int state_a = 0; //for the state machine
unsigned long time1a = 0; //Timer
unsigned long time0a = 0; //Value at start of timer
#define aFlashColor 240,240,180 //Change the numbers (0-255) in this line for different flash colors
#define aHoldColor  240,240,10 //Change the numbers (0-255) in this line for different hold colors

//x button press variables
int state_x = 0; //for the state machine
unsigned long time1x = 0; //Timer
unsigned long time0x = 0; //Value at start of timer
#define xFlashColor 180,180,250 //Change the numbers (0-255) in this line for different flash colors
#define xHoldColor  50,50,220 //Change the numbers (0-255) in this line for different hold colors

//y button press variables
int state_y = 0; //for the state machine
unsigned long time1y = 0; //Timer
unsigned long time0y = 0; //Value at start of timer
#define yFlashColor 240,255,240 //Change the numbers (0-255) in this line for different flash colors
#define yHoldColor  100,255,100 //Change the numbers (0-255) in this line for different hold colors

//l button press variables
int state_l = 0; //for the state machine
unsigned long time1l = 0; //Timer
unsigned long time0l = 0; //Value at start of timer
#define lFlashColor 80,220,255 //Change the numbers (0-255) in this line for different flash colors
#define lHoldColor  10,180,255 //Change the numbers (0-255) in this line for different hold colors

//r button press variables
int state_r = 0; //for the state machine
unsigned long time1r = 0; //Timer
unsigned long time0r = 0; //Value at start of timer
#define rFlashColor 255,50,255 //Change the numbers (0-255) in this line for different flash colors
#define rHoldColor  255,10,255 //Change the numbers (0-255) in this line for different hold colors

//z button press variables
int state_z = 0; //for the state machine
unsigned long time1z = 0; //Timer
unsigned long time0z = 0; //Value at start of timer
#define zFlashColor 205,205,100 //Change the numbers (0-255) in this line for different flash colors
#define zHoldColor  205,205,50 //Change the numbers (0-255) in this line for different hold colors

//start (s) button press variables
int state_s = 0; //for the state machine
unsigned long time1s = 0; //Timer
unsigned long time0s = 0; //Value at start of timer
#define sFlashColor 255,255,255 //Change the numbers (0-255) in this line for different flash colors
#define sHoldColor  200,200,200 //Change the numbers (0-255) in this line for different hold colors

//xystart reset variables
int state_xys = 0; //for the state machine
unsigned long time1xys = 0; //Timer
unsigned long time0xys = 0; //Value at start of timer
const unsigned long resetDelay = 100; //How long to wait holding x+y+start to recalibrate

//strobe light variables
int stateStrobe=0; //for the state machine
unsigned long time1Strobe = 0; //Timer
unsigned long time0Strobe = 0; //Value at start of timer
const unsigned long delayStrobe = 1; //how long to hold before switching
bool alt=0; //whether you're lighting up inside or outside LEDs

#define DATA_PIN 2 //this line is only needed for NeoPixels. Data line = D2
//Wire the DotStars (APA102s) to the SCK (pin 13) and MOSI (pin 11) on arduino nano for SPI communication

void setup() 
{ 
//  Optional Serial monitor prints for debugging purposes  
  Serial.begin(2000000);
  Serial.println("Initializing LEDs");

  //Uncomment this next line for APA102s (Dotstars). Note: Color values are RBG not GBR like NeoPixels, so colors will be different
  FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS); //what type of LEDs to use
  

  //Uncomment this next line for WS2812s (NeoPixels). Note: Color values are GBR not RBG like Dotstars, so colors will be different
//  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void readControllerData(int bits)     //Function that gets called for reading the controller+GameCube data
{
  bool *rawDataPtr = rawData;    //makes a pointer to increment through the different memory locations of rawData
  while(bits>0)                  //while loop to go through all 89 bits of data
  {
        while( readPin(controllerDataPin)==0 ) //while the line is pulled low, get stuck
        {
        }
        while( readPin(controllerDataPin)==1 ) //while the line is high, get stuck... this has the effect of reading a falling edge with the 2 while loops
        {
        }
        asm volatile( MICROSECOND_NOPS MICROSECOND_NOPS ); //wait 2 microseconds
    
        *rawDataPtr = readPin(controllerDataPin);           //stores the data in 1 section of rawData
         ++rawDataPtr;                                      //increments the pointer to the next spot in the array
         bits--;                                            //decreases the bits by 1
  }
}

void sendRawData() //Function for sending messages to LEDs after reading the data
{
    interpretData(); //interpret the raw serial data into usable data with similar values to Melee
    xysStateMachine(); //State machine for recalibrating the LEDs with X+Y+start
    EVERY_N_MILLISECONDS( 5 ) { gHue++; } //function to increment every once in a while for the idle animation

    //To go to different modes
    if(!dDownPrev && dDown)  //if it was a 0 and then is a 1 (it was pressed down) then increment to the next mode
    {
      modeCount = modeCount + 1;
      mode = modeCount %5; //5 modes
//      Serial.print("the mode is: ");
//      Serial.println(mode); //Optional Serial Prints
    }
    dDownPrev = dDown;
    //end setup before modes
    
    /////////////////////////////////////////////////modes
      
      if (mode == 0) //Mode 0: Reactive craziness on every input
      {
        lightUpButtons(); //Attach the state machines for lighting up the buttons
        if (x==0&&y==0&&a==0&&b==0&&l==0&&r==0&&z==0&&s==0) //if no button is pressed
        {
          if (xCnew>=.2875||yCnew>=.2875) //if the C stick is being pressed
          {
            for(int i=0;i<5;i++)
            {
              //The order of xCnew and yCnew as well as the *255 can be changed to change the colors
              leds[i] = CRGB(yCnew*255,xCnew*255,xCnew*255); //light up based on C stick inputs
            }
          }
          else //the c stick is not being pressed
          {
            for(int i=0;i<5;i++)
            { 
              //The order of xAnew and yAnew as well as the *255 can be changed to change the colors
              leds[i] = CRGB(xAnew*255,yAnew*255,xAnew*255); //light up based on analog stick inputs
            }
          }
          if(xCnew<.2875&&yCnew<.2875&&xAnew<.2875&&yAnew<.2875) //if none of the sticks are pressed
          {
            for(int i=0;i<5;i++)
            { 
              //The order of Lnew and Rnew as well as the *255 can be changed to change the colors
              leds[i] = CRGB(Lnew*255,Rnew*255,Rnew*255); //light up based on the analog triggers
            }
          }
        }

        FastLED.show();
        
      }
      else if (mode == 1)//mode 1, idle animation
      {
            fill_rainbow(leds, NUM_LEDS, gHue, 10); //FastLED built in function for rainbow animation
            FastLED.show();
      }
      else if (mode == 2) //mode 2, LED notches
      {
        if (yAnew>.9&&xAnew>.3&&xAnew<.35||xAnew>.9&&yAnew>.3&&yAnew<.35) //if the values are within a certain range
        {
          for(int i=0;i<5;i++)
          { 
            //The order of xAnew and yAnew as well as the *255 can be changed to change the colors
            leds[i] = CRGB(yAnew*255,xAnew*255,xAnew*255); //light up based on analog stick inputs
          }
          FastLED.show();
        }
        else
        {
          for(int i=0;i<5;i++)
          { 
            leds[i] = CRGB(Black); //Just show black
          }
          FastLED.show();
        }
      }
      else if (mode == 3) //mode 3, Flash white for rumble
      {
        if (rumble==1)
        {
          for(int i=0;i<5;i++)
          {   
            leds[i] = CRGB(White);
          }
          FastLED.show();
        }
        else
        {
          for(int i=0;i<5;i++)
          {   
            leds[i] = CRGB(Black);
          }
          FastLED.show();
        }
      
      }
      else if (mode == 4) //mode 4, Strobe Light
      {
        strobeLight(); //Strobe light state machine
      }
      else //It should never be here
      {
        Serial.println("error");
      }
    ////////////////////////////////////end modes
}

void loop() 
{ 
    //Fairly self explanitory main loop. It just reads and then writes. Only issue is the sendRawData function can't take up too long or it will not read the data in time for the next polling cycle
    //readControllerData takes at least .36 ms to execute. Super Smash Bros Melee polls every 8ms. So sendRawData can't take more than 7.64ms to execute.
    //If you are attempting to modify sendRawData try to incorporate functions that don't take a long time to execute, such as millis() instead of delay()
    
    noInterrupts(); //interrupts will mess up the timing of the readControllerData function
    readControllerData(dataStringLength);   
    interrupts(); //allow interrupts for things like Serial prints and other such arduino things
    sendRawData(); //Interpret the data and tell the LEDs what to do, don't take over 7.64 ms to do it tho
}
void interpretData() {
    //for calculating the origin offset for calculating Melee analog inputs
    if (originPollNumber < 20) //Use the 20th poll for calculating inputs
    {
      started=false; //We're at the beginning
      originPollNumber++; //now we're 1 step closer to not the beginning
    }
    else
    {
      started=true; //No longer at the beginning
    }

    //setup before modes
    a = rawData[32]; //the a button is the 32nd bit in the Gamecube+controller data string
    b = rawData[31];
    x = rawData[30];
    y = rawData[29];
    l = rawData[34];
    r = rawData[35];
    z = rawData[36];
    s = rawData[28];
    rumble = rawData[23]; 
    dDown = rawData[38];

    /////////////////////////////////Analog data calculations start
    xA = 0; //Inital x Axis 8 bit value
    yA = 0;
    xC = 0;
    yC = 0;
    L = 0;
    R = 0;
    //write rawData into the unsigned 8 bit number for each analog input
    for(int i=7; i>=0;i--){xA = xA+(rawData[48-i] << i);} //Analog stick x axis
    for(int i=7; i>=0;i--){yA = yA+(rawData[56-i] << i);} //Analog stick y axis
    for(int i=7; i>=0;i--){xC = xC+(rawData[64-i] << i);} //C stick x axis
    for(int i=7; i>=0;i--){yC = yC+(rawData[72-i] << i);} //C stick y axis
    for(int i=7; i>=0;i--){L = L+(rawData[80-i] << i);}   //L analog
    for(int i=7; i>=0;i--){R = R+(rawData[88-i] << i);}   //R analog

    if (started == false) //Only do this if the controller was just plugged in (20th polling cycle)
    {
      xAorigin = xA-128;  //calibrate the origin offsets
      yAorigin = yA-128;
      xCorigin = xC-128; 
      yCorigin = yC-128; 
      Lorigin = L;
      Rorigin = R;
    }
    
    //Convert the raw controller numbers to (close to) Melee numbers
    xAresult=((xA-xAorigin)-128)/80.0; //Factor in the origins that were found at the beginning with the origin poll
    yAresult=((yA-yAorigin)-128)/80.0;
    xCresult=((xC-xCorigin)-128)/80.0;
    yCresult=((yC-yCorigin)-128)/80.0;
    Lresult = (L-Lorigin)/140.0;
    Rresult = (R-Rorigin)/140.0;

    //calculate the exact melee values
    hA = sqrt(yAresult*yAresult+xAresult*xAresult); //hA = hypotenuse of analog stick
    hC = sqrt(yCresult*yCresult+xCresult*xCresult); //hC = hypotenuse of C stick
    maxhAnew=max(1.0,hA); //round the hypotenuse to 1.0 if it's over 1.0
    maxhCnew=max(1.0,hC);
    xAnew=abs(xAresult/maxhAnew); //take the absolute value so you don't give a negative number to the LEDs
    yAnew=abs(yAresult/maxhAnew);
    xCnew=abs(xCresult/maxhCnew);
    yCnew=abs(yCresult/maxhCnew);
    if (Lresult<0){Lresult=0.0;} //don't return a negative number for the leds
    if (Rresult<0){Rresult=0.0;}
    Lnew=abs(Lresult);
    Rnew=abs(Rresult);

    //create melee deadzones
    if (xAnew<.2875){xAnew=0.0;} 
    if (yAnew<.2875){yAnew=0.0;}
    if (xCnew<.2875){xCnew=0.0;}
    if (yCnew<.2875){yCnew=0.0;}
    if (Lnew<.3){Lnew=0.0;}
    if (Rnew<.3){Rnew=0.0;}
    if (Lnew>1.0){Lnew=1.0;}
    if (Rnew>1.0){Rnew=1.0;}
    /////////////////////////////////////////Analog data calculations end
}

void bStateMachine() { //State machine to do a small flash and then hold a color every time b is pressed. All other button press state machines follow the exact same logic.
  switch(state_b) //Switch statement acts as a state machine
  {
  case 0: //RESET. Just go to mode 1.
      state_b = 1;
    break;
  case 1: //WAIT
      //Do nothing.  Only the top level loop can force us out of this state into state 2 "TURN ON"
    break;
  case 2: //TURN ON
      for(int i=0;i<5;i++) //Set the LEDs color to bFlashColor
      {   
        leds[i] = CRGB(bFlashColor);
      }
      FastLED.show();
      time0b = millis(); //start the timer on the flash
      state_b = 3; //go to the next state
    break;
    case 3: //STAY ON
      //Wait for time to elapse, then proceed to HOLD COLOR
      time1b = millis();
      if (time1b - time0b > buttonDelay) {state_b = 4;}  //if enough time has elapsed (flash is done), turn the hold color on
    break;
    case 4: //HOLD COLOR: Turn LEDs to bHoldColor
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(bHoldColor);
      }
      FastLED.show();
      state_b = 5; //next state
    break;
    case 5: //blue
      if (b==0) {state_b = 0;} //if B goes low again, reset to the beginning.
    break;    
  }
}
void aStateMachine() { //State machine to do a small flash and then hold a color every time a is pressed.
switch(state_a)
  {
  case 0: //RESET  
      state_a = 1;
    break;
  case 1: //WAIT
      //Do nothing.  Only the top level loop can force us out of this state into state 2 "TURN ON"
    break;
  case 2: //TURN white red 
      for(int i=0;i<5;i++) //Set the LEDs color to aFlashColor
      {   
        leds[i] = CRGB(aFlashColor);
      }
      FastLED.show();
      time0b = millis(); //update the timer for the flash part
      state_a = 3; //go to the next state
    break;
    case 3: //ON
      //Wait for time to elapse, then proceed to HOLD COLOR
      time1a = millis();
      if (time1a - time0a > buttonDelay) {state_a = 4;}  
    break;
    case 4: //HOLD COLOR: Turn LEDs to aHoldColor
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(aHoldColor);
      }
      FastLED.show();
      state_a = 5; //next state
    break;
    case 5: //White  
      if (a==0) {state_a = 0;} //if a goes low again, reset to the beginning.
    break;    
  }
}
void xStateMachine() { //State machine to do a small flash and then hold a color every time x is pressed.
switch(state_x)
  {
  case 0: //RESET 
      state_x = 1;
    break;
  case 1: //WAIT
      //Do nothing.  Only the top level loop can force us out of this state into state 2 "TURN ON"
    break;
  case 2: //TURN Flash ON
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(xFlashColor);
      }
      FastLED.show();
      time0x = millis();
      state_x = 3;
    break;
    case 3: //ON
      //Wait for time to elapse, then proceed to HOLD COLOR
      time1x = millis();
      if (time1x - time0x > buttonDelay) {state_x = 4;}  
    break;
    case 4: //HOLD COLOR
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(xHoldColor);
      }
      FastLED.show();
      state_x = 5; 
    break;
    case 5:   
      if (x==0) {state_x = 0;} 
    break;    
  }
}
void yStateMachine() { //State machine to do a small flash and then hold a color every time y is pressed.
  switch(state_y) //Switch statement acts as a state machine
  {
  case 0: //RESET  
      state_y = 1;
    break;
  case 1: //WAIT
      //Do nothing.  Only the top level loop can force us out of this state into state 2 "TURN ON"
    break;
  case 2: //TURN ON
      for(int i=0;i<5;i++) //Set the LEDs color to bFlashColor
      {   
        leds[i] = CRGB(yFlashColor);
      }
      FastLED.show();
      time0y = millis(); //start the timer on the flash
      state_y = 3; //go to the next state
    break;
    case 3: //STAY ON
      //Wait for time to elapse, then proceed to HOLD COLOR
      time1y = millis();
      if (time1y - time0y > buttonDelay) {state_y = 4;}  //if enough time has elapsed (flash is done), turn the hold color on
    break;
    case 4: //HOLD COLOR: Turn LEDs to bHoldColor
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(yHoldColor);
      }
      FastLED.show();
      state_y = 5; //next state
    break;
    case 5: //blue
      if (y==0) {state_y = 0;} //if y goes low again, reset to the beginning.
    break;    
  }
}
void lStateMachine() { //State machine to do a small flash and then hold a color every time l is pressed.
switch(state_l)
  {
  case 0: //RESET 
      state_l = 1;
    break;
  case 1: //WAIT
      //Do nothing.  Only the top level loop can force us out of this state into state 2 "TURN ON"
    break;
  case 2: //TURN ON
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(lFlashColor);
      }
      FastLED.show();
      time0l = millis();
      state_l = 3;
    break;
    case 3: //ON
      //Wait for time to elapse, then proceed to HOLD COLOR
      time1l = millis();
      if (time1l - time0l > buttonDelay) {state_l = 4;}  
    break;
    case 4: //HOLD COLOR
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(lHoldColor);
      }
      FastLED.show();
      state_l = 5; 
    break;
    case 5: //White  
      if (l==0) {state_l = 0;} 
    break;    
  }
}
void rStateMachine() { //State machine to do a small flash and then hold a color every time r is pressed.
switch(state_r)
  {
  case 0: //RESET 
      state_r = 1;
    break;
  case 1: //WAIT
      //Do nothing.  Only the top level loop can force us out of this state into state 2 "TURN ON"
    break;
  case 2: //TURN ON
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(rFlashColor);
      }
      FastLED.show();
      time0r = millis();
      state_r = 3;
    break;
    case 3: //ON
      //Wait for time to elapse, then proceed to HOLD COLOR
      time1r = millis();
      if (time1r - time0r > buttonDelay) {state_r = 4;}  
    break;
    case 4: //HOLD COLOR
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(rHoldColor);
      }
      FastLED.show();
      state_r = 5; 
    break;
    case 5: //White  
      if (r==0) {state_r = 0;} 
    break;    
  }
}
void zStateMachine() { //State machine to do a small flash and then hold a color every time z is pressed.
switch(state_z)
  {
  case 0: //RESET 
      state_z = 1;
    break;
  case 1: //WAIT
      //Do nothing.  Only the top level loop can force us out of this state into state 2 "TURN ON"
    break;
  case 2: //TURN ON
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(zFlashColor);
      }
      FastLED.show();
      time0z = millis();
      state_z = 3;
    break;
    case 3: //ON
      //Wait for time to elapse, then proceed to HOLD COLOR
      time1z = millis();
      if (time1z - time0z > buttonDelay) {state_z = 4;}  
    break;
    case 4: //HOLD COLOR
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(zHoldColor);
      }
      FastLED.show();
      state_z = 5; 
    break;
    case 5: //White  
      if (z==0) {state_z = 0;} 
    break;    
  }
}
void sStateMachine() { //State machine to do a small flash and then hold a color every time z is pressed.
switch(state_s)
  {
  case 0: //RESET 
      state_s = 1;
    break;
  case 1: //WAIT
      //Do nothing.  Only the top level loop can force us out of this state into state 2 "TURN ON"
    break;
  case 2: //TURN ON
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(sFlashColor);
      }
      FastLED.show();
      time0s = millis();
      state_s = 3;
    break;
    case 3: //ON
      //Wait for time to elapse, then proceed to HOLD COLOR
      time1s = millis();
      if (time1s - time0s > buttonDelay) {state_s = 4;}  
    break;
    case 4: //HOLD COLOR
      for(int i=0;i<5;i++)
      {   
        leds[i] = CRGB(sHoldColor);
      }
      FastLED.show();
      state_s = 5; 
    break;
    case 5: //White  
      if (s==0) {state_s = 0;} 
    break;    
  }
}

void xysStateMachine() { //State machine to reset the origin for the leds when x+y+start is held for a while
switch(state_xys)
  {
  case 0: //WAIT
  //do nothing unless the condition is met
  if (x&&y&&s) //if x and y and start are being pressed, go to the next step
  {
    state_xys=1;
  }
    break;
  case 1: //Begin timer
    time0xys = millis(); //timer0 started
    state_xys = 2;
    break;
  case 2: //Running timer
    time1xys = millis(); //timer1 ticking
    if (x==0||y==0||s==0) //if you stopped pressing x+y+start
    {
      state_xys = 3; //it was a false alarm
    }
    if (time1xys-time0xys>resetDelay) //if you exceeded the amount of time we set for a sufficiently long xystart hold
    {
      state_xys = 4; //confirmed
    }
    break;
  case 3: //False Alarm
    time1xys=time0xys; //reset the timer
    state_xys = 0; //reset to the beginning
    break;
  case 4: //Confirmed trying to reset
    started=false; //act as if the controller was just plugged in, redo the origin poll
    originPollNumber=0;
    mode=1; //also go to the idle animation to confirm the reset worked
    state_xys = 0; //reset the state machine
    break;    
  }
}

void strobeLight() { //State machine to do a strobe light animation indefinitely
  switch(stateStrobe)
  {
    case 0: //Alt? Is this only going to light up 0,2,4 or will it light up 1,3?
      if (alt==0)
      {
        stateStrobe=1; //normal mode
      }
      else
      {
        stateStrobe=3; //alt mode
      }
    break;
    case 1: //Turn on Normal LEDs (0,2,4)
      time0Strobe=millis();
      leds[0] = CRGB(200,200,200);
      leds[2] = CRGB(200,200,200);
      leds[4] = CRGB(200,200,200);
      FastLED.show();
      stateStrobe=2;
    break;
    case 2: //On Normal
      time1Strobe=millis();
      if (time1Strobe-time0Strobe>delayStrobe) {stateStrobe=5;} //if the sufficient time has passed turn off the LEDs
    break;
    case 3: //Turn on Alt (1,3)
      time0Strobe=millis();
      leds[1] = CRGB(255,255,255);
      leds[3] = CRGB(255,255,255);
      FastLED.show();
      stateStrobe=4;
    break;
    case 4: //On Alt
      time1Strobe=millis();
      if (time1Strobe-time0Strobe>delayStrobe) {stateStrobe=5;} //if the sufficient time has passed turn off the LEDs
    break;  
    case 5: //Turn off
      time0Strobe=millis(); //update the timer
      alt=!alt; //switch to the other mode
      for(int i=0;i<5;i++) //turn the LEDs off
      {   
        leds[i] = CRGB(Black); 
      }
      FastLED.show(); 
      stateStrobe=6;     
    break;
    case 6: //Wait OFF
      time1Strobe=millis();
      if(time1Strobe-time0Strobe>delayStrobe) {stateStrobe=0;} //wait until enough time has passed then go to the beginning
    break;
  }
}
void lightUpButtons(){//Declarations for lighting up buttons using state machines 

  bStateMachine(); //attach all the state machines
  aStateMachine();
  xStateMachine();
  yStateMachine();
  lStateMachine();
  rStateMachine();
  zStateMachine();
  sStateMachine();
  if(!aPrev && a) {state_a = 2;}  //if it was a 0 and then is a 1 (it was pressed down) then turn on the state machine
  aPrev = a; //a update
  if(!bPrev && b) {state_b = 2;}  //if it was a 0 and then is a 1 (it was pressed down) then turn on the state machine
  bPrev = b; //b update
  if(!xPrev && x) {state_x = 2;}  //if it was a 0 and then is a 1 (it was pressed down) then turn on the state machine
  xPrev = x; //x update
  if(!yPrev && y) {state_y = 2;}  //if it was a 0 and then is a 1 (it was pressed down) then turn on the state machine
  yPrev = y; //y update
  if(!lPrev && l) {state_l = 2;}  //if it was a 0 and then is a 1 (it was pressed down) then turn on the state machine
  lPrev = l; //l update
  if(!rPrev && r) {state_r = 2;}  //if it was a 0 and then is a 1 (it was pressed down) then turn on the state machine
  rPrev = r; //r update
  if(!zPrev && z) {state_z = 2;}  //if it was a 0 and then is a 1 (it was pressed down) then turn on the state machine
  zPrev = z; //z update
  if(!sPrev && s) {state_s = 2;}  //if it was a 0 and then is a 1 (it was pressed down) then turn on the state machine
  sPrev = s; //s update
}
