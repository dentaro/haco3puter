using namespace std;
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <cmath>
#include "baseGame.h"
#include <chrono>
#include <time.h>
#include <fstream>
#include "runLuaGame.h"
#include <sstream>
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Unified.h>
#include "Editor.h"
#include "Channel.hpp"
// #include <esp_now.h>
// #include <WiFi.h>
#include <M5GFX_DentaroUI.hpp>
#include <map>
#include "M5Cardputer.h"

#define SOUND_OFF_STATE 0
#define SOUND_ON_STATE  1

bool sfxflag = false;
uint8_t sfxNo = 0;
uint8_t wavNo = 0;
uint8_t sfxChNo = 0;
uint8_t sfxVol = 0;
float sfxspeed = 100;

uint8_t loopStart = 0;
uint8_t loopEnd = 63;
uint8_t looplen = (loopEnd - loopStart)+1;
float bpm = 120;

int8_t soundState = SOUND_OFF_STATE;
uint8_t musicNo = 255;
// uint8_t musicSpeed = 64;
Channel* channels = new Channel();
// // //音関連
uint8_t buffAreaNo = 0;
uint8_t gEfectNo = 0;
uint8_t effectVal = 0.0f;
uint8_t toneTickNo = 0;
uint8_t sfxTickNo = 0;
uint8_t instrument = 0;
uint8_t targetChannelNo = 0;//描画編集する効果音番号を設定（sfx(n)のnで効果音番号を指定することで作った効果音がなる）
uint8_t tickTime = 125;//125ms*8chはbpm60
uint8_t tickSpeed = 5;//連動してない
uint8_t patternNo = 0;//0~63

TaskHandle_t taskHandle[2];
SemaphoreHandle_t syncSemaphore;
SemaphoreHandle_t nextFrameSemaphore;

bool textMoveF = false;
bool shiftF = false;

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
#define TFT_SOUNDEDIT_MODE 3

int gameState = 0;
uint8_t toolNo = 0;

uint8_t masterVol = 0;

String data = "> ";

//星のデータ用
std::vector<std::array<float, 2>> bsParamFloat;
std::vector<std::array<uint8_t, 1>> bsParamInt8t;
std::vector<std::array<uint16_t, 1>> bsParamInt16t;
//スプライト用PNGの一時確保用
// const int G_IMGBUF88_0_SIZE = 32;
// const int G_IMGBUF64_SIZE = 8192;
// const int G_IMGBUF2_SIZE = 32768;
// std::vector<uint8_t> gIMGBuf88_0(G_IMGBUF88_0_SIZE);
// std::vector<uint8_t> gIMGBuf64(G_IMGBUF64_SIZE);
// std::vector<uint8_t> gIMGBuf2(G_IMGBUF2_SIZE);


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

uint64_t frame = 0;

bool sndbufferedF = false;
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

int fps=60;//デフォルト

using namespace std;

#define MAX_CHAR 512
#define FORMAT_SPIFFS_IF_FAILED true

#define BUF_PNG_NUM 0

#define CH1_VIEW_MODE 1
#define CH8_VIEW_MODE 8

uint8_t mainVol = 180;

uint8_t ch_viewmode = CH8_VIEW_MODE;

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
int divnum = 8;
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
int8_t soundNo = -1;
float soundSpeed = 1.0;
bool musicflag = false;
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

/** 画像をSPIFFSから読み込む */
// void readIMG() {
//   auto file = SPIFFS.open("/init/initspr.png", "r");
//   file.read(gIMGBuf64, 8192);
//   file.close();
// }

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
    uint8_t* gIMGBuf2 = new uint8_t[4096];

    auto file = SPIFFS.open(mapFileName.c_str(), "r");
    file.read(gIMGBuf2, 32768/divnum);
    file.close();

    spriteMap.drawPng(gIMGBuf2, 32768/divnum, 0, (int32_t)(-MAPWH * n / divnum));

    delete[] gIMGBuf2;

  // auto file = SPIFFS.open(mapFileName.c_str(), "r");
  // file.read(gIMGBuf2, 32768);
  // file.close();

  // spriteMap.drawPng(gIMGBuf2,32768, 0, (int32_t)(-MAPWH*n/divnum));
    
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

