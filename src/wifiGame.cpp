#include "wifiGame.h"
// #include <LovyanGFX_DentaroUI.hpp>
#include <Preferences.h>
// extern Tunes tunes;
// LGFX tft;
extern M5Canvas tft;
// extern M5Canvas tft(&M5Cardputer.Display);
// extern LGFX_Sprite tft;
extern int* buttonState;
// extern wifiDebugRequest;

Preferences preferences;

void WifiGame::init(bool isSelf){
  // SPIFFS.begin();

  // if(isSelf){//1
  //   initAP();
  // }else{//0
    // initSTA();
  // }
  server.begin();
  this->resume();
}

int WifiGame::initSTA(){
  // String ssid = "";//preferences.getString("WIFI_SSID");
  // String password = "";//preferences.getString("WIFI_PASSWD");

  // File fr = SPIFFS.open(WIFIPASS_FILE, "r");// ⑩ファイルを読み込みモードで開く
  // for(int i= 0;i<2;i++){//
  // String _readStr = fr.readStringUntil(',');// ⑪,まで１つ読み出し
  //   if(i==0){
  //     ssid = _readStr;
  //     // preferences.putString("WIFI_SSID", ssid);
  //     }else if(i==1){
  //     password = _readStr;
  //     // preferences.putString("WIFI_PASSWD", password);
  //     }
  // }
  // fr.close();	// ⑫	ファイルを閉じる
  
  preferences.end();

  // WiFi.disconnect(true);
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid.c_str(), password.c_str());
  // WiFi.begin("wf310", "2670222a");
  // Serial.print("Connecting to");
  // Serial.println(ssid);

  // tft.fillScreen(TFT_BLACK);
  // tft.setTextSize(1);
  // tft.setTextColor(TFT_WHITE, TFT_BLUE);
  // tft.setCursor(0, 10);
  // tft.print("Starting WiFi...");
  // tft.setCursor(0, 20);
  // tft.print(ssid);



  // while(WiFi.status() != WL_CONNECTED){
  //   delay(500);
  //   Serial.print(".");
  // }

  // unsigned long startTime = millis();
  // while (WiFi.status() != WL_CONNECTED) {
  //   if (millis() - startTime >= 1000) { // 1秒以上経過した場合は再試行
  //     tft.println("");
  //     tft.println("Failed to connect to WiFi.");
  //     return -1; // エラーコードを返すなど、エラー処理を追加する
  //   }
  //   delay(500);
  //   tft.print(".");
  // }
  

  // tft.println("");
  // tft.println("WiFi connected.");
  // tft.println("IP address: ");
  // tft.println(WiFi.localIP());

  // tft.setCursor(0, 30);
  // tft.print(WiFi.localIP());

  // tft.setPivot(0, 0);
  // tft.pushRotateZoom(&M5Cardputer.Display, 40, 3, 0, 1, 1);

  return 0;
}
String WifiGame::randomString(String prefix, int n){
  String ret = prefix;
  for(int i = 0; i < n; i ++){
    ret += (char)('0' + random(10));
  }
  return ret;
}
void WifiGame::initAP(){
  WiFi.disconnect(true);
  String ssid = randomString("o-bako-", 5);
  String password = randomString("", 8);
  bool result = WiFi.softAP(ssid.c_str(), password.c_str());
  if(!result){
    Serial.println("error softAP");
  }

  IPAddress ip(192,168,0,1);
  IPAddress subnet(255,255,255,0);
  WiFi.softAPConfig(ip,ip,subnet);
  WiFi.softAPIP();

  tft.println("self WiFi started");
  tft.println("IP address: ");
  tft.println(WiFi.softAPIP());

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setCursor(0, 10);
  tft.print("Starting WiFi...");

  tft.setCursor(0, 30);
  tft.print(ssid);
  tft.setCursor(0, 40);
  tft.print(password);

  tft.setCursor(0, 50);
  tft.print(WiFi.softAPIP().toString());
}

void WifiGame::assignSetting(String* key, String* value, String* ssid, String* password){
  if(*key == "WIFIID"){
    *ssid = *value;
  }
  if(*key == "WIFIPASS"){
    *password = *value;
  }
}

