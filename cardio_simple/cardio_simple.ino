#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "eeprom_crc.h"
#include "utils.h"

const unsigned long freeEEPROM = 1638348161;

const int L01_PIN = 16;
const int L02_PIN = 5;
const int MODE1_PIN = 4;
const int MODE2_PIN = 0;
const int SETUP_PIN = 2;

const int MES_COUNT = 125;

bool isSetup = false;
int setupMillis = 0;

bool isDisconnectedElectrodes = false;

char* addres = "test";
int port = 80;

inline void setupPins()
{
  pinMode(L01_PIN, INPUT);
  pinMode(L02_PIN, INPUT);
  pinMode(MODE1_PIN, INPUT);
  pinMode(MODE2_PIN, INPUT);
  pinMode(SETUP_PIN, INPUT);
  blink(LED_BUILTIN, HIGH);
}

inline bool checkEEPROM()
{
  unsigned long blankEEPROM = eeprom_crc();
  return blankEEPROM == freeEEPROM;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Ann's cardiograph");
  Serial.print("Setup pins...\t");
  //Set pin modes for AD8232 contact verification pins
  setupPins();
  Serial.println("OK");
  blink(LED_BUILTIN, HIGH);
  Serial.print("Check EEPROM...\t");
  EEPROM.begin(512);
  if (checkEEPROM())
  {
    blink(LED_BUILTIN, HIGH);
    blink(LED_BUILTIN, HIGH);
    Serial.print("EEPROM is free. Saving base settings...\t");
    Serial.println("OK");
  }
  else
  {
    blink(LED_BUILTIN, HIGH);
    Serial.print("OK. Load settings...\t");
  }
}

void loop() {
  if (digitalRead(SETUP_PIN))
  {
      if (!isSetup)
      {
        setupMillis = millis();
        isSetup = true;
      }
      else 
      {
        if ((millis() - setupMillis) >= 1000)
        {
          isSetup = false;
          //TODO: остановка работы, начало настройки
        }
      }
  }
  else
  {
    isSetup = false;
  }
  // put your main code here, to run repeatedly:
  if ((digitalRead(L01_PIN) == 1) || (digitalRead(L02_PIN) == 1))
  {
    if(!isDisconnectedElectrodes) 
    {
      Serial.println("Electrode connection lost.");
      isDisconnectedElectrodes = true;
    }
  }
  else 
  {
    if (isDisconnectedElectrodes)
    {
      Serial.println("Electrode connection restored.");
      isDisconnectedElectrodes = false;
    }
    int P0 = 0;
    for(int i = 0; i < MES_COUNT; i++)
      P0 += analogRead(A0);
    Serial.println(P0/MES_COUNT);
  }
  delay(1);
}
