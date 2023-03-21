#include <SoftwareSerial.h> //Software Serial library
SoftwareSerial espSerial(3, 4);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
#define USE_ARDUINO_INTERRUPTS true
String mySSID = "IoT";       // WiFi SSID
String myPWD = "1234qwer"; // WiFi Password
String myAPI = "6XOBGRZVNNWBTV64";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1";
String myFIELD2 = "field2";
String myFIELD3 = "field3";
String myFIELD4 = "field4";
String myFIELD5 = "field5";


#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define ONE_WIRE_BUS 2
#define SensorPin A0

OneWire oneWire(ONE_WIRE_BUS);

const int LED13 = 13;
int Threshold = 550;

PulseSensorPlayground pulseSensor;  // Creates an object
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

//unsigned long millisCurrent;
//unsigned long millisLast = 0;
//unsigned long millisElapsed = 0;

//int sampleBufferValue = 0;

void setup(){
  Serial.begin(9600); //init serial 9600
  espSerial.begin(9600);
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
}

void loop(){
  long measurement1 = getPulseValue();
  float measurement2 = getxaccelerationValue();
  float measurement3 = getyaccelerationValue();
  float measurement4 = getzaccelerationValue();
  long measurement5 = getTempValue();
  delay(50);
 // Serial.print("measurement = ");
  Serial.println(measurement1);
  Serial.println(measurement2);
  Serial.println(measurement3);
  Serial.println(measurement4);
  Serial.println(measurement5);
  /*if (measurement > 1000){
    digitalWrite(LED_Pin, HIGH);
  }
  else{
    digitalWrite(LED_Pin, LOW); 
  }*/
   String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(measurement1)+"&"+ myFIELD2 +"="+String(measurement2)+"&"+ myFIELD3 +"="+String(measurement3)+"&"+ myFIELD4 +"="+String(measurement4)+"&"+ myFIELD5 +"="+String(measurement5);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
}

long getPulseValue(){
  delay(10);
  pulseSensor.analogInput(SensorPin);
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    Serial.println("PulseSensor object created!");
  }
  int myBPM = pulseSensor.getBeatsPerMinute();      // Calculates BPM
  if (pulseSensor.sawStartOfBeat()) {               // Constantly test to see if a beat happened
    Serial.println("♥  A HeartBeat Happened ! "); // If true, print a message
    Serial.print("BPM: ");
    Serial.println(myBPM);                        // Print the BPM value
    }
  
  return myBPM;
}
float getxaccelerationValue(){
  delay(10);
   if(!accel.begin())
   {
      Serial.println("No ADXL345 sensor detected.");
      while(1);
   }
   sensors_event_t event;
   accel.getEvent(&event);
   float xacceleration = event.acceleration.x;
   Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");

   return xacceleration;
}
float getyaccelerationValue(){
  delay(10);
   if(!accel.begin())
   {
      Serial.println("No ADXL345 sensor detected.");
      while(1);
   }
   sensors_event_t event;
   accel.getEvent(&event);
   float yacceleration = event.acceleration.y;
   Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");

   return yacceleration;
}
float getzaccelerationValue(){
  delay(10);
   if(!accel.begin())
   {
      Serial.println("No ADXL345 sensor detected.");
      while(1);
   }
   sensors_event_t event;
   accel.getEvent(&event);
   float zacceleration = event.acceleration.z;
   Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");

   return zacceleration;
}
float getTempValue(){
  delay(10);
  int val;
  int tempPin = 1;
  val = analogRead(tempPin);
  float mv = ( val/1024.0)*5000;
  float cel = mv/10;
  float farh = (cel*9)/5 + 32;
  Serial.print("TEMPRATURE = ");
  Serial.print(cel);

   return cel;
}

String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    //Serial.print(response);
  return response;
  }
}
