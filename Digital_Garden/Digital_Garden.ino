#include <Wire.h> //Libreria per comunicazione I2C
#include <dht.h> //Libreria sensori umidità e temperatura
#include "RTClib.h"//Libreria RTC
#include <SoftwareSerial.h> //Libreria Software Seriale

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
int soilMoistureValue=0;  //1
int rainValue=0;          //2
float tempValue=0;            //3
float humidValue=0;           //4
float litreValue=0;           //5
int errors=0;                 //6
int contrast=35;
int count=0;

long lastRead=0;
long lastLCD=0;

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
 Serial.begin(9600);               //This is the setup function where the serial port is initialised
 //Serial3.begin(9600);
 attachInterrupt(0, rpm, RISING);  //and the interrupt is attached
 
 RTCSetup();
 
 lcdSetup();
}




                                  
void loop () 
{
  
    readSensors();
   // delay(1000);
 
}

void readSensors(){
  
 litreValue+=flowRead(); //Misura flusso
 if(millis()-lastRead>=600000){
     DateTime timeNow=RTCRead();                         //NOW time
     
     soilMoistureValue=soilMoistureRead();  //SoilMoistureValue
     rainValue=rainRead();                  //rainValue
     readAM2301(am2301a_PIN);               //tempValue+humidValue
     lastRead=millis();
 }
 
 
}


float flowRead(){
  NbTopsFan = 0;                    //Set NbTops to 0 ready for calculations
   sei();                            //Enables interrupts
   delay (1000);                     //Wait 1 second
   cli();                            //Disable interrupts
   Calc = (NbTopsFan* 7.33);    //(Pulse frequency x 60) / 7.5Q = flow rate in L/hour
   Serial.print (Calc);         //Prints the number calculated above
  // Serial3.print(char(int(Calc+100))+char(Calc-int(Calc)));
   Serial.print (" L/hour\r\n");     //Prints "L/hour" and returns a new line
   return Calc;
}

DateTime RTCRead(){
   DateTime now = rtc.now();
    /*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    */
    return now;
}

int soilMoistureRead(){
  return analogRead(soilMoisture_PIN);
}

int rainRead(){
  return analogRead(rain_PIN);
}


void readAM2301(int am2301){//Lettura e visualizzazione dati sensore temperatura ed umidità DHT
  // READ DATA
  //Serial.print("DHT22, \t");
  //Serial.print(am2301);
  int chk = DHT.read22(am2301);
  switch (chk)
  {
    case DHTLIB_OK:  
              
		//Serial.print(" OK,\t"); 
              // DISPLAY DATA
                tempValue=DHT.temperature;
                humidValue=DHT.humidity;
                //Out of range error:
                if(tempValue<=-80||tempValue>60){
                  tempValue=24.0;
                  errors+=1;
                }
              break;
    case DHTLIB_ERROR_CHECKSUM: 
                //Checksum error:

                errors+=1;
                //
		break;
    case DHTLIB_ERROR_TIMEOUT: 
                //Timeout error:

               
		errors+=1;
                
		break;
    default: 
              //Unknown error:
		
		errors+=1;
		break;
  }
  

  delay(1000);

  
}


