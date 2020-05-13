#include <FastLED.h>

const uint8_t controllerDataPin = 4;
#define readPin(controllerDataPin)    ((PINB&(1<<(controllerDataPin)))>>controllerDataPin) //read pin quickly
#define dataStringLength              63
bool rawData[dataStringLength];
#define MICROSECOND_NOPS "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"   //Assembly for 1 microsecond using clock pulses on a 16MHz processor. Needs to be changed for other clock speed processors.

//xystart reset variables. If you press x+y+start for a half second it recalibrates the sticks
uint8_t state_xys = 0; //for the state machine
unsigned short time1xys = 0; //Timer
unsigned short time0xys = 0; //Value at start of timer
const unsigned short resetDelay = 200; //How long to wait holding x+y+start to recalibrate

//origin poll declarations for whether to calibrate the controller or not
bool started = false;
uint8_t originPollNumber = 0;

byte modeCount = 0; //To switch between modes
byte mode = 0; //Which mode we're in

#define DATA_PIN 0

#define NUM_LEDS 4
CRGB leds[NUM_LEDS];

bool a; 
bool b;
bool x;
bool y;
bool r; //r hard press
bool l; //l hard press
bool z;
bool s; //start button
bool dDown;
bool dUp;
bool dLeft;
bool dRight;


bool dDownPrev; //previous state of dpad down so that you can switch modes

//declarations for the analog parts of the data
uint8_t xA=0;   //Analog stick x axis
uint8_t yA=0;   //Analog stick y axis
signed int xAorigin = 0; //Origin offsets to be calculated for Melee
signed int yAorigin = 0; 

//Post-calibrated variables for analog sections
float xAresult = 0;
float yAresult = 0;

//after more math to convert them to very close to melee values
float hA = 0; //hA = hypotenuse of analog stick
float hC = 0;
float maxhAnew = 0; //Hypotenuse max'd to 1.0
float maxhCnew = 0;
float xAnew = 0; //variable from 0-1.0 for calculating LED states, x axis Analog stick
float yAnew = 0; //y axis Analog stick

uint8_t state_wave = 5;
uint8_t wave1=0;
uint8_t wave2=0;
uint8_t wave3=0;
#define waveRate  15
bool waveDone=false;
uint8_t amountWaved=0;

//abstart reset variables. If you press a+b+start for a half second it goes to mode 0
uint8_t state_abs = 0; //for the state machine
unsigned short time1abs = 0; //Timer
unsigned short time0abs = 0; //Value at start of timer
//const unsigned short resetDelay = 200; //How long to wait holding x+y+start to recalibrate

//declarations for rainbow (goombow) animation
uint8_t state_goombow = 0;
uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;
#define goombowRate  1


void setup()
{
  pinMode(controllerDataPin, INPUT_PULLUP);
  //pinMode(0, OUTPUT);
//  pinMode(2, OUTPUT);
//  pinMode(PB1, OUTPUT);

//  TCCR0A = 0;

  TCCR0B &= ~(1<<CS02); //0
  TCCR0B |= (1<<CS01); //1
  TCCR0B &= ~(1<<CS00); //0 //prescaler for timer0 (for reading end of polling)

  TCCR1 |= (1<<CS13); //1
  TCCR1 |= (1<<CS12); //1
  TCCR1 |= (1<<CS11);//1
  TCCR1 |= (1<<CS10);//1 prescaler 

  //green pb1
  //red pb5
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  cli();

  //FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
}

