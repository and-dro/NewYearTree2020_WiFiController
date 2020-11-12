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
    String settings;

    const size_t capacity = JSON_OBJECT_SIZE(15) + 400;
    Serial.println(capacity);
    DynamicJsonDocument doc(capacity);
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


