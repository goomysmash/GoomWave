#include <FastLED.h> //This library is required to run goomwave

//Changable settings at the beginning
const uint8_t controllerDataPin = 4; //Data pin for the controller. Keep this the same if possible to avoid dealing with changing the next line too.
#define readPin(controllerDataPin)    ((PIND&(1<<(controllerDataPin)))>>controllerDataPin)//If you change controller data pin, you may need to change "PIND" to "PINB" or otherwise
#define DATA_PIN 2 //data going to the LEDs. Change this if you want to use a different pin
#define NUM_LEDS 13 //This particular version of goomwave is for 5 LEDs on the back of a controller
//End changable settings

//Data reading delcarations
#define dataStringLength              63 //for calculating the length of the data we're taking in from the data line
bool rawData[dataStringLength];          //array for holding the raw data from the data line
#define MICROSECOND_NOPS "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"   //Assembly for 1 microsecond using clock pulses on a 16MHz processor. Needs to be changed for other clock speed processors.

//Declarations for FastLED
CRGB leds[NUM_LEDS];                     //required declaration for the FastLED library for how many LEDs we're using

//mode declarations
uint8_t mode = 0; //Which mode we're in

//origin poll declarations for whether to calibrate the controller or not
bool started = false;
uint8_t originPollNumber = 0;

//xystart reset variables. If you press x+y+start for a half second it recalibrates the sticks
uint8_t state_xys = 0; //for the state machine
unsigned short time1xys = 0; //Timer
unsigned short time0xys = 0; //Value at start of timer
const unsigned short resetDelay = 200; //How long to wait holding x+y+start to recalibrate

//abstart reset variables. If you press a+b+start for a half second it goes to mode 0
uint8_t state_abs = 0; //for the state machine
unsigned short time1abs = 0; //Timer
unsigned short time0abs = 0; //Value at start of timer
//const unsigned short resetDelay = 200; //How long to wait holding x+y+start to recalibrate

////------------------------------//// begin buttonstates declarations
//State machine variable declarations
const uint8_t buttonDelay = 10; //How long the button will flash bright before holding its color
uint8_t time0 = 0; //time at the beginning of the timer

//b button press variables
uint8_t state_b = 0; //for the state machine
#define bFlashColor 240,255,240 //Change the numbers (0-255) in this line for different flash colors
#define bHoldColor  100,255,100 //Change the numbers (0-255) in this line for different hold colors

//a button press variables
uint8_t state_a = 0; //for the state machine
#define aFlashColor 240,240,180 //Change the numbers (0-255) in this line for different flash colors
#define aHoldColor  240,240,10 //Change the numbers (0-255) in this line for different hold colors

//x button press variables
uint8_t state_x = 0; //for the state machine
#define xFlashColor 180,180,250 //Change the numbers (0-255) in this line for different flash colors
#define xHoldColor  50,50,220 //Change the numbers (0-255) in this line for different hold colors

//y button press variables
uint8_t state_y = 0; //for the state machine
#define yFlashColor 240,25,240 //Change the numbers (0-255) in this line for different flash colors
#define yHoldColor  100,25,100 //Change the numbers (0-255) in this line for different hold colors

//l button press variables
uint8_t state_l = 0; //for the state machine
#define lFlashColor 80,220,255 //Change the numbers (0-255) in this line for different flash colors
#define lHoldColor  10,180,255 //Change the numbers (0-255) in this line for different hold colors

//r button press variables
uint8_t state_r = 0; //for the state machine
#define rFlashColor 255,50,255 //Change the numbers (0-255) in this line for different flash colors
#define rHoldColor  255,10,255 //Change the numbers (0-255) in this line for different hold colors

//z button press variables
uint8_t state_z = 0; //for the state machine
#define zFlashColor 205,205,100 //Change the numbers (0-255) in this line for different flash colors
#define zHoldColor  205,205,50 //Change the numbers (0-255) in this line for different hold colors

//start (s) button press variables
uint8_t state_s = 0; //for the state machine
#define sFlashColor 255,255,255 //Change the numbers (0-255) in this line for different flash colors
#define sHoldColor  200,200,200 //Change the numbers (0-255) in this line for different hold colors
////------------------------------//// end buttonstates

/////=========================================///// begin button/stick declarations
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
/////=========================================///// end button/stick declarations

//////////_______________________///////// Begin LED animation declarations
uint8_t led1Brightness = 0; //for the anime flash
uint8_t led2Brightness = 0;
uint8_t led3Brightness = 0;
uint8_t led4Brightness = 0;
uint8_t led5Brightness = 0;

uint8_t state_anime = 0; //state machine declarations for anime flashes
uint8_t state_animeReverse = 0;

bool justChanged = true; //if the mode just changed
bool animeDone=false; //if the anime flash is done

uint8_t state_rippleA = 0;

uint8_t state_goombow = 0;
uint8_t state_goombowOrange = 0;
uint8_t state_goombowPurple = 0;
uint8_t state_goombowTurquoise = 0;

uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;

uint8_t ledLayer1 = 0;
uint8_t ledLayer2 = 0;
uint8_t ledLayer3 = 0;
uint8_t ledLayer4 = 0;

#define goombowRate  1
#define ledRate       20 //rate for anime flash
#define ledRatePuddle       20 //rate for ripple animation
//////////_______________________///////// End LED animation declarations

void setup() //initial setup
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  //attach the LED type. In this case we're using neopixels. Other LED types are possible.
  Serial.begin(9600);
  pinMode(controllerDataPin, INPUT_PULLUP); //set the controller data pin to have a pullup resistor and an input
  
  TCCR0B &= ~(1 << CS02); //0
  TCCR0B |= (1 << CS01); //1
  TCCR0B &= ~(1 << CS00); //0 //prescaler for timer0 (for reading end of polling)
  
  TCCR2B |= (1 << CS22); //1
  TCCR2B |= (1 << CS21); //1 1/1/1 is maximum setting (1024)
  TCCR2B |= (1 << CS20); //1 prescaler for timer2 (for synchronizing timing accross different polling rates
  cli();//Turn off interrupts
} 