void showMusicInfo(){
    tft.setCursor(128,1);
    if(patternNo<10)tft.print("0");
    tft.print(patternNo);

    tft.setCursor(148,1);
    // tft.print("SP");
    if(tickSpeed<10)tft.print("0");
    tft.print(tickSpeed);
    
tft.drawLine(128, 9, 160, 9, gethaco3Col(6));

    // tft.fillRect(128, 10, 32, 20, gethaco3Col(13));
    tft.setCursor(128,11);
    for(int n = 0;n<4;n++){tft.print(channels->patterns[patternNo][n]);}
    tft.setCursor(128,21);
    for(int n = 0;n<4;n++){tft.print(channels->patterns[patternNo][4+n]);}

tft.drawLine(128,29, 160,29, gethaco3Col(6));

    // tft.fillRect(138, 30, 32, 40, gethaco3Col(2));
    tft.setCursor(138,31);
    tft.print("IN");
    tft.print(instrument);
    tft.fillRect(128, 30, 10, 9, gethaco3Col(instrument+4));

tft.drawLine(128, 39, 160, 39, gethaco3Col(6));

    tft.setCursor(138,41);
    tft.print(" ");
    // tft.print(instrument);
    // tft.fillRect(128, 40, 10, 9, gethaco3Col(instrument+8));

tft.drawLine(128, 49, 160, 49, gethaco3Col(6));

    tft.setCursor(138,51);
    tft.print("CH");
    tft.print(targetChannelNo);
    tft.fillRect(128, 50, 10, 9, gethaco3Col(targetChannelNo));

tft.drawLine(128, 59, 160,59, gethaco3Col(6));

    tft.setCursor(138,61);
    tft.print("EF");
    tft.print(gEfectNo);
    tft.fillRect(128, 60, 10, 9, gethaco3Col(gEfectNo));

tft.drawLine(128, 69, 160,69, gethaco3Col(6));
    int vw=2;
    int vh=1;
    for(int vx=0; vx<16; vx++){
      if(masterVol > 16*vx){
        tft.fillRect(128+vx*vw, 71+(7-vx/2), vw, vx/2, gethaco3Col(8));
      }
    }
    tft.setCursor(128, 71);
    tft.print(masterVol);
tft.drawLine(128, 79, 160,79, gethaco3Col(6));

}


void controlMusicVisual(){
      for(int i = 0; i<32; i++)//32音書き換える
      {
        uint8_t efn = channels->notedata[targetChannelNo][i].effectNo;
        uint8_t ins = channels->notedata[targetChannelNo][i].instrument;
        uint8_t vol = channels->notedata[targetChannelNo][i].volume;
        uint8_t oct = channels->notedata[targetChannelNo][i].octave;
        uint8_t pit = channels->notedata[targetChannelNo][i].pitch;

        // if (ui.getEvent() != NO_EVENT)
        // {
        //   if((ui.getPos().x-40)>>2==i){//押した1音だけの音色を変える
        //     if(96>ui.getPos().y && 0<=ui.getPos().y){ //上の領域で
        //       //音程を変える
        //       pit = (96 - ui.getPos().y)>>2 % 12;
        //       oct = 4;//floor((96 - ui.getPos().y)>>2 / 12)+3;
        //       //音色を変える
        //       ins = instrument;
        //       //エフェクトを変える
        //       efn = gEfectNo;
        //     }
            
        //     if(128>ui.getPos().y && 96<=ui.getPos().y){//下の領域で
        //       //音量を変える
        //       vol = (124 - ui.getPos().y)>>2 % 8;
        //     }
        //     buffAreaNo = patternNo%2;
        //     channels->resetTones(i, targetChannelNo, pit, oct, vol, ins, efn, buffAreaNo);//ピッチ,オクターブなどトーンを置き換える
        //   }
        // }

        if(toneTickNo == i){

          tft.fillRect(i*4, 92-(pit + (oct-4)*12)*4, 4,4, gethaco3Col(10));
          tft.fillRect(i*4, 124-(vol)*4, 4,4, gethaco3Col(10));

        }else{
          if(efn!=0)tft.drawLine(i*4, 92-(pit + (oct-4)*12)*4+3, i*4+3, 92-(pit + (oct-4)*12)*4+3, gethaco3Col(efn+8));

          tft.fillRect(i*4, 92-(pit + (oct-4)*12)*4, 4,3, gethaco3Col(ins+4));
          tft.fillRect(i*4, 124-(vol)*4, 4,4, gethaco3Col(3));
        }

      }
}

