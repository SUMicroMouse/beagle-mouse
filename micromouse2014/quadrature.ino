/*
    The sample code for driving one way motor encoder. For 2 Motors 
    link:
       A pin -- digital 2
       B pin -- digital 4
       A pin -- digital 5
       B pin -- digital 6
       
    Beacuse of the interrupt port with different board, the following code is only used in UNO and mega2560.
    If you want to use Leonardo or Romeo,you should change digital pin 3 instead of digital pin 2.
    See the link for detail http://arduino.cc/en/Reference/AttachInterrupt
*/

const byte encoder0pinA = 2;                  //A pin -> the interrupt pin 0
const byte encoder0pinB = 4;                  //B pin -> the digital pin 4
const byte encoder1pinA = 5;
const byte encoder1pinB = 6;
byte encoder1PinALast;
byte encoder0PinALast;
int duration;
int duration1;                               //the number of the pulses
boolean Direction;                            //the rotation direction 
boolean Direction1;
 
void setup(){  
  Serial.begin(57600);                       //Initialize the serial port
  EncoderInit();                              //Initialize the module
  EncoderInit1();
}
 
void loop(){
  Serial.print("Pulse:");
  Serial.println(duration);
  Serial.print("Pulse:");
  Serial.println(duration1);
  duration = 0;
  duration1 = 0;
  delay(100);
}

void EncoderInit1(){
  Direction1 = true;
  pinMode(encoder1pinB,INPUT);
  attachInterrupt(0,wheelSpeed1,CHANGE);
}

void EncoderInit(){
  Direction = true;                            //default -> Forward  
  pinMode(encoder0pinB,INPUT);  
  attachInterrupt(0, wheelSpeed, CHANGE);
}
 
void wheelSpeed(){
  int Lstate = digitalRead(encoder0pinA);
  if((encoder0PinALast == LOW) && Lstate==HIGH){
    int val = digitalRead(encoder0pinB);
    if(val == LOW && Direction){
      Direction = false;                       //Reverse
    }
    else if(val == HIGH && !Direction){
      Direction = true;                        //Forward
    }
  }
  encoder0PinALast = Lstate;
  if(!Direction)  duration++;
  else  duration--;
}

void wheelSpeed1(){
  int Lstate1 = digitalRead(enncoder1pinA);
  if((encoder1PinALast == LOW) && Lstate1 = HIGH){
    int val = digitalRead(encoder1pinB);
    if(val == LOW && Direction1){
      Direction = false){
        Direction1 = false;
      }
      else if(val == HIGH && !Direction1){
        Direction1 = true;
      }
    }
    encoder1PinALast = Lstate1;
    if(!Direction) duration1++;
    else duration1--;
  }
  
