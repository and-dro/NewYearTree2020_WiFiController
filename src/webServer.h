#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "private.h"

#ifndef WIFI_SID
#define WIFI_SID "wifi_sid"; // переопределяется в private.h
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "wifi_password"; // переопределяется в private.h
#endif

const char* ssid     = WIFI_SID;
const char* password = WIFI_PASSWORD;

ESP8266WebServer server(80);

#include "indexPage.h"

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleGetSettings() {
  char state[50];
  sprintf(state, "{\"brightness\":%d,\"speed\":%d,\"change\":%d,\"mode\":%d}", RemoteState.brightness, RemoteState.speed, ModeChanger.value, RemoteState.mode);
  server.send(200, "text/json", state);
}

void handleSetSettings(){
  Serial.println(server.args());
  for (uint8_t i = 0; i < server.args(); i++) {
    if(server.argName(i) == "b") RemoteState.brightness = atoi(server.arg(i).c_str());
    if(server.argName(i) == "s") RemoteState.speed = atoi(server.arg(i).c_str());
    if(server.argName(i) == "m") RemoteState.mode = atoi(server.arg(i).c_str());
    if(server.argName(i) == "c") ModeChanger.value = atoi(server.arg(i).c_str());
  }
  server.send(200, "text/json", "{\"result\":true}");
  
  remoteStateSend();
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


void webSeverStart()
{

  WiFi.mode(WIFI_STA);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  bool state = false;
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    digitalWrite(LED_BUILTIN,  state ? HIGH : LOW);
  }
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("NewYear2020")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/set", HTTP_GET, handleGetSettings);
  server.on("/set", HTTP_PUT, handleSetSettings);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

}

void webServerProcessRequest()
{
  server.handleClient();
  MDNS.update();
}
