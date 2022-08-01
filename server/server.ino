#include "WiFiEsp.h"
#include "env.h"
#include <SPI.h>
#include <SD.h>

char ssid[] = SSID;
char pass[] = PASSWORD;

int status = WL_IDLE_STATUS;

WiFiEspServer server(80);

String http_req;

File webFile;

byte clientBuf[64];
int clientCount = 0;

void setup()
{
  Serial.begin(9600);
  WiFi.init(&Serial);
  WiFi.config(IPAddress(SERVERIP));

  if (WiFi.status() == WL_NO_SHIELD){
    while (true);
  }
  while (status != WL_CONNECTED){
    status = WiFi.begin(ssid, pass);
  }
  server.begin();

  if (!SD.begin(10)){
    return;
  }
}

void loop()
{
  WiFiEspClient client = server.available();
  if (client){
    boolean isCurrentLineBlank = true;
    while (client.connected()){
      if (client.available()){
        char request = client.read();
        if(http_req.length() < 80){
          http_req += request;
        }
        if(request == '\n' && isCurrentLineBlank){
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          webFile = SD.open("index.htm");
            if (webFile) {
              while (webFile.available()) {
                clientBuf[clientCount] = webFile.read();
                clientCount++;
                if(clientCount > 63){
                  client.write(clientBuf, 64);
                  clientCount = 0;
                }
              }
              if(clientCount > 0) client.write(clientBuf, clientCount);
              webFile.close();
            }

          if (request == '\n') {
            isCurrentLineBlank = true;
          } else if (request != '\r') {
            isCurrentLineBlank = false;
          }
        }
      }
    }
    delay(1);
    client.stop();
    http_req = "";
  }
}