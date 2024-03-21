
#include "baseGame.h"

#include <string> // 必要に応じて追加
#include <chrono>

#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include "runLuaGame.h"
#include <sstream>

#include <Arduino.h>
#include <FS.h>
#include <SD.h>

// #include "SPIFFS.h"
#include <SPIFFS.h>
#include <M5Unified.h>
// #include "Tunes.h"
#include "Editor.h"
// #include <PS2Keyboard.h>
// #include "Speaker_Class.hpp"
// #include "Channel.hpp"
// #include <esp_now.h>
// #include <WiFi.h>
#include <M5GFX_DentaroUI.hpp>
#include <map>
#include "M5Cardputer.h"

uint8_t toolNo = 0;
bool textMoveF = false;
bool shiftF = false;

float effectVal;
bool enemyF = false;
uint8_t enemyX = 0;
uint8_t enemyY = 0;
uint8_t enemyTransCn = 0;
String enemyPath = "";

static int menu_x = 2;
static int menu_y = 20;
static int menu_w = 120;
static int menu_h = 30;
static int menu_padding = 36;

#define KEYBOARD_DATA 22//32
#define KEYBOARD_CLK  19//33

// #define SPEAKER_PIN 25

#define MAPWH 16//マップのpixelサイズ
// #define BUF_PNG_NUM 0
// #define TFT_WIDTH 160
// #define TFT_HEIGHT 128
#define TFT_WIDTH_HALF 80
#define TFT_HEIGHT_HALF 64

#define VEC_FRAME_COUNT 10

#define TFT_RUN_MODE 0
#define TFT_EDIT_MODE 1
// #define TFT_WIFI_MODE 2

int gameState = 0;


String data = "> ";

// Every call to read() returns a single byte for each
// keystroke.  These configure what byte will be returned
// for each "special" key.  To ignore a key, use zero.
#define PS2_TAB				9
#define PS2_ENTER			13
#define PS2_BACKSPACE			127
#define PS2_ESC				27
#define PS2_INSERT			0
#define PS2_DELETE			127
#define PS2_HOME			36
#define PS2_END				35
#define PS2_PAGEUP			25
#define PS2_PAGEDOWN			26
#define PS2_UPARROW			11
#define PS2_LEFTARROW			8
#define PS2_DOWNARROW			10
#define PS2_RIGHTARROW			21
#define PS2_F1				0
#define PS2_F2				0
#define PS2_F3				0
#define PS2_F4				0
#define PS2_F5				0
#define PS2_F6				0
#define PS2_F7				0
#define PS2_F8				0
#define PS2_F9				0
#define PS2_F10				0
#define PS2_F11				0
#define PS2_F12				0
#define PS2_SCROLL			0
#define PS2_EURO_SIGN			0

#define PS2_INVERTED_EXCLAMATION	161 // ¡
#define PS2_CENT_SIGN			162 // ¢
#define PS2_POUND_SIGN			163 // £
#define PS2_CURRENCY_SIGN		164 // ¤
#define PS2_YEN_SIGN			165 // ¥
#define PS2_BROKEN_BAR                 	166 // ¦
#define PS2_SECTION_SIGN		167 // §
#define PS2_DIAERESIS			168 // ¨
#define PS2_COPYRIGHT_SIGN		169 // ©
#define PS2_FEMININE_ORDINAL		170 // ª
#define PS2_LEFT_DOUBLE_ANGLE_QUOTE	171 // «
#define PS2_NOT_SIGN			172 // ¬
#define PS2_HYPHEN			173
#define PS2_REGISTERED_SIGN		174 // ®
#define PS2_MACRON			175 // ¯
#define PS2_DEGREE_SIGN			176 // °
#define PS2_PLUS_MINUS_SIGN		177 // ±
#define PS2_SUPERSCRIPT_TWO		178 // ²
#define PS2_SUPERSCRIPT_THREE		179 // ³
#define PS2_ACUTE_ACCENT		180 // ´
#define PS2_MICRO_SIGN			181 // µ
#define PS2_PILCROW_SIGN		182 // ¶
#define PS2_MIDDLE_DOT			183 // ·
#define PS2_CEDILLA			184 // ¸
#define PS2_SUPERSCRIPT_ONE		185 // ¹
#define PS2_MASCULINE_ORDINAL		186 // º
#define PS2_RIGHT_DOUBLE_ANGLE_QUOTE	187 // »
#define PS2_FRACTION_ONE_QUARTER	188 // ¼
#define PS2_FRACTION_ONE_HALF		189 // ½
#define PS2_FRACTION_THREE_QUARTERS	190 // ¾
#define PS2_INVERTED_QUESTION_MARK	191 // ¿
#define PS2_A_GRAVE			192 // À
#define PS2_A_ACUTE			193 // Á
#define PS2_A_CIRCUMFLEX		194 // Â
#define PS2_A_TILDE			195 // Ã
#define PS2_A_DIAERESIS			196 // Ä
#define PS2_A_RING_ABOVE		197 // Å
#define PS2_AE				198 // Æ
#define PS2_C_CEDILLA			199 // Ç
#define PS2_E_GRAVE			200 // È
#define PS2_E_ACUTE			201 // É
#define PS2_E_CIRCUMFLEX		202 // Ê
#define PS2_E_DIAERESIS			203 // Ë
#define PS2_I_GRAVE			204 // Ì
#define PS2_I_ACUTE			205 // Í
#define PS2_I_CIRCUMFLEX		206 // Î
#define PS2_I_DIAERESIS			207 // Ï
#define PS2_ETH				208 // Ð
#define PS2_N_TILDE			209 // Ñ
#define PS2_O_GRAVE			210 // Ò
#define PS2_O_ACUTE			211 // Ó
#define PS2_O_CIRCUMFLEX		212 // Ô
#define PS2_O_TILDE			213 // Õ
#define PS2_O_DIAERESIS			214 // Ö
#define PS2_MULTIPLICATION		215 // ×
#define PS2_O_STROKE			216 // Ø
#define PS2_U_GRAVE			217 // Ù
#define PS2_U_ACUTE			218 // Ú
#define PS2_U_CIRCUMFLEX		219 // Û
#define PS2_U_DIAERESIS			220 // Ü
#define PS2_Y_ACUTE			221 // Ý
#define PS2_THORN			222 // Þ
#define PS2_SHARP_S			223 // ß
#define PS2_a_GRAVE			224 // à
#define PS2_a_ACUTE			225 // á
#define PS2_a_CIRCUMFLEX		226 // â
#define PS2_a_TILDE			227 // ã
#define PS2_a_DIAERESIS			228 // ä
#define PS2_a_RING_ABOVE		229 // å
#define PS2_ae				230 // æ
#define PS2_c_CEDILLA			231 // ç
#define PS2_e_GRAVE			232 // è
#define PS2_e_ACUTE			233 // é
#define PS2_e_CIRCUMFLEX		234 // ê
#define PS2_e_DIAERESIS			235 // ë
#define PS2_i_GRAVE			236 // ì
#define PS2_i_ACUTE			237 // í
#define PS2_i_CIRCUMFLEX		238 // î
#define PS2_i_DIAERESIS			239 // ï
#define PS2_eth				240 // ð
#define PS2_n_TILDE			241 // ñ
#define PS2_o_GRAVE			242 // ò
#define PS2_o_ACUTE			243 // ó
#define PS2_o_CIRCUMFLEX		244 // ô
#define PS2_o_TILDE			245 // õ
#define PS2_o_DIAERESIS			246 // ö
#define PS2_DIVISION			247 // ÷
#define PS2_o_STROKE			248 // ø
#define PS2_u_GRAVE			249 // ù
#define PS2_u_ACUTE			250 // ú
#define PS2_u_CIRCUMFLEX		251 // û
#define PS2_u_DIAERESIS			252 // ü
#define PS2_y_ACUTE			253 // ý
#define PS2_thorn			254 // þ
#define PS2_y_DIAERESIS			255 // ÿ