void showMusicVisual(){//操作できないタイプ
    for(int i = 0; i<32; i++)//32音書き換える
    {
      uint8_t bufNo = patternNo%2;
      uint8_t efn = channels->notedata[targetChannelNo][i+bufNo*32].effectNo;
      uint8_t ins = channels->notedata[targetChannelNo][i+bufNo*32].instrument;
      uint8_t vol = channels->notedata[targetChannelNo][i+bufNo*32].volume;
      uint8_t oct = channels->notedata[targetChannelNo][i+bufNo*32].octave;
      uint8_t pit = channels->notedata[targetChannelNo][i+bufNo*32].pitch;

      if(toneTickNo == i){
        tft.fillRect(i*4, 92-(pit + (oct-4)*12)*4, 4,4, gethaco3Col(10));
        tft.fillRect(i*4, 124-(vol)*4, 4,4, gethaco3Col(10));
      }else{
        if(efn!=0)tft.drawLine(i*4, 92-(pit + (oct-4)*12)*4+3, i*4+3, 92-(pit + (oct-4)*12)*4+3, gethaco3Col(efn+8));

        tft.fillRect(i*4, 92-(pit + (oct-4)*12)*4, 4,3, gethaco3Col(ins+4));
        tft.fillRect(i*4, 124-(vol)*4, 4,4, gethaco3Col(3));
      }
    }
}


void showMusicVisual8ch(){//操作できないタイプ

for(uint8_t chx = 0; chx<2; chx++)
{
  for(uint8_t chy = 0; chy<4; chy++)
  {
    if(chx*4+chy == targetChannelNo){
      tft.drawRect(chx*64, chy*32, 64,32, gethaco3Col(8));
    }

    //バッファ表示
    for(int i = 0; i<32; i++)//32音書き換える
    {
      uint8_t bufNo = (patternNo+1)%2;
      uint8_t efn = channels->notedata[chx*4+chy][i+bufNo*32].effectNo;
      uint8_t ins = channels->notedata[chx*4+chy][i+bufNo*32].instrument;
      uint8_t vol = channels->notedata[chx*4+chy][i+bufNo*32].volume;
      uint8_t oct = channels->notedata[chx*4+chy][i+bufNo*32].octave;
      uint8_t pit = channels->notedata[chx*4+chy][i+bufNo*32].pitch;

      if(efn!=0)
      tft.drawPixel(i*2+(64*chx)+1,(23-(pit + (oct-4)*12))+(32*chy), gethaco3Col(5));

      tft.drawPixel(i*2+(64*chx),  (23-(pit + (oct-4)*12))+(32*chy), gethaco3Col(5));
      tft.drawPixel(i*2+(64*chx),  (31-(vol))+(32*chy), gethaco3Col(5));
      
    }

    //現在の音データ表示
    for(int i = 0; i<32; i++)//32音書き換える
    {
      uint8_t bufNo = patternNo%2;
      uint8_t efn = channels->notedata[chx*4+chy][i+bufNo*32].effectNo;
      uint8_t ins = channels->notedata[chx*4+chy][i+bufNo*32].instrument;
      uint8_t vol = channels->notedata[chx*4+chy][i+bufNo*32].volume;
      uint8_t oct = channels->notedata[chx*4+chy][i+bufNo*32].octave;
      uint8_t pit = channels->notedata[chx*4+chy][i+bufNo*32].pitch;

      if(toneTickNo == i){
        tft.drawPixel(i*2+(64*chx),  (23-(pit + (oct-4)*12))+(32*chy), gethaco3Col(10));
        tft.drawPixel(i*2+(64*chx),  (31-(vol))+(32*chy), gethaco3Col(10));
      }else{
        if(efn!=0)
        tft.drawPixel(i*2+(64*chx)+1,(23-(pit + (oct-4)*12))+(32*chy), gethaco3Col(efn+8));

        tft.drawPixel(i*2+(64*chx),  (23-(pit + (oct-4)*12))+(32*chy), gethaco3Col(ins+4));
        tft.drawPixel(i*2+(64*chx),  (31-(vol))+(32*chy), gethaco3Col(3));
      }
    }
      
    }
  }
}


