#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include "eeprom_crc.h"
#include "utils.h"
#include <deque>

//Постоянные значения используемые в прошивке
const String serial = "c1b0b6d5-68ee-4823-8cb2-5151d2bf1d8f";
//CRC32 сумма пустой EEPROM
const unsigned long freeEEPROM = 1638348161;

const int L01_PIN = 16;   //D0
const int L02_PIN = 5;    //D1
const int SETUP_PIN = 4;  //D2
const int MODE_PIN = 14; //D5

//Изменяемые значения
String SSID = "Lind";
String PASSWORD = "123ntcmrf4";

String HOST = "http://192.168.5.5:5000/";

int MES_COUNT = 125;
int CYCLE_LENGTH_MS = 1000;
int MAX_LENGTH = 1000;

int setupMillis = 0;
int currentSessionId = 0;
unsigned long startSessionTimePoint = 0;
unsigned long timePoint = 0;

struct measure
{
    int value;
    unsigned long millisDelta;
};

std::deque<measure> measures;

//Глобальные переменные флаги
bool isSetup = false;

bool isDisconnectedElectrodes = false;

bool wifiWorkMode = true;

inline void setupPins()
{
  pinMode(L01_PIN, INPUT);
  pinMode(L02_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);
  pinMode(SETUP_PIN, INPUT);
  blink(LED_BUILTIN, HIGH);
}

inline bool checkEEPROM()
{
  unsigned long blankEEPROM = eeprom_crc();
  return blankEEPROM == freeEEPROM;
}

inline bool registerOnServer(int &sessionId)
{
    bool requestResultFlag = false;
    WiFiClient client;
    HTTPClient http;
    String dest = HOST + "registereddevice";
    if (http.begin(client, dest))
    {
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String postData = "serial=" + serial;
        int httpCode = http.POST(postData);
        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {
                sessionId = http.getString().toInt();
                requestResultFlag = true;
            }
            http.end();
        }
    }
    return requestResultFlag;
}

inline int measurePotential()
{
    int P0 = 0;
    for(int i = 0; i < MES_COUNT; i++)
      P0 += analogRead(A0);
    return P0 / MES_COUNT;
}

inline bool checkTime()
{
    return (millis() - timePoint) >= CYCLE_LENGTH_MS;
}

inline bool sendMeasure(int value, int millisDelta)
{
    bool requestResultFlag = false;
    WiFiClient client;
    HTTPClient http;
    String dest = HOST + "measurement";
    if (http.begin(client, dest))
    {
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String postData = "SessionId=" + String(currentSessionId) + "&Value=" + String(value) + "&MillisDelta=" + String(millisDelta);
        int httpCode = http.POST(postData);
        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {
                requestResultFlag = true;
            }
            http.end();
        }
    }
    return requestResultFlag;
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
    //TODO: запись значений по умолчанию
    Serial.println("OK");
  }
  else
  {
    blink(LED_BUILTIN, HIGH);
    //TODO:Считывание значений
    Serial.print("OK. Load settings...\t");
  }
  wifiWorkMode = digitalRead(MODE_PIN);
  if (wifiWorkMode)
  {
    Serial.println("Workmode: Wi-Fi.");
    WiFi.mode(WIFI_STA);
    Serial.print("Connection to AP...\t");
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) 
    { 
        delay(500); 
        Serial.print("."); 
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("OK");
      Serial.print("Register on server...\t");
      bool result = registerOnServer(currentSessionId);
      if (result)
      {
          startSessionTimePoint = millis();
          timePoint = startSessionTimePoint;
          Serial.printf("OK. SessionID: %d\f", currentSessionId);
      }
      else
      {
          Serial.println("Failed!");
      }
    }
    else
    {
      Serial.println("Failed!");
    }
  }
}

void loop() 
{
  if (digitalRead(SETUP_PIN))
  {
      if (!isSetup)
      {
        setupMillis = millis();
        isSetup = true;
      }
      else 
      {
        if ((millis() - setupMillis) >= 3000)
        {
          isSetup = false;
          setupMillis = 0;
          blink(LED_BUILTIN, HIGH);
          Serial.println("Setup mode");
          //TODO: остановка работы, начало настройки
        }
      }
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
    int potential = measurePotential();
    unsigned long millisDelta = millis() - startSessionTimePoint;
    if (wifiWorkMode)
    {
        measure m;
        m.value = potential;
        m.millisDelta = millisDelta;
        if (measures.size() < MAX_LENGTH)
        {
            measures.push_back(m);
        }
        else
        {
            measures.pop_front();
            measures.push_back(m);
        }
        if (checkTime())
        {
            if (WiFi.status() == WL_CONNECTED)
            {
                bool error = false;
                for (int i = 0; i < measures.size(); i++)
                {
                    measure temp = measures[i];
                    if (error)
                        break;
                    int j = 0;
                    while (!sendMeasure(temp.value, temp.millisDelta))
                    {
                        j++;
                        if (j > 4)
                        {
                            Serial.println("Error sending measure");
                            error = true;
                            break;
                        }
                    }
                }
                if (!error)
                    measures.clear();
            }
        }
    }
    else
    {
        Serial.println(potential);
    }
  }
}
