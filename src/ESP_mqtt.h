#ifndef __ESP_MQTT_H__
#define __ESP_MQTT_H__

#include <PubSubClient.h>
#include <WiFi.h>

#include "global.h"
#include "ESP_wifi.h"

// Add your MQTT Broker IP address, example:
//tcp://192.168.1.13:1883

WiFiClient mqtt_client;
PubSubClient client(mqtt_client);

//const char* lanternHeader = "/lantern/";
//const char* controlString = "/control";
//const char* soundString = "/audio";
//
//String lanternControl;
//String lanternSound;

unsigned long mqttConnectionTimer = millis();
unsigned long voltageTimer = millis();



void MqttCallback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  Serial.println();
  String messageTemp;

  //  char* lanternHeader = "/lantern/";
  //
  //  doConcat(lanternHeader, mqtt_panID, lanternHeader);
  //
  //  char* control = "/control";
  //
  //  doConcat(lanternHeader, control, control);


  // control events
  String control = String() + "/cabane/control";

  // audio events
  String smoke = String() + "/cabane/smoke";


  // callbacks

  if (String(topic) == control) {
    Serial.println("GOT CONTROL STRING");
    restart_esp32();
  } else if(String(topic) == smoke){
    smokeState = true;
    Serial.println("SETTING SMOKE TO: " + smoke);
  }
}



void LoopMQTT() {
  if (!client.connected()) {
    DisplayConnectionCode(3);
    ConnectToMQTT();
  }
  client.loop();
}

void ConnectToMQTT() {
  Serial.println("Attempting MQTT connection...");
      // Serial.println(mqttConnectionTimer);
  while (!client.connected()) {
    if (millis() - mqttConnectionTimer > 5000) {
      mqttConnectionTimer = millis();
      
      if (client.connect(esp_hostName_char)) {
        Serial.println("Connected to MQTT broker");
        client.subscribe("/cabane/#");
        Serial.print("Subscribed to: ");
        Serial.println("/cabane/#");
        AllOff();
      } else {
        Serial.print("MQTT connection failed, rc=");
        Serial.print(client.state());
        Serial.println(" trying again in 5 seconds");

        //      delay(5000);
      }
    }
  }
}

void InitMQTT() {
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(MqttCallback);
}
#endif // __ESP_MQTT_H__