#define PS2_KEYMAP_SIZE 136

// Speaker_Class* speaker;

uint64_t frame = 0;

int isEditMode;
bool firstBootF = true;
bool difffileF = false;//前と違うファイルを開こうとしたときに立つフラグ

std::deque<int> buttonState;//ボタンの個数未定
// int buttonState[9];
uint8_t sprno;
uint8_t repeatnum;
// std::vector<std::vector<uint8_t>> rowData(16);

int gWx;
int gWy;
int gSpr8numX = 8;
int gSpr8numY = 8;
int gSprw = 8;
int gSprh = 8;

int HACO3_C0    = 0x0000;
int HACO3_C1    = 6474;//27,42,86 
int HACO3_C2    = 35018;
int HACO3_C3    = 1097;
int HACO3_C4    = 45669;
int HACO3_C5    = 25257;
int HACO3_C6    = 50712;
int HACO3_C7    = 65436;
int HACO3_C8    = 63496;//0xF802;
int HACO3_C9    = 64768;
int HACO3_C10   = 65376;
int HACO3_C11   = 1856;
int HACO3_C12   = 1407;
int HACO3_C13   = 33715;
int HACO3_C14   = 64341;
int HACO3_C15   = 65108;

uint8_t clist2[16][3] =
  {
  { 0,0,0},//0: 黒色
  { 27,42,86 },//1: 暗い青色
  { 137,24,84 },//2: 暗い紫色
  { 0,139,75 },//3: 暗い緑色
  { 183,76,45 },//4: 茶色
  { 97,87,78 },//5: 暗い灰色
  { 194,195,199 },//6: 明るい灰色
  { 255,241,231 },//7: 白色
  { 255,0,70 },//8: 赤色
  { 255,160,0 },//9: オレンジ
  { 255,238,0 },//10: 黄色
  { 0,234,0 },//11: 緑色
  { 0,173,255 },//12: 水色
  { 134,116,159 },//13: 藍色
  { 255,107,169 },//14: ピンク
  { 255,202,165}//15: 桃色
  };

enum struct FileType {
  LUA,
  JS,
  BMP,
  PNG,
  TXT,
  OTHER
};

//キーボード関連
Editor editor;

int keychar;//キーボードから毎フレーム入ってくる文字

//esp-idfのライブラリを使う！
//https://qiita.com/norippy_i/items/0ed46e06427a1d574625
// #include <driver/adc.h>//アナログボタンはこのヘッダファイルを忘れないように！！

using namespace std;

#define MAX_CHAR 512
#define FORMAT_SPIFFS_IF_FAILED true

#define BUF_PNG_NUM 0

uint8_t mainVol = 180;

//outputmode最終描画の仕方
// int outputMode = FAST_MODE;//50FPS程度128*128 速いけど小さい画面　速度が必要なモード
int outputMode = WIDE_MODE;//20FPS程度240*240 遅いけれどタッチしやすい画面　パズルなど

uint8_t xpos, ypos = 0;
uint8_t colValR = 0;
uint8_t colValG = 0;
uint8_t colValB = 0;

uint8_t charSpritex = 0;
uint8_t charSpritey = 0;
int pressedBtnID = -1;//この値をタッチボタン、物理ボタンの両方から操作してbtnStateを間接的に操作している

// esp_now_peer_info_t slave;

int mapsprnos[16];
// int mapsprnos[16] = { 20, 11, 32, 44, 53, 49, 54, 32, 52, 41, 46, 42, 45, 50, 43, 38 };

const uint8_t RGBValues[][3] PROGMEM = {//16bit用
  {0, 0, 0},     // 0: 黒色=なし
  {24, 40, 82},  // 1: 暗い青色
  {140, 24, 82}, // 2: 暗い紫色
  {0, 138, 74},  // 3: 暗い緑色
  {181, 77, 41}, // 4: 茶色 
  {99, 85, 74},  // 5: 暗い灰色
  {198, 195, 198}, // 6: 明るい灰色
  {255, 243, 231}, // 7: 白色
  {255, 0, 66},  // 8: 赤色
  {255, 162, 0}, // 9: オレンジ
  {255, 239, 0}, // 10: 黄色
  {0, 235, 0},   // 11: 緑色
  {0, 174, 255}, // 12: 水色
  {132, 117, 156}, // 13: 藍色
  {255, 105, 173}, // 14: ピンク
  {255, 203, 165}  // 15: 桃色
};

// M5GFX M5Cardputer.Display;//LGFXを継承

M5GFX_DentaroUI ui(&M5Cardputer.Display);
M5Canvas tft(&M5Cardputer.Display);
// M5Canvas scaler(&M5Cardputer.Display);

// #include "MapDictionary.h"
// MapDictionary& dict = MapDictionary::getInstance();

M5Canvas sprite88_roi = M5Canvas(&tft);
M5Canvas sprite11_roi = M5Canvas(&tft);
M5Canvas sprite64 = M5Canvas();
// uint8_t sprite64cnos[4096];//64*64

// uint8_t sprite64cnos[PNG_SPRITE_HEIGHT * PNG_SPRITE_WIDTH];//64*128
std::vector<uint8_t> sprite64cnos_vector;

M5Canvas buffSprite = M5Canvas(&tft);
M5Canvas sprite88_0 = M5Canvas(&tft);

// M5Canvas mapTileSprites[1];
// static M5Canvas sliderSprite( &tft );//スライダ用

BaseGame* game;
// Tunes tunes;
String appfileName = "";//最初に実行されるアプリ名
String savedAppfileName = "";
// String txtName = "/init/txt/sample.txt";//実行されるファイル名

uint8_t mapsx = 0;
uint8_t mapsy = 0;
String mapFileName = "/init/map/0.png";
int readmapno = 0;
int divnum = 1;
bool readMapF = false;
//divnumが大きいほど少ない領域で展開できる(2の乗数)
// M5Canvas spritebg[16];//16種類のスプライトを背景で使えるようにする
M5Canvas spriteMap;//地図用スプライト

// uint8_t mapArray[MAPWH][MAPWH];
// uint8_t mapArray[MAPH][MAPW];
uint8_t mapArray[16][20];
bool mapready = false;

int8_t sprbits[128];//fgetでアクセスするスプライト属性を格納するための配列

