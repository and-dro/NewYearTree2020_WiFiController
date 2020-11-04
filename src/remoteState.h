#include <Wire.h>

struct remoteState
{
  uint8_t mode;
  uint8_t speed;
  uint8_t brightness;
  uint8_t reserv1;
  uint8_t reserv2;
  uint8_t reserv3;
};

static volatile remoteState RemoteState;

void remoteStateInit()
{
  RemoteState.mode = 1;
  RemoteState.brightness = 128;
  RemoteState.speed = 128;
}

template <typename T> int I2C_writeAnything (const T& value)
 {
   const byte * p = (const byte*) &value;
   unsigned int i;
   for (i = 0; i < sizeof value; i++)
         Wire.write(*p++);
   return i;
 } 
 
template <typename T> int I2C_readAnything(T& value)
 {
   byte * p = (byte*) &value;
   unsigned int i;
   for (i = 0; i < sizeof value; i++)
         *p++ = Wire.read();
   return i;
 }

void remoteStateSend()
{
    Serial.println("Send state:");
    Serial.print("mode:");
    Serial.println(RemoteState.mode);
    Serial.print("speed:");
    Serial.println(RemoteState.brightness);
    Serial.print("brightness:");
    Serial.println(RemoteState.speed);


    Wire.beginTransmission(8);
    I2C_writeAnything(RemoteState);
    Wire.endTransmission();
}
