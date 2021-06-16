
#include <SoftwareSerial.h>                 
#define BT_RXD 5                              
#define BT_TXD 4                             
SoftwareSerial bluetooth(BT_RXD, BT_TXD);    

#include <Wire.h>                             
#include <LiquidCrystal_I2C.h>               
LiquidCrystal_I2C lcd(0x27, 16, 2);          

#include <Adafruit_NeoPixel.h>             
Adafruit_NeoPixel RGB_LED = Adafruit_NeoPixel(3, 6, NEO_GRB); 
int R_Val, G_Val, B_Val = 0;                 



#include <Servo.h>                           
Servo servo;                                
int servo_pin = 13;                          
int init_angle = 180;                        
int angle = init_angle;                     
int control_num = 0;                          

int piezo = 7;                              
int tones[] = {261, 523};                    
int IRPin = 10;                              
int Caps = 0;                                

void setup() {
  Serial.begin(9600);                         
  bluetooth.begin(9600);                     
  lcd.init();                                
  lcd.backlight();                            

  RGB_LED.begin();                           
  RGB_LED.setBrightness(100);                 
  RGB_LED.show();

  

  servo.attach(servo_pin);                    

  //********* 침입자 감지 Setup() *********//
  pinMode(piezo, OUTPUT);                     
  pinMode(IRPin, INPUT);                     
}

void loop() {
  Serial_CON();                              
  Bluetooths_CON();                        
  Smart_light(R_Val, G_Val, B_Val);          
  angle = Smart_Door(angle);                 
  if (Caps == 1) {                           
    Smart_Alarm();                        
  }
  Serial.println("#-----------------------------------------------#");
}

void Bluetooths_CON() {
  if (bluetooth.available()) {              
    if (bluetooth.find("R")) {                
      R_Val = bluetooth.parseInt();         
      Serial.print("R"); Serial.print(R_Val);
    }
    if (bluetooth.find("G")) {               
      G_Val = bluetooth.parseInt();          
      Serial.print("G"); Serial.print(G_Val);
    }
    if (bluetooth.find("B")) {               
      B_Val = bluetooth.parseInt();         
      Serial.print("B"); Serial.print(B_Val);
    }
    if (bluetooth.find("D")) {                
      angle = bluetooth.parseInt();          
      control_num = 1;
      Serial.print("D"); Serial.print(angle);
    }
    if (bluetooth.find("CapsON")) {           
      Serial.print("CapsON");
      angle = init_angle;
      R_Val = 0;
      G_Val = 0;
      B_Val = 0;
      Caps = 1;
    }
  }
}
void Serial_CON() {
  if (Serial.available()) {               
    if (Serial.find("R")) {              
      R_Val = Serial.parseInt();          
      Serial.print("R"); Serial.print(R_Val);
    }
    if (Serial.find("G")) {              
      G_Val = Serial.parseInt();       
      Serial.print("G"); Serial.print(G_Val);
    }
    if (Serial.find("B")) {              
      B_Val = Serial.parseInt();          .
      Serial.print("B"); Serial.print(B_Val);
    }
    if (Serial.find("D")) {               
      angle = Serial.parseInt();          
      control_num = 1;
      Serial.print("D"); Serial.println(angle);
    }
    if (Serial.find("CapsON")) {           
      Serial.println("CapsON");
      angle = init_angle;
      R_Val = 0;
      G_Val = 0;
      B_Val = 0;
      Caps = 1;
    }
  }
}
//****************** 스마트 전등 함수 ******************//
void Smart_light(int R, int G, int B) {      
  float LED_Color = RGB_LED.Color(R, G, B);  
  for (int i = 0; i < 3 ; i++) {
    RGB_LED.setPixelColor(i, LED_Color);     
    RGB_LED.show();
    delay(10);
  }
  Serial.print("R: "); Serial.print(R_Val);
  Serial.print(", G: "); Serial.print(G_Val);
  Serial.print(", B: "); Serial.println(B_Val);
}




//****************** 스마트 도어 함수 ******************//
int Smart_Door(int angles) {
  if (angles != init_angle) {                   
    control_num++;                            
    lcd.setCursor(0, 0);
    lcd.print("Door Open Count ");
    lcd.setCursor(0, 1);
    lcd.print("         = "); lcd.print(control_num); lcd.print("ms ");

    if (control_num == 30) {                 
      control_num = 0;                        
      angles = init_angle;                           
    }
  }
  else {                                  
    control_num = 0;                       
    angles = init_angle;                           
  Serial.print("angle : "); Serial.print(angles);
  Serial.print(", control : "); Serial.println(control_num);
  servo.write(angles);                    
  delay(100);

  return angles;                             
}


//****************** 침입자 경보시스템 함수 ******************//

void Smart_Alarm() {
  while (1) {
    Serial.println("경비시스템 작동중!!");
    lcd.setCursor(0, 0); lcd.print("Security system ");
    lcd.setCursor(0, 1); lcd.print(" IRsenser off   ");

    Smart_light(000, 000, 000);             
    if (digitalRead(IRPin) == LOW) {        
      Serial.println("경비시스템 침입자 발생!!!");
      lcd.setCursor(0, 0); lcd.print("!!!emergency!!! ");
      lcd.setCursor(0, 1); lcd.print(" IRsenser on    ");

      for (int i = 0; i < 2; i++) {           
        tone(piezo, tones[i]);                
        Smart_light(000, 000, 255);         
        delay(600);                           
        Smart_light(255, 000, 000);         
        delay(600);                          
      }
      noTone(piezo);                        
    }
    if (bluetooth.available()) {              
      if (bluetooth.find("CapsOFF")) {        
        Smart_light(000, 000, 000);           
        Caps = 0;
        break;                               
      }
    }
    if (Serial.available()) {           
      if (Serial.find("CapsOFF")) {      
        Smart_light(000, 000, 000);         
        Caps = 0;
        break;                             
      }
    }
    delay(10);
  }
}