char buf[MAX_CHAR];
// char str[100];//情報表示用
int mode = 0;//記号モード //0はrun 1はexit
// int gameState = 0;
String appNameStr = "init";
int soundNo = -1;
float soundSpeed = 1.0;
int musicNo = -1;
bool musicflag = false;
bool sfxflag = false;
bool toneflag = false;
bool firstLoopF = true;

float sliderval[2] = {0,0};
bool optionuiflag = false;
// uint64_t frame = 0;
// float radone = PI/180;

// float sinValues[90];// 0から89度までの91個の要素

int addUiNum[4];
int allAddUiNum = 0;

// bool downloadF = true;
// bool isCardMounted = false; // SDカードがマウントされているかのフラグ

int xtile = 0;
int ytile = 0;
float ztile = 0.0;

int xtileNo = 29100;
int ytileNo = 12909;

M5Canvas sprref;
String oldKeys[BUF_PNG_NUM];

uint8_t gIMGBuf88_0[32];
uint8_t gIMGBuf64[8192];
uint8_t gIMGBuf2[32768];

/** 画像をSPIFFSから読み込む */
void readIMG() {
  auto file = SPIFFS.open("/init/initspr.png", "r");
  file.read(gIMGBuf64, 8192);
  file.close();
}

int vol_value; //analog値を代入する変数を定義
int statebtn_value; //analog値を代入する変数を定義
int jsx_value; //analog値を代入する変数を定義
int jsy_value; //analog値を代入する変数を定義
// getSign関数をMapDictionaryクラス外に移動
Vector2<int> getSign(int dirno) {
    if (dirno == -1) {
        // 方向を持たない場合、(0.0, 0.0, 0.0)を返す
        return {0, 0};
    } else {
        float dx = (dirno == 0 || dirno == 1 || dirno == 7) ? 1.0 : ((dirno == 3 || dirno == 4 || dirno == 5) ? -1.0 : 0.0);
        float dy = (dirno == 1 || dirno == 2 || dirno == 3) ? 1.0 : ((dirno == 5 || dirno == 6 || dirno == 7) ? -1.0 : 0.0);
        return {int(dx), int(dy)};
    }
}


uint16_t gethaco3Col(uint8_t haco3ColNo) {
    uint16_t result = ((static_cast<uint16_t>(clist2[haco3ColNo][0]) >> 3) << 11) |
                      ((static_cast<uint16_t>(clist2[haco3ColNo][1]) >> 2) << 5) |
                       (static_cast<uint16_t>(clist2[haco3ColNo][2]) >> 3);
    return result;
}

vector<string> split(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

// 送信コールバック
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // char macStr[18];
  // snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
  //          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // tft.print("Last Packet Sent to: ");
  // tft.println(macStr);
  // tft.print("Last Packet Send Status: ");
  // tft.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }

// 受信コールバック
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  char msg[1];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // tft.printf("Last Packet Recv from: %s\n", macStr);//MACアドレスを表示させる
  tft.printf("Last Packet Recv Data(%d): ", data_len);
  for ( int i = 0 ; i < data_len ; i++ ) {
    msg[1] = data[i];
    tft.print(msg[1]);
  }
  tft.println("");
}

Vector2<int> getKey2Sign(String _currentKey, String _targetKey) {
    int slashPos = _currentKey.indexOf('/'); // '/'の位置を取得
    if (slashPos != -1) { // '/'が見つかった場合
        String numA_str = _currentKey.substring(0, slashPos); // '/'より前の部分を取得
        String numB_str = _currentKey.substring(slashPos + 1); // '/'より後の部分を取得
        int numA_current = numA_str.toInt(); // 数字に変換
        int numB_current = numB_str.toInt(); // 数字に変換
        
        slashPos = _targetKey.indexOf('/'); // '/'の位置を取得
        if (slashPos != -1) { // '/'が見つかった場合
            numA_str = _targetKey.substring(0, slashPos); // '/'より前の部分を取得
            numB_str = _targetKey.substring(slashPos + 1); // '/'より後の部分を取得
            int numA_target = numA_str.toInt(); // 数字に変換
            int numB_target = numB_str.toInt(); // 数字に変換
            
            int dx = numA_target - numA_current;
            int dy = numB_target - numB_current;
            
            return {dx, dy};
        }
    }
    
    return {0, 0}; // デフォルトの値
}

Vector3<float> currentgpos = {0,0,0};;
Vector3<float> prePos= {0.0, 0.0, 0.0};
Vector3<float> currentPos = {0,0,0};
Vector3<float> diffPos = {0.0,0.0,0.0};

int dirNos[9];
int shouldNo = 0;
int downloadLimitNum = 0;
String targetKey = "";
float tileZoom = 15.0;
float bairitu = 1.0;

std::vector<String> temporaryKeys;
std::vector<String> previousKeys;
std::vector<String> writableKeys;
std::vector<String> downloadKeys;
std::vector<String> predownloadKeys;
std::vector<String> allKeys;
std::vector<String> preallKeys;

void printDownloadKeys() {
  Serial.println("Download Keys:");
  for (const auto& key : downloadKeys) {
      Serial.print(key);
  }
  Serial.println("");
}

void reboot()
{
  ESP.restart();
}

FileType detectFileType(String *appfileName)
{
  if(appfileName->endsWith(".js")){
    return FileType::JS;
  }else if(appfileName->endsWith(".lua")){
    return FileType::LUA;
  }else if(appfileName->endsWith(".bmp")){
    return FileType::BMP;
  }else if(appfileName->endsWith(".png")){
    return FileType::PNG;
  }else if(appfileName->endsWith(".txt")){
    return FileType::TXT;
  }
  return FileType::OTHER;
}

String *targetfileName;
BaseGame* nextGameObject(String* _appfileName, int _gameState, String _mn)
{

  switch(detectFileType(_appfileName)){
    case FileType::JS:  
      // game = new RunJsGame(); 
      break;
    case FileType::LUA: 
      game = new runLuaGame(_gameState, _mn);
      break;
    case FileType::TXT: 
      // game = new RunJsGame(); 
      // //ファイル名がもし/init/param/caldata.txtなら
      // if(*_appfileName == CALIBRATION_FILE)
      // {
      //   ui.calibrationRun(M5Cardputer.Display);//キャリブレーション実行してcaldata.txtファイルを更新して
      //   drawLogo();//サイドボタンを書き直して
      // }
      // appfileName = "/init/txt/main.js";//txtエディタで開く
      break; //txteditorを立ち上げてtxtを開く
    case FileType::BMP: // todo: error
      game = NULL;
      break;
    case FileType::PNG: // todo: error
      // game = new RunJsGame(); 
      // appfileName = "/init/png/main.js";//pngエディタで開く
      break;
    case FileType::OTHER: // todo: error
      game = NULL;
      break;
  }

  return game;

}

// char *A;

uint32_t preTime;
// void setFileName(String s){
//   appfileName = s;
// }

void runFileName(String s){
  
  //ui.setConstantGetF(false);//初期化処理 タッチポイントの常時取得を切る
  
  appfileName = s;
  mode = 1;//exit to run

}