void loop()
{
  verifyEndPoll();
  align();
  readControllerData(dataStringLength);
  interpretData();
  absStateMachine(); ////if a and b and start is pressed for long enough go to mode 0
  xysStateMachine();
  sei();
  //////////// start LED stuff
  
  switchModes();
  //XABY = leds(0,1,2,3)
  if (mode == 0) //Mode 0: off
  {
    if (TCNT1>12) //if a certain amount of time has passed
    { 
      wave(); //state machine for wave animation
      TCNT1=0; //reset the timer
    }
    if(started==false) //if we're at the beginning
    {
      state_wave=0; //turn on the animation
    }
    if(waveDone==false) //either do a forward or backward wave depending on what you did last
    {
      leds[0]=CRGB(wave1,wave1,wave1);
      leds[1]=CRGB(wave2,wave2,wave2);
      leds[2]=CRGB(wave3,wave3,wave3);
      leds[3]=CRGB(wave2,wave2,wave2);
    } 
    else
    {
      leds[0]=CRGB(wave3,wave3,wave3);
      leds[1]=CRGB(wave2,wave2,wave2);
      leds[2]=CRGB(wave1,wave1,wave1);
      leds[3]=CRGB(wave2,wave2,wave2);
    }
    if (amountWaved>=3) //if you waved enough, stop and be blank
    {
      state_wave=6;
      leds[0]=CRGB(0,0,0);
      leds[1]=CRGB(0,0,0);
      leds[2]=CRGB(0,0,0);
      leds[3]=CRGB(0,0,0);
    }
    
    FastLED.show();
    
  }
  else if (mode == 1)//mode 1 nonreactive OEM colors
  {
    leds[0]=CRGB(255,255,255);
    leds[1]=CRGB(0,255,0);
    leds[2]=CRGB(255,0,0);
    leds[3]=CRGB(255,255,255);
    FastLED.show();
  }
  else if (mode == 2) //mode 2 nonreactive red A, XYB white
  {
    leds[0]=CRGB(255,255,255);
    leds[1]=CRGB(255,0,0);
    leds[2]=CRGB(255,255,255);
    leds[3]=CRGB(255,255,255);
    
    FastLED.show();
  }
  else if (mode == 3) //mode 3 react to buttons
  {
    leds[0]=CRGB(0,0,0);
    leds[1]=CRGB(0,0,0);
    leds[2]=CRGB(0,0,0);
    leds[3]=CRGB(0,0,0);
    if (x==1)
    {
      leds[0]=CRGB(255,255,255);
    }
    if (a==1)
    {
      leds[1]=CRGB(255,0,0);
    }
    if (b==1)
    {
      leds[2]=CRGB(255,255,255);
    }
    if (y==1)
    {
      leds[3]=CRGB(255,255,255);
    }
    FastLED.show();
  }
  else if (mode == 4) //mode 4 react to stick
  {

    leds[0] = CRGB(xAnew*255,yAnew*255,xAnew*255);
    leds[1] = CRGB(xAnew*255,yAnew*255,xAnew*255);
    leds[2] = CRGB(xAnew*255,yAnew*255,xAnew*255);
    leds[3] = CRGB(xAnew*255,yAnew*255,xAnew*255);
    FastLED.show();
  }
  else if (mode == 5) //mode 5 rainbow
  {
    if (TCNT1>12) //if a certain amount of time has passed
    { 
      goombow();
      leds[0]=CRGB(red,green,blue);
      leds[1]=CRGB(red,green,blue);
      leds[2]=CRGB(red,green,blue);
      leds[3]=CRGB(red,green,blue);
      TCNT1=0;
    }
    FastLED.show();
  }
  else if (mode == 6) //mode 6 led notches
  {
    if (yAnew>.9&&xAnew>.3&&xAnew<.38||xAnew>.9&&yAnew>.3&&yAnew<.38) //if the values are within a certain range
    {
      for(int i=0;i<NUM_LEDS;i++)
      { 
        //The order of xAnew and yAnew as well as the *255 can be changed to change the colors
        leds[i] = CRGB(yAnew*255,xAnew*255,xAnew*255); //light up based on analog stick inputs
      }
      FastLED.show();
    }
    else
    {
      for(int i=0;i<NUM_LEDS;i++)
      { 
        leds[i] = CRGB(0,0,0); //Just show black
      }
      FastLED.show();
    }
  }
  //////////// end LED stuff
  cli();
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
    mode=5; //also go to 4 to confirm the reset worked
    originPollNumber=0;
    state_xys = 0; //reset the state machine
    break;    
  }
}


void goombow(){ //rainbow animation state machine
  switch(state_goombow){
    case 0: //set red
      red=0;
      green=0;
      blue=0;
      state_goombow=1;
      break;
    case 1: //inc green (inc red, dec blue)
      (green+goombowRate>=255)?(green=255):(green=green+goombowRate);
      (red+goombowRate>=255)?(red=255):(red=red+goombowRate);
      (blue-goombowRate<=0)?(blue=0):(blue=blue-goombowRate);
      if(green==255)
      {
        state_goombow=2;
      }
      break;
    case 2: //dec red (inc green, dec blue)
      (red-goombowRate<=0)?(red=0):(red=red-goombowRate);
      (green+goombowRate>=255)?(green=255):(green=green+goombowRate);
      (blue-goombowRate<=0)?(blue=0):(blue=blue-goombowRate);
      if(red==0)
      {
        state_goombow=3;
      }
      break;
    case 3: //inc blue (inc green, dec red)
      (blue+goombowRate>=255)?(blue=255):(blue=blue+goombowRate);
      (green+goombowRate>=255)?(green=255):(green=green+goombowRate);
      (red-goombowRate<=0)?(red=0):(red=red-goombowRate);
      if(blue==255)
      {
        state_goombow=4;
      }
      break;
    case 4: //dec green (inc blue, dec red)
      (green-goombowRate<=0)?(green=0):(green=green-goombowRate);
      (blue+goombowRate>=255)?(blue=255):(blue=blue+goombowRate);
      (red-goombowRate<=0)?(red=0):(red=red-goombowRate);
      if(green==0)
      {
        state_goombow=5;
      }
      break;
    case 5: //inc red (inc blue, dec green)
      (red+goombowRate>=255)?(red=255):(red=red+goombowRate);
      (blue+goombowRate>=255)?(blue=255):(blue=blue+goombowRate);
      (green-goombowRate<=0)?(green=0):(green=green-goombowRate);
      if(red==255)
      {
        state_goombow=6;
      }
      break;
    case 6: //dec blue (inc red, dec green)
      (blue-goombowRate<=0)?(blue=0):(blue=blue-goombowRate);
      (red+goombowRate>=255)?(red=255):(red=red+goombowRate);
      (green-goombowRate<=0)?(green=0):(green=green-goombowRate);
      if(blue==0)
      {
        state_goombow=1;
      }
      break;
  }
}