void loop() //main loop where modes happen
{
  verifyEndPoll(); //wait and make sure you're at the end of a poll and not in the middle
  align(); //make sure you're aligned at the beginning of the poll reading cycle
  readControllerData(dataStringLength); //read in the data
  interpretData(); //process the data into usable numbers
  xysStateMachine(); //if x and y and start is pressed for long enough recalibrate and go to mode 1
  absStateMachine(); ////if a and b and start is pressed for long enough go to mode 0
  sei(); //enable interrupts
  //////////// start LED stuff
  switchModes(); //setup to be able to switch modes with Dpad down+b
  
  if (mode == 0) //Mode 0: off
  { 
    
    if (TCNT2 > 200) //if a certain amount of time has passed. To synchronize accross different polling rates
    {
      anime(); //first part of anime flash
      animeReverse(); //second part of anime flash
      TCNT2 = 0; //Reset the timer
    }
    leds[0]=CRGB(led1Brightness,led1Brightness,led1Brightness); //led brightness is calculated in anime state machines
    leds[1]=CRGB(led2Brightness,led2Brightness,led2Brightness);
    leds[2]=CRGB(led3Brightness,led3Brightness,led3Brightness);
    leds[3]=CRGB(led4Brightness,led4Brightness,led4Brightness);
    leds[4]=CRGB(led5Brightness,led5Brightness,led5Brightness);
    leds[5]=CRGB(led5Brightness,led5Brightness,led5Brightness);
    leds[6]=CRGB(led4Brightness,led4Brightness,led4Brightness);
    leds[7]=CRGB(led3Brightness,led3Brightness,led3Brightness);
    leds[8]=CRGB(led2Brightness,led2Brightness,led2Brightness);
    leds[9]=CRGB(led2Brightness,led2Brightness,led2Brightness);
    leds[10]=CRGB(led2Brightness,led2Brightness,led2Brightness);
    leds[11]=CRGB(led1Brightness,led1Brightness,led1Brightness);
    leds[12]=CRGB(led2Brightness,led2Brightness,led2Brightness);
       
    if (animeDone==true) //if the 2 anime flash animations are done
    {
      leds[0]=CRGB(0,0,0); //turn the leds off (now mode 0 will be off mode)
      leds[0]=CRGB(0,0,0);
      leds[0]=CRGB(0,0,0);
      leds[0]=CRGB(0,0,0);
      leds[0]=CRGB(0,0,0);
    }
    FastLED.show();    
  }
  
  else if (mode == 1) //mode 1 react to buttons and stick
  {
    red=0; //make the background black
    green=0;
    blue=0;
    
    if(yAnew==0&&xAnew==0){ //if the stick isn't pressed, don't light up the stick
      leds[5]=CRGB(red,green,blue);
    }
    else{ //if it is being pressed light up the stick
      leds[5] = CRGB(yAnew*255,xAnew*255,xAnew*255); //light up based on analog stick inputs
    }
    if(yCnew==0&&xCnew==0){
      leds[12]=CRGB(red,green,blue);
    }
    else{
      leds[12] = CRGB(yCnew*255,xCnew*255,yCnew*255); //light up based on analog stick inputs
    }
    (a==1)?(leds[10]=CRGB(0,255,0)):(leds[10]=CRGB(red,green,blue));
    (b==1)?(leds[9]=CRGB(255,0,0)):(leds[9]=CRGB(red,green,blue));
    (x==1)?(leds[11]=CRGB(255,255,255)):(leds[11]=CRGB(red,green,blue));
    (y==1)?(leds[8]=CRGB(255,255,255)):(leds[8]=CRGB(red,green,blue));
    (s==1)?(leds[7]=CRGB(255,255,255)):(leds[7]=CRGB(red,green,blue));
    if(dDown==1||dUp==1||dLeft==1||dRight==1){
      leds[6]=CRGB(255*dDown,255*dUp,(127*dLeft)+(127*dRight));
    }
    else{
      leds[6] = CRGB(red,green,blue);
    }
    if(Lnew==0){
//      leds[2]=CRGB(red,green,blue);
      leds[3]=CRGB(red,green,blue);
      leds[4]=CRGB(red,green,blue);
    }
    else{
//      leds[2]=CRGB(Lnew*255,Lnew*255,Lnew*255);
      leds[3]=CRGB(Lnew*255,Lnew*255,Lnew*255);
      leds[4]=CRGB(Lnew*255,Lnew*255,Lnew*255);
    }
    if(Rnew==0){
      leds[0]=CRGB(red,green,blue);
      leds[1]=CRGB(red,green,blue);
//      leds[2]=CRGB(red,green,blue);
    }
    else{
      leds[0]=CRGB(Rnew*255,Rnew*255,Rnew*255);
      leds[1]=CRGB(Rnew*255,Rnew*255,Rnew*255);
//      leds[2]=CRGB(Rnew*255,Rnew*255,Rnew*255);
    }
    if(l==1){
      leds[2]=CRGB(0,255,255);
      leds[3]=CRGB(0,255,255);
      leds[4]=CRGB(0,255,255);
    }
    else
    {
      leds[2]=CRGB(red,green,blue);
    }
    if(r==1){
      leds[0]=CRGB(0,0,255);
      leds[1]=CRGB(0,0,255);
      leds[2]=CRGB(0,0,255);
    }
    if(z==1){
      leds[0]=CRGB(255,0,255);
      leds[1]=CRGB(255,0,255);
      leds[2]=CRGB(255,0,255);
    }
    FastLED.show();
  }
  
  else if (mode == 2) //mode 2 abxy ripple
  { 
    if((!aPrev&&a)||(!bPrev&&b)||(!xPrev&&x)||(!yPrev&&y)) //start the ripple right as a button is pressed
    {
      state_rippleA=0;
    }
    aPrev=a; //then update the button presses
    bPrev=b;
    xPrev=x;
    yPrev=y;
    if (TCNT2 > 200)
    {
      rippleA(); //state machine that does the ripple animation
      TCNT2 = 0;
    }
    //LED layers: 1st: 11,9,8. 2nd: 7,2,1,12. 3rd:6,5,0,3. 4th: 4
    leds[0]=CRGB(ledLayer3*.5,0,ledLayer3);
    leds[1]=CRGB(ledLayer2*.5,0,ledLayer2);
    leds[2]=CRGB(ledLayer2*.5,0,ledLayer2);
    leds[3]=CRGB(ledLayer3*.5,0,ledLayer3);
    leds[4]=CRGB(ledLayer4*.5,0,ledLayer4);
    leds[5]=CRGB(ledLayer3*.5,0,ledLayer3);
    leds[6]=CRGB(ledLayer3*.5,0,ledLayer3);
    leds[7]=CRGB(ledLayer2*.5,0,ledLayer2);
    leds[8]=CRGB(ledLayer1*.5,0,ledLayer1);
    leds[9]=CRGB(ledLayer1*.5,0,ledLayer1);
    leds[10]=CRGB(ledLayer1*.5,0,ledLayer1);
    leds[11]=CRGB(ledLayer1*.5,0,ledLayer1);
    leds[12]=CRGB(ledLayer2*.5,0,ledLayer2);
    if(a==1)
    {
      leds[10]=CRGB(255,255,255);
    }
    if(b==1)
    {
      leds[9]=CRGB(255,255,255);
    }
    if(x==1)
    {
      leds[11]=CRGB(255,255,255);
    }
    if(y==1)
    {
      leds[8]=CRGB(255,255,255);
    }
    FastLED.show();
  }
  
  else if (mode ==3) //mode 3 LED notches
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
  
  else if (mode == 4)//mode 4 regular goombow w/ react
  {
    if(justChanged==true) //if we just changed modes, reset the animation to the beginning
    {
      state_goombow=0;
    }
    if(TCNT2>200)
    {
      goombow(); //goomy's rainbow animation state machine
      TCNT2=0;
    }
    for(int i=0; i<NUM_LEDS; i++){
      leds[i]=CRGB(red,green,blue); //red green blue are changed in goombow()
    }
    if(yAnew==0&&xAnew==0){
      leds[5]=CRGB(red,green,blue);
    }
    else{
      leds[5] = CRGB(yAnew*255,xAnew*255,xAnew*255); //light up based on analog stick inputs
    }
    if(yCnew==0&&xCnew==0){
      leds[12]=CRGB(red,green,blue);
    }
    else{
      leds[12] = CRGB(yCnew*255,xCnew*255,yCnew*255); //light up based on analog stick inputs
    }
    (a==1)?(leds[10]=CRGB(255,255,255)):(leds[10]=CRGB(red,green,blue));
    (b==1)?(leds[9]=CRGB(255,255,255)):(leds[9]=CRGB(red,green,blue));
    (x==1)?(leds[11]=CRGB(255,255,255)):(leds[11]=CRGB(red,green,blue));
    (y==1)?(leds[8]=CRGB(255,255,255)):(leds[8]=CRGB(red,green,blue));
    (s==1)?(leds[7]=CRGB(255,255,255)):(leds[7]=CRGB(red,green,blue));
    if(dDown==1||dUp==1||dLeft==1||dRight==1){
      leds[6]=CRGB(255*dDown,255*dUp,(127*dLeft)+(127*dRight));
    }
    else{
      leds[6] = CRGB(red,green,blue);
    }
    if(Lnew==0){
//      leds[2]=CRGB(red,green,blue);
      leds[3]=CRGB(red,green,blue);
      leds[4]=CRGB(red,green,blue);
    }
    else{
//      leds[2]=CRGB(Lnew*255,Lnew*255,Lnew*255);
      leds[3]=CRGB(Lnew*255,Lnew*255,Lnew*255);
      leds[4]=CRGB(Lnew*255,Lnew*255,Lnew*255);
    }
    if(Rnew==0){
      leds[0]=CRGB(red,green,blue);
      leds[1]=CRGB(red,green,blue);
//      leds[2]=CRGB(red,green,blue);
    }
    else{
      leds[0]=CRGB(Rnew*255,Rnew*255,Rnew*255);
      leds[1]=CRGB(Rnew*255,Rnew*255,Rnew*255);
//      leds[2]=CRGB(Rnew*255,Rnew*255,Rnew*255);
    }
    if(r==1){
      leds[0]=CRGB(0,0,255);
      leds[1]=CRGB(0,0,255);
      leds[2]=CRGB(0,0,255);
    }
    if(l==1){
      leds[2]=CRGB(0,255,255);
      leds[3]=CRGB(0,255,255);
      leds[4]=CRGB(0,255,255);
    }
    if(z==1){
      leds[0]=CRGB(255,0,255);
      leds[1]=CRGB(255,0,255);
      leds[2]=CRGB(255,0,255);
    }
    FastLED.show();
  }
  
  else if (mode == 5) //goombow turquoise
  {
    if(justChanged==true)
    {
      state_goombowTurquoise=0;
    }
    
    if(TCNT2>200)
    {
      goombowTurquoise();
      TCNT2=0;
    }
    for(int i=0; i<NUM_LEDS; i++){
      leds[i]=CRGB(red,green,blue);
    }
    if(yAnew==0&&xAnew==0){
      leds[5]=CRGB(red,green,blue);
    }
    else{
      leds[5] = CRGB(yAnew*255,xAnew*255,xAnew*255); //light up based on analog stick inputs
    }
    if(yCnew==0&&xCnew==0){
      leds[12]=CRGB(red,green,blue);
    }
    else{
      leds[12] = CRGB(yCnew*255,xCnew*255,yCnew*255); //light up based on analog stick inputs
    }
    (a==1)?(leds[10]=CRGB(255,255,255)):(leds[10]=CRGB(red,green,blue));
    (b==1)?(leds[9]=CRGB(255,255,255)):(leds[9]=CRGB(red,green,blue));
    (x==1)?(leds[11]=CRGB(255,255,255)):(leds[11]=CRGB(red,green,blue));
    (y==1)?(leds[8]=CRGB(255,255,255)):(leds[8]=CRGB(red,green,blue));
    (s==1)?(leds[7]=CRGB(255,255,255)):(leds[7]=CRGB(red,green,blue));
    if(dDown==1||dUp==1||dLeft==1||dRight==1){
      leds[6]=CRGB(255*dDown,255*dUp,(127*dLeft)+(127*dRight));
    }
    else{
      leds[6] = CRGB(red,green,blue);
    }
    if(Lnew==0){
//      leds[2]=CRGB(red,green,blue);
      leds[3]=CRGB(red,green,blue);
      leds[4]=CRGB(red,green,blue);
    }
    else{
//      leds[2]=CRGB(Lnew*255,Lnew*255,Lnew*255);
      leds[3]=CRGB(Lnew*255,Lnew*255,Lnew*255);
      leds[4]=CRGB(Lnew*255,Lnew*255,Lnew*255);
    }
    if(Rnew==0){
      leds[0]=CRGB(red,green,blue);
      leds[1]=CRGB(red,green,blue);
//      leds[2]=CRGB(red,green,blue);
    }
    else{
      leds[0]=CRGB(Rnew*255,Rnew*255,Rnew*255);
      leds[1]=CRGB(Rnew*255,Rnew*255,Rnew*255);
//      leds[2]=CRGB(Rnew*255,Rnew*255,Rnew*255);
    }
    if(r==1){
      leds[0]=CRGB(0,0,255);
      leds[1]=CRGB(0,0,255);
      leds[2]=CRGB(0,0,255);
    }
    if(l==1){
      leds[2]=CRGB(0,255,255);
      leds[3]=CRGB(0,255,255);
      leds[4]=CRGB(0,255,255);
    }
    if(z==1){
      leds[0]=CRGB(255,0,255);
      leds[1]=CRGB(255,0,255);
      leds[2]=CRGB(255,0,255);
    }
    FastLED.show();
  }

  else if (mode == 6) //goombow Orange
  {
    if(justChanged==true)
    {
      state_goombowOrange=0;
    }
    if(TCNT2>200)
    {
      goombowOrange();
      TCNT2=0;
    }
    for(int i=0; i<NUM_LEDS; i++){
      leds[i]=CRGB(red,green,blue);
    }
    if(yAnew==0&&xAnew==0){
      leds[5]=CRGB(red,green,blue);
    }
    else{
      leds[5] = CRGB(yAnew*255,xAnew*255,xAnew*255); //light up based on analog stick inputs
    }
    if(yCnew==0&&xCnew==0){
      leds[12]=CRGB(red,green,blue);
    }
    else{
      leds[12] = CRGB(yCnew*255,xCnew*255,yCnew*255); //light up based on analog stick inputs
    }
    (a==1)?(leds[10]=CRGB(255,255,255)):(leds[10]=CRGB(red,green,blue));
    (b==1)?(leds[9]=CRGB(255,255,255)):(leds[9]=CRGB(red,green,blue));
    (x==1)?(leds[11]=CRGB(255,255,255)):(leds[11]=CRGB(red,green,blue));
    (y==1)?(leds[8]=CRGB(255,255,255)):(leds[8]=CRGB(red,green,blue));
    (s==1)?(leds[7]=CRGB(255,255,255)):(leds[7]=CRGB(red,green,blue));
    if(dDown==1||dUp==1||dLeft==1||dRight==1){
      leds[6]=CRGB(255*dDown,255*dUp,(127*dLeft)+(127*dRight));
    }
    else{
      leds[6] = CRGB(red,green,blue);
    }
    if(Lnew==0){
//      leds[2]=CRGB(red,green,blue);
      leds[3]=CRGB(red,green,blue);
      leds[4]=CRGB(red,green,blue);
    }
    else{
//      leds[2]=CRGB(Lnew*255,Lnew*255,Lnew*255);
      leds[3]=CRGB(Lnew*255,Lnew*255,Lnew*255);
      leds[4]=CRGB(Lnew*255,Lnew*255,Lnew*255);
    }
    if(Rnew==0){
      leds[0]=CRGB(red,green,blue);
      leds[1]=CRGB(red,green,blue);
//      leds[2]=CRGB(red,green,blue);
    }
    else{
      leds[0]=CRGB(Rnew*255,Rnew*255,Rnew*255);
      leds[1]=CRGB(Rnew*255,Rnew*255,Rnew*255);
//      leds[2]=CRGB(Rnew*255,Rnew*255,Rnew*255);
    }
    if(r==1){
      leds[0]=CRGB(0,0,255);
      leds[1]=CRGB(0,0,255);
      leds[2]=CRGB(0,0,255);
    }
    if(l==1){
      leds[2]=CRGB(0,255,255);
      leds[3]=CRGB(0,255,255);
      leds[4]=CRGB(0,255,255);
    }
    if(z==1){
      leds[0]=CRGB(255,0,255);
      leds[1]=CRGB(255,0,255);
      leds[2]=CRGB(255,0,255);
    }
    FastLED.show();
    
  }

  else if (mode == 7) //goombow Purple
  {
    if(justChanged==true)
    {
      state_goombowPurple=0;
    }
    if(TCNT2>200)
    {
      goombowPurple();
      TCNT2=0;
    }
    for(int i=0; i<NUM_LEDS; i++){
      leds[i]=CRGB(red,green,blue);
    }
    if(yAnew==0&&xAnew==0){
      leds[5]=CRGB(red,green,blue);
    }
    else{
      leds[5] = CRGB(yAnew*255,xAnew*255,xAnew*255); //light up based on analog stick inputs
    }
    if(yCnew==0&&xCnew==0){
      leds[12]=CRGB(red,green,blue);
    }
    else{
      leds[12] = CRGB(yCnew*255,xCnew*255,yCnew*255); //light up based on analog stick inputs
    }
    (a==1)?(leds[10]=CRGB(255,255,255)):(leds[10]=CRGB(red,green,blue));
    (b==1)?(leds[9]=CRGB(255,255,255)):(leds[9]=CRGB(red,green,blue));
    (x==1)?(leds[11]=CRGB(255,255,255)):(leds[11]=CRGB(red,green,blue));
    (y==1)?(leds[8]=CRGB(255,255,255)):(leds[8]=CRGB(red,green,blue));
    (s==1)?(leds[7]=CRGB(255,255,255)):(leds[7]=CRGB(red,green,blue));
    if(dDown==1||dUp==1||dLeft==1||dRight==1){
      leds[6]=CRGB(255*dDown,255*dUp,(127*dLeft)+(127*dRight));
    }
    else{
      leds[6] = CRGB(red,green,blue);
    }
    if(Lnew==0){
//      leds[2]=CRGB(red,green,blue);
      leds[3]=CRGB(red,green,blue);
      leds[4]=CRGB(red,green,blue);
    }
    else{
//      leds[2]=CRGB(Lnew*255,Lnew*255,Lnew*255);
      leds[3]=CRGB(Lnew*255,Lnew*255,Lnew*255);
      leds[4]=CRGB(Lnew*255,Lnew*255,Lnew*255);
    }
    if(Rnew==0){
      leds[0]=CRGB(red,green,blue);
      leds[1]=CRGB(red,green,blue);
//      leds[2]=CRGB(red,green,blue);
    }
    else{
      leds[0]=CRGB(Rnew*255,Rnew*255,Rnew*255);
      leds[1]=CRGB(Rnew*255,Rnew*255,Rnew*255);
//      leds[2]=CRGB(Rnew*255,Rnew*255,Rnew*255);
    }
    if(r==1){
      leds[0]=CRGB(0,0,255);
      leds[1]=CRGB(0,0,255);
      leds[2]=CRGB(0,0,255);
    }
    if(l==1){
      leds[2]=CRGB(0,255,255);
      leds[3]=CRGB(0,255,255);
      leds[4]=CRGB(0,255,255);
    }
    if(z==1){
      leds[0]=CRGB(255,0,255);
      leds[1]=CRGB(255,0,255);
      leds[2]=CRGB(255,0,255);
    }
    FastLED.show();
  }

  else if (mode == 8) //stationary
  {
    leds[0]=CRGB(255,255,255);
    leds[1]=CRGB(255,255,255);
    leds[2]=CRGB(255,255,255);
    leds[3]=CRGB(255,255,255);
    leds[4]=CRGB(255,255,255);
    leds[5]=CRGB(255,255,255);
    leds[6]=CRGB(255,255,255);
    leds[7]=CRGB(255,255,255);
    leds[8]=CRGB(255,255,255);
    leds[9]=CRGB(255,0,0);
    leds[10]=CRGB(0,255,255);
    leds[11]=CRGB(255,255,255);
    leds[12]=CRGB(255,255,255);
    FastLED.show();
  }
  
  //////////// end LED stuff
  cli();
}