bool createChannels()
{
uint8_t addTones[8];
String line;
int j = 0;

String filePath = "/init/sound/patterns/" + String(musicNo) + ".csv";

// ファイルが存在するかを確認する
  if (SPIFFS.exists(filePath)) {
  }else {
      // ファイルが存在しない場合の処理
      Serial.println("File does not exist");
      musicNo = 255;
  }
  // ファイルが存在する場合は開く
  File fr = SPIFFS.open(filePath, "r");

  if (!fr)// ファイルのオープンに失敗した場合の処理
  {
    Serial.println("Failed to open patterns.csv");
    return true; // とりあえず進む
  }

  // j = 0;
  while (fr.available()) // 64行だけ読み込む
  {
    line = fr.readStringUntil('\n'); // 64行文のパターン（小節）があります
    line.trim();                      // 空白を削除

    if (!line.isEmpty())
    {
      int commaIndex = line.indexOf(',');
      if (commaIndex != -1)
      {
        String val = line.substring(0, commaIndex);
        addTones[0] = val.toInt();

        for (int i = 1; i < 8; i++)
        {
          int nextCommaIndex = line.indexOf(',', commaIndex + 1);
          if (nextCommaIndex != -1)
          {
            val = line.substring(commaIndex + 1, nextCommaIndex);
            addTones[i] = val.toInt();
            commaIndex = nextCommaIndex;
          }
          else
          {
            // Handle the case where there is no trailing comma
            val = line.substring(commaIndex + 1);
            addTones[i] = val.toInt();
            break; // Exit the loop since we reached the end of the line
          }
        }

        for(uint8_t n=0; n<CHANNEL_NUM; n++){
          channels->setPatterns(j, n, addTones[n]);
        }
        
        j++;
      }
    }
  }
  fr.close();
  
  //すべてが終わったらtrueを返す
  return true;
}

uint8_t sfxlistNo = 0;
uint8_t sfxnos[8] ={0,1,2,3,4,5,6,7};
void readsfxlist() {
  // 読み込む効果音を外部ファイルを使い指定する
  File fr = SPIFFS.open("/init/sound/sfxes/sfxlist"+String(sfxlistNo)+".txt", "r");
  String line;
  int index = 0; // インデックスを追加
  while (fr.available()) {
    line = fr.readStringUntil('\n');
    if (!line.isEmpty()) {
      int commaIndex = line.indexOf(',');
      String val = line.substring(0, commaIndex);
      if (val.toInt() != 0) { // 0 でないことを確認
        sfxnos[index] = val.toInt(); // インデックスを使用して sfxnos 配列に値を代入
        index++; // インデックスをインクリメント
        if (index >= 8) break; // sfxnos 配列がオーバーフローしないようにする
      }
    }
  }
  fr.close();
}


bool readsfx()//行32列9のデータ
{
  //読み込む効果音を外部ファイルを使い指定する
  readsfxlist();
  // トーンファイルを読み込む
  File fr;
  for (int sfxno = 0; sfxno < SFX_NUM; sfxno++)
    {
    uint8_t addTones[32];
    String line;
    int j = 0;

    fr = SPIFFS.open("/init/sound/sfxes/"+String(sfxnos[sfxno])+".csv", "r");
    if (!fr)
    {
      Serial.println("Failed to open tones.csv");
      return true;//とりあえず進む
    }
    while (fr.available())
    {
      line = fr.readStringUntil('\n');
      // line.trim(); // 空白を削除
      if (!line.isEmpty())
      {
        int commaIndex = line.indexOf(',');
        if (commaIndex != -1)
        {
          String val = line.substring(0, commaIndex);//一個目はここで読み込む

          addTones[0] = val.toInt();
          if(j==0)channels->sfxdata[sfxno][0].onoffF = addTones[0];
          else if(j==1)channels->sfxdata[sfxno][0].instrument = addTones[0];//pitch
          else if(j==2)channels->sfxdata[sfxno][0].pitch = addTones[0];
          else if(j==3){
            channels->sfxdata[sfxno][0].octave = addTones[0];
            channels->sfxdata[sfxno][0].hz = channels->calculateFrequency(
              channels->sfxdata[sfxno][0].pitch, 
              channels->sfxdata[sfxno][0].octave);
          }
          else if(j==4)channels->sfxdata[sfxno][0].sfxno = addTones[0];
          else if(j==5)channels->sfxdata[sfxno][0].volume = addTones[0];

          for (int i = 1; i < 32; i++)
          {

            int nextCommaIndex = line.indexOf(',', commaIndex + 1);
            if (nextCommaIndex != -1)
            {
              val = line.substring(commaIndex + 1, nextCommaIndex);
              addTones[i] = val.toInt();
              commaIndex = nextCommaIndex;

              if(j==0)channels->sfxdata[sfxno][i].onoffF = addTones[i];
              else if(j==1)channels->sfxdata[sfxno][i].instrument = addTones[i];//pitch
              else if(j==2)channels->sfxdata[sfxno][i].pitch = addTones[i];
              else if(j==3){
                channels->sfxdata[sfxno][i].octave = addTones[i];
                channels->sfxdata[sfxno][i].hz = channels->calculateFrequency(
                  channels->sfxdata[sfxno][i].pitch, 
                  channels->sfxdata[sfxno][i].octave);
              }
              else if(j==4)channels->sfxdata[sfxno][i].sfxno = addTones[i];
              else if(j==5)channels->sfxdata[sfxno][i].volume = addTones[i];

            }
            
          }
          j++;
        }
      }
    }
    
  }
  fr.close();

  //すべてが終わったらtrueを返す
  return true;
}

