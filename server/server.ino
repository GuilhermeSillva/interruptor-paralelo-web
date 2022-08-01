#include "WiFiEsp.h"
#include "env.h"
#include <SPI.h>
#include <SD.h>

char ssid[] = SSID;
char pass[] = PASSWORD;

int status = WL_IDLE_STATUS;

WiFiEspServer server(80);

RingBuffer buf(8);

void setup() {
  Serial.begin(9600);
  WiFi.init( & Serial);
  WiFi.config(IPAddress(SERVERIP));

  if (WiFi.status() == WL_NO_SHIELD) {
    while (true);
  }
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
  }
  server.begin();
}

void loop() {
  WiFiEspClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char request = client.read();
        buf.push(request);
      }
    }
    client.stop();
  }
}