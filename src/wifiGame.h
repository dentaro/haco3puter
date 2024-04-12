#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
#include <M5GFX_DentaroUI.hpp>
#include "SPIFFS.h"
#include "baseGame.h"
// #include "Tunes.h"
#include "M5Cardputer.h"

#ifndef WIFI_GAME_H
#define WIFI_GAME_H

// #define MAX_CHAR 256

class WifiGame
{
  public:
    enum HTTPPhase{
      INIT,
      METHOD,
      HEADER,
      BODY
    };
    enum HTTPEncodePhase{
      RAW,
      PERCENT,
      PERCENT1
    };


    // WiFiServer server;
    // WifiGame():server(80){
    // }
    
    WifiGame(){
    }
    int buttonState[CTRLBTNNUM];
    void init(bool);
    int initSTA();
    String randomString(String prefix, int n);
    void initAP();
    int run(int remainTime);
    void pause();
    void resume();
    void assignSetting(String* key, String* value, String* ssid, String* password);
    int c2hex(char c);
    void getHandler(WiFiClient *c, String path);
    void postHandler(WiFiClient *c, String path, String body);
};

#endif
