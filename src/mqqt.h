#include "private.h"

#ifndef MQQT_USER
#define MQQT_USER "mqqt_user_name"; // переопределяется в private.h
#endif

#ifndef MQQT_PASSWORD
#define MQQT_PASSWORD "mqqt_password"; // переопределяется в private.h
#endif

#define MQQT_DEVICE_ID MQQT_USER "_Lights2020_01"
#define MQQT_DEVICE_TOPIC "Lights"
#define MQQT_SERVER "srv1.clusterfly.ru"
#define MQQT_PORT 9124

WiFiClient espClient;
PubSubClient mqttClient(espClient);
int countFailMqtt = 0;

void mqttCallback(char* topic, byte* payload, unsigned int length) 
{
  String s_payload;
  
  Serial.println();
  Serial.print(F("MQTT message arrived ["));
  Serial.print(topic);
  Serial.print(F("] "));
  for (uint i = 0; i < length; i++) {
    s_payload += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

String mqtt_topic(String topic) {
  String ret_topic;
    ret_topic = MQQT_USER;
    ret_topic += "/";
    // ret_topic += MQQT_DEVICE_TOPIC;
    // ret_topic += "/";
    ret_topic += topic;
    return ret_topic;
}

ulong nextOut;

void mqqtServiceStart()
{
    Serial.println();
    Serial.print(F("Trying to connect MQTT... "));
    Serial.print(MQQT_SERVER);
    Serial.print(F(":"));
    Serial.println(String(MQQT_PORT));

    mqttClient.setServer(MQQT_SERVER, MQQT_PORT);
    mqttClient.setCallback(mqttCallback);

    Serial.println(F("..Connecting to MQTT..."));
    Serial.print("User name: ");
    Serial.println(MQQT_USER);
    Serial.print("Device ID: ");
    Serial.println(MQQT_DEVICE_ID);

    if (mqttClient.connect(MQQT_DEVICE_ID, MQQT_USER, MQQT_PASSWORD)) 
    {
        countFailMqtt = 0;
        Serial.println(F("connected. "));
        mqttClient.subscribe(mqtt_topic("test").c_str());

        mqttClient.publish(mqtt_topic(F("test")).c_str(), "on", false);
        
        nextOut = millis() + 15000;
    }
    else
    {
        Serial.print(F("failed with state = ")); Serial.println(mqttClient.state());
        countFailMqtt++;
    }
}
void reconnect(){

}


void mqqtServiceTick()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
    if(nextOut < millis())
    {
      nextOut = millis() + 15000;
      if(mqttClient.connected())
      {
        // mqttClient.publish(mqtt_topic(F("test")).c_str(), String(millis()).c_str(), false);   
      }
      else
      {
        Serial.println("Mqqt not connected");
      }
      
  }

}
void mqqtNewSettings()
{
  Serial.println("new settings");
}