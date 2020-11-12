struct modeChanger
{
    uint8_t value;  // положение ползунка полученное из web-интерфейса. 0..255, первые 5 - считаем что выключено, 255 - часто меням
    uint8_t activeValue; // обработанное положение ползунка
    uint8_t activeEffect;
    ulong LastTime;
    ulong Duration;
} ModeChanger;

void modeChangerLocalStateToRemoteState()
{
    if(LocalState.state && (RemoteState.mode == 0 || LocalStateModeDisabled(RemoteState.mode)))
    {
        RemoteState.mode = LocalStateGetNextMode();
        Serial.print("New on mode from off: ");
        Serial.println(RemoteState.mode);
        remoteStateSend();
    }
    if(!LocalState.state && RemoteState.mode != 0)
    {
        LocalState.LastMode = RemoteState.mode;
        RemoteState.mode = 0;
    }
    
}

void modeChangerTick()
{
    if(ModeChanger.activeEffect != RemoteState.mode)
    {
        ModeChanger.activeEffect = RemoteState.mode;
        ModeChanger.activeValue = !ModeChanger.value; // интервал смены эффекта устанавливаем при смене эффекта и при движении ползунка
    }

    ulong now = millis();

    if(ModeChanger.activeValue != ModeChanger.value) // ползунок был сдвинут (или была смена активного эффекта)?
    {
        ModeChanger.activeValue = ModeChanger.value;
        Serial.print("New mode chager: ");
        Serial.println(ModeChanger.value);

        if(ModeChanger.activeValue < 5 || ModeChanger.activeEffect == 0)
        {
            ModeChanger.LastTime = 0; // выключем смену эффектов
        }
        else
        {
            ModeChanger.Duration = map(255 - ModeChanger.activeValue, 0, 235, 30, 60 * 5) * 1000;
            ModeChanger.Duration += random(map(255 - ModeChanger.activeValue, 0, 235, 10, 60 * 3)) * 1000;
            Serial.print("Time to next effect: ");
            Serial.print(ModeChanger.Duration / 1000);
            Serial.println(" sec.");
            ModeChanger.LastTime = now;
        }
    }
    if(ModeChanger.LastTime > 0 && now - ModeChanger.LastTime >= ModeChanger.Duration)
    {
        ModeChanger.activeEffect = 0; // чтобы на следующем цикле установить интервал до следующей эффекта
        RemoteState.mode = LocalStateGetNextMode();

        Serial.print("New random mode: ");
        Serial.println(RemoteState.mode);
        remoteStateSend();
    }
}