void goombowTurquoise(){ //goombow but omitting red
  switch(state_goombowTurquoise) {
    case 0://nothing
      green=255;
      blue=0;
      red=0;
      state_goombowTurquoise=1;
      break;
    case 1: //inc blue (inc green, dec red)
      (blue+goombowRate>=255)?(blue=255):(blue=blue+goombowRate);
      (green+goombowRate>=255)?(green=255):(green=green+goombowRate);
      (red-goombowRate<=0)?(red=0):(red=red-goombowRate);
      if(blue==255)
      {
        state_goombowTurquoise=2;
      }
      break;
    case 2: //dec green (inc blue, dec red)
      (green-goombowRate<=0)?(green=0):(green=green-goombowRate);
      (blue+goombowRate>=255)?(blue=255):(blue=blue+goombowRate);
      (red-goombowRate<=0)?(red=0):(red=red-goombowRate);
      if(green==0)
      {
        state_goombowTurquoise=3;
      }
      break;
    case 3: //inc red (inc blue, dec green)
      (blue+goombowRate>=255)?(blue=255):(blue=blue+goombowRate);
      (green+goombowRate>=255)?(green=255):(green=green+goombowRate);
      (red-goombowRate<=0)?(red=0):(red=red-goombowRate);
      if(green==255)
      {
        state_goombowTurquoise=4;
      }
      break;
    case 4: //dec blue (inc red, dec green)
      (blue-goombowRate<=0)?(blue=0):(blue=blue-goombowRate);
      (green+goombowRate>=255)?(green=255):(green=green+goombowRate);
      (red-goombowRate<=0)?(red=0):(red=red-goombowRate);
      if(blue==0)
      {
        state_goombowTurquoise=1;
      }
      break;
    
  }
}


