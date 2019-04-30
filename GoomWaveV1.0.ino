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
 */

#include <FastLED.h>    //Publicly available library for sending data to LEDs
                        
#define PIN_READ( pin )  ((PIND&(1<<(pin)))>>pin)     //This line uses bit shifting to read directly from registers
                                                      //The first part of this is (PIND&(1<<(pin))) which is non-zero (B01000000 = 128) when pin 7 goes high and 0 when it's low.
                                                      //>>pin converts the first part back into a 1 or a 0 instead of a 128 or a 0
                                                      //So overall this line just spits out a 1 or a 0 depending on whether pin is high or low like a faster version of digitalRead()
    
unsigned char rawData[ 128 ];                         //Setting aside memory for the data string of the Gamecube signal + controller signal serial string (128 may be larger than needed as the string is only 89 long)
#define MICROSECOND_NOPS "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" //Assembly code defining how long a microsecond is
#define GC_PIN        7 //Pin that reads the GameCube data
                        //It appears only certain pins work with this code for some reason, pin 8 did not work when I tried it, more investigation needed
#define GC_PREFIX    25 //the amount of bits that the Gamecube sends to the controller before it responds
#define GC_BITCOUNT  64 //the amount of bits that the controller sends after it reads the Gamecube's serial message
                        //total serial string is 25+64= 89 bits long

// How many leds in your strip?
#define NUM_LEDS 5

//Colors are RedBlueGreen (RBG) format
#define Black     0, 0, 0
#define Red       255, 0, 0
#define Blue      0, 255, 0
#define Green     0, 0, 255
#define White     255, 255, 255
#define Purple    255, 255, 0

// Define the array of leds
CRGB leds[NUM_LEDS]; 

//Variable declarations for buttons and mode states (b = TRUE means b is pressed, b = FALSE means b is not pressed, etc.)
bool a; 
bool b;
bool x;
bool y;
bool dDown;
bool dDownPrev; //previous state of dpad down so that you can switch modes
byte modeCount = 0;
byte mode;

void setup() 
{ 
//  Optional Serial monitor prints for debugging purposes  
  Serial.begin(9600);
  Serial.println("Initializing LEDs");
       FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
}

template< unsigned int pin >      
void read_oneWire( unsigned char bits )     //Function that gets called for reading the controller+GameCube data
{
    unsigned char *rawDataPtr = rawData;    //makes a pointer to increment through the different memory locations of rawData
read_loop:
      // Wait for the line to go high then low.
      while( !PIN_READ(GC_PIN)) ; 
      while( PIN_READ(GC_PIN));
  
      // Wait ~2us between line reads
      //this is what the camecube controller protocol looks like
      //http://www.int03.co.uk/crema/hardware/gamecube/gc-control.html
      asm volatile( MICROSECOND_NOPS MICROSECOND_NOPS );
  
      // Read a bit from the line and store as a byte in "rawData"
      *rawDataPtr = PIN_READ(pin);
       ++rawDataPtr; //increment the pointer to go to the next position
      if( --bits == 0 ) 
      {
      return; //once you read all 89 bits exit the loop
      }
    goto read_loop; //If all 89 bits haven't been read yet repeat the read loop
}

inline void sendRawData( unsigned char first, unsigned char count ) //Function for sending messages to LEDs after reading the data
{
    //setup before modes
    a = rawData[32]; //the a button is the 32nd bit in the Gamecube+controller data string
    b = rawData[31];
    x = rawData[30];
    y = rawData[29];
    dDown = rawData[38];
    
    if(!dDownPrev && dDown)  //if it was a 0 and then is a 1 (it was pressed down) then increment to the next mode
    {
      modeCount = modeCount + 1;
      mode = modeCount %6; //6 modes
      Serial.print("the mode is: ");
      Serial.println(mode);
    }
    dDownPrev = dDown;
    //end setup before modes
    
    /////////////////////////////////////////////////modes
      if (mode == 0)//mode 0, off
      {
        for(int i=0;i<5;i++)
        {  
          leds[i] = CRGB(Black);
        }
        FastLED.show();
      }
      else if (mode == 1) //mode 1, constant colors
      {
        for(int i=0;i<5;i++)
        {    
          leds[i] = CRGB(White);
        }
        FastLED.show();
      }
      else if (mode == 2) //mode 1, constant colors
      {
        for(int i=0;i<5;i++)
        {   
          leds[i] = CRGB(Red);
        }
        FastLED.show();
      }
      else if (mode == 3) //mode 1, constant colors
      {
        for(int i=0;i<5;i++)
        {   
          leds[i] = CRGB(Green);
        }
        FastLED.show();
      }
      else if (mode == 4) //mode 1, constant colors
      {
        for(int i=0;i<5;i++)
        {   
        leds[i] = CRGB(Blue);
        }
        FastLED.show();
      }
      else if (mode == 5) //mode 2, reactive
      {
        for(int i=0;i<5;i++)
        { 
          leds[i] = CRGB(Purple);
        }
        FastLED.show();
        }
      else
      {
        Serial.println("error");
      }
    ////////////////////////////////////end modes
}


void loop() 
{ 
    //Fairly self explanitory main loop. It just reads and then writes. Only issue is the sendRawData function can't take up too long or it will not read the date in time for the next polling cycle
    noInterrupts();
    read_oneWire< GC_PIN >( GC_PREFIX + GC_BITCOUNT );
    
    interrupts();
    sendRawData( GC_PREFIX , GC_BITCOUNT );
}