void WifiGame::getHandler(WiFiClient *c, String path){
  String filePath = "";

  if(path.startsWith("/setting/")){
    c->println("HTTP/1.1 200 OK");
    c->println("Content-type: text/html; charset=utf-8"); // UTF-8を指定
    // c->println("Content-type: text/html");
    c->println();
    c->println("<form method='POST' action='/action/wifiset'>");
    c->println("ssid: <input type='text' name='ssid'>");
    c->println("password: <input type='text' name='password'>");
    c->println("<input type='submit'>");
    c->println("</form>");
  }
  if(path.startsWith("/list/")){
    // /list/[path]
    filePath = path.substring(6);

    File root = SPIFFS.open("/" + filePath);
    File f = root.openNextFile();

    c->println("HTTP/1.1 200 OK");
    c->println("Content-type: text/html; charset=utf-8"); // UTF-8を指定
    // c->println("Content-type: text/html");
    c->println();
    c->println("<h1>haco3 file list</h1>");

    while(f){
      if(f.isDirectory()){
        c->print(f.path());
        c->println("/");
      }else{
        c->print("<li>");
        c->print(f.path());
        c->print(" <a href='/file");
        c->print(f.path());
        c->print("'>");
        c->print("view</a>");

        c->print(" <a href='/edit");
        c->print(f.path());
        c->print("'>");
        c->print("edit</a>");

        c->print(" <a href='/html");
        c->print(f.path());
        c->print("'>");
        c->print("html</a>");

        c->print("<form style='display:inline;' method='POST' action='/delete");
        c->print(f.path());
        c->println("'>");
        c->println("<input type='submit' value='DELETE'>");
        c->println("</form>");

        c->print("</li>");
      }
      f = root.openNextFile();
    }
  }

  if(path.startsWith("/file/") || path.startsWith("/edit/") || path.startsWith("/html/")){
    // /file/[path]
    // /edit/[path]
    // /html/[path]

    bool isEdit = path.startsWith("/edit/");
    bool isHtml = path.startsWith("/html/");

    path = path.substring(6);

    if(isHtml || isEdit){
      c->println("HTTP/1.1 200 OK");
      c->println("Content-type: text/html; charset=utf-8"); // UTF-8を指定
      // c->println("Content-type: text/html");
      c->println();
    }else{
      c->println("HTTP/1.1 200 OK");
      c->println("Content-type: text/text; charset=utf-8"); // UTF-8を指定
      // c->println("Content-type: text/text");
      c->println();
    }

    if(isEdit){
      c->print("<form method='POST' action='");
      c->print("/file/");
      c->print(path);
      c->print("'><textarea style='width:100%;height:100%;' name='body'>");
    }

    File f = SPIFFS.open("/" + path);
    while(f.available()){
      byte b = f.read();
      if(isEdit){
        switch(b){
          case '&': c->print("&amp;"); break;
          case '>': c->print("&gt;"); break;
          case '<': c->print("&lt;"); break;
          default:
            c->write(b);
        }
      }else{
        c->write(b);
      }
    }

    if(isEdit){
      c->println("</textarea>");
      c->println("<div style='position:absolute;right:0px;top:0px;'><input type='submit' value='save'></div>");
      c->println("</form>");

      c->println("<div style='position:absolute;right:0px;top:30px;'><form method='POST' action='/action/reboot'><input type='submit' value='reload'></form></div>");
    }
    return;
  }

}


// void WifiGame::getHandler(WiFiClient *c, String path){
//   String filePath = "";

//   if(path.startsWith("/setting/")){
//     c->println("HTTP/1.1 200 OK");
//     c->println("Content-type: text/html");
//     c->println();
//     c->println("<form method='POST' action='/action/wifiset'>");
//     c->println("ssid: <input type='text' name='ssid'>");
//     c->println("password: <input type='text' name='password'>");
//     c->println("<input type='submit'>");
//     c->println("</form>");
//   }
//   if(path.startsWith("/list/")){
//     // /list/[path]
//     filePath = path.substring(6);

//     File root = SPIFFS.open("/" + filePath);
//     File f = root.openNextFile();

//     c->println("HTTP/1.1 200 OK");
//     c->println("Content-type: text/html");
//     c->println();
//     c->println("<h1>o-bako file list</h1>");