void absStateMachine() { //State machine to reset the origin for the leds when x+y+start is held for a while
switch(state_abs)
  {
  case 0: //WAIT
  //do nothing unless the condition is met
  if (a&&b&&s) //if x and y and start are being pressed, go to the next step
  {
    state_abs=1;
  }
    break;
  case 1: //Begin timer
    time0abs = millis(); //timer0 started
    state_abs = 2;
    break;
  case 2: //Running timer
    time1abs = millis(); //timer1 ticking
    if (a==0||b==0||s==0) //if you stopped pressing x+y+start
    {
      state_abs = 3; //it was a false alarm
    }
    if (time1abs-time0abs>resetDelay) //if you exceeded the amount of time we set for a sufficiently long xystart hold
    {
      state_abs = 4; //confirmed
    }
    break;
  case 3: //False Alarm
    time1abs=time0abs; //reset the timer
    state_abs = 0; //reset to the beginning
    break;
  case 4: //Confirmed trying to reset
    mode=0; //also go to 1 to confirm the reset worked
    state_abs = 0; //reset the state machine
    break;
  }
}


void wave(){ //state machine for the startup wave animation
  switch(state_wave){
  case 0: //1st rising, 2nd falling 3rd falling
    (wave1+waveRate>=255)?(wave1=255):(wave1=wave1+waveRate);
    (wave2-waveRate<=0)?(wave2=0):(wave2=wave2-waveRate);
    (wave3-waveRate<=0)?(wave3=0):(wave3=wave3-waveRate);
    if(wave1>=127){
      state_wave=1;
    }
    break;
  case 1: //2nd rising, 1st rising 3rd falling
    (wave1+waveRate>=255)?(wave1=255):(wave1=wave1+waveRate);
    (wave2+waveRate>=255)?(wave2=255):(wave2=wave2+waveRate);
    (wave3-waveRate<=0)?(wave3=0):(wave3=wave3-waveRate);
    if(wave1>=255){
      state_wave=2;
    }
    break;
  case 2: //3rd rising 1st falling, 2nd rising
    (wave1-waveRate<=0)?(wave1=0):(wave1=wave1-waveRate);
    (wave2+waveRate>=255)?(wave2=255):(wave2=wave2+waveRate);
    (wave3+waveRate>=255)?(wave3=255):(wave3=wave3+waveRate);
    if(wave2>=255){
      state_wave=3;
    }
    break;
  case 3: //3rd rising, 2nd falling, 1st falling
    (wave1-waveRate<=0)?(wave1=0):(wave1=wave1-waveRate);
    (wave2-waveRate<=0)?(wave2=0):(wave2=wave2-waveRate);
    (wave3+waveRate>=255)?(wave3=255):(wave3=wave3+waveRate);
    if(wave3>=255){
      state_wave=4;
    }
    break;
  case 4: //3rd falling, 2nd falling, 1st falling
    (wave1-waveRate<=0)?(wave1=0):(wave1=wave1-waveRate);
    (wave2-waveRate<=0)?(wave2=0):(wave2=wave2-waveRate);
    (wave3-waveRate<=0)?(wave3=0):(wave3=wave3-waveRate);
    if(wave3<=0){
      state_wave=5;
    }
    break;
  case 5: //end
    waveDone=!waveDone;
    amountWaved++;
    state_wave=0;
    break;
  case 6://do nothing
    break;
  }
}

void switchModes()
{
  if(!dDownPrev && dDown&&b)  //if it was a 0 and then is a 1 (it was pressed down) then increment to the next mode
  {
    mode++;
    mode = mode %7; //7 modes
  }
  dDownPrev = dDown;
}