void goombowPurple(){ //goombow but omitting green
  switch(state_goombowPurple) {
    case 0://nothing
      red=255;
      blue=0;
      green=0;
      state_goombowPurple=1;
      break;
    case 1: //inc blue (inc red, dec green)
      (blue+goombowRate>=255)?(blue=255):(blue=blue+goombowRate);
      (red+goombowRate>=255)?(red=255):(red=red+goombowRate);
      (green-goombowRate<=0)?(green=0):(green=green-goombowRate);
      if(blue==255)
      {
        state_goombowPurple=2;
      }
      break;
    case 2: //dec red (inc blue, dec green)
      (red-goombowRate<=0)?(red=0):(red=red-goombowRate);
      (blue+goombowRate>=255)?(blue=255):(blue=blue+goombowRate);
      (green-goombowRate<=0)?(green=0):(green=green-goombowRate);
      if(red==0)
      {
        state_goombowPurple=3;
      }
      break;
    case 3: //inc green (inc blue, dec red)
      (blue+goombowRate>=255)?(blue=255):(blue=blue+goombowRate);
      (red+goombowRate>=255)?(red=255):(red=red+goombowRate);
      (green-goombowRate<=0)?(green=0):(green=green-goombowRate);
      if(red==255)
      {
        state_goombowPurple=4;
      }
      break;
    case 4: //dec blue (inc green, dec red)
      (blue-goombowRate<=0)?(blue=0):(blue=blue-goombowRate);
      (red+goombowRate>=255)?(red=255):(red=red+goombowRate);
      (green-goombowRate<=0)?(green=0):(green=green-goombowRate);
      if(blue==0)
      {
        state_goombowPurple=1;
      }
      break;
    
  }
}


