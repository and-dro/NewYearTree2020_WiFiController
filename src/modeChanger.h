struct modeChanger
{
    uint8_t value;
    uint8_t activeValue;
    uint8_t activeEffect;
    unsigned long changeTime;
} ModeChanger;

void modeChangerTick()
{
    if(ModeChanger.activeEffect != RemoteState.mode)
    {
        ModeChanger.activeEffect = RemoteState.mode;
        ModeChanger.activeValue = ModeChanger.value + 1;
    }
    if(ModeChanger.activeValue != ModeChanger.value)
    {
        ModeChanger.activeValue = ModeChanger.value;
        if(ModeChanger.activeValue < 20 || ModeChanger.activeEffect == 0)
        {
            ModeChanger.changeTime = 0;
        }
        else
        {
            uint16_t duration = map(255 - ModeChanger.activeValue, 0, 235, 30, 60 * 5) * 1000;
            duration += random(map(255 - ModeChanger.activeValue, 0, 235, 10, 60 * 3)) * 1000;
            Serial.print("time to change: ");
            Serial.println(duration);
            ModeChanger.changeTime = millis() +  duration;
        }
    }
    if(ModeChanger.changeTime > 0 && ModeChanger.changeTime < millis())
    {
        ModeChanger.activeEffect = 0;
        RemoteState.mode = random(1,4);
        Serial.print("new random mode: ");
        Serial.println(RemoteState.mode);
        remoteStateSend();
    }
}