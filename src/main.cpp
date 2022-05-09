#include <ArtnetWifi.h>
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>


#include "global.h"
#include "ESP_artnet.h"
#include "ESP_fastled.h"
#include "ESP_mqtt.h"
#include "ESP_wifi.h"

// // Wifi settings
// const char *ssid = "Silva-WIFI";
// const char *password = "silvaFTW";

// // LED settings
// const int numLeds = 86;                    // CHANGE FOR YOUR SETUP
// const int numberOfChannels = numLeds * 3; // Total number of channels you want to receive (1 led = 3 channels)
// const byte dataPin = 19;
// CRGB leds[numLeds];

// // Art-Net settings
// ArtnetWifi artnet;
// const int startUniverse = 40; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.

// // Check if we got all universes
// const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
// bool universesReceived[maxUniverses];
// bool sendFrame = 1;
// connect to wifi – returns true if successful or false if not

unsigned long smokeMillis = 0;


void setup()
{
  Serial.begin(115200);
  pinMode(smokePin, OUTPUT);
  digitalWrite(smokePin, HIGH);

  InitLeds();
  InitWifi();
  InitMQTT();
  InitArtnet();
  
  
}

boolean cycleCheck(unsigned long *lastMillis, unsigned int cycle) 
{
  unsigned long currentMillis = millis();
  if(currentMillis - *lastMillis >= cycle)
  {
    *lastMillis = currentMillis;
    return true;
  }
  else
    return false;
}

boolean smokeCheck(unsigned long *lastMillis, unsigned int cycle) 
{
  unsigned long currentMillis = millis();
  if(currentMillis - *lastMillis >= cycle)
  {
    if(smokeState) *lastMillis = currentMillis;
    // smokeState = true;
    digitalWrite(smokePin, HIGH);
    return true;
  }
  else{
    smokeState = false;
    digitalWrite(smokePin, LOW);
    return false;
  }
    
}

void loop()
{
  
  smokeCheck(&smokeMillis, 8000);

   // Artnet
  ReadArtnet();
  // MQTT
  LoopMQTT();

  // LoopSmoke(5000);
  
}



