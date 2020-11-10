#include <Arduino.h>

#include "remoteState.h"
#include "modeChanger.h"
#include "webServer.h"
#include "mqqt.h"

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,  HIGH);
  delay(4000);
  digitalWrite(LED_BUILTIN,  LOW);
  Serial.begin(9600); 
  
  Wire.begin(D1, D2); // задаем i2c мост через контакты SDA=D1 и SCL=D2 на NodeMCU

  remoteStateInit();

  ModeChanger.value = 128;

  webSeverStart();
  mqqtServiceStart();
  
  remoteStateSend();
  
 }

static bool state = false;

void loop() 
{
  if(state != ((millis() / 500) % 2 == 0))
  {
    state = !state;
    digitalWrite(LED_BUILTIN,  state ? HIGH : LOW );
    Serial.print('.');
  }
  webServerProcessRequest();
  mqqtServiceTick();
  modeChangerTick();
  
}