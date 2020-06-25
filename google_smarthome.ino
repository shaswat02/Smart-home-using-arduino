#include <LiquidCrystal.h> //Library for LCD display
#include <SoftwareSerial.h> //Library for serial connection with ESP

SoftwareSerial ESP(2,3); //ESP is connected to 12 and 13 pin of Arduino
LiquidCrystal lcd(13,12,11,10,9,8);
 
String sendData = "GET https://api.thingspeak.com/channels/1077713/feeds.json?api_key=EVXNQBXO0TB4UOLN&results=2"; //API Key from thingspeak
String output = ""; //Initialize a null string variable 
#define Relay1 6
#define Relay2 7

void setup() 
{
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  lcd.begin(16, 2); //Initialise 16*2 LCD
  lcd.print(" Google assist  ");  
  lcd.setCursor(0, 1);
  lcd.print("using ESP8266-01");  
  delay(2000);

  Serial.begin (9600);
  delay(100);
  ESP.begin(9600);
  
  
    ESP_talk("AT",1000);
    ESP_talk("AT+RST",1000);
    ESP_talk("AT+CWMODE=1",5000);
    lcd.clear();
    lcd.print("connecting WiFi..");
    ESP_talk("AT+CWJAP=\"AntO\",\"Shaswat's iPhone\"",10000); //Shaswat's iPhone is wifi credential
    lcd.print("Wifi Connected");
    Serial.println("Wifi Connected"); 
    delay(1000);  
     lcd.clear();
}

void loop() {

  String cmd = "AT+CIPSTART=\"TCP\",\"";   // Establishing TCP connection// AT+CIPSTART=4,"TCP","google.com",80
 cmd += "184.106.153.149";   //  api.thingspeak.com
 cmd += "\",80";   // port 80
 ESP_talk(cmd,1000);
    delay(100); 
    ESP_talk("AT+CIPSEND=76",1000);  
    delay(100);
  output = "";
    ESP_talk(sendData,1000);
    delay(100);

char incoming_value=output[126];
    Serial.print("incoming_value is : ");   
    Serial.println(incoming_value);
    lcd.clear();
    lcd.print("Listning...."); //Intro Message line 1
    lcd.setCursor(0, 1);

    if (incoming_value == '0') //light should be off
    {
      lcd.print("Light is ON :-( " ); 
      digitalWrite(Relay1,LOW);
    }
    if (incoming_value == '1') //light should be off
    {
      lcd.print(":-) Light is OFF"); 
      digitalWrite(Relay1,HIGH);
    }
    
}

void ESP_talk(String AT_cmd, const int timeout)
{
  
  Serial.print("Sent: ");
  Serial.print(AT_cmd);
  ESP.println(AT_cmd); //print to ESP through software serial 
  Serial.println("");//Move to next line 
  
  
  long int time = millis();

  output=""; //clear the string
  
  while ( (time + timeout) > millis())
  {
    while (ESP.available())
    {
      char i = ESP.read(); // read one char 
      output += i; //Combine char to string 
    }
  }
  
  Serial.println("Received: ");
  Serial.print(output);
  
}