int getcno2tftc(uint8_t _cno){
  switch (_cno)
  {
  case 0:return HACO3_C0;break;
  case 1:return HACO3_C1;break;
  case 2:return HACO3_C2;break;
  case 3:return HACO3_C3;break;
  case 4:return HACO3_C4;break;
  case 5:return HACO3_C5;break;
  case 6:return HACO3_C6;break;
  case 7:return HACO3_C7;break;
  case 8:return HACO3_C8;break;
  case 9:return HACO3_C9;break;
  case 10:return HACO3_C10;break;
  case 11:return HACO3_C11;break;
  case 12:return HACO3_C12;break;
  case 13:return HACO3_C13;break;
  case 14:return HACO3_C14;break;
  case 15:return HACO3_C15;break;

  default:
  return HACO3_C0;
    break;
  }
}


// タイマー
hw_timer_t * timer = NULL;

void readFile(fs::FS &fs, const char * path) {
   File file = fs.open(path);
   while(file.available()) file.read();
  //  while(file.available()) Serial.print(file.read());
}

//ファイル書き込み
void writeFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        return;
    }
    file.print(message);
}

void deleteFile(fs::FS &fs, const char * path){
   Serial.print("Deleting file: ");
   Serial.println(path);
   if(fs.remove(path)) Serial.print("− file deleted\n\r");
   else { Serial.print("− delete failed\n\r"); }
}

void listDir(fs::FS &fs){
   File root = fs.open("/");
   File file = root.openNextFile();
   while(file){
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.print(file.size());
      file = root.openNextFile();
   }
}

String rFirstAppName(String _wrfile){
  File fr = SPIFFS.open(_wrfile.c_str(), "r");// ⑩ファイルを読み込みモードで開く
  String _readStr = fr.readStringUntil('\n');// ⑪改行まで１行読み出し
  fr.close();	// ⑫	ファイルを閉じる
  return _readStr;
}



void getOpenConfig()
{
  File fr;

  fr = SPIFFS.open(SPRBITS_FILE, "r");
  for (int i = 0; i < 128; i++) {
    String _readStr = fr.readStringUntil(','); // ,まで１つ読み出し
    std::string _readstr = _readStr.c_str();

    // 改行を取り除く処理
    const char CR = '\r';
    const char LF = '\n';
    std::string destStr;
    for (std::string::const_iterator it = _readstr.begin(); it != _readstr.end(); ++it) {
      if (*it != CR && *it != LF && *it != '\0') {
        destStr += *it;
      }
    }

    _readstr = destStr;

    uint8_t bdata = 0b00000000;
    uint8_t bitfilter = 0b10000000; // 書き換え対象ビット指定用

    for (int j = 0; j < _readstr.length(); ++j) {
        char ch = _readstr[j];
        // Serial.print(ch);
        if (ch == '1') {
            bdata |= bitfilter; // 状態を重ね合わせて合成
        }
        bitfilter = bitfilter >> 1; // 書き換え対象ビットを一つずらす
    }

    sprbits[i] = bdata;

  }
  fr.close();

  fr = SPIFFS.open("/init/param/openconfig.txt", "r");
  String line;
  while (fr.available()) {
    line = fr.readStringUntil('\n');
    if (!line.isEmpty()) {
      int commaIndex = line.indexOf(',');
        String val = line.substring(0, commaIndex);
        if(val != NULL){
          appfileName =  val;

          // Serial.print(appfileName.c_str());
          // Serial.println("<-");

        }else {
          appfileName = "/init/main.lua";//configファイルが壊れていても強制的に値を入れて立ち上げる
        }
          int nextCommaIndex = line.indexOf(',', commaIndex + 1);//一つ先のカンマ区切りの値に進める
          if (nextCommaIndex != -1) {
            val = line.substring(commaIndex + 1, nextCommaIndex);
            if(val.toInt() != NULL){
              isEditMode = val.toInt();
              Serial.print("editmode[");Serial.print(isEditMode);Serial.println("]");
            }else{
              isEditMode = 0;//configファイルが壊れていても強制的に値を入れて立ち上げる
            }
          }
    }
  }
  fr.close();

  Serial.print(appfileName.c_str());
  Serial.println("<---");

  string str1 = appfileName.c_str();
  int i=0;
  char delimiter = '/';

  std::vector<std::string> result = split(str1, delimiter);

    // 分割結果の表示
    for (const std::string& s : result) {
        if(i==1){
        appNameStr = s.c_str();

        // Serial.print("/" + appNameStr + "/mapinfo.txt");
        // Serial.println("<-------");

        fr = SPIFFS.open("/" + appNameStr + "/mapinfo.txt", "r");// ⑩ファイルを読み込みモードで開く
      }
      i++;
    }
      
  // アプリで使うマップ名を取得する
  String _readStr;
  while (fr.available()) {
      String line = fr.readStringUntil('\n');
      int j = 0; // 列のインデックス
      int startIndex = 0;

      if (!line.isEmpty()) {
          // Serial.print(line);
          // Serial.println("<--------");

          while (j < 16) {
              int commaIndex = line.indexOf(',', startIndex);

              // if (j < 16) { // 0から15番目まで
                  if (commaIndex != -1) {
                      String columnValue = line.substring(startIndex, commaIndex);
                      mapsprnos[j] = atoi(columnValue.c_str());
                  } else {
                      // 行の末尾まで達した場合
                      mapsprnos[j] = atoi(line.substring(startIndex).c_str());
                  }
              if (commaIndex == -1) {
                  // 行の末尾まで達した場合
                  break;
              }

              startIndex = commaIndex + 1;
              j++;
          }

          i++;
      }
  }
  fr.close();
  mapFileName = "/init/param/map/"+_readStr;
}

void setOpenConfig(String fileName, uint8_t _isEditMode) {
  char numStr[64];//64文字まで
  sprintf(numStr, "%s,%d,", 
    fileName.c_str(), _isEditMode
  );

  String writeStr = numStr;  // 書き込み文字列を設定
  File fw = SPIFFS.open("/init/param/openconfig.txt", "w"); // ファイルを書き込みモードで開く
  fw.println(writeStr);  // ファイルに書き込み
  // savedAppfileName = fileName;
  delay(50);
  fw.close(); // ファイルを閉じる
}

