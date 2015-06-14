
#include <Wire.h> //Libreria per comunicazione I2C
#include <dht.h> //Libreria sensori umidità e temperatura
#include "RTClib.h"//Libreria RTC
#include <SoftwareSerial.h> //Libreria Software Seriale
#include <RTClib.h>//Libreria RTC
#include "floatToString.h"



//PIN
#define soilMoisture_PIN A0
#define rain_PIN A1
#define am2301a_PIN A2 //PIN Sensore Umidità e temperatura AM2301: A2
#define hallsensor_PIN 2                //Pin sensore flusso D2 (interrupt)
#define valve_PIN 3 //Pin Solenoid Valve

//PIN SCHERMETTINO
#define vddPin 16    // ArduinoA2
#define gndPin 17    // ArduinoA3
#define sdaPin 18    // ArduinoA4
#define sclPin 19    // ArduinoA5
#define I2Cadr 0x3e  // Fixed



//VARIABILI
volatile int NbTopsFan;            //measuring the rising edges of the signal
float Calc;
int soilMoistureValue = 0; //1
int rainValue = 0;        //2
float tempValue = 0;          //3
float humidValue = 0;         //4
float litreValue = 0;         //5
int errors = 0;               //6
int contrast = 35;
int count = 0;

long deltaTimeFlow = 0;

DateTime timeNow;
float hourNow = 0;
int minuteNow = 0;
int hourAlarm1 = 0;
int minAlarm1 = 0;
int hourAlarm2 = 0;
int minAlarm2 = 0;

long secondsNow = 0;
long secondsAlarm1 = 0;
long secondsAlarm2 = 1191;

long waterBegin = 0;
long lastRead = 0;
long lastLCD = 0;

long lastCommunication = 0;

boolean debugging = true;
boolean water1 = false;
boolean water2 = false;
boolean isWatering = false;

int duration = 2; //2 minuti

int maxSoil = 800;
int maxRain = 600;


char buffer1[8];
char buffer2[2];
String s; //String for debugging in Serial



dht DHT;
RTC_DS1307 rtc;

void rpm ()                        //This is the function that the interrupt calls
{
  NbTopsFan++;                     //This function measures the rising and falling edge of signal
}


void setup()
{


  pinMode(am2301a_PIN, INPUT);       //initializes am2301 (pin D2) as an input
  pinMode(hallsensor_PIN, INPUT);       //initializes digital pin 2 as an input
  pinMode(valve_PIN, OUTPUT);       //initializes digital pin 2 as an input
  Serial.begin(115200);               //This is the setup function where the serial port is initialised
  //Serial3.begin(9600);
  attachInterrupt(0, rpm, RISING);  //and the interrupt is attached

  RTCSetup();

  lcdSetup();

  //Set Alarm
  setAlarm(540, 1); //9 mattina
  setAlarm(1202, 2); //9 di sera
  // Alarm.alarmRepeat(hourAlarm1,minAlarm1,0, MorningAlarm);  // 8:30am every day
  //Alarm.alarmRepeat(hourAlarm2,minAlarm2,0, EveningAlarm);  // 8:30am every day
  interrupts();
  Serial.println("-------------------------> Restarted");
}





void loop ()
{
  if (errors > 0) {
    digitalWrite(13, HIGH);
  }
  RTCRead();
  readSensors();
  lcdVisualization();
  waterControl();
  //delay(1000);
  if ((millis() / 1000 - lastCommunication) >= 10) { //Comunica i dati una volta ogni 10 minuti
    communication();
    if (debugging) {
      SerialComm();
    }
    lastCommunication = millis() / 1000;
  }
}

void readSensors() {

  litreValue += flowRead(); //Misura flusso
  if (millis() / 1000 - lastRead >= 3) { // Legge sensori una volta ogni 3 secondi
    //DateTime timeNow = RTCRead();                       //NOW time

    soilMoistureValue = soilMoistureRead(); //SoilMoistureValue
    rainValue = rainRead();                //rainValue
    readAM2301(am2301a_PIN);               //tempValue+humidValue
    lastRead = millis() / 1000;

  }




}


