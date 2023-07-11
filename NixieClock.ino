//Ar2  -> .
//Ar3  -> 3
//Ar4  -> 0
//Ar5  -> 74HC595_14 DS
//Ar6  -> 74HC595_12 ST_CP
//Ar7  -> 74HC595_11 SH_CP
//Ar8  -> X
//Ar9  -> X
//Ar10 -> B4
//Ar11 -> B1
//Ar12 -> B2
//Ar13 -> B3
// DS3231:  SDA pin   -> Arduino Analog 4 or the dedicated SDA pin
//          SCL pin   -> Arduino Analog 5 or the dedicated SCL pin

#include <DS3231.h>
DS3231  rtc(SDA, SCL);
Time  t;

// to both 74HC595 SH_CP (pin 11, clock pin)
int clockPin = 7;
// to both 74HC595 ST_CP (pin 12, latch pin)
int latchPin = 6;
// to 74HC595_digit DS (pin 14)
int dataPin = 5;
// connect 74HC595_digit's Q7' (pin 9) to 74HC595_nixie's DS (pin 14)

int b1 = 11;
int b2 = 12;
int b3 = 13;
int b4 = 10;

int dig_dot = 2;
int dig_three = 4;
int dig_zero = 3;

int mode = 1;
int delayms = 5;
int b1state ;
int b2state ;
int b3state ;
int b4state ;
int diver[8]={0,0,0,0,0,0,0,0};
int count=10;

const byte nixie[8] = {  // light which nixie tube
  B10000000,
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B00000100,
  B00000010,
  B00000001,
};
const byte digit[9] = {  // display which digit
  B00000000, // for 1,3,dot
  B00000010, 
  B00000001, //2
  B00000100, //9
  B10000000, 
  B00010000, 
  B00100000, 
  B01000000, 
  B00001000,  
};


void setup() {
  // 將 latchPin, clockPin, dataPin 設置為輸出
  Serial.begin(9600);
  rtc.begin();
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(dig_dot, OUTPUT);
  pinMode(dig_zero, OUTPUT);
  pinMode(dig_three, OUTPUT);  
  pinMode(b1,INPUT_PULLUP);
  pinMode(b2,INPUT);
  pinMode(b3,INPUT);
  pinMode(b4,INPUT);
  t=rtc.getTime();
}


void loop() {     
  b1state = digitalRead(b1);
  if (b1state != 1){
    mode++;
    if (mode > 3){
      mode = 0;
    }
    nixie_ctrl(1,mode,0);   
    delay(1000);
  }
  
  switch(mode){
    case 0:
    //worldline divergence mode
    nixie_ctrl(1,diver[0],0);
    nixie_ctrl(2,10,1);
    for (int i=3 ; i < 9 ; i++){
    nixie_ctrl(i,diver[i-1],0);  
      }        
    break;
    
    case 1:
    //time mode  
    nixie_ctrl(1,t.hour/10,0);
    nixie_ctrl(2,t.hour%10,0);
    nixie_ctrl(3,10,1);
    nixie_ctrl(4,t.min/10,0);
    nixie_ctrl(5,t.min%10,0);
    nixie_ctrl(6,10,1);
    nixie_ctrl(7,t.sec/10,0);
    nixie_ctrl(8,t.sec%10,0);  
    break;
    
    case 2:
    //date mode
    nixie_ctrl(1,t.year/1000,0);
    nixie_ctrl(2,(t.year/100)%10,0);
    nixie_ctrl(3,(t.year/10)%10,0);
    nixie_ctrl(4,t.year%10,0);
    nixie_ctrl(5,t.mon/10,1);
    nixie_ctrl(6,t.mon%10,0);
    nixie_ctrl(7,t.date/10,1);
    nixie_ctrl(8,t.date%10,0);          
    break;
    
    case 3:
    //test mode
    nixie_ctrl(1,t.sec%10,diver[3]%2);
    nixie_ctrl(2,t.sec%10,diver[3]%2);
    nixie_ctrl(3,t.sec%10,diver[3]%2);
    nixie_ctrl(4,t.sec%10,diver[3]%2);
    nixie_ctrl(5,t.sec%10,diver[3]%2);
    nixie_ctrl(6,t.sec%10,diver[3]%2);
    nixie_ctrl(7,t.sec%10,diver[3]%2);
    nixie_ctrl(8,t.sec%10,diver[3]%2);          
    break;    
  }

  
  static unsigned long OledTimer=millis();  
  if (millis() - OledTimer >=1000) {
    OledTimer=millis();
    t=rtc.getTime();
    count++;
    
    if (count>=10){
    diver[0] = div_gen();
    for (int i=1 ; i < 8 ; i++){  
      diver[i] = random(10);
    }
    for(int i=0; i <7 ; i++){
      Serial.print(diver[i]);
    }
    Serial.println(diver[7]);
    count=0;
    }    
   }   
}



int div_gen(){
   int temp;  
   temp = random(1000);
   if (temp > 997){
    return 5;
   }
   else {
    if (temp >990){
      return 4;
    }
    else{
      if (temp >960){
        return 3;
      }
      else {
        if (temp > 920){
          return 2;
        }
        else{
          if (temp >850){
            return 1;
          }
          else{
            return 0;            
          }
        }
      }
    }
   }
}


void nixie_ctrl(int nixie_num, int digit_num, int dot){        
//  push nixie first ,then digit
    digitalWrite(latchPin, LOW);
    digitalWrite(dig_dot, LOW);
    digitalWrite(dig_zero, LOW);
    digitalWrite(dig_three, LOW);
      
    shiftOut(dataPin, clockPin, MSBFIRST, nixie[nixie_num-1]);
    if (digit_num==0){
      shiftOut(dataPin, clockPin, MSBFIRST, digit[0]);
      digitalWrite(dig_zero, HIGH);
      digitalWrite(latchPin, HIGH);
    }
    else{
      if(digit_num==3){
      shiftOut(dataPin, clockPin, MSBFIRST, digit[0]);
      digitalWrite(dig_three, HIGH);
      digitalWrite(latchPin, HIGH);        
      }
      else{
        if (digit_num==9){
          shiftOut(dataPin, clockPin, MSBFIRST, digit[3]);
          digitalWrite(latchPin, HIGH);  
        }
        else{
          if(digit_num==10){
          shiftOut(dataPin, clockPin, MSBFIRST, digit[0]);
          digitalWrite(latchPin, HIGH);    
          }
          else{
          shiftOut(dataPin, clockPin, MSBFIRST, digit[digit_num]);
          digitalWrite(latchPin, HIGH);      
          }
        }
      }
    }
//    switch(digit_num){
//      case 0:{
//        shiftOut(dataPin, clockPin, MSBFIRST, digit[0]);
//        digitalWrite(dig_zero, HIGH);
//      }
//      case 3:{
//        shiftOut(dataPin, clockPin, MSBFIRST, digit[0]);
////        digitalWrite(dig_three, 1);
//      }
//      case 9:{
//        shiftOut(dataPin, clockPin, MSBFIRST, digit[3]);
//      }      
//      case 10:{
//        shiftOut(dataPin, clockPin, MSBFIRST, digit[0]);
//      }
//      default:{
//        shiftOut(dataPin, clockPin, MSBFIRST, digit[digit_num]);
//      }
//    }
    if (dot==1){
      digitalWrite(dig_dot,1);
    }

    delay(2);    
}