bool readTones(uint8_t _patternNo)//行32列9のデータ
{
  // トーンファイルを読み込む
  File fr;
  for (int chno = 0; chno < CHANNEL_NUM; chno++)
    {
    uint8_t addTones[32];
    String line;
    int j = 0;
    uint8_t patternID = 0;
    
    patternID = channels->getPatternID( _patternNo, chno);

    fr = SPIFFS.open("/init/sound/tones/"+String(patternID)+".csv", "r");
    if (!fr)
    {
      Serial.println("Failed to open tones.csv");
      return true;//とりあえず進む
    }
    while (fr.available())
    {
      line = fr.readStringUntil('\n');
      // line.trim(); // 空白を削除
      if (!line.isEmpty())
      {
        int commaIndex = line.indexOf(',');
        if (commaIndex != -1)
        {
          String val = line.substring(0, commaIndex);

          for (int i = 0; i < 32; i++)
          {
            int nextCommaIndex = line.indexOf(',', commaIndex + 1);
            if (nextCommaIndex != -1)
            {
              val = line.substring(commaIndex + 1, nextCommaIndex);
              addTones[i] = val.toInt();
              commaIndex = nextCommaIndex;

              uint8_t bufNo = (_patternNo+1)%2;
              if(j==0)channels->notedata[chno][i+bufNo*32].onoffF = addTones[i];
              else if(j==1)channels->notedata[chno][i+bufNo*32].instrument = addTones[i];//pitch
              else if(j==2)channels->notedata[chno][i+bufNo*32].pitch = addTones[i];
              else if(j==3){
                channels->notedata[chno][i+bufNo*32].octave = addTones[i];
                channels->notedata[chno][i+bufNo*32].hz = channels->calculateFrequency(
                  channels->notedata[chno][i+bufNo*32].pitch, 
                  channels->notedata[chno][i+bufNo*32].octave);
              }
              else if(j==4)channels->notedata[chno][i+bufNo*32].sfxno = addTones[i];
              else if(j==5)channels->notedata[chno][i+bufNo*32].volume = addTones[i];

            }
            
          }
          j++;
        }
      }
    }
    
  }
  fr.close();

  //すべてが終わったらtrueを返す
  return true;
}

void createChTask(void *pvParameters) {
    while (true) {
      while (!createChannels()) {
          delay(10);
      }

      sndbufferedF = readTones(patternNo);

      xSemaphoreGive(syncSemaphore);
      delay(10);
    }
}

void sfxTask(void *pvParameters) {
  while (true) {
if(sfxflag==true){
      channels->begin();
      channels->setVolume(masterVol); // 0-255

      for(int n=0;n < 32;n++){
        sfxTickNo = n;
        channels->sfx(sfxChNo, sfxNo, wavNo, sfxVol, sfxspeed);
      }
      channels->stop();

      if(sfxTickNo == 31){sfxflag=false;sfxTickNo=0;}
    }
        delay(1);
  }
}

