#define MQQT_SETTNGS_FILE "/mqqt.json"

struct mqqtSettings
{
    String server;
    uint16_t port;
    String user;
    String password;
    String client;
    String device;
    bool mqqtActive;
} MqqtSettins;

String getMqqtSettingsAsJSON()
{
    if(MqqtSettins.server == "")MqqtSettins.server = "srv1.clusterfly.ru";
    if(MqqtSettins.port == 0) MqqtSettins.port = 9124;
    
    String settings;

    const size_t capacity = JSON_OBJECT_SIZE(15) + 400;
 
    DynamicJsonDocument doc(capacity);
    doc["mqqt"] = true;
    doc["server"] = MqqtSettins.server;
    doc["port"] = MqqtSettins.port;
    doc["user"] = MqqtSettins.user;
    doc["password"] = MqqtSettins.password;
    doc["client"] = MqqtSettins.client;
    doc["device"] = MqqtSettins.device;
    doc["mqqtActive"] = MqqtSettins.mqqtActive;    

    serializeJson(doc, settings);

    Serial.println(settings);
    return settings;
}

bool setMqqtSettingsFromJSON(String settings)
{
    const size_t capacity = JSON_OBJECT_SIZE(15) + 400;
    DynamicJsonDocument doc(capacity);
    
    DeserializationError error = deserializeJson(doc, settings);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        Serial.println(settings);
        return false;
    }    
    
    MqqtSettins.server = doc["server"].as<String>();
    MqqtSettins.port = doc["port"].as<uint16_t>();
    MqqtSettins.user = doc["user"].as<String>();
    MqqtSettins.password = doc["password"].as<String>();
    MqqtSettins.client = doc["client"].as<String>();
    MqqtSettins.device = doc["device"].as<String>();
    MqqtSettins.mqqtActive = doc["mqqtActive"].as<bool>();

    return true;
}

void saveMqqtSettingsToFile(){
  File f = LittleFS.open(MQQT_SETTNGS_FILE, "w");
  if (!f)
  {
    Serial.println(F("mqqt file open4write failed"));
  }
  else
  {
    Serial.println(F("mqqt writing LFS file"));
    f.print(getMqqtSettingsAsJSON());
    f.close();
    Serial.println(F("mqqt file closed"));
  }
}

void mqqtSettingsInit()
{
  File f = LittleFS.open(MQQT_SETTNGS_FILE, "r");
  if (!f)
  {
    Serial.println(F("mqqt file open4read failed"));
  }
  else
  {
    Serial.println(F("mqqt read LFS file"));
    String settings = f.readString();
    Serial.println(settings);
    if(settings.startsWith("{") && settings.endsWith("}"))
    {
        setMqqtSettingsFromJSON(settings);
    }
    else
    {
        Serial.println(F("no JSON in file"));
    }
    f.close();
    Serial.println(F("mqqt file closed"));
  }
}