int readMap()
{
  mapready = false;

  for(int n = 0; n<divnum; n++)
  {
    // spriteMap.drawPngFile( SPIFFS, mapFileName, 0, (int32_t)(-MAPWH*n/divnum) );

  auto file = SPIFFS.open(mapFileName.c_str(), "r");
  file.read(gIMGBuf2, 32768);
  file.close();

  spriteMap.drawPng(gIMGBuf2,32768, 0, (int32_t)(-MAPWH*n/divnum));
    
    for(int32_t j = 0; j<MAPWH/divnum; j++){
      for(int32_t i = 0; i<MAPWH; i++){

        int k = j+(MAPWH/divnum)*(n);//マップ下部
        colValR = uint8_t(spriteMap.readPixelRGB(i,j).R8());
        colValG = uint8_t(spriteMap.readPixelRGB(i,j).G8());
        colValB = uint8_t(spriteMap.readPixelRGB(i,j).B8());

  //16ビットRGB（24ビットRGB）
        if(colValR==0&&colValG==0&&colValB==0){//0: 黒色=なし
          mapArray[i][k] = mapsprnos[0];//20;
        }else if(colValR==24&&colValG==40&&colValB==82){//{ 27,42,86 },//1: 暗い青色
          mapArray[i][k] = mapsprnos[1];//11;//5*8+5;
        }else if(colValR==140&&colValG==24&&colValB==82){//{ 137,24,84 },//2: 暗い紫色
          mapArray[i][k] = mapsprnos[2];//32;//5*8+5;
        }else if(colValR==0&&colValG==138&&colValB==74){//{ 0,139,75 },//3: 暗い緑色
          mapArray[i][k] = mapsprnos[3];//44;//5*8+5;
        }else if(colValR==181&&colValG==77&&colValB==41){//{ 183,76,45 },//4: 茶色 
          mapArray[i][k] = mapsprnos[4];//53;//5*8+5;
        }else if(colValR==99&&colValG==85&&colValB==74){//{ 97,87,78 },//5: 暗い灰色
          mapArray[i][k] = mapsprnos[5];//49;
        }else if(colValR==198&&colValG==195&&colValB==198){//{ 194,195,199 },//6: 明るい灰色
          mapArray[i][k] = mapsprnos[6];//54;//5*8+5;
        }else if(colValR==255&&colValG==243&&colValB==231){//{ 255,241,231 },//7: 白色
          mapArray[i][k] = mapsprnos[7];//32;
        }else if(colValR==255&&colValG==0&&colValB==66){//{ 255,0,70 },//8: 赤色
          mapArray[i][k] = mapsprnos[8];//52;
        }else if(colValR==255&&colValG==162&&colValB==0){//{ 255,160,0 },//9: オレンジ
          mapArray[i][k] = mapsprnos[9];//41;//5*8+5;
        }else if(colValR==255&&colValG==239&&colValB==0){//{ 255,238,0 },//10: 黄色
          mapArray[i][k] = mapsprnos[10];//46;
        }else if(colValR==0&&colValG==235&&colValB==0){//{ 0,234,0 },//11: 緑色
          mapArray[i][k] = mapsprnos[11];//42;
        }else if(colValR==0&&colValG==174&&colValB==255){//{ 0,173,255 },//12: 水色
          mapArray[i][k] = mapsprnos[12];//45;//5*8+5;
        }else if(colValR==132&&colValG==117&&colValB==156){//{ 134,116,159 },//13: 藍色
          mapArray[i][k] = mapsprnos[13];//50;
        }else if(colValR==255&&colValG==105&&colValB==173){//{ 255,107,169 },//14: ピンク
          mapArray[i][k] = mapsprnos[14];//43;//5*8+5;
        }else if(colValR==255&&colValG==203&&colValB==165){//{ 255,202,165}//15: 桃色
          mapArray[i][k] = mapsprnos[15];//38;//5*8+5;
        }
        if(i==MAPWH-1 && k==MAPWH-1){mapready = true;return 1;}//読み込み終わったら1をリターン
      }
    }
  }
  // spriteMap.deleteSprite();//メモリに格納したら解放する
  return 1;
}

using namespace std;
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <cmath>

// vector<string> split(string& input, char delimiter)
// {
//     istringstream stream(input);
//     string field;
//     vector<string> result;
//     while (getline(stream, field, delimiter)) {
//         result.push_back(field);
//     }
//     return result;
// }

// void setTFTedit(int _iseditmode){

//     tft.setPsram( false );//DMA利用のためPSRAMは切る
//     tft.createSprite( TFT_WIDTH, TFT_HEIGHT );//PSRAMを使わないギリギリ
//     tft.startWrite();//CSアサート開始

// }

// int btn(int btnno){
//   return buttonState[btnno];//ボタンの個数未定
// }

void reboot(String _fileName, uint8_t _isEditMode)
{
  setOpenConfig(_fileName, _isEditMode);
  editor.setCursorConfig();//カーソルの位置を保存
  delay(100);
  ESP.restart();
}

void restart(String _fileName, int _isEditMode)
{
  setOpenConfig(_fileName, _isEditMode);
  
  editor.setCursorConfig();//カーソルの位置を保存
  delay(100);

  firstBootF = false;
  setup();

  // tunes.pause();
  game->pause();
  free(game);
  firstLoopF = true;
  toneflag = false;
  sfxflag = false;
  musicflag = false;
  // txtName = _fileName;
  game = nextGameObject(&_fileName, gameState, mapFileName);//ファイルの種類を判別して適したゲームオブジェクトを生成
  game->init();//resume()（再開処理）を呼び出し、ゲームで利用する関数などを準備
  // tunes.resume();
}


// void broadchat() {
//   if ("/init/chat/m.txt" == NULL) return;
//   File fp = SPIFFS.open("/init/chat/m.txt", FILE_READ); // SPIFFSからファイルを読み込み

//   if (!fp) {
//     editor.editorSetStatusMessage("Failed to open file");
//     return;
//   }

//   std::vector<uint8_t> data;
//   while (fp.available()) {
//     char c = fp.read();
//     data.push_back(c);
//     if (data.size() >= 150) {
//       esp_err_t result = esp_now_send(slave.peer_addr, data.data(), data.size());
//       tft.println(result);
//       data.clear(); // データを送信したらクリア
//       if (result != ESP_OK) {
//         editor.editorSetStatusMessage("Failed to send message");
//         fp.close();
//         return;
//       }
//     }
//   }

//   // ファイルの残りのデータを送信
//   if (data.size() > 0) {
//     esp_err_t result = esp_now_send(slave.peer_addr, data.data(), data.size());
//     if (result != ESP_OK) {
//       editor.editorSetStatusMessage("Failed to send message");
//       fp.close();
//       return;
//     }
//   }

//   fp.close();
//   editor.editorSetStatusMessage("Message sent");
// }


