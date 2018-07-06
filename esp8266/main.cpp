#include "ota.h"
#include "tg.h"
#include <ESP8266WebServer.h>

int relay_pin = D1;

bool wd_start = false;

unsigned long wd_start_delay = 15*60*1000;
unsigned long wd_delay = 60*1000;
unsigned long wd_last = 0;

ESP8266WebServer server(80);

void relay_off() {
  digitalWrite(relay_pin, LOW);
  wd_last = millis();
  Serial.print(millis());Serial.print(" ");Serial.println("OFF");
}

void handleWD() {
  wd_start = true;
  relay_off();
  server.send(200, "text/html",  "OK\n");
  Serial.print(millis());Serial.print(" ");Serial.println("Clear counter");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  ota_setup();

  pinMode(relay_pin, OUTPUT);
  relay_off();

  server.on("/wd", handleWD);
  server.begin();
  Serial.println("HTTP server started");
  sendMessage("ESP restart");
}

void relay_on() {
  Serial.print(millis());Serial.print(" ");Serial.println("ON");
  digitalWrite(relay_pin, HIGH);
  delay(3000);
  wd_start = false;
  relay_off();
}



void loop() {
  ota_handle();
  server.handleClient();

  if (wd_start && millis() > wd_delay + wd_last) {
    sendMessage("ESP wd_delay + wd_last timeout");
    relay_on();
  }
  if ((!wd_start) && millis() > wd_start_delay ) {
    sendMessage("ESP wd_start_delay");
    relay_on();
    ESP.restart();
  }

}
