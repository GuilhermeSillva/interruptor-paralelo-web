#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "env.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool firebaseSingupOK = false;
unsigned long sendDataPrevMillis = 0;

const int quartoRelePort = D5;
const int quartoLampSensor = A0;

void setupWifi(){
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
}

void setupFirebase(){
  config.api_key = APIKEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth, "", "")){
    firebaseSingupOK = true;
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setupRelePort(int relePort){
  pinMode(relePort, OUTPUT);
}

void turnOffRele(int relePort){
  digitalWrite(relePort, LOW);
}

void toggleReleValue(int relePort){
  digitalWrite(relePort, !digitalRead(relePort));
}

void toggleLampValue(bool newValue){
  if(Firebase.RTDB.setBool(&fbdo, "/quartoLamp", newValue)){
    Serial.println(F("Inserido com sucesso!"));
  } else {
    Serial.println(F("Erro!"));
    Serial.println("MOTIVO: " + fbdo.errorReason());
  }
}

void quartoLampListener(){
  int currentLampValueSensor = analogRead(quartoLampSensor);
  if(Firebase.RTDB.getBool(&fbdo, "/quartoLamp")){
    bool currentLampValueDB = fbdo.boolData();
    Serial.println(currentLampValueDB);

    if((currentLampValueDB) && currentLampValueSensor < 950) toggleLampValue(false);
    if(!(currentLampValueDB) && currentLampValueSensor > 300) toggleLampValue(true);

  } else {
    Serial.println(fbdo.errorReason());
  }
}

void quartoLampReleListener(){
  if(Firebase.RTDB.getBool(&fbdo, "/quartoLampRele")){
    bool currentReleValue = fbdo.boolData();
    Serial.println(currentReleValue);
    if(digitalRead(quartoRelePort) != currentReleValue) toggleReleValue(quartoRelePort);

  } else {
    Serial.println(fbdo.errorReason());
  }
}

void setupListeners(){
  if(Firebase.ready() && firebaseSingupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    quartoLampListener();
    quartoLampReleListener();
  }
}

void setup(){
  Serial.begin(115200);
  setupRelePort(quartoRelePort);
  turnOffRele(quartoRelePort);
  setupWifi();
  setupFirebase();
}

void loop(){
  setupListeners();
}