uint8_t readpixel(int i, int j)
{
        // int k = j+(MAPWH/divnum)*(n);//マップ下部
        colValR = sprite64.readPixelRGB(i,j).R8();
        colValG = sprite64.readPixelRGB(i,j).G8();
        colValB = sprite64.readPixelRGB(i,j).B8();

  //16ビットRGB（24ビットRGB）
        if(colValR==0&&colValG==0&&colValB==0){//0: 黒色=なし
          return 0;//20;
        }else if(colValR==24&&colValG==40&&colValB==82){//{ 27,42,86 },//1: 暗い青色
          return 1;//11;//5*8+5;
        }else if(colValR==140&&colValG==24&&colValB==82){//{ 137,24,84 },//2: 暗い紫色
          return 2;//32;//5*8+5;
        }else if(colValR==0&&colValG==138&&colValB==74){//{ 0,139,75 },//3: 暗い緑色
          return 3;//44;//5*8+5;
        }else if(colValR==181&&colValG==77&&colValB==41){//{ 183,76,45 },//4: 茶色 
          return 4;//53;//5*8+5;
        }else if(colValR==99&&colValG==85&&colValB==74){//{ 97,87,78 },//5: 暗い灰色
          return 5;//49;
        }else if(colValR==198&&colValG==195&&colValB==198){//{ 194,195,199 },//6: 明るい灰色
          return 6;//54;//5*8+5;
        }else if(colValR==255&&colValG==243&&colValB==231){//{ 255,241,231 },//7: 白色
          return 7;//32;
        }else if(colValR==255&&colValG==0&&colValB==66){//{ 255,0,70 },//8: 赤色
          return 8;//52;
        }else if(colValR==255&&colValG==162&&colValB==0){//{ 255,160,0 },//9: オレンジ
          return 9;//41;//5*8+5;
        }else if(colValR==255&&colValG==239&&colValB==0){//{ 255,238,0 },//10: 黄色
          return 10;//46;
        }else if(colValR==0&&colValG==235&&colValB==0){//{ 0,234,0 },//11: 緑色
          return 11;//42;
        }else if(colValR==0&&colValG==174&&colValB==255){//{ 0,173,255 },//12: 水色
          return 12;//45;//5*8+5;
        }else if(colValR==132&&colValG==117&&colValB==156){//{ 134,116,159 },//13: 藍色
          return 13;//50;
        }else if(colValR==255&&colValG==105&&colValB==173){//{ 255,107,169 },//14: ピンク
          return 14;//43;//5*8+5;
        }else if(colValR==255&&colValG==203&&colValB==165){//{ 255,202,165}//15: 桃色
          return 15;//38;//5*8+5;
        }
}


static void tone_up(bool holding)
{
  static int tone_hz;
  if (!holding) { tone_hz = 100; }
  // speaker->tone(++tone_hz, 1000, 1);
}

static void bgm_play_stop(bool holding = false)
{
  // if (holding) { return; }
  // if (speaker->isPlaying(0))
  // {
  //   speaker->stop(0);
  // }
  // else
  // {
  //   speaker->playWav(wav_with_header, sizeof(wav_with_header), ~0u, 0, true);
  // }
}

static void m_volume_up(bool)
{
  // int v = speaker->getVolume() + 1;
  // if (v < 256) { speaker->setVolume(v); }
}

static void m_volume_down(bool)
{
  // int v = speaker->getVolume() - 1;
  // if (v >= 0) { speaker->setVolume(v); }
}

static void c_volume_up(bool)
{
  // int v = speaker->getChannelVolume(0) + 1;
  // if (v < 256) { speaker->setChannelVolume(0, v); }
}

static void c_volume_down(bool)
{
  // int v = speaker->getChannelVolume(0) - 1;
  // if (v >= 0) { speaker->setChannelVolume(0, v); }
}

struct menu_item_t
{
  const char* title;
  void (*func)(bool);
};

// static const menu_item_t menus[] =
// {
//   { "tone"      , tone_up       },
//   { "play/stop" , bgm_play_stop },
//   { "ms vol u"  , m_volume_up   },
//   { "ms vol d"  , m_volume_down },
//   { "ch vol u"  , c_volume_up   },
//   { "ch vol d"  , c_volume_down },
// };
// const uint8_t menu_count = sizeof(menus) / sizeof(menus[0]);

uint8_t cursor_index = 0;


void safeReboot(){
  editor.setCursorConfig(0,0,0);//カーソルの位置を強制リセット保存
      delay(50);

      //ui.setConstantGetF(false);//初期化処理 タッチポイントの常時取得を切る
      appfileName = "/init/main.lua";
      
      firstLoopF = true;
      toneflag = false;
      sfxflag = false;
      musicflag = false;

      editor.editorSave(SPIFFS);//SPIFFSに保存
      delay(100);//ちょっと待つ
      reboot(appfileName, TFT_RUN_MODE);//現状rebootしないと初期化が完全にできない
}


