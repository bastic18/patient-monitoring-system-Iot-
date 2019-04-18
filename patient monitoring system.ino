/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)


*/

// include the library code:
#include <LiquidCrystal.h>
#include<Wire.h>
#include <SoftwareSerial.h>

//**********ENTER IP ADDRESS OF SERVER******************//

 #define HOST_IP "172.16.192.48"    //IP address of HOST server 
 #define HOST_PORT "1020"     //Port for HOST server
#define idNumber "620108615"    //Your ID number


//**********PIN DEFINITIONS******************//



#define espRX 9
#define espTX 10
#define espTimeout_ms 300

SoftwareSerial esp(espRX, espTX);

int buzz = 8;
String params = "";
String paramsOld = "";
String espCommandString = "";
String post = "";
String response = "";
String alert="no";
String alertType="";
int alertButton = 2;
//int state = 0;
//volatile byte state = HIGH;
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ;
int minVal=265;
int maxVal=402;
double sitAngle = 0;
double lyingAngle = 0;
String pos = " ";
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
unsigned long currentTime = 0;
unsigned long lastSent = 0;
int value=0;
float volts=0;
float temp=0.0;
float temp_f= 0.0;
int sensorPin = A1;
double alpha = 0.75;
int period = 100;
double change = 0.0;
double minval = 0.0;
int actual=0;

void setup() {
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(13,OUTPUT);
   pinMode(alertButton, INPUT);
  
 
 

    Wire.begin();
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
//  attachInterrupt(digitalPinToInterrupt(alertButton), alertCheck, LOW);
   Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  esp.begin(9600);
  esp
  .print("AT+CWJAP=\"MonaConnect\",\"\"\r\n"); //Connect to wifi
  //esp.print("AT+CWJAP=\"netis\",\"password\"\r\n"); //Connect to wifi
  delay(1000);
  
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
 // lcd.print(millis() / 1000);

temp_c();
heart_rate();
getAngle();
alertCheck();
currentTime=millis(); //checks how much time has passed since the system has been turned on
  if((currentTime-lastSent)>5000){
    getParams();  
     espUpdate();  //Sends the information to the remote server
    lastSent = millis();
  }
    
  delay(250);

 
 
}


          void heart_rate(){
            
            
              static double oldValue = 0;
              static double oldChange = 0;
           
              int rawValue = analogRead (sensorPin);
              double value = alpha * oldValue + (1 - alpha) * rawValue;
           
            //  Serial.print (rawValue);
             // Serial.print (",");
              actual= value/10;
             // Serial.println (actual);
              oldValue = value;
           
              delay (period);
              //lcd.clear();
             

            if (actual<=60 ){
              //lcd.clear();

            lcd.setCursor (0,1);
            lcd.print("HR-TOO LOW ");
            digitalWrite(13,HIGH);
              
              }

              if (actual>=120 ){
              //lcd.clear();

            lcd.setCursor (0,1);
            lcd.print("HR-TOO HIGH ");
            digitalWrite(13,HIGH);
              
              }
              if (actual >61 && actual<120){
                //lcd.clear();
            lcd.setCursor (0,1);
            
            lcd.print("HR= ");
            lcd.print(actual);
            lcd.print (" bpm");
            digitalWrite(13,LOW);
            }
          
          
              
            }
          
          
          
            void temp_c(){
                
           value= analogRead(A0);
           volts= (value/1024.0)*5.0;
           
           temp= volts-0.5; 
           temp= temp *100;
           temp_f= temp*9/5 +32;
          
          //Serial.print("Temp= ");
          //Serial.println(temp);
          lcd.setCursor (0,0);
          lcd.print("T=");
          lcd.print(temp);
          lcd.print ("C");
          
          
          delay(500);
              
              
              }



       void getAngle(){
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr,14,true);
      AcX=Wire.read()<<8|Wire.read();
      AcY=Wire.read()<<8|Wire.read();
      AcZ=Wire.read()<<8|Wire.read();
      int xAng = map(AcX,minVal,maxVal,-90,90);
      int yAng = map(AcY,minVal,maxVal,-90,90);
      int zAng = map(AcZ,minVal,maxVal,-90,90);
    
      sitAngle= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);           //Chcks sitting positon
      lyingAngle= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);      //Checks lying postion
    //  Serial.println("gyroscope data follows");
     // Serial.println(sitAngle);
     // Serial.println(lyingAngle);
    
      //Assigns patient positions based on the angle read from the gyroscope
     if(lyingAngle>330 && lyingAngle<30 && sitAngle>310 || sitAngle<20){
          pos = "BAK";
         // Serial.println(" BACK ");
          lcd.setCursor (9,0);
          lcd.print("GYR=");
          lcd.print(pos);
          }
        else if( lyingAngle>90 && lyingAngle<150 && sitAngle>80 && sitAngle<280){
          pos = "RGT";
         // Serial.println(" RIGHT ");
          lcd.setCursor (9,0);
          lcd.print("GYR=");
          lcd.print(pos);}
       else if(lyingAngle<190 && lyingAngle>160 && sitAngle>160 && sitAngle<210){
          pos = "DWN";
         // Serial.println(" DOWN ");
          lcd.setCursor (9,0);
          lcd.print("GYR=");
          lcd.print(pos);
          
          }
          else if((lyingAngle>190 && lyingAngle<360)   && (sitAngle>240 && sitAngle<320)){
          pos = "SIT";
         // Serial.println(" SIT ");
          lcd.setCursor (9,0);
          lcd.print("GYR=");
          lcd.print(pos);
          
          }
          else if((lyingAngle>220 && lyingAngle<280) && (sitAngle>140 && sitAngle<355) ||  sitAngle<80){
          pos = "LFT";
         // Serial.println(" LEFT ");
          lcd.setCursor (9,0);
          lcd.print("GYR=");
          lcd.print(pos);
          
          }
          else {
            pos="AWK";
         // Serial.println(" AWK ");
          lcd.setCursor (9,0);
          lcd.print("GYR=");
          lcd.print(pos);
            }
    }