void goombowOrange(){ //goombow but omitting blue
  switch(state_goombowOrange) {
    case 0://nothing
      red=255;
      green=0;
      blue=0;
      state_goombowOrange=1;
      break;
    case 1: //inc green (inc red, dec blue)
      (green+goombowRate>=255)?(green=255):(green=green+goombowRate);
      (red+goombowRate>=255)?(red=255):(red=red+goombowRate);
      (blue-goombowRate<=0)?(blue=0):(blue=blue-goombowRate);
      if(green==255)
      {
        state_goombowOrange=2;
      }
      break;
    case 2: //dec red (inc green, dec blue)
      (red-goombowRate<=0)?(red=0):(red=red-goombowRate);
      (green+goombowRate>=255)?(green=255):(green=green+goombowRate);
      (blue-goombowRate<=0)?(blue=0):(blue=blue-goombowRate);
      if(red==0)
      {
        state_goombowOrange=3;
      }
      break;
    case 3: //inc blue (inc green, dec red)
      (green+goombowRate>=255)?(green=255):(green=green+goombowRate);
      (red+goombowRate>=255)?(red=255):(red=red+goombowRate);
      (blue-goombowRate<=0)?(blue=0):(blue=blue-goombowRate);
      if(red==255)
      {
        state_goombowOrange=4;
      }
      break;
    case 4: //dec green (inc blue, dec red)
      (green-goombowRate<=0)?(green=0):(green=green-goombowRate);
      (red+goombowRate>=255)?(red=255):(red=red+goombowRate);
      (blue-goombowRate<=0)?(blue=0):(blue=blue-goombowRate);
      if(green==0)
      {
        state_goombowOrange=1;
      }
      break;
    
  }
}

