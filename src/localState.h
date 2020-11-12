struct localState
{
  bool state;
  bool modes[4];
  uint8_t LastMode;
};

static volatile localState LocalState;

void localStateInit()
{
    LocalState.state = true;
    LocalState.modes[0] = true;
    LocalState.modes[1] = true;
    LocalState.modes[2] = true;
    LocalState.modes[3] = false;
}

String LocalStateToMode()
{
    String mode = LocalState.state ? "1" : "0";
    for(uint8 i = 0; i < sizeof(LocalState.modes); i++)
    {
        mode += LocalState.modes[i] ? "1" : "0";
    }
    return mode;
}

void LocalModeSet(String mode)
{
    // для обмена со внешним интерфейсом используем одну строку. 0 - вкл/выкл, 1-4 - разрешения для режимов 
    if(mode.length() == 5)
    {
        LocalState.state = mode[0] == '1';
        LocalState.modes[0] = mode[1] == '1';
        LocalState.modes[1] = mode[2] == '1';
        LocalState.modes[2] = mode[3] == '1';
        LocalState.modes[3] = mode[4] == '1';
    }
    Serial.print("out mode: ");
    Serial.println(LocalStateToMode());
}

String LocalStateInfo()
{
    String state = "on";
    state += ", active mode: ";
    state += RemoteState.mode;
    return state;
}

bool LocalStateModeDisabled(uint8_t mode)
{
    // проверям после получения команды - могли запретить использовать текущий режим, тогда выберем следующий
    if(mode > sizeof(LocalState.modes)) return true;
    Serial.println(LocalState.modes[mode - 1]);
    return  LocalState.modes[mode - 1] == false;
}

uint8_t LocalStateGetNextMode()
{ // определим следующий режимработы
    uint8_t newMode = 0;
    uint8_t activeModes = 0;
    for(uint8 i = 0; i < sizeof(LocalState.modes); i++)
    {
        if(LocalState.modes[i]) activeModes++;
    }
    if(activeModes == 0)
    {
        LocalState.modes[0] = true;
        activeModes = 1;
    }
    if(LocalState.LastMode == 0) // переключение из рабочего состояния, следующий режим случайный
    {

        uint8_t newModeIndex = random(activeModes);
        for(uint8 i = 0; i < sizeof(LocalState.modes); i++)
        {
            if(LocalState.modes[i])
            {
                if(newModeIndex != 0)
                {
                    newModeIndex--;
                }
                else
                {
                    newMode = i + 1;
                    break;
                }
            }
        }
    }
    else
    { // получение режима после включения - надо взять следующий разрешенный после того, на котором было выключение
        for(uint8 i = 0; i < sizeof(LocalState.modes); i++)
        { // возьмем следующий
            if(LocalState.modes[i] && (i + 1 > LocalState.LastMode)) {newMode = i + 1;break;};
        }
        if(newMode == 0)
        { // следующего не нашли, возьмем первый
            for(uint8 i = 0; i < sizeof(LocalState.modes); i++)
            {
                if(LocalState.modes[i]) {newMode = i + 1;break;};
            }
        }
        LocalState.LastMode = 0;
    }

    return newMode;
}