// network section 




void espSend(String command)
{
     char responseChar = 0;

    esp.print(command); // send the read character to the esp

     unsigned long time = millis();


    while((millis() - time) < 300)
    {
      while(esp.available())
      {
        responseChar = (esp.read());
        Serial.print(responseChar);
        response += responseChar;
      }
    }
}

void espUpdate()
{
  
    digitalWrite(13, HIGH);

    paramsOld = params;

    response  = "";
    Serial.println("New Params");
    delay(100);

    espCommandString ="";

    espCommandString = "AT+CIPSTART=\"TCP\",\"";
    espCommandString+= HOST_IP;
    espCommandString+= "\",";
    espCommandString+= HOST_PORT;
    espCommandString+="\r\n";

    espSend(espCommandString);    //starts the connection to the server
    delay(100);

        post = "POST /espPost HTTP/1.1\r\nHost: ";
        post += HOST_IP;
        post += "\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";

        espCommandString ="";
        espCommandString = "AT+CIPSEND=";
        espCommandString += post.length();
        espCommandString += "\r\n";

        espSend(espCommandString);    //sends post length
        delay(100);

        espSend(post);      //sends POST request with the parameters 
        delay(100);
    //Serial.println ("the post from module "+post);
        espSend("AT+CIPCLOSE\r\n");   //closes server connection
        delay(100); 

        
        Serial.println("response: "+response);
        digitalWrite(13, LOW);
       // Serial.println ("the post from module "+post);
  
}


void espInit(int espBaud)
{
    esp.begin(9600); 

    while(esp.available())
    {
      esp.read();
      delay(50);
    }

    esp.write("AT+RST\r\n");

    while(!esp.available());

    while(esp.available())
    {
      Serial.print(esp.readString());
    }

    delay(3000);  //gives ESP some time to get IP
}




    void getParams()
{
    params = "id=gg";
    params = "alert_=";
    params += alert;
    params += "&temp=";
    params += temp;
    params += "&bpm=";
    params += actual;
    params +="&pos=";
    params +=pos;

   Serial.println("this is the param function    "+params);
}



//void alertCheck(){
//  int state = digitalRead(8);
//  delay(200);
//  Serial.println("dolo");
//  Serial.println(state);
//  if(state == LOW){
//    alert = "yes";
//    alertType = "Attend to patient";
//    Serial.println(alertType);
//    Serial.println(alert);
//    digitalWrite(buzz, HIGH);
//  }
//  if(state == HIGH){
//    alert = "no";
//    alertType = "No alerts";
//     Serial.println(alertType);
//     Serial.println(alert);
//    digitalWrite(buzz, LOW);
//  }
//}
void alertCheck(){
  int state = digitalRead(alertButton);
  if(state == HIGH){
    alert = "yes";
    alertType = "Attend to patient";
   //  Serial.println(alertType);
  //  Serial.println(alert);
    digitalWrite(buzz, HIGH);
    lcd.setCursor (12,1);
    lcd.print("!YES");
    
  }
  if(state == LOW){
    alert = "no";
    alertType = "No alerts";
  //   Serial.println(alertType);
 // Serial.println(alert);
    digitalWrite(buzz, LOW);
    lcd.setCursor (12,1);
    lcd.print("! NO");
    
  }
}