//     while(f){
//       if(f.isDirectory()){
//         c->print(f.name());
//         c->println("/");
//       }else{
//         c->print("<li>");
//         c->print(f.name());
//         c->print(" <a href='/file");
//         c->print(f.name());
//         c->print("'>");
//         c->print("view</a>");

//         c->print(" <a href='/edit");
//         c->print(f.name());
//         c->print("'>");
//         c->print("edit</a>");

//         c->print(" <a href='/html");
//         c->print(f.name());
//         c->print("'>");
//         c->print("html</a>");

//         c->print("<form style='display:inline;' method='POST' action='/delete/");
//         c->print(f.name());
//         c->println("'>");
//         c->println("<input type='submit' value='DELETE'>");
//         c->println("</form>");

//         c->print("</li>");
//       }
//       f = root.openNextFile();
//     }
//   }

//   if(path.startsWith("/file/") || path.startsWith("/edit/") || path.startsWith("/html/")){
//     // /file/[path]
//     // /edit/[path]
//     // /html/[path]

//     bool isEdit = path.startsWith("/edit/");
//     bool isHtml = path.startsWith("/html/");

//     path = path.substring(6);

//     if(isHtml || isEdit){
//       c->println("HTTP/1.1 200 OK");
//       c->println("Content-type: text/html");
//       c->println();
//     }else{
//       c->println("HTTP/1.1 200 OK");
//       c->println("Content-type: text/text");
//       c->println();
//     }

//     if(isEdit){
//       c->print("<form method='POST' action='");
//       c->print("/file/");
//       c->print(path);
//       c->print("'><textarea style='width:100%;height:100%;' name='body'>");
//     }

//     File f = SPIFFS.open("/" + path);
//     while(f.available()){
//       byte b = f.read();
//       if(isEdit){
//         switch(b){
//           case '&': c->print("&amp;"); break;
//           case '>': c->print("&gt;"); break;
//           case '<': c->print("&lt;"); break;
//           default:
//             c->write(b);
//         }
//       }else{
//         c->write(b);
//       }
//     }

//     if(isEdit){
//       c->println("</textarea>");
//       c->println("<div style='position:absolute;right:0px;top:0px;'><input type='submit' value='save'></div>");
//       c->println("</form>");

//       c->println("<div style='position:absolute;right:0px;top:30px;'><form method='POST' action='/action/reboot'><input type='submit' value='reload'></form></div>");
//     }
//     return;
//   }

// }
int WifiGame::c2hex(char c){
  switch(c){
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'A': return 10;
    case 'B': return 11;
    case 'C': return 12;
    case 'D': return 13;
    case 'E': return 14;
    case 'F': return 15;
  }
  return 0;
}
void WifiGame::postHandler(WiFiClient *c, String path, String body){
  String filePath = "";

  if(path.startsWith("/exec/")){
    // TODO:execute lua
  }else if(path.startsWith("/delete/")){
    path = path.substring(8);
    if(SPIFFS.remove(path)){
      c->println("HTTP/1.1 200 OK");
      c->println("Content-type: text/plain; charset=utf-8"); // UTF-8を指定
      // c->println("Content-type: text/plain");
      c->println();
      c->println("OK");
    }else{
      c->println("HTTP/1.1 200 OK");
      c->println("Content-type: text/plain; charset=utf-8"); // UTF-8を指定
      // c->println("Content-type: text/plain");
      c->println();
      c->println("remove fail");
    }
  }else if(path.startsWith("/action/reboot")){
    Serial.println("action reboot");
    c->println("HTTP/1.1 200 OK");
    c->println("Content-type: text/plain; charset=utf-8"); // UTF-8を指定
    // c->println("Content-type: text/plain");
    c->println();
    c->println("OK");

    pause();
    Serial.println("restart");
    delay(1000 * 1);
    ESP.restart();
  }else{
    c->println("HTTP/1.1 400 BadRequest");
    c->println("Content-type: text/plain; charset=utf-8"); // UTF-8を指定
    // c->println("Content-type: text/plain");
    c->println();
    c->println("400 BadRequest");
  }
}

void WifiGame::pause(){
  server.end();
  WiFi.disconnect();
}

void WifiGame::resume(){
}
// int WifiGame::run(int remainTime){

//   WiFiClient client = server.available();
//   if(client){
//     // tunes.pause();
//     Serial.println("New Client.");
//     String currentLine = "";
//     HTTPPhase phase = INIT;
//     bool isPost = false;
//     int len = 0;
//     String body = "";
//     String path = "";