void setup()
{

  auto cfg = M5.config();
  // M5.begin(cfg);
  M5Cardputer.begin(cfg);
  
  int textsize = M5Cardputer.Display.height() / 60;
  if (textsize == 0) {
      textsize = 1;
  }
  M5Cardputer.Display.setTextSize(textsize);

  // pinMode(OUTPIN_0, OUTPUT);
  // pinMode(INPIN_0, INPUT);
  
  ui.begin( M5Cardputer.Display, 16, 1);
  Serial.begin(115200);
  // Serial.begin(9600);
  // keyboard.begin(KEYBOARD_DATA, KEYBOARD_CLK);

  editor.getCursorConfig("/init/param/editor.txt");//エディタカーソルの位置をよみこむ

  delay(50);

  if(firstBootF == true){
    difffileF = false;

    #if !defined(__MIPSEL__)
      while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
      #endif
      // Serial.println("Keyboard Start");

    if (!SPIFFS.begin(true))
    {
      // Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  }
  
  readIMG();

  getOpenConfig();//最初に立ち上げるゲームのパスとモードをSPIFFSのファイルopenconfig.txtから読み込む

  // if(firstBootF == false){
  //   tft.deleteSprite();
  //   delay(100);
  // }

  // setTFTedit(TFT_RUN_MODE);
  tft.setPsram( false );//DMA利用のためPSRAMは切る
  tft.createSprite( TFT_WIDTH, TFT_HEIGHT );//PSRAMを使わないギリギリ
  tft.startWrite();//CSアサート開始

  sprite88_0.setPsram(false );
  sprite88_0.setColorDepth(16);//子スプライトの色深度
  sprite88_0.createSprite(8, 8);//ゲーム画面用スプライトメモリ確保

  // auto file = SPIFFS.open(mapFileName.c_str(), "r");
  // file.read(gIMGBuf88_0, 32);
  // file.close();

  sprite88_0.drawPng(gIMGBuf88_0, 32,0,0);

  sprite64.setPsram(false );
  sprite64.setColorDepth(16);//子スプライトの色深度
  sprite64.createSprite(PNG_SPRITE_WIDTH, PNG_SPRITE_HEIGHT);//ゲーム画面用スプライトメモリ確保//wroomだと64*128だとメモリオーバーしちゃう問題を色番号配列にして回避した

  // sprite64.drawPngFile(SPIFFS, "/init/initspr.png", 0, 0);//一時展開する
  // sprite64.drawBmpFile(SPIFFS, "/init/initspr.bmp", 0, 0);//一時展開する
  sprite64.drawPng(gIMGBuf64, 8192,0,0);

  sprite64cnos_vector.clear();//初期化処理

  for(int y = 0; y < PNG_SPRITE_HEIGHT; y++) {
      for(int x = 0; x < PNG_SPRITE_WIDTH; x++) {
        if(x%2 == 0){
          uint8_t pixel_data = (readpixel(x, y) << 4) | (readpixel(x + 1, y) & 0b00001111);
          sprite64cnos_vector.push_back(pixel_data);
        }
      }
  }

  //破棄
  // sprite64.deleteSprite();

  //psram使えない-------------------------------------------

  sprite88_roi.setPsram(false );
  sprite88_roi.setColorDepth(16);//子スプライトの色深度
  sprite88_roi.createSprite(8, 8);//ゲーム画面用スプライトメモリ確保

  sprite11_roi.setPsram(false );
  sprite11_roi.setColorDepth(16);//子スプライトの色深度
  sprite11_roi.createSprite(1, 1);//ゲーム画面用スプライトメモリ確保

  spriteMap.setPsram(false );
  spriteMap.setColorDepth(16);//子スプライトの色深度
  spriteMap.createSprite(MAPW, MAPH/divnum);//マップ展開用スプライトメモリ確保

  //gameオブジェクトが立ち上がる前にinitの準備しないとフォントが変わる
  editor.initEditor(tft);

  // if(firstBootF == true)
  // {
  // appfileName = "/min/main.lua";
  // isEditMode = 0;
  // mapFileName = "/init/map/0.png";
  readMap();
  delay(50);
  game = nextGameObject(&appfileName, gameState, mapFileName);//ホームゲームを立ち上げる（オブジェクト生成している）
  game->init();//（オブジェクト生成している）
  // tunes.init();//（オブジェクト生成している）
  // }

  frame=0;

  editor.readFile(SPIFFS, appfileName.c_str());
  editor.editorOpen(SPIFFS, appfileName.c_str());
  editor.editorSetStatusMessage("Press ESCAPE to save file");



  

  savedAppfileName = appfileName;//起動したゲームのパスを取得しておく
  firstBootF = false;


  //キー取得
  //luaプログラムがバグで起動不能になった場合、ESCを押しながらリセットをかけると、メニューに戻れるようにする
  //ESCボタンで強制終了

  // if (keyboard.available()) {
  //   keychar = keyboard.read();
  //   if (keychar == PS2_ESC) {
  //     safeReboot();
  //   }
  // }

  
  
}

void loop()
{

  M5Cardputer.update();

  if (M5Cardputer.Keyboard.isPressed() == 0) {
      keychar = NULL;
      if(pressedBtnID != -1){buttonState[pressedBtnID] = -1;}
        pressedBtnID = -1;//リセット   
  }
  
  
  if (M5Cardputer.Keyboard.isChange()) {
    
    if (M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();//キー状態を取得しておく

        if (status.fn) {
          textMoveF=!textMoveF;//スイッチング
        }

        if (status.shift) {
          shiftF=!shiftF;//スイッチング
        }

        if (status.enter) {
          pressedBtnID = 6;
        }
        

        for (auto i : status.word) {
          Point2D_t keyPos;
          keyPos = M5Cardputer.Keyboard.keyList().at(0);//直前に押されたキーのxy要素を取り出す
          
          // keychar = M5Cardputer.Keyboard.getKeyValue(keyPos).value_first;

          if(!shiftF){
            keychar = M5Cardputer.Keyboard.getKeyValue(keyPos).value_first;//対応する文字コードを格納
          }
          else if(shiftF){
            keychar = M5Cardputer.Keyboard.getKeyValue(keyPos).value_second;//対応する文字コードを格納
          }

        
        // if (status.fn&&keychar == PS2_LEFTARROW) {keychar = PS2_LEFTARROW;editor.editorProcessKeypress(keychar, SPIFFS);}
        // if (status.fn&&keychar == PS2_DOWNARROW) {keychar = PS2_DOWNARROW;editor.editorProcessKeypress(keychar, SPIFFS);}
        // if (status.fn&&keychar == PS2_UPARROW) {keychar = PS2_UPARROW;editor.editorProcessKeypress(keychar, SPIFFS);}

        if (isEditMode == TFT_RUN_MODE) {
        
        if (M5Cardputer.Keyboard.isKeyPressed('~')) {
        pressedBtnID = 0;
      } else if (M5Cardputer.Keyboard.isKeyPressed(',')) {
        pressedBtnID = 1;
      } else if (M5Cardputer.Keyboard.isKeyPressed('/')) {
        pressedBtnID = 2;
      } else if (M5Cardputer.Keyboard.isKeyPressed(';')) {
        pressedBtnID = 3;
      } else if (M5Cardputer.Keyboard.isKeyPressed('.')) {
        pressedBtnID = 4;
      }else if (M5Cardputer.Keyboard.isKeyPressed('<')) {
        // keychar = PS2_LEFTARROW;editor.editorProcessKeypress(keychar,SPIFFS);
        pressedBtnID = 7;
      } else if (M5Cardputer.Keyboard.isKeyPressed('?')) {
        // keychar = PS2_RIGHTARROW;editor.editorProcessKeypress(keychar,SPIFFS);
        pressedBtnID = 8;
      } else if (M5Cardputer.Keyboard.isKeyPressed(':')) {
        // keychar = PS2_UPARROW;editor.editorProcessKeypress(keychar,SPIFFS);
        pressedBtnID = 5;
      } else if (M5Cardputer.Keyboard.isKeyPressed('>')) {
        // keychar = PS2_DOWNARROW;editor.editorProcessKeypress(keychar,SPIFFS);
        pressedBtnID = 6;
      } else if (M5Cardputer.Keyboard.isKeyPressed('|')) {
        pressedBtnID = 9;
      } else {
        
        //通常の文字
        if(keychar != -1){
          editor.editorProcessKeypress(keychar, SPIFFS);
          // Serial.println(keychar);
          }
          // Serial.println(keychar);
        }

      }else if (isEditMode == TFT_EDIT_MODE) {

        
        if(textMoveF){
          
             if (M5Cardputer.Keyboard.isKeyPressed(',')) {
            keychar = PS2_LEFTARROW;editor.editorMoveCursor(keychar);
          } else if (M5Cardputer.Keyboard.isKeyPressed('/')) {
            keychar = PS2_RIGHTARROW;editor.editorMoveCursor(keychar);
          } else if (M5Cardputer.Keyboard.isKeyPressed(';')) {
            keychar = PS2_UPARROW;editor.editorMoveCursor(keychar);
          } else if (M5Cardputer.Keyboard.isKeyPressed('.')) {
            keychar = PS2_DOWNARROW;editor.editorMoveCursor(keychar);
          } else if (M5Cardputer.Keyboard.isKeyPressed('`')) {
            pressedBtnID = 0;
          } 
        }else{

          if (M5Cardputer.Keyboard.isKeyPressed('~')) {
            pressedBtnID = 0;
          } else if (M5Cardputer.Keyboard.isKeyPressed('|')) {
            pressedBtnID = 9;
          } else {
            
            //通常の文字
            if(keychar != -1){
              editor.editorProcessKeypress(keychar, SPIFFS);
              // Serial.println(keychar);
              }
              // Serial.println(keychar);
            }
        }
      
        }

      }

      if (status.enter) {
        keychar = PS2_ENTER;
        editor.editorProcessKeypress(keychar, SPIFFS);
      }

      if (status.del) {
        keychar = PS2_DELETE;
        editor.editorProcessKeypress(keychar, SPIFFS);
      }      
    }
  }

  // editor.update(tft, SPIFFS, SD, keychar);

  if(pressedBtnID != -1){
    // editor.update(tft, SPIFFS, keychar);
  }
  
  // if(pressedBtnID == -1){
  //   buttonState[pressedBtnID] = -1;
  // }else if(pressedBtnID>=0&&pressedBtnID<=6){//押されたものだけの値をあげる
  //   buttonState[pressedBtnID]++;
  // }
  
  // buttonState[pressedBtnID]++;

  // // // for(int i = 0; i < buttonState.size(); i ++){
  // for(int i = 0; i < 7; i ++){
  //   if(pressedBtnID == -1){
  //     buttonState[i] = 0;
  //   }else if(pressedBtnID == i){//押されたものだけの値をあげる
  //     buttonState[i] ++;
  //   }
  // }
  
  uint32_t now = millis();
  uint32_t remainTime= (now - preTime);
  preTime = now;

  if( isEditMode == TFT_RUN_MODE ){
    //ゲーム内のprint時の文字設定をしておく
    tft.setTextSize(1);//サイズ
    tft.setFont(&lgfxJapanGothicP_8);//日本語可
    tft.setCursor(0, 0);//位置
    tft.setTextWrap(true);

    // == tune task ==
    // tunes.run();

    // == game task ==
    mode = game->run(remainTime);//exitは1が返ってくる　mode=１ 次のゲームを起動

    if(pressedBtnID == 9){//(|)
    // appfileName = 
      editor.editorSave(SPIFFS);//SPIFFSに保存
      delay(100);//ちょっと待つ
      reboot(appfileName, TFT_EDIT_MODE);//現状rebootしないと初期化が完全にできない
      // restart(appfileName, 0);//初期化がうまくできない（スプライトなど）
      // broadchat();//ファイルの中身をブロードキャスト送信する（ファイルは消えない）
    }


    //ESCボタンで強制終了
    if (pressedBtnID == 0)
    { // reload

      // editor.setCursorConfig(0,0,0);//カーソルの位置を強制リセット保存
      // delay(50);

      appfileName = "/init/main.lua";
      
      firstLoopF = true;
      toneflag = false;
      sfxflag = false;
      musicflag = false;

      mode = 1;//exit
    }

    if (pressedBtnID == 9999)
    { // reload
      mode = 1;//exit
      pressedBtnID = -1;
    }

    if(mode != 0){ // exit request//次のゲームを立ち上げるフラグ値、「modeが１＝次のゲームを起動」であれば
      // tunes.pause();
      game->pause();
      // ui.clearAddBtns();//個別のゲーム内で追加したタッチボタンを消去する
      free(game);
      firstLoopF = true;
      toneflag = false;
      sfxflag = false;
      musicflag = false;
      // txtName = appfileName;
      game = nextGameObject(&appfileName, gameState, mapFileName);//ファイルの種類を判別して適したゲームオブジェクトを生成
      game->init();//resume()（再開処理）を呼び出し、ゲームで利用する関数などを準備
      // tunes.resume();
    }

    // ui.showTouchEventInfo( tft, 0, 100 );//タッチイベントを視覚化する
    // ui.showInfo( tft, 0, 100+8 );//ボタン情報、フレームレート情報などを表示します。

    if(enemyF){

      sprite64.setPsram(false);
      sprite64.setColorDepth(16);    // 子スプライトの色深度
      sprite64.createSprite(48, 48); // ゲーム画面用スプライトメモリ確保

      // sprite64.drawPngFile(SPIFFS, enemyPath, enemyX, enemyY);//sprite64に展開する
      sprite64.pushRotateZoom(&tft, enemyX, enemyY, 0, 1, 1, gethaco3Col(enemyTransCn));

      sprite64.deleteSprite();//消す

      // tft.drawPngFile(SPIFFS, enemyPath, enemyX, enemyY);//直接展開する
    }

      if(toolNo != 0){
        if(toolNo==1){//カラーパレット
          for(int j = 0; j<8; j++){
            for(int i = 0; i<2; i++){
              M5Cardputer.Display.fillRect(i*16,j*16,16,16,gethaco3Col(j*2+i));
            }
          }
        }

        toolNo = 0;
      }

     //最終出力
    tft.setPivot(0, 0);
    tft.pushRotateZoom(&M5Cardputer.Display, 40, 3, 0, 1, 1);

    if(pressedBtnID == 5){//PAGEUP(:)//キーボードからエディタ再起動
      // restart(appfileName, 1);//appmodeでリスタートかけるので、いらないかも
    }
  }
  else if(isEditMode == TFT_EDIT_MODE)
  {
    
    editor.editorRefreshScreen(tft);

    float codeunit = 128.0/float(editor.getNumRows());
    float codelen = codeunit * 14;//14は表示行数
    // int codelen = int(codelen_f + 0.5); // 四捨五入して整数に変換する
    
    float curpos = codeunit*editor.getCy();
    float codepos = codeunit * (editor.getCy() - editor.getScreenRow());
    // int codepos = int(codepos_f + 0.5); // 四捨五入して整数に変換する
    
    if(!textMoveF)
    tft.fillRect(156,0, 4,128, HACO3_C5);//コードの全体の長さを表示
    else
    tft.fillRect(156,0, 4,128, HACO3_C9);//コードの全体の長さを表示

    if(!shiftF)
    tft.fillRect(156,int(codepos), 4,codelen, HACO3_C6);//コードの位置と範囲を表示
    else
    tft.fillRect(156,int(codepos), 4,codelen, HACO3_C12);//コードの位置と範囲を表示

    if(curpos>=int(codepos)+codelen)//すこしはみ出たら表示コード内に入れる
    {
      if(codeunit>=1)curpos = int(codepos)+codelen - codeunit;
      else curpos = int(codepos)+codelen - 1;
    }

    if(codeunit>=1){tft.fillRect(155, int(curpos), 4, codeunit, HACO3_C8);}//コードの位置と範囲を表示
    else{tft.fillRect(155, int(curpos), 4, 1, HACO3_C8);}//１ピクセル未満の時は見えなくなるので１に
    
     //最終出力
    tft.setPivot(0, 0);
    tft.pushRotateZoom(&M5Cardputer.Display, 40, 3, 0, 1, 1);
    
    if(pressedBtnID == 0)//(|)メニュー画面へ
    {
      editor.setCursorConfig(0,0,0);//カーソルの位置を保存
      delay(50);
      restart("/init/main.lua", TFT_RUN_MODE);
    }

    if(pressedBtnID == 9){//(|)
    // appfileName = 
      editor.editorSave(SPIFFS);//SPIFFSに保存
      delay(100);//ちょっと待つ
      reboot(appfileName, TFT_RUN_MODE);//現状rebootしないと初期化が完全にできない
      // restart(appfileName, 0);//初期化がうまくできない（スプライトなど）
      // broadchat();//ファイルの中身をブロードキャスト送信する（ファイルは消えない）
    }

  }

  // int x      = rand() % M5Cardputer.Display.width();
  // int y      = rand() % M5Cardputer.Display.height();
  // int r      = (M5Cardputer.Display.width() >> 4) + 2;
  // uint16_t c = rand();
  // M5Cardputer.Display.fillCircle(x, y, r, c);
  // draw_function(&M5Cardputer.Display);

 

  frame++;
  if(frame > 18446744073709551615)frame = 0;
  delay(1);
  
}