void musicTask(void *pvParameters) {
  while (true) {
    
    // if(musicflag){
    // 何らかの条件が満たされるまで待機
    if(sndbufferedF){//次の音がバッファされたら
      if (xSemaphoreTake(syncSemaphore, portMAX_DELAY)) {
      // 同期が取れたらここに入る
      
      if(musicflag){
        channels->begin();
        channels->setVolume(masterVol); // 0-255
        channels->note(0, toneTickNo, patternNo+loopStart);
        channels->note(1, toneTickNo, patternNo+loopStart);
        channels->note(2, toneTickNo, patternNo+loopStart);
        channels->note(3, toneTickNo, patternNo+loopStart);       
        channels->note(4, toneTickNo, patternNo+loopStart);
        channels->note(5, toneTickNo, patternNo+loopStart);
        //効果音は別にならす(ひとまず同発2ch)
        // channels->note(6, toneTickNo, patternNo+loopStart);
        // channels->note(7, toneTickNo, patternNo+loopStart); 
        channels->stop();

        toneTickNo++;
        toneTickNo %= TONE_NUM;

        if (toneTickNo == 0) {
            patternNo++;

            // if (patternNo >= PATTERN_NUM) {
            //     patternNo = 0;
            // }
            if (patternNo <= loopStart) {//現在の進行パタンがスタート位置より手前なら
                patternNo = loopStart;
            }else if (patternNo > loopEnd) {//現在の進行パタンがエンド位置より後ろなら
                patternNo = loopStart;
            }
        }

      }else{
        channels->setVolume(0); // 0-255
      }

      xSemaphoreGive(syncSemaphore);
      }
    }
    delay(1);
  }
}

void setup()
{
  Serial.begin(115200);
  auto cfg = M5.config();
  // M5.begin(cfg);
  M5Cardputer.begin(cfg);
  SPIFFS.begin();
  ui.begin( M5Cardputer.Display, 16, 1);

  readsfx();

  // 同期用セマフォの作成
  syncSemaphore = xSemaphoreCreateBinary();
  nextFrameSemaphore = xSemaphoreCreateBinary();

  // createChannelsTask タスクの作成（コア0で実行）
  xTaskCreatePinnedToCore(
    createChTask,
    "createChTask",
    8192,//~3000だと非力だけど動く//2048,4096なら動く//1024だと動かない
    NULL,
    1,
    &taskHandle[0],  // タスクハンドルを取得
    0//0だと落ちる
  );

  // musicTask タスクの作成（コア1で実行）
  xTaskCreatePinnedToCore(
    musicTask,
    "musicTask",
    4096,////1024だと動く//1500だと非力だけど動く//2048だと動く
    NULL,
    3,
    &taskHandle[1],//NULL,// タスクハンドルを取得
    1 // タスクを実行するコア（0または1）
  );

  // sfxTask タスクの作成（コア1で実行）
  xTaskCreatePinnedToCore(
    sfxTask,
    "sfxcTask",
    4096,////1024だと動く//1500だと非力だけど動く//2048だと動く
    NULL,
    2,
    &taskHandle[1],//NULL,// タスクハンドルを取得
    1 // タスクを実行するコア（0または1）
  );

  editor.getCursorConfig("/init/param/editor.txt");//エディタカーソルの位置をよみこむ

  delay(50);
  
  getOpenConfig();//最初に立ち上げるゲームのパスとモードをSPIFFSのファイルopenconfig.txtから読み込む

  tft.setPsram( false );//DMA利用のためPSRAMは切る
  tft.createSprite( TFT_WIDTH, TFT_HEIGHT );//PSRAMを使わないギリギリ
  tft.startWrite();//CSアサート開始

  sprite88_0.setPsram(false );
  sprite88_0.setColorDepth(16);//子スプライトの色深度
  sprite88_0.createSprite(8, 8);//ゲーム画面用スプライトメモリ確保

  uint8_t* gIMGBuf88_0 = new uint8_t[32];
  sprite88_0.drawPng(gIMGBuf88_0, 32,0,0);
  delete[] gIMGBuf88_0;

  sprite64.setPsram(false );
  sprite64.setColorDepth(16);//子スプライトの色深度
  sprite64.createSprite(PNG_SPRITE_WIDTH, PNG_SPRITE_HEIGHT);//ゲーム画面用スプライトメモリ確保//wroomだと64*128だとメモリオーバーしちゃう問題を色番号配列にして回避した

  uint8_t* gIMGBuf64 = new uint8_t[8192];
  auto file = SPIFFS.open("/init/initspr.png", "r");
  file.read(gIMGBuf64, 8192);
  sprite64.drawPng(gIMGBuf64, 8192,0,0);
  file.close();
  delete[] gIMGBuf64;

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
  sprite64.deleteSprite();

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
    
  game = nextGameObject(&appfileName, gameState, mapFileName);//ホームゲームを立ち上げる（オブジェクト生成している）
  game->init();//（オブジェクト生成している）

  frame=0;

  editor.initEditor(tft);
  editor.readFile(SPIFFS, appfileName.c_str());
  editor.editorOpen(SPIFFS, appfileName.c_str());
  editor.editorSetStatusMessage("Press ESCAPE to save file");

  savedAppfileName = appfileName;//起動したゲームのパスを取得しておく
  firstBootF = false;

  if (isEditMode == TFT_SOUNDEDIT_MODE) {
    patternNo = 0;
  }
  // masterVol = 0;

  musicflag = false;
  masterVol = 0;
  
}

