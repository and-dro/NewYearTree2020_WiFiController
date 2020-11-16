struct localState
{
  bool state;
  bool modes[4];
  uint8_t lastMode;
  uint8_t speed;
  uint8_t brightness;
  uint8_t nextMode;
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
        uint8_t nextMode = 0;
        uint8_t diffCnt = 0;
        for(uint8 i = 1; i < sizeof(LocalState.modes); i++)
        {
            bool switchValue = mode[i] == '1';
            if(LocalState.modes[i-1] != switchValue)
            {
                diffCnt++;
                LocalState.modes[i-1] = switchValue;
                if(switchValue) nextMode = i;
            }
        }
        if(diffCnt == 1 && nextMode != 0)LocalState.nextMode = nextMode;
    }
    Serial.print("out mode: ");
    Serial.println(LocalStateToMode());
}

String getPart(ulong uptime, uint8_t divideBy)
{
    uint8_t value = uptime % divideBy;
    if(value == 0) return "00";
    return value < 10 ? "0"+String(value) : String(value);
}

String uptimeText()
{
    ulong uptime = millis() / 1000;
    
    String ss = getPart(uptime, 60);
    uptime = uptime / 60;
    String mm = getPart(uptime, 60);
    uptime = uptime / 60;
    String hh = String(uptime);
    
    return hh + ":" + mm + ":" + ss;
}

String LocalStateInfo()
{
    String state = "on";
    state += ", active mode: ";
    state += RemoteState.mode;
    state += ", uptime: ";
    state += uptimeText();
    return state;
}

bool LocalStateModeDisabled(uint8_t mode)
{
    // проверям после получения команды - могли запретить использовать текущий режим, тогда выберем следующий
    if(mode > sizeof(LocalState.modes)) return true;
    if(LocalState.nextMode!=0) return true;

    Serial.println(LocalState.modes[mode - 1]);
    return  LocalState.modes[mode - 1] == false;
}

uint8_t LocalStateGetNextMode()
{ // определим следующий режимработы
    uint8_t newMode = 0;
    if(LocalState.nextMode!=0) 
    {
        // при добавлении режима в список разрешенных сразу на него переключаемся
        newMode = LocalState.nextMode;
        LocalState.nextMode = 0;
        return newMode;
    }
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
    if(LocalState.lastMode == 0) // переключение из рабочего состояния, следующий режим случайный
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
            if(LocalState.modes[i] && (i + 1 > LocalState.lastMode)) {newMode = i + 1;break;};
        }
        if(newMode == 0)
        { // следующего не нашли, возьмем первый
            for(uint8 i = 0; i < sizeof(LocalState.modes); i++)
            {
                if(LocalState.modes[i]) {newMode = i + 1;break;};
            }
        }
        LocalState.lastMode = 0;
    }

    return newMode;
}