//     while(client.connected()){
//       Serial.println("while");
//       Serial.println(client.remoteIP());
//       if(client.available()){
//         Serial.println("available");
//         char c = client.read();
//         Serial.write(c);
//         if(c == '\n' && phase != BODY){
//           Serial.println("end get");
//           if(currentLine.length() == 0){
//             if(isPost){
//               phase = BODY;
//               if(len == 0){
//                 postHandler(&client, path, "");
//                 break; // end of request
//               }
//             }else{
//               // GET REQUEST
//               getHandler(&client, path);
//               break; // end of request
//             }
//           }else{
//             switch(phase){
//               case INIT:
//                 if(currentLine.startsWith("GET ")){
//                   path = currentLine.substring(4, currentLine.indexOf(" ", 4));
//                   phase = METHOD;
//                 }
//                 if(currentLine.startsWith("POST ")){
//                   path = currentLine.substring(5, currentLine.indexOf(" ", 5));
//                   phase = METHOD;
//                   isPost = true;
//                 }
//                 break;
//               case METHOD:
//                 if(currentLine.startsWith("Content-Length: ")){
//                   len = currentLine.substring(16).toInt();
//                   phase = HEADER;
//                 }
//                 break;
//               case HEADER:
//                 break;
//               case BODY:
//                 break;
//             }
//             currentLine = "";
//           }
//         }else if(c != '\r' && phase != BODY){
//           currentLine += c;
//         }else if(phase == BODY){
//           if(path.startsWith("/file/")){
//             String filePath = path.substring(6);

//             Serial.println("POST BODY");
//             while(client.available() && len > 0){
//               char c = client.read();
//               Serial.write(c);
//               len --;
//               if(c == '='){
//                 break;
//               }
//             }

//             File f = SPIFFS.open("/" + filePath, FILE_WRITE);
//             HTTPEncodePhase ePhase = RAW;
//             uint8_t first;
//             Serial.println("POST BODY after =");
//             while(client.available() && len > 0){
//               char c = client.read();
//               Serial.write(c);
//               len --;
//               switch(ePhase){
//                 case RAW:
//                   if(c == '%'){
//                     ePhase = PERCENT;
//                   }else if(c == '+'){
//                     f.write(' ');
//                   }else{
//                     f.write(c);
//                   }
//                   break;
//                 case PERCENT:
//                   first = c2hex(c);
//                   ePhase = PERCENT1;
//                   break;
//                 case PERCENT1:
//                   f.write((first << 4) + c2hex(c));
//                   ePhase = RAW;
//                   break;
//               }
//             }
//             f.close();
//             client.println("HTTP/1.1 200 OK");
//             client.println("Content-type: text/plain");
//             client.println();
//             client.println("OK");
//             return 1;
//           }
//           if(path.startsWith("/action/wifiset")){
//             client.println("HTTP/1.1 200 OK");
//             client.println("Content-type: text/plain");
//             client.println();
//             client.println("OK");

//             enum DecodePhase{
//               KEY,
//               VALUE
//             };
//             DecodePhase mode = KEY;
//             String key;
//             String value;
//             String ssid;
//             String password;
//             while(true){
//               if(c == '&'){
//                 assignSetting(&key, &value, &ssid, &password);
//                 key = "";
//                 value = "";

//                 mode = KEY;
//               }else if(c == '='){
//                 mode = VALUE;
//               }else{
//                 if(mode == KEY){
//                   key += c;
//                 }else if(mode == VALUE){
//                   value += c;
//                 }
//               }
//               // todo: len is not correct
//               if(len == 0)break;
//               if(!client.available())break;

//               c = client.read();
//               len --;
//             }
//             assignSetting(&key, &value, &ssid, &password);

//             preferences.begin("wifi-config");

//             // File fr = SPIFFS.open(WIFIPASS_FILE, "r");// ⑩ファイルを読み込みモードで開く
//             // for(int i= 0;i<2;i++){//
//             // String _readStr = fr.readStringUntil(',');// ⑪,まで１つ読み出し
//             //   if(i==0){
//             //     ssid = _readStr;
//             //     preferences.putString("WIFI_SSID", ssid);
//             //     }else if(i==1){
//             //     password = _readStr;
//             //     preferences.putString("WIFI_PASSWD", password);
//             //     }
//             // }
//             // fr.close();	// ⑫	ファイルを閉じる

