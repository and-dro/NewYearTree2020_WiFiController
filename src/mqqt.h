WiFiClient espClient;
PubSubClient mqttClient(espClient);
int countFailMqtt = 0;

String mqtt_topic(String topic) {
  String ret_topic;

  ret_topic = MqqtSettins.user;
  ret_topic += "/";
  ret_topic += MqqtSettins.device;
  ret_topic += "/";
  ret_topic += topic;
  return ret_topic;
}

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
  if(String(topic) == mqtt_topic(F("commands/status")))
  {
    if(s_payload == "0" && LocalState.state)
    {
      LocalState.state = false;
      modeChangerLocalStateToRemoteState();
    }
    if(s_payload == "1" && !LocalState.state)
    {
      LocalState.state = true;
      modeChangerLocalStateToRemoteState();    
    }
  }

  Serial.println();
}

ulong lastOut;
ulong lastReconnect;
remoteState MqqtRemoteState;

void publishState()
{
  if(lastOut == 0 || MqqtRemoteState.mode != RemoteState.mode)
  {
    Serial.println(F("publish status"));
    MqqtRemoteState.mode = RemoteState.mode;
    mqttClient.publish(mqtt_topic(F("state/status")).c_str(), MqqtRemoteState.mode != 0 ? "1" : "0", false);
  }
  if(lastOut == 0 || MqqtRemoteState.brightness != RemoteState.brightness)
  {
    MqqtRemoteState.brightness = RemoteState.brightness;
    //mqttClient.publish(mqtt_topic(F("brightness")).c_str(), String(map(MqqtRemoteState.brightness, 0,255,0,100)).c_str(), false);
  }
  if(lastOut == 0 || MqqtRemoteState.speed != RemoteState.speed)
  {
    MqqtRemoteState.speed = RemoteState.speed;
    //mqttClient.publish(mqtt_topic(F("speed")).c_str(), String(map(MqqtRemoteState.speed, 0,255,0,100)).c_str(), false);
  }
  lastOut = millis();
}

void connectToMqqtServer()
{
  if(!MqqtSettins.mqqtActive) return;
  
  // На случай, если надо будет делать защищенное соединение, то смотреть здесь:
  // https://github.com/copercini/esp8266-aws_iot/

  Serial.println();
  Serial.print(F("Trying to connect MQTT... "));
  Serial.print(MqqtSettins.server);
  Serial.print(F(":"));
  Serial.println(MqqtSettins.port);
  
  mqttClient.setServer(MqqtSettins.server.c_str(), MqqtSettins.port);
  mqttClient.setCallback(mqttCallback);

  Serial.println(F("..Connecting to MQTT..."));
  Serial.print("User name: ");
  Serial.println(MqqtSettins.user);
  Serial.print("Password: ");
  Serial.println(MqqtSettins.password);

  if (mqttClient.connect(MqqtSettins.device.c_str(), MqqtSettins.user.c_str(), MqqtSettins.password.c_str()))
  {
    countFailMqtt = 0;
    lastOut = 0;
    lastReconnect = 0;

    Serial.println(F("connected. "));
    
    mqttClient.subscribe(mqtt_topic(F("commands/status")).c_str());
    
    publishState();
  }
  else
  {
    Serial.print(F("failed with state = ")); 
    Serial.println(mqttClient.state());
    countFailMqtt++;
    if(countFailMqtt > 10)
    {
      Serial.println(F("MQQT deactivated..."));
      MqqtSettins.mqqtActive = false;
    }
  }
}

void mqqtServiceStart()
{
  connectToMqqtServer();
}

void reconnect()
{
  if(!MqqtSettins.mqqtActive) return;

  ulong now= millis();
  if(now - lastReconnect > 60000)
  {
    Serial.println(F("try to reconnect"));
    lastReconnect = now;
    connectToMqqtServer();
  }
}

void mqqtNewSettings()
{
  if(mqttClient.connected())
  {
    Serial.println(F("..Disnnecting from MQTT..."));
    mqttClient.disconnect();
  }
  lastReconnect = 0;
  reconnect();
}

void mqqtServiceTick()
{
  if(!MqqtSettins.mqqtActive) return;
  mqttClient.loop();
  ulong now = millis();
  if(now - lastOut > 1000)
  {
    if(mqttClient.connected())
    {
      publishState();
    }
    else
    {
      Serial.println(F("Mqqt not connected"));
      reconnect();
    }
    lastOut = now;
  }
}