void buttonPressColors()
{
  
  if (y==1)
  {
    leds[0] = CRGB(255,255,255);
    FastLED.show();
  }
  else
  {
    leds[0] = CRGB(0,0,0);
    FastLED.show();
  }
  
  if (b==1)
  {
    leds[1] = CRGB(255,0,0);
    FastLED.show();
  }
  else
  {
    leds[1] = CRGB::Black;
    FastLED.show();
  }
  
  if (a==1)
  {
    leds[2] = CRGB(0,255,0);
    FastLED.show();
  }
  else
  {
    leds[2] = CRGB::Black;
    FastLED.show();
  }
  
  if (x==1)
  {
    leds[3] = CRGB(255,255,255);
    FastLED.show();
  }
  else
  {
    leds[3] = CRGB::Black;
    FastLED.show();
  }
}

void verifyEndPoll()
{
  TCNT0=0; //reset the timer
  while(1) //loop this until you are for sure not in a polling cycle
  {
    if(readPin(controllerDataPin)==0) //if the line goes low
    {
      TCNT0=0; //reset the timer
    }
    if(TCNT0>200) //if enough time has passed
    {
      break; //go to the next step
    }
  }
}


void interpretData()
{
  if (originPollNumber < 20) //Use the 20th poll for calculating inputs
  {
//    leds[0]=CRGB(0,0,0);
//    FastLED.show();
    started=false; //We're at the beginning
    originPollNumber++; //now we're 1 step closer to not the beginning
  }
  else
  {
//    leds[0]=CRGB(244,30,100);
//    FastLED.show();
    started=true; //No longer at the beginning
  }
  xA = 0; //Inital x Axis 8 bit value
  yA = 0;

  for(int i=7; i>=0;i--){xA = xA+(rawData[22-i] << i);} //Analog stick x axis
  for(int i=7; i>=0;i--){yA = yA+(rawData[30-i] << i);} //Analog stick y axis

  if (started == false) //Only do this if the controller was just plugged in (20th polling cycle)
  {
    xAorigin = xA-128;  //calibrate the origin offsets
    yAorigin = yA-128;
  }

  //Convert the raw controller numbers to (close to) Melee numbers
  xAresult=((xA-xAorigin)-128)/80.0; //Factor in the origins that were found at the beginning with the origin poll
  yAresult=((yA-yAorigin)-128)/80.0;

  //calculate the exact melee values
  hA = sqrt(yAresult*yAresult+xAresult*xAresult); //hA = hypotenuse of analog stick
  maxhAnew=max(1.0,hA); //round the hypotenuse to 1.0 if it's over 1.0
  xAnew=abs(xAresult/maxhAnew); //take the absolute value so you don't give a negative number to the LEDs
  yAnew=abs(yAresult/maxhAnew);

  //create melee deadzones
  if (xAnew<.2875){xAnew=0.0;} 
  if (yAnew<.2875){yAnew=0.0;}

  /////////////////////////////////////////Analog data calculations end
  
  s=rawData[2];
  y=rawData[3];
  x=rawData[4];
  b=rawData[5];
  a=rawData[6];
  l=rawData[8];
  r=rawData[9];
  z=rawData[10];
  dUp=rawData[11];
  dDown=rawData[12];
  dRight=rawData[13];
  dLeft=rawData[14];
}


void align()
{
  uint16_t buffers=0x0000; //set the buffer to all 0
  while (buffers!=0b00000000011000000) //wait for the correct section of the beginning message to shift in
  {
    buffers = (buffers << 1) | readBit(); //continuallly read in data
  }
  asm volatile(MICROSECOND_NOPS MICROSECOND_NOPS MICROSECOND_NOPS); //wait until the middle of the next bit
  
  while( readPin(controllerDataPin)==0 ){} 
  while( readPin(controllerDataPin)==1 ){}
  while( readPin(controllerDataPin)==0 ){}
  while( readPin(controllerDataPin)==1 ){}
  while( readPin(controllerDataPin)==0 ){}
  while( readPin(controllerDataPin)==1 ){}
 //make sure to go through the next 3 bits (the timing of the controller section can be different 
 //than the console data so these falling/rising edge statements are needed to align them
}

static inline bool readBit()
{
  while( readPin(controllerDataPin)==0 ){} //wait for falling edge
  while( readPin(controllerDataPin)==1 ){}
  //falling edge has happened
  asm volatile("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" );
  return readPin(controllerDataPin); 
}


void readControllerData(int bits)     //Function that gets called for reading the controller+GameCube data
{
  bool *rawDataPtr = rawData;    //makes a pointer to increment through the different memory locations of rawData  
  while(bits>0)                  //while loop to go through all 89 bits of data
  {
    while( readPin(controllerDataPin)==0 ){}
    while( readPin(controllerDataPin)==1 ){}
    asm volatile( "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" );
    *rawDataPtr = readPin(controllerDataPin);  
    ++rawDataPtr;                                      //increments the pointer to the next spot in the array
    bits--;                                            //decreases the bits by 1
  }
}