bool btnpF = false;
bool prebtnpF = false;
int btnptick = 0;
int prebtnptick = 0;
int btnpms = 0;


unsigned long startTime = millis();

void loop()
{
  // 現在の時間を取得する
  uint32_t currentTime = millis();
  // 前フレーム処理後からの経過時間を計算する
  uint32_t elapsedTime = currentTime - startTime;
  // 前フレームからの経過時間を計算する
  uint32_t remainTime = (currentTime - preTime);
  preTime = currentTime;

  M5Cardputer.update();

  if (M5Cardputer.Keyboard.isPressed() == 0) {
      keychar = NULL;
      if(pressedBtnID != -1){buttonState[pressedBtnID] = -1;}
        pressedBtnID = -1;//リセット   
  }

  if (M5Cardputer.Keyboard.isChange()) {
    btnptick = 0;
    btnpms = 0;
  }

  if (M5Cardputer.Keyboard.isPressed()) 
  {
    //ボタンが押されているときだけtickがカウントされる
    
    if(btnpms <= 250){

      if(btnpms==0){btnpF = true; btnptick++;}//最初の0だけtrue
      else{btnpF = false;pressedBtnID=-1;}

    }else{
      if(btnpms%500 >= 250){
        // Serial.println("定期的にtrue");
        btnpF = true;
      }else{
        btnpF = false;
      }
      if(btnpF!= prebtnpF)btnptick++;
    }

    btnpms += elapsedTime;
    
    if (btnptick!=prebtnptick) {
    
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

          if(!shiftF){
            keychar = M5Cardputer.Keyboard.getKeyValue(keyPos).value_first;//対応する文字コードを格納
          }
          else if(shiftF){
            keychar = M5Cardputer.Keyboard.getKeyValue(keyPos).value_second;//対応する文字コードを格納
          }

        // if (status.fn&&keychar == PS2_LEFTARROW) {keychar = PS2_LEFTARROW;editor.editorProcessKeypress(keychar, SPIFFS);}
        // if (status.fn&&keychar == PS2_DOWNARROW) {keychar = PS2_DOWNARROW;editor.editorProcessKeypress(keychar, SPIFFS);}
        // if (status.fn&&keychar == PS2_UPARROW) {keychar = PS2_UPARROW;editor.editorProcessKeypress(keychar, SPIFFS);}
      if (isEditMode == TFT_SOUNDEDIT_MODE) {

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
        pressedBtnID = 7;
      } else if (M5Cardputer.Keyboard.isKeyPressed('?')) {
        pressedBtnID = 8;
      } else if (M5Cardputer.Keyboard.isKeyPressed(':')) {
        pressedBtnID = 5;
      } else if (M5Cardputer.Keyboard.isKeyPressed('>')) {
        pressedBtnID = 6;
      } else if (M5Cardputer.Keyboard.isKeyPressed('|')) {
        pressedBtnID = 9;
      } else {
        
        //通常の文字
        if(keychar != -1){
          editor.editorProcessKeypress(keychar, SPIFFS);
          }
        }

      }else if (isEditMode == TFT_RUN_MODE) {
        
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
        pressedBtnID = 7;
      } else if (M5Cardputer.Keyboard.isKeyPressed('?')) {
        pressedBtnID = 8;
      } else if (M5Cardputer.Keyboard.isKeyPressed(':')) {
        pressedBtnID = 5;
      } else if (M5Cardputer.Keyboard.isKeyPressed('>')) {
        pressedBtnID = 6;
      } else if (M5Cardputer.Keyboard.isKeyPressed('|')) {
        pressedBtnID = 9;
      } else {
        
        //通常の文字
        if(keychar != -1){
          editor.editorProcessKeypress(keychar, SPIFFS);
          }
        }

      }else if (isEditMode == TFT_EDIT_MODE){
        if(textMoveF){
          
             if (M5Cardputer.Keyboard.isKeyPressed(',')) {
            keychar = PS2_LEFTARROW;editor.editorMoveCursor(keychar);
          } else if (M5Cardputer.Keyboard.isKeyPressed('/')) {
            keychar = PS2_RIGHTARROW;editor.editorMoveCursor(keychar);
          } else if (M5Cardputer.Keyboard.isKeyPressed(';')) {
            keychar = PS2_UPARROW;editor.editorMoveCursor(keychar);
          } else if (M5Cardputer.Keyboard.isKeyPressed('.')) {
            keychar = PS2_DOWNARROW;editor.editorMoveCursor(keychar);
          } else if (M5Cardputer.Keyboard.isKeyPressed('|')) {
            pressedBtnID = 9;
          }else if (M5Cardputer.Keyboard.isKeyPressed('`')) {
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
    prebtnpF = btnpF;
    prebtnptick = btnptick;
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



    // 経過時間が1/30秒以上経過した場合
if (elapsedTime >= 1000/fps||fps==-1) {

  if(isEditMode == TFT_SOUNDEDIT_MODE){
    if (btnpF) {
      if(pressedBtnID == 3){
        targetChannelNo -=1;
      }else if(pressedBtnID == 4){
        targetChannelNo +=1;
      }
      targetChannelNo%=8;
      
      if(pressedBtnID == 1){
        masterVol -=8;
      }else if(pressedBtnID == 2){
        masterVol +=8;
      }
      masterVol%=256;

      if (pressedBtnID == 9) {

        if(ch_viewmode == CH1_VIEW_MODE){
          ch_viewmode = CH8_VIEW_MODE;
          return;
        }else if(ch_viewmode == CH8_VIEW_MODE){
          ch_viewmode = CH1_VIEW_MODE;
          return;
        }
      }
      // if (keychar == PS2_ENTER) {
      // }
    }

    tft.fillScreen(0);
    if(ch_viewmode == CH1_VIEW_MODE){
      controlMusicVisual();
    }else if(ch_viewmode == CH8_VIEW_MODE){
      showMusicVisual8ch();
    }
    
    showMusicInfo();
    //最終出力
    tft.setPivot(0, 0);
    tft.pushRotateZoom(&M5Cardputer.Display, 40, 3, 0, 1, 1);

    if (pressedBtnID == 0)
    {
      reboot("/init/main.lua", TFT_RUN_MODE);
    }

  }else if( isEditMode == TFT_RUN_MODE ){

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

      appfileName = "/init/main.lua";

      patternNo = 0;//音楽開始位置を0にリセット

      firstLoopF = true;
      toneflag = false;
      sfxflag = false;
      musicflag = false;

      mode = 1;//exit
      // 星用のベクター配列使用後は要素数を0にする
      bsParamFloat.resize(0);
      bsParamInt8t.resize(0);
      bsParamInt16t.resize(0);
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
      fps = 60;
      // txtName = appfileName;
      game = nextGameObject(&appfileName, gameState, mapFileName);//ファイルの種類を判別して適したゲームオブジェクトを生成
      game->init();//resume()（再開処理）を呼び出し、ゲームで利用する関数などを準備
      // tunes.resume();
    }
    
    // ui.showTouchEventInfo( tft, 0, 100 );//タッチイベントを視覚化する
    if(patternNo<10)M5Cardputer.Display.fillRect(200,120,16,8,TFT_BLACK);
    ui.showInfo( M5Cardputer.Display, 200, 112 );//ボタン情報、フレームレート情報などを表示します。
    M5Cardputer.Display.setCursor(200,120);
    M5Cardputer.Display.println(patternNo);
    

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
      editor.editorSave(SPIFFS);//SPIFFSに保存
      delay(100);//ちょっと待つ
      reboot(appfileName, TFT_RUN_MODE);//現状rebootしないと初期化が完全にできない
      // restart(appfileName, 0);//初期化がうまくできない（スプライトなど）
      // broadchat();//ファイルの中身をブロードキャスト送信する（ファイルは消えない）
    }

  }

  startTime = currentTime;
}

  frame++;
  if(frame > 18446744073709551615)frame = 0;
  delay(1);
  
}