//             preferences.putString("WIFI_SSID", ssid);

//             preferences.putString("WIFI_PASSWD", password);

//             client.println(ssid);
//             client.println(password);
//           }
//           break; // end of request
//         }
//       }
//     }
//     client.stop();
//     Serial.println("Client Disconnected.");
//     // tunes.resume();
//   }

//   return 0;
// }
int WifiGame::run(int remainTime){

  WiFiClient client = server.available();
  if(client){
    // tunes.pause();
    Serial.println("New Client.");
    String currentLine = "";
    HTTPPhase phase = INIT;
    bool isPost = false;
    int len = 0;
    String body = "";
    String path = "";

    while(client.connected()){
      Serial.println("while");
      Serial.println(client.remoteIP());
      if(client.available()){
        Serial.println("available");
        char c = client.read();
        Serial.write(c);
        if(c == '\n' && phase != BODY){
          Serial.println("end get");
          if(currentLine.length() == 0){
            if(isPost){
              phase = BODY;
              if(len == 0){
                postHandler(&client, path, "");
                break; // end of request
              }
            }else{
              // GET REQUEST
              getHandler(&client, path);
              break; // end of request
            }
          }else{
            switch(phase){
              case INIT:
                if(currentLine.startsWith("GET ")){
                  path = currentLine.substring(4, currentLine.indexOf(" ", 4));
                  phase = METHOD;
                }
                if(currentLine.startsWith("POST ")){
                  path = currentLine.substring(5, currentLine.indexOf(" ", 5));
                  phase = METHOD;
                  isPost = true;
                }
                break;
              case METHOD:
                if(currentLine.startsWith("Content-Length: ")){
                  len = currentLine.substring(16).toInt();
                  phase = HEADER;
                }
                break;
              case HEADER:
                break;
              case BODY:
                break;
            }
            currentLine = "";
          }
        }else if(c != '\r' && phase != BODY){
          currentLine += c;
        }else if(phase == BODY){
          if(path.startsWith("/file/")){
            String filePath = path.substring(6);

            Serial.println("POST BODY");
            while(client.available() && len > 0){
              char c = client.read();
              Serial.write(c);
              len --;
              if(c == '='){
                break;
              }
            }

            File f = SPIFFS.open("/" + filePath, FILE_WRITE);
            HTTPEncodePhase ePhase = RAW;
            uint8_t first;
            Serial.println("POST BODY after =");
            while(client.available() && len > 0){
              char c = client.read();
              Serial.write(c);
              len --;
              switch(ePhase){
                case RAW:
                  if(c == '%'){
                    ePhase = PERCENT;
                  }else if(c == '+'){
                    f.write(' ');
                  }else{
                    f.write(c);
                  }
                  break;
                case PERCENT:
                  first = c2hex(c);
                  ePhase = PERCENT1;
                  break;
                case PERCENT1:
                  f.write((first << 4) + c2hex(c));
                  ePhase = RAW;
                  break;
              }
            }
            f.close();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: text/plain");
            client.println();
            client.println("OK");
            return 1;
          }
          if(path.startsWith("/action/wifiset")){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: text/plain");
            client.println();
            client.println("OK");

            enum DecodePhase{
              KEY,
              VALUE
            };
            DecodePhase mode = KEY;
            String key;
            String value;
            String ssid;
            String password;
            while(true){
              if(c == '&'){
                assignSetting(&key, &value, &ssid, &password);
                key = "";
                value = "";

                mode = KEY;
              }else if(c == '='){
                mode = VALUE;
              }else{
                if(mode == KEY){
                  key += c;
                }else if(mode == VALUE){
                  value += c;
                }
              }
              // todo: len is not correct
              if(len == 0)break;
              if(!client.available())break;

              c = client.read();
              len --;
            }
            assignSetting(&key, &value, &ssid, &password);

            preferences.begin("wifi-config");
            preferences.putString("WIFI_SSID", ssid);
            preferences.putString("WIFI_PASSWD", password);

            client.println(ssid);
            client.println(password);
          }
          break; // end of request
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
    // tunes.resume();
  }

  return 0;
}