float flowRead() {

  //Enables interrupts
  //Wait 1 second
  //Disable interrupts
  Calc = (NbTopsFan * 7.33);   //(Pulse frequency x 60) / 7.5Q = flow rate in L/hour
  // Serial.print (Calc);         //Prints the number calculated above
  // Serial3.print(char(int(Calc+100))+char(Calc-int(Calc)));
  //Serial.print (" L/hour\r\n");     //Prints "L/hour" and returns a new line
  NbTopsFan = 0;                    //Set NbTops to 0 ready for calculations
  return Calc;
}

DateTime RTCRead() {
  timeNow = rtc.now();
  hourNow = timeNow.hour();
  minuteNow = timeNow.minute();
  //secondsNow = (now.hour() * 60) + now.minute();

  //  Serial.print(now.year(), DEC);
  //  Serial.print('/');
  //  Serial.print(now.month(), DEC);
  //  Serial.print('/');
  //  Serial.print(now.day(), DEC);
  //  Serial.print(' ');
  //  Serial.print(now.hour(), DEC);
  //  Serial.print(':');
  //  Serial.print(now.minute(), DEC);
  //  Serial.print(':');
  //  Serial.print(now.second(), DEC);
  //  Serial.println();

  return timeNow;
}

int soilMoistureRead() {
  return map(analogRead(soilMoisture_PIN), 0, 1024, 1024, 0);
}

int rainRead() {
  return analogRead(rain_PIN);
}


void readAM2301(int am2301) { //Lettura e visualizzazione dati sensore temperatura ed umidità DHT
  // READ DATA
  //Serial.print("DHT22, \t");
  //Serial.print(am2301);
  int chk = DHT.read22(am2301);
  switch (chk)
  {
    case DHTLIB_OK:

      //Serial.print(" OK,\t");
      // DISPLAY DATA
      tempValue = DHT.temperature;
      humidValue = DHT.humidity;
      //Out of range error:
      if (tempValue <= -80 || tempValue > 60) {
        tempValue = 24.0;
        errors *= 10;
        errors += 1;
      }
      break;
    case DHTLIB_ERROR_CHECKSUM:
      //Checksum error:
      errors *= 10;
      errors += 2;
      //
      break;
    case DHTLIB_ERROR_TIMEOUT:
      //Timeout error:


      errors *= 10;
      errors += 3;

      break;
    default:
      //Unknown error:
      errors *= 10;
      errors += 4;
      break;
  }


  //delay(1000);


}



void setAlarm(long seconds, int alarm) {
  if (alarm == 1) {
    secondsAlarm1 = seconds;
  } else if (alarm == 2) {
    secondsAlarm2 = seconds;
  }
}


boolean waterControl() {
  if (soilMoistureValue < maxSoil) {
    if ((secondsNow >= secondsAlarm1) && (secondsNow <= (secondsAlarm1 + duration))) {
      watering();
      water1 = true;
    }
    else if ((secondsNow >= secondsAlarm2) && (secondsNow <= (secondsAlarm2 + duration))) {
      watering();
      water2 = true;

    }
  }
  else stopWatering();


}

void watering() {
  if (!isWatering) {
    digitalWrite(valve_PIN, HIGH);
    isWatering = true;
    waterBegin = millis() / 1000;
  }
}

void stopWatering() {
  if (isWatering) {
    digitalWrite(valve_PIN, LOW);
    isWatering = false;
  }
}


void SerialComm() {
  s = String(hourNow).substring(0, 2) + ":" + String(minuteNow);
  s += " Litres: " + String(litreValue);
  s += " Soil: " + String(soilMoistureValue);
  s += " Rain: " + String(rainValue);
  s += " Temp: " + String(tempValue);
  s += " Humid: " + String(humidValue);
  s += " Water: " + String(isWatering);
  s += " Errors: " + String(errors);
  s += " Alarm1: " + String(secondsAlarm1);
  s += " Alarm2: " + String(secondsAlarm2);
  Serial.println(s);


}