void goombow(){
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

void rippleA(){//ripple animation for abxy making a ripple through the controller
  switch(state_rippleA)
  {
  case 0: //nothing
    state_rippleA=1;
    break;
  case 1: //Triggered to start (in high level if a==1 leds=255
    state_rippleA=2;
    break;
  case 2: //1 inc (2,3,4 dec) 
    (ledLayer4-ledRatePuddle<0)?(ledLayer4=0):(ledLayer4=ledLayer4-ledRatePuddle);
    (ledLayer3-ledRatePuddle<0)?(ledLayer3=0):(ledLayer3=ledLayer3-ledRatePuddle);
    (ledLayer2-ledRatePuddle<0)?(ledLayer2=0):(ledLayer2=ledLayer2-ledRatePuddle);
    (ledLayer1+ledRatePuddle>255)?(ledLayer1=255):(ledLayer1=ledLayer1+ledRatePuddle); 
    if (ledLayer1>=127)
    {
      state_rippleA=3;
    }
    break;  
  case 3: //1,2 inc (3, 4 dec)
    // if 1>255 or 2>127, case 4
    (ledLayer4-ledRatePuddle<0)?(ledLayer4=0):(ledLayer4=ledLayer4-ledRatePuddle);
    (ledLayer3-ledRatePuddle<0)?(ledLayer3=0):(ledLayer3=ledLayer3-ledRatePuddle);
    (ledLayer2+ledRatePuddle>255)?(ledLayer2=255):(ledLayer2=ledLayer2+ledRatePuddle);
    (ledLayer1+ledRatePuddle>255)?(ledLayer1=255):(ledLayer1=ledLayer1+ledRatePuddle);
    if(ledLayer1>=255)
    {
      state_rippleA=4;
    }
    break;
  case 4: //2,3 inc 1 dec (4 dec)
    (ledLayer4-ledRatePuddle<0)?(ledLayer4=0):(ledLayer4=ledLayer4-ledRatePuddle);
    (ledLayer3+ledRatePuddle>255)?(ledLayer3=255):(ledLayer3=ledLayer3+ledRatePuddle);
    (ledLayer2+ledRatePuddle>255)?(ledLayer2=255):(ledLayer2=ledLayer2+ledRatePuddle);
    (ledLayer1-ledRatePuddle<0)?(ledLayer1=0):(ledLayer1=ledLayer1-ledRatePuddle);
    if(ledLayer2>=255)//if 2>=255 case 5
    {
      state_rippleA=5;
    }
    break;
  case 5: //3,4 inc, 2,1 dec
    (ledLayer4+ledRatePuddle>255)?(ledLayer4=255):(ledLayer4=ledLayer4+ledRatePuddle);
    (ledLayer3+ledRatePuddle>255)?(ledLayer3=255):(ledLayer3=ledLayer3+ledRatePuddle);
    (ledLayer2-ledRatePuddle<0)?(ledLayer2=0):(ledLayer2=ledLayer2-ledRatePuddle);
    (ledLayer1-ledRatePuddle<0)?(ledLayer1=0):(ledLayer1=ledLayer1-ledRatePuddle);
    if(ledLayer3>=255)// if 3>255 case 6
    {
      state_rippleA=6;
    }
    break;
  case 6: //4 inc, 3,2 dec (1 dec)
    (ledLayer4+ledRatePuddle>255)?(ledLayer4=255):(ledLayer4=ledLayer4+ledRatePuddle);
    (ledLayer3-ledRatePuddle<0)?(ledLayer3=0):(ledLayer3=ledLayer3-ledRatePuddle);
    (ledLayer2-ledRatePuddle<0)?(ledLayer2=0):(ledLayer2=ledLayer2-ledRatePuddle);
    (ledLayer1-ledRatePuddle<0)?(ledLayer1=0):(ledLayer1=ledLayer1-ledRatePuddle);
    if(ledLayer4>=255)// if 4>255 case 7
    {
      state_rippleA=7;
    }
    break;
  case 7: //4,3 dec (1,2 dec)
    (ledLayer4-ledRatePuddle<0)?(ledLayer4=0):(ledLayer4=ledLayer4-ledRatePuddle);
    (ledLayer3-ledRatePuddle<0)?(ledLayer3=0):(ledLayer3=ledLayer3-ledRatePuddle);
    (ledLayer2-ledRatePuddle<0)?(ledLayer2=0):(ledLayer2=ledLayer2-ledRatePuddle);
    (ledLayer1-ledRatePuddle<0)?(ledLayer1=0):(ledLayer1=ledLayer1-ledRatePuddle);
    if(ledLayer4<=127)// if 4<127 case 8
    {
      state_rippleA=8;
    }
    break;
  case 8: //4 dec (1,2,3 dec)
    (ledLayer4-ledRatePuddle<0)?(ledLayer4=0):(ledLayer4=ledLayer4-ledRatePuddle);
    (ledLayer3-ledRatePuddle<0)?(ledLayer3=0):(ledLayer3=ledLayer3-ledRatePuddle);
    (ledLayer2-ledRatePuddle<0)?(ledLayer2=0):(ledLayer2=ledLayer2-ledRatePuddle);
    (ledLayer1-ledRatePuddle<0)?(ledLayer1=0):(ledLayer1=ledLayer1-ledRatePuddle);
    if(ledLayer4<=0)// if 4<=0 case 9
    {
      state_rippleA=9;
    }
    break;
  case 9: //done (4,3,2,1)
    
    break;
  }
}

void verifyEndPoll() //verify we're at the end of a poll by waiting for enough time to pass without the line going low
{
  TCNT0 = 0; //reset the timer
  while (1) //loop this until you are for sure not in a polling cycle
  {
    if (readPin(controllerDataPin) == 0) //if the line goes low
    {
      TCNT0 = 0; //reset the timer
    }
    if (TCNT0 > 200) //if enough time has passed
    {
      break; //go to the next step
    }
  }
}

static inline bool readBit() //how we read an individual bit
{
  while ( readPin(controllerDataPin) == 0 ) {} //wait for falling edge
  while ( readPin(controllerDataPin) == 1 ) {}
  //falling edge has happened
  asm volatile("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" );
  return readPin(controllerDataPin);
}

void align() //make sure you're reading the correct type of poll and correct alignment of poll
{
  uint16_t buffers = 0x0000; //set the buffer to all 0
  while (buffers != 0b00000000011000000) //wait for the correct section of the beginning message to shift in
  {
    buffers = (buffers << 1) | readBit(); //continuallly read in data
  }
  asm volatile(MICROSECOND_NOPS MICROSECOND_NOPS MICROSECOND_NOPS); //wait until the middle of the next bit

  while ( readPin(controllerDataPin) == 0 ) {} //make sure to go through the next 3 bits (the timing of the controller section can be different
  while ( readPin(controllerDataPin) == 1 ) {} //than the console data so these falling/rising edge statements are needed to align them)
  while ( readPin(controllerDataPin) == 0 ) {}
  while ( readPin(controllerDataPin) == 1 ) {}
  while ( readPin(controllerDataPin) == 0 ) {}
  while ( readPin(controllerDataPin) == 1 ) {}
}

void readControllerData(int bits) //Function that gets called for reading the controller+GameCube data
{
  bool *rawDataPtr = rawData;    //makes a pointer to increment through the different memory locations of rawData
  while (bits > 0)               //while loop to go through all 89 bits of data
  {
    while ( readPin(controllerDataPin) == 0 ) {} 
    while ( readPin(controllerDataPin) == 1 ) {} //wait for the falling edge
    asm volatile( "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" ); //wait 2 microseconds
    *rawDataPtr = readPin(controllerDataPin);          //save the data into *rawDataPtr
    ++rawDataPtr;                                      //increments the pointer to the next spot in the array
    bits--;                                            //decreases the bits by 1
  }
}

void switchModes() //switch modes when b+dpad down is pressed
{
  if (!dDownPrev && dDown && b) //if it was a 0 and then is a 1 (it was pressed down) then increment to the next mode
  {
    mode++; //increment to the next mode
    mode = mode % 9; //8 modes. Make it so it loops back to mode 0 after mode 5.
    justChanged=true; //set the flag that we just changed modes
  }
  else
  {
    justChanged=false;
  }
  dDownPrev = dDown; //update dDownPrev
}

void interpretData() //interpret the data so that it's in an easily usable form
{
  if (originPollNumber < 20) //Use the 20th poll for calculating inputs
  {
    started = false; //We're at the beginning
    originPollNumber++; //now we're 1 step closer to not the beginning
  }
  else
  {
    started = true; //No longer at the beginning
  }
  
/////////////////////////////////////////Analog data calculations begin  
//read data from rawData into xA, xC etc bytes
  xA=0;
  yA=0;
  xC=0;
  yC=0;
  L=0;
  R=0;
  for(int i = 7; i >= 0; i--) {
    xA = xA + (rawData[22 - i] << i); //Analog stick x axis
  }
  for(int i = 7; i >= 0; i--) {
    yA = yA + (rawData[30 - i] << i); //Analog stick y axis
  }
  for(int i = 7; i >= 0; i--) {
    xC = xC + (rawData[38 - i] << i); //C stick x axis
  } 
  for(int i = 7; i >= 0; i--) {
    yC = yC + (rawData[46 - i] << i); //C stick y axis
  } 
  for(int i = 7; i >= 0; i--) {
    L = L + (rawData[54 - i] << i);
  }   //L analog
  for(int i = 7; i >= 0; i--) {
    R = R + (rawData[62 - i] << i);
  }   //R analog


  if (started == false) //Only do this if the controller was just plugged in (20th polling cycle)
  {
    xAorigin = xA - 128; //calibrate the origin offsets
    yAorigin = yA - 128;
    xCorigin = xC - 128; // C stick 
    yCorigin = yC - 128; 
    Lorigin = L; //L
    Rorigin = R;
  }

  //Convert the raw controller numbers to (close to) Melee numbers
  xAresult = ((xA - xAorigin) - 128) / 80.0; //Factor in the origins that were found at the beginning with the origin poll
  yAresult = ((yA - yAorigin) - 128) / 80.0;
  xCresult = ((xC - xCorigin) - 128) / 80.0; //C stick
  yCresult = ((yC - yCorigin) - 128) / 80.0;
  Lresult = (L - Lorigin) / 140.0;
  Rresult = (R - Rorigin) / 140.0;

  //calculate the exact melee values
  hA = sqrt(yAresult * yAresult + xAresult * xAresult); //hA = hypotenuse of analog stick
  hC = sqrt(yCresult * yCresult + xCresult * xCresult); //hC = hypotenuse of C stick
  maxhAnew = max(1.0, hA); //round the hypotenuse to 1.0 if it's over 1.0
  maxhCnew = max(1.0, hC);
  xAnew = abs(xAresult / maxhAnew); //take the absolute value so you don't give a negative number to the LEDs
  yAnew = abs(yAresult / maxhAnew);
  xCnew = abs(xCresult / maxhCnew); //C stick
  yCnew = abs(yCresult / maxhCnew);
  if (Lresult<0){Lresult=0.0;} //don't return a negative number for the leds
  if (Rresult<0){Rresult=0.0;}
  Lnew=abs(Lresult);
  Rnew=abs(Rresult);

  //create melee deadzones
  if (xAnew < .2875) {
    xAnew = 0.0;
  }
  if (yAnew < .2875) {
    yAnew = 0.0;
  }
  if (xCnew < .2875) {//C stick
    xCnew = 0.0;
  }
  if (yCnew < .2875) {
    yCnew = 0.0;
  }
  if (Lnew<.3){
    Lnew=0.0;}
  if (Rnew<.3){
    Rnew=0.0;}
  if (Lnew>1.0){
    Lnew=1.0;}
  if (Rnew>1.0){
    Rnew=1.0;}
  
/////////////////////////////////////////Analog data calculations end

  //buttons
  s = rawData[2];
  y = rawData[3];
  x = rawData[4];
  b = rawData[5];
  a = rawData[6];
  l = rawData[8];
  r = rawData[9];
  z = rawData[10];
  dUp = rawData[11];
  dDown = rawData[12];
  dRight = rawData[13];
  dLeft = rawData[14];
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
    mode=4; //also go to 1 to confirm the reset worked
    originPollNumber=0;
    state_xys = 0; //reset the state machine
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


void anime() {//Do an anime glasses flare
 
  switch(state_anime) //Switch statement acts as a state machine
  {
  case 0: //OFF AND WAITING
    if (started==false)
    {
//      Serial.println("case 0: off");
      state_anime=1;
    }
    break;
  case 1: //TRIGGERED TO START
    //go to start of animation
//    Serial.println("case 1: start");
    state_anime=2;
    break;
  case 2: //INCREASE LED 1
//    Serial.println("case 2: inc 1");
    led1Brightness=led1Brightness+ledRate;
    if(led1Brightness>=127)
    {
      state_anime=3;
    }
    break;
  case 3: //INCREASE LED 1,2
//    Serial.println("case 3: inc 1,2");
    led1Brightness=led1Brightness+ledRate;
    led2Brightness=led2Brightness+ledRate;
    if(led1Brightness>=(255-ledRate))
    {
      state_anime=4;
    }
    break;
  case 4: //INCREASE LED 2,3 DECREASE 1
//    Serial.println("case 4: inc 2,3, dec 1");  
    led1Brightness=led1Brightness-ledRate;
    led2Brightness=led2Brightness+ledRate;
    led3Brightness=led3Brightness+ledRate;
    if(led2Brightness>=(255-ledRate))
    {
      state_anime=5;
    }
    break;
  case 5: //INCREASE LED 3,4 DECREASE 1,2
//    Serial.println("case 5: inc 3,4, dec 1,2"); 
    led1Brightness=led1Brightness-ledRate;
    led2Brightness=led2Brightness-ledRate;
    led3Brightness=led3Brightness+ledRate;
    led4Brightness=led4Brightness+ledRate;
    if(led3Brightness>=(255-ledRate))
    {
      led1Brightness=0;
      state_anime=6;
    }
    break;
  case 6: //INCREASE LED 4,5 DECREASE 2,3
//    Serial.println("case 6: inc 4,5, dec 2,3"); 
    led2Brightness=led2Brightness-ledRate;
    led3Brightness=led3Brightness-ledRate;
    led4Brightness=led4Brightness+ledRate;
    led5Brightness=led5Brightness+ledRate;
    
    if(led4Brightness>=(255-ledRate))
    {
      led2Brightness=0;
      state_anime=7;
    }
    break;
  case 7: //INCREASE LED 5 DECREASE 3,4
//    Serial.println("case 7: inc 5, dec 3,4"); 
    led3Brightness=led3Brightness-ledRate;
    led4Brightness=led4Brightness-ledRate;
    led5Brightness=led5Brightness+ledRate;
    if(led5Brightness>=(255-ledRate))
    {
      led3Brightness=0;
      state_anime=8;
    }
    break;
  case 8: //DECREASE 4,5
//    Serial.println("case 8: dec 4,5");
    led4Brightness=led4Brightness-ledRate;
    led5Brightness=led5Brightness-ledRate;
    if(led5Brightness<=127)
    {
      led4Brightness=0;
      state_anime=9;
    }
    break;
  case 9: //DECREASE 5
//    Serial.println("case 9: dec 5");
    led5Brightness=led5Brightness-ledRate;
    if(led5Brightness<=0)
    {
      led5Brightness=0;
      state_anime=10; //animation is done
    }
    break;
  case 10: //done state
    if(state_animeReverse == 10)
    {
      state_anime=0; //animation is done
    }
    break;
  }
}

void animeReverse() {//Do an anime glasses flare the other way
  
  switch(state_animeReverse) //Switch statement acts as a state machine
  {
  case 0: //OFF AND WAITING
    if (state_anime==10)
    {
//      Serial.println("case 0: off");
      state_animeReverse=1;
    }
    break;
  case 1: //TRIGGERED TO START
    //go to start of animation
//    Serial.println("case 1: start");
    state_animeReverse=2;
    break;
  case 2: //INCREASE LED 5
//    Serial.println("case 2: inc 5");
    led5Brightness=led5Brightness+ledRate;
    if(led5Brightness>=127)
    {
      state_animeReverse=3;
    }
    break;
  case 3: //INCREASE LED 5,4
//    Serial.println("case 3: inc 5,4");
    led5Brightness=led5Brightness+ledRate;
    led4Brightness=led4Brightness+ledRate;
    if(led5Brightness>=(255-ledRate))
    {
      state_animeReverse=4;
    }
    break;
  case 4: //INCREASE LED 4,3 DECREASE 5
//    Serial.println("case 4: inc 4,3, dec 5");  
    led5Brightness=led5Brightness-ledRate;
    led4Brightness=led4Brightness+ledRate;
    led3Brightness=led3Brightness+ledRate;
    if(led4Brightness>=(255-ledRate))
    {
      state_animeReverse=5;
    }
    break;
  case 5: //INCREASE LED 3,2 DECREASE 5,4
//    Serial.println("case 5: inc 3,2, dec 5,4"); 
    led5Brightness=led5Brightness-ledRate;
    led4Brightness=led4Brightness-ledRate;
    led3Brightness=led3Brightness+ledRate;
    led2Brightness=led2Brightness+ledRate;
    if(led3Brightness>=(255-ledRate))
    {
      led5Brightness=0;
      state_animeReverse=6;
    }
    break;
  case 6: //INCREASE LED 2,1 DECREASE 4,3
//    Serial.println("case 6: inc 2,1, dec 4,3"); 
    led4Brightness=led4Brightness-ledRate;
    led3Brightness=led3Brightness-ledRate;
    led2Brightness=led2Brightness+ledRate;
    led1Brightness=led1Brightness+ledRate;
    
    if(led2Brightness>=(255-ledRate))
    {
      led4Brightness=0;
      state_animeReverse=7;
    }
    break;
  case 7: //INCREASE LED 1 DECREASE 3,2
//    Serial.println("case 7: inc 1, dec 3,2"); 
    led3Brightness=led3Brightness-ledRate;
    led2Brightness=led2Brightness-ledRate;
    led1Brightness=led1Brightness+ledRate;
    if(led1Brightness>=(255-ledRate))
    {
      led3Brightness=0;
      state_animeReverse=8;
    }
    break;
  case 8: //DECREASE 2,1
//    Serial.println("case 8: dec 2,1");
    led2Brightness=led2Brightness-ledRate;
    led1Brightness=led1Brightness-ledRate;
    if(led1Brightness<=127)
    {
      led2Brightness=0;
      state_animeReverse=9;
    }
    break;
  case 9: //DECREASE 1
//    Serial.println("case 9: dec 1");
    led1Brightness=led1Brightness-ledRate;
    if(led1Brightness<=0)
    {
      led1Brightness=0;
      state_animeReverse=10; //animation is done
    }
    break;
  case 10: //DECREASE 1
    animeDone=true;
    justChanged=true;
    mode=4; //go to rainbow mode
    state_animeReverse=11;
    break;
  case 11: //do nothing
    break;
  }
}
