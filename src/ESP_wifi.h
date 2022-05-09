#ifndef __ESP_WIFI_H__
#define __ESP_WIFI_H__

#include <HTTPClient.h>
#include "global.h"


// headers (C++)
void SetNetworkInfo();
void Register();
void OnStartUpdateStatus();
void ConnectToMQTT();
void LoopUpdateLanternInfo();

void printLine()
{
  Serial.println();
  for (int i = 0; i < 30; i++)
    Serial.print("-");
  Serial.println();
}

void connectToWiFi(const char * ssid, const char * pwd)
{
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  WiFi.setHostname(mac.c_str());

  printLine();
  Serial.println("Connecting to WiFi network: " + String(ssid));
  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED)
  {
    DisplayConnectionCode(0);
  }
  
  SetNetworkInfo();
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(esp_ip);

  doConcat(mqtt_topicParent, "", mqtt_clientTopic);
  const char* wildcard = "/#";
  doConcat(mqtt_clientTopic, wildcard, mqtt_clientTopicWildCard);
  
  // Register();
  // OnStartUpdateStatus();
  
  // AllOff();
}

void InitWifi() {
  connectToWiFi(networkName, networkPswd);
}

void SetNetworkInfo(){
  esp_macAddress = WiFi.macAddress();
  esp_ip = WiFi.localIP().toString();
  esp_hostName_String = WiFi.getHostname();
  esp_hostName_char = WiFi.getHostname();
}
#endif // __ESP_WIFI_H__