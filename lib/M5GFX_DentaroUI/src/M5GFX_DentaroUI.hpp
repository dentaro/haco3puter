#ifndef M5GFX_DENTARO_UI_HPP
#define M5GFX_DENTARO_UI_HPP
#define FLICK_DIST 4
#define HOLDING_SEC 160000
#define TAP_TO_TAP_SEC 220000
#define FORMAT_SPIFFS_IF_FAILED true
#pragma once //インクルードガード

// #include "M5GFX_DentaroUI.hpp"
// #include "TouchBtn.hpp"

#include "TouchBtn.hpp"
#include <deque>

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#include <limits.h>
#include <stddef.h>
#include <vector>

#include <iostream>
#include <string>
#include <cstdint>//std::
#include <sstream>

#include <fstream>
#include <chrono>
#include <thread>

#include <SPIFFS.h>
#include "DentaroVector.hpp"
#include "DentaroPhisics.hpp"
using namespace std;

//開発者表記
#define CHAR_1_BYTE_5 5//=EN 5文字
#define CHAR_1_BYTE_9 9//=EN 9文字
#define CHAR_3_BYTE_5 15//=JP 5文字
#define CHAR_3_BYTE_9 27//=JP 9文字


//開発者表記
#define CHAR_3_BYTE 0//=JP
#define CHAR_1_BYTE 2//=EN
#define NUMERIC 4
//ユーザー表記(Arduino側でしか使わない)
#define JP 0
#define EN 2



#define SHOW_NAMED 0
#define SHOW_ALL 1

#define TOUCH_MODE 0
#define PHYSICAL_MODE 1
#define ANIME_MODE 2

#define TOGGLE_MODE true

#define PARENT_LCD 0
#define PARENT_SPRITE 1

#define ROW_MODE false
#define COL_MODE true

// #define SHIFT_NUM 5//シフトパネルの数
#define HENKAN_NUM 57

#define PRESET_BTN_NUM 20

#define VISIBLE true
#define INVISIBLE false

#define CALIBRATION_FILE "/init/param/caldata.txt"


//--地図用
#define BUF_PNG_NUM 1

// マイクロ秒単位での現在の時間を取得
// uint64_t micros() {
//     using namespace std::chrono;
//     return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
// }

class mySerial {
public:
    // フォーマット文字列と可変長引数を受け取って、文字列に整形するメソッド
    template<typename... Args>
    void printf(const char* format, Args... args) {
        // 文字列ストリームを作成し、フォーマット文字列と可変長引数を書き込む
        std::ostringstream stream;
        write(stream, format, args...);
        // 文字列を取得して出力
        std::cout << stream.str() << std::endl;
    }

    // printlnメソッド
    void println(const std::string& str) {
        std::cout << str << std::endl;
    }

    // printメソッド
    void print(const std::string& str) {
        std::cout << str;
    }

private:
    // writeメソッド：文字列ストリームにフォーマット文字列と可変長引数を書き込む再帰関数
    template<typename T, typename... Args>
    void write(std::ostringstream& stream, const char* format, T value, Args... args) {
        while (*format) {
            if (*format == '%') {
                // フォーマット文字列から変換仕様子を読み取って、引数を文字列に変換して書き込む
                if (*(format + 1) == 'd') {
                    stream << value;
                    // 引数を進める
                    format += 2;
                    write(stream, format, args...);
                    return;
                }
            }
            // 普通の文字はそのままストリームに書き込む
            stream << *format++;
        }
    }

    // 引数がない場合の終了条件
    void write(std::ostringstream& stream, const char* format) {
        while (*format) {
            stream << *format++;
        }
    }
};

// mySerial Serial;

class MyTFT_eSPI : public M5GFX {
  public:

    void drawObako(uint16_t* data){
    int _width = 128;
    int _height = 128;

    pushImage(0,0,128,128,data);
    }
};

class MyTFT_eSprite : public LGFX_Sprite {
  public:
    MyTFT_eSprite(MyTFT_eSPI* tft): LGFX_Sprite(tft){
      _mytft = tft;
    }

    void myDrawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color){
      drawFastHLine(x, y, w, color);
      drawFastHLine(x, y + h - 1, w, color);
      // Avoid drawing corner pixels twice
      drawFastVLine(x, y+1, h-2, color);
      drawFastVLine(x + w - 1, y+1, h-2, color);
    }

    void drawObako(){
      _mytft->drawObako(_img);
    }

  private:
    uint16_t* _img;
    MyTFT_eSPI* _mytft;
    MyTFT_eSprite * _mysprite;
  };

class RetClass {
public:
  int btnID = -1;
  int btnNo = -1;
  // bool toggleVal = true;
  lgfx::v1::touch_point_t sp;
  
  void reset(){
    btnID = -1;//-1;
    btnNo = -1;
    sp.x = 0;
    sp.y = 0;
  }

  int setBtnID_ret(int _btnID){
    btnID = _btnID;
    return btnID;
  }

  int setBtnNO_ret(int _btnNo){
    btnNo = _btnNo;
    return btnNo;
  }

  int ret0_return_btnID(int _btnID){
    btnID = _btnID;
    return btnID;
  }

  int ret1_return_btnNo(int _btnNo){
    btnNo = _btnNo;
    return btnNo;
  }

  void func01(int _btnID, int _x, int _y, int _eventState, int _runEventNo) {   // 関数プロトタイプ「void xxx(int,int,int,int);」
    btnID = _btnID;
    sp.x = _x;
    sp.y = _y;

    if(_eventState!=-1&&_eventState!=0&&_eventState!=5&&_eventState!=6&&_eventState!=7&&_eventState!=8){//触れていないときNONEと、待機中WAIT、移動中MOVEのイベントは表示しない
      //Serial.print("BUTTON_NO_" + std::to_string(_btnID));
            
      // if(_eventState == 3) //Serial.println("TAP");
      // if(_eventState == 4) //Serial.println("W_TAP");
      // if(_eventState == 10) //Serial.println("RIGHT");
      // if(_eventState == 12) //Serial.println("UP");
      // if(_eventState == 14) //Serial.println("LEFT");
      // if(_eventState == 16) //Serial.println("DOWN");

      // if(_eventState == 11) //Serial.println("RIGHT_UP");
      // if(_eventState == 13) //Serial.println("LEFT_UP");
      // if(_eventState == 15) //Serial.println("LEFT_DOWN");
      // if(_eventState == 17) //Serial.println("RIGHT_DOWN");
            
      // if(_eventState == 2) //Serial.println("DRAG");
            
      // // FrameEvent
      // if(_eventState == 6) //Serial.println("TOUCH");
      // if(_eventState == 7) //Serial.println("WAIT");
      // if(_eventState == 8) //Serial.println("MOVE");
      // if(_eventState == 9) //Serial.println("RELEASE");
      
    }
  }
};

class RetClass;

class UiContainer{
  public:
  int id = -1;
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int row = 0;
  int col = 0;

  int r0 = 0;
  int r1 = 0;
  int a = 0;
  int n = 0;

  //円形ボタン
  int c_d = 0;//中心から現在のタッチポイントまでの距離current_distance
  int c_a = 0;//中心から現在のタッチポイントがなす角度current_angle
  int p_a = 0;//前フレームのcurrent_angle= pre_angle
  int diff_a = 0;//c_a - p_a;

  int b_sNo = 0;
  int b_num = 0;
  int eventNo = -1; //MULTI_EVENT=-1
  int parentID = 0;
  std::string label = "";
  bool toggle_mode = false;
};

class RetClass;
class M5GFX_DentaroUI;

class Panel {
  public:
  std::vector<TouchBtn*> touchBtns;

  int id = -1;
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int row = 0;
  int col = 0;

  int r0 = 0;
  int r1 = 0;
  int a = 0;
  int n = 0;
  
  //円形ボタン
  int c_d = 0;//中心から現在のタッチポイントまでの距離current_distance
  int c_a = 0;//中心から現在のタッチポイントがなす角度current_angle
  int p_a = 0;//前フレームのcurrent_angle= pre_angle
  int diff_a = 0;//c_a - p_a;

  int b_sNo = 0;
  int b_num = 0;
  int eventNo = -1; //MULTI_EVENT=-1
  int parentID = 0;
  std::string label = "";
  std::string btn_name = "0";
  // std::string shift_btn_name = "ば";
  
  bool toggle_mode = false;

  int btn_mode = -1;
  int ui_no = -1;
  bool visibleF = true;
  bool availableF = true;
  bool selectBtnF = false;


  Panel() {
    // デフォルトコンストラクタ
  }

  ~Panel() {
    clearTouchBtns();  // Panel オブジェクトが破棄されるときに TouchBtn オブジェクトも解放
  }

  // std::string getHenkanChar(int _henkanListNo, int _kanaShiftNo)
  // {
  //   return kanalist[_henkanListNo][_kanaShiftNo];
  // }

//LGFX_Sprite&  _lcd
  void panelbtnDraw(M5GFX&  _lcd, int _x, int _y, int _w, int _h, std::string _btn_name, uint8_t _btnID, uint8_t _kanaShiftNo)
  {
    _lcd.fillRoundRect( _x, _y, _w, _h, 5, TFT_BLACK );
    _lcd.drawRoundRect( _x, _y, _w, _h, 5, TFT_WHITE );

    if(btn_mode != TOUCH_PRE_MODE){
      //  if( visibleF == true ){
      _lcd.setTextColor( TFT_WHITE );
      _lcd.setTextSize(1);
      _lcd.setCursor(_x+4 , _y+10 );
      _lcd.setFont(&lgfxJapanGothicP_8);
      _lcd.setTextWrap(true);
      _lcd.setTextScroll(true);
      uint8_t touchBtnID = _btnID - 20 - PRESET_BTN_NUM;

      if(btn_mode == TOUCH_FLICK_MODE){
        if(_btnID >= 40 ){
          if(_kanaShiftNo==0){
            touchBtns[touchBtnID]->shift_btn_name  = _btn_name;
          }
          else
          {
            if (_btn_name != "ま" || _btn_name != "な")
              {
                
              }
          }

          _lcd.print( touchBtns[_btnID - 20 - PRESET_BTN_NUM]->shift_btn_name.c_str() );

        }else{
          std::string btn_name_headline = "";
          if(_btn_name == "#"){btn_name_headline = "#@|&";}
          else if(_btn_name == "A"){btn_name_headline = "ABC";}
          else if(_btn_name == "D"){btn_name_headline = "DEF";}
          else if(_btn_name == "G"){btn_name_headline = "GHI";}
          else if(_btn_name == "J"){btn_name_headline = "JKL";}
          else if(_btn_name == "M"){btn_name_headline = "MNO";}
          else if(_btn_name == "P"){btn_name_headline = "PQRS";}
          else if(_btn_name == "T"){btn_name_headline = "TUV";}
          else if(_btn_name == "W"){btn_name_headline = "WXYZ";}
          else if(_btn_name == "^"){btn_name_headline = "^[$]\\";}
          else if(_btn_name == "'"){btn_name_headline = "'<\">";}
          else if(_btn_name == "."){btn_name_headline = ".?!";}

          else if(_btn_name == "%"){btn_name_headline = "%#@&";}
          else if(_btn_name == "a"){btn_name_headline = "abc";}
          else if(_btn_name == "d"){btn_name_headline = "def";}
          else if(_btn_name == "g"){btn_name_headline = "ghi";}
          else if(_btn_name == "j"){btn_name_headline = "jkl";}
          else if(_btn_name == "m"){btn_name_headline = "mno";}
          else if(_btn_name == "p"){btn_name_headline = "pqrs";}
          else if(_btn_name == "t"){btn_name_headline = "tuv";}
          else if(_btn_name == "w"){btn_name_headline = "wxyz";}
          else if(_btn_name == "-"){btn_name_headline = "-(/)_";}
          else if(_btn_name == ":"){btn_name_headline = ":'\"";}
          else if(_btn_name == "."){btn_name_headline = ".?!";}
          else {btn_name_headline = _btn_name;}

          _lcd.print( btn_name_headline.c_str() );
        }

      }else{
        _lcd.print( _btn_name.c_str());
      }

      // _lcd.print( _btn_name );
    }
  }


  void setBtnName(std::string _btn_name){
    btn_name = _btn_name;
  }

  void setSelectBtnF(bool _selectBtnF){
    selectBtnF = _selectBtnF;
  }

  lgfx::v1::touch_point_t getTouchPoint(int _x, int _y){
    lgfx::v1::touch_point_t tp;
    tp.x = _x;
    tp.y = _y;
    return tp;
  }

void addTouchBtn(int _gBtnID, int _b_x, int _b_y,int _b_w, int _b_h, int _btn_mode, int _eventNo, int _xy_mode) {

    TouchBtn* touchBtn = new TouchBtn();  // 新しい TouchBtn オブジェクトを動的に確保
    touchBtns.push_back(touchBtn);  // vector に追加
    // label = std::to_string(_gBtnID);//通らない
    // 整数を文字列に変換してストリームに挿入
    std::stringstream ss; // 文字列ストリームを作成
    ss << _gBtnID;
    // 文字列ストリームから文字列を取得
    label = ss.str();

    if(_btn_mode==TOUCH_BTN_MODE||_btn_mode==0){
      btn_name = "BTN_"+std::to_string(_gBtnID);
      // touchBtn->initBtn(_gBtnID, label, _b_x, _b_y, _b_w, _b_h, getTouchPoint(x, y), _btn_mode);
      touchBtn->initSlider(_gBtnID, label, _b_x, _b_y, _b_w, _b_h, getTouchPoint(x, y), _btn_mode, _eventNo, _xy_mode);
    }
    else if(_btn_mode==TOUCH_SLIDER_MODE){
      btn_name = "SLIDER_"+std::to_string(_gBtnID);
      touchBtn->initSlider(_gBtnID, label, _b_x, _b_y, _b_w, _b_h, getTouchPoint(x, y), _btn_mode, _eventNo, _xy_mode);
    }
    else if(_btn_mode==TOUCH_FLICK_MODE){
      btn_name = "FLICK_"+std::to_string(_gBtnID);
      touchBtn->initFlick(_gBtnID, label, _b_x, _b_y, _b_w, _b_h, getTouchPoint(x, y), _btn_mode);
    }
    touchBtn->setVisibleF(true);
    touchBtn->setAvailableF(true);
  }

  void clearTouchBtns() {
    for (TouchBtn* touchBtn : touchBtns) {
      delete touchBtn;  // TouchBtn オブジェクトを解放
    }
    touchBtns.clear();  // vector をクリア
  }
};


class Ui{//パネルを集めてUIを作るクラス
  public:
  std::vector<Panel *> panels;

  int id = -1;
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;

  Ui() {
    // デフォルトコンストラクタ
  }

  ~Ui() {
    clearUis();  // Panel オブジェクトが破棄されるときに TouchBtn オブジェクトも解放
  }

  void clearUis() {
    for (Panel* panel : panels) {
      if (panel != nullptr) {
        delete panel;  // TouchBtn オブジェクトを解放
      }
    }
    panels.clear();  // vector をクリア
  }
};


struct Vec2{
  double x;
  double y;

  Vec2() = default; // デフォルトコンストラクタ
  Vec2(double _x, double _y) // コンストラクタ
  : x(_x)
  , y(_y) {}
};

class FlickPanel
{
  // private:
  public:
  int b_No = -1;
  std::deque<std::string> text_list; //パネルテキスト配置用デック配列
  FlickPanel(){};

  void new_text_list(int _btnID, std::string _btnsString )
  {
    if(_btnID==0)text_list[_btnID] = _btnsString;
    else text_list.push_back(_btnsString);
  }
};

class MapTile
{
  // private:
  public:
  int  posId = -1;
  int  posNo = -1;
  bool readF = false;
  int  mapName = -1;
  int  mapNo = -1;
  int  preMapNo = 0;
  int objNo = -1;
  int MapNo = 0;
  int latPos, lonPos = 0;
  bool existF = false;
  // int xtile, ytile, ztile = 0;
  int xtilePos, ytilePos = 0;
  int xtileNo, ytileNo, ztileNo = 0;//IDで使う
  int preXtileNo, preYtileNo, preZtileNo = 0;//
  int addXTileNo, addYTileNo = 0;//中央からの相対的なタイルナンバー -1,1
  int preAddXTileNo, preAddYTileNo = 0;//中央からの相対的なタイルナンバー -1,1
  std::string m_url;
  LGFX_Sprite buff_sprite;
  LGFX_Sprite* buff_sprite_p;
  MapTile(){};

  void begin(int _objNo, int _xtile, int _ytile,int _ztile, std::string _m_url)
  {
    objNo = _objNo;

    m_url = _m_url;

    buff_sprite.setPsram(USE_PSRAM);
    buff_sprite.setColorDepth(TILE_COL_DEPTH);//子スプライトの色深度
    buff_sprite.createSprite(256, 256);//子スプライトメモリ確保
    buff_sprite.setPaletteGrayscale();
    // buff_sprite.drawPngFile(SD, m_url,
    //                         0, 0,
    //                         256, 256,
    //                         0, 0, 1.0, 1.0,
    //                         datum_t::top_left);
    buff_sprite_p = &buff_sprite;//オブジェクトのアドレスをコピーして、ポインタを格納
  }

  LGFX_Sprite* getSpritePtr(){return buff_sprite_p;}
  int getObjNo(){return objNo;}

  void setPreXtileNo(int _preXtileNo){preXtileNo = _preXtileNo;}
  void setPreYtileNo(int _preYtileNo){preYtileNo = _preYtileNo;}
  void setXtileNo(int _xtileNo){xtileNo = _xtileNo;}
  void setYtileNo(int _ytileNo){ytileNo = _ytileNo;}

  void setXtilePos(int _xtilePos){xtilePos = _xtilePos;}
  void setYtilePos(int _ytilePos){ytilePos = _ytilePos;}

  int getXtileNo(){return xtileNo;}
  int getYtileNo(){return ytileNo;}

  int getPreXtileNo(){return preXtileNo;}
  int getPreYtileNo(){return preYtileNo;}

  int getXtilePos(){return xtilePos;}
  int getYtilePos(){return ytilePos;}

  void setAddX( int _addXTileNo){addXTileNo = _addXTileNo;}
  void setAddY(int _addYTileNo){addYTileNo = _addYTileNo;}
  int getAddX(){return addXTileNo;}
  int getAddY(){return addYTileNo;}

  void setPreAddX( int _preAddXTileNo){preAddXTileNo = _preAddXTileNo;}
  void setPreAddY(int _preAddYTileNo){preAddYTileNo = _preAddYTileNo;}
  int getPreAddX(){return preAddXTileNo;}
  int getPreAddY(){return preAddYTileNo;}

  void setMapReadF( bool _readF){readF = _readF;}
  bool getMapReadF(){return readF;};
  // void setMapName( int _mapName){mapName = _mapName;}

  int getMapName(){return mapName;}
  void setMapNo(int _mapNo){mapNo = _mapNo;}
  int getMapNo(){return mapNo;}

  void setPreMapNo(int _preMapNo){preMapNo = _preMapNo;}
  int getPreMapNo(){return preMapNo;}

  void setPosNo(int _posNo){posNo = _posNo;}
  int getPosNo(){return posNo;}

  void setExistF(bool _existF){existF = _existF;}
  bool getExistF(){return existF;}
};

class M5GFX_DentaroUI {

  private:
    uint32_t eventBits = 0b00000000000000000000000000000000;
    uint8_t tapCount = 0;//タップカウンタ
    uint8_t lastTapCount = 0;
    bool jadgeF = false;


    lgfx::v1::touch_point_t tp;
    lgfx::v1::touch_point_t sp;
    lgfx::v1::touch_point_t sp2;
    lgfx::v1::touch_point_t layoutSpritePos; //レイアウト用
    lgfx::v1::touch_point_t lcdPos;//レイアウト用
    uint32_t btnState = 0x00000000;
    uint16_t touchState = 0x00000000;
    unsigned long touchStartTime = 0;
    unsigned long preTouchStartTime = 0;
    unsigned long lastTappedTime = 0;
    unsigned long firstTappedTime = 0;

    unsigned long sTime = 0;
    unsigned long tappedTime = 0;
    
    int eventState = -1;
    int flickState = -1;
    int tap_flick_thre = 82000;//タップとフリックの閾値
    int dist_thre = 40;
    int uiId = -1;
    int gPanelID = 0;
    int gBtnID = 0;
    uint constantBtnID = 9999;
    bool constantGetF = false;
    int selectBtnID = -1;
    int selectBtnBoxID = -1;
    int runEventNo =  -1;
    int parentID = 0;//初期値0 =　PARENT_LCD
    bool availableF = false;
    int uiMode = TOUCH_MODE;
    std::deque<TouchBtn*> touch_btn_list; //ボタン配置用デック配列
    std::deque<TouchBtn*> flick_touch_btn_list; //フリックボタン配置用デック配列
    std::deque<FlickPanel*> flickPanels;
    int showFlickPanelNo = 0;

    int timeCnt = 0;
    uint16_t clist[5] = {0,0,0,0,0};
    int preEventState = -1;
    int AngleCount = 0;
    int panels_num = 0;
    int uiAddBoxes_num = 0;

    UiContainer flickPanel;//9個の文字を入れる
    int layoutSprite_w = 0;
    int layoutSprite_h = 0;
    bool toggle_mode = false;

    std::string m_url = "";
    RetClass obj_ret;
    int shiftNum = 3;
    int charMode = CHAR_3_BYTE; //日本語
    //int charMode = CHAR_1_BYTE; //英語

    int charNumMode = CHAR_3_BYTE_5;//日本語5文字
    int sec, psec;
    int fps = 0;
    int frame_count = 0;
    bool use_flickUiSpriteF = false;
    int charNo=0;
    std::string flickString = "";
    std::string flickStr = "";
    std::string flickStrDel = "";
    std::string preFlickChar = "";
    std::string previewFlickChar = "";
    std::string finalChar ="";
    int fpNo = 0;
    // int kanaShiftNo = 0;
    bool selectModeF = false;
    int curbtnID;//現在の行番号
    int curKanaRowNo = 0;
    int curKanaColNo = 0;
    bool touchCalibrationF = false;
    int touchZoom = 1;
    int addBtnNum = 0;

    // PhysicBtn phbs;
    // uint16_t calData[8] = {326,3722,259,185,3776,3655,3776,243};//y
    uint16_t calData[8] = {3811,3853,345,3789,3711,403,1086,430};//daiso
    //uint16_t calData[8] = {558,3648,447,396,3599,3622,3625,324};
    // uint16_t calData[8];// = {0,1,0,273,319,1,319,264};//core2
    uint16_t calDataOK = 0;

    int LeftBtnUiID = 0;
    int RightBtnUiID = 0;
    int FlickUiID = 0;

    bool uiOpenF = true;

    //--Map用
    MapTile* MapTiles[9];
    bool DownloadF = false;
    bool mataidaF =false;
    // double latPos = 35.667995;
    // double lonPos = 139.7532971887966;//139.642076;
    // int tileZoom = 15;//8; //座標計算用

    int xtile = 0;
    int ytile = 0;
    int ztile = 0;
    int xtileNo = 0;
    int ytileNo = 0;

    int dirID = 0;

    int preXtileNo = 0;
    int preYtileNo = 0;
    int preXtile = 0;
    int preYtile = 0;

    int preDirID = 0;

    float vx = 0;
    float vy = 0;
    std::string host = "";
    //dirID
    //|6|7|8|
    //|5|0|1|
    //|4|3|2|
    // int mapNolist[9][2];
    // int tilePositons[9][4];
    int mapNoArray[9]= {-1,-1,-1,-1,-1,-1,-1,-1,-1};

    float matrix[6];
    float matrix_list[9][6] = {
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0}
    };

    //        //dirID
    //        //|6|7|8|
    //        //|5|0|1|
    //        //|4|3|2|
    int addTPosList[9][2] = {
      { 0, 0},{ 1, 0},{ 0, 1},
      { -1,0},{ 0,-1},{ 1,-1},
      {1, 1},{-1,1},{ -1,-1}
    };

    int gPosId = -1;

    // int preReadXtileNo[9];
    // int preReadYtileNo[9];

    bool allpushF = false;
    int ecnt = 0;

    std::string ROI_m_url ="";

    //----Map用ここまで
    int phbtnState[4];//物理ボリューム
    int flickShiftNo = 0;
    // int preflickShiftNo = 0;
    int kanaShiftNo = 0;

public:

  std::vector<Ui*> uis;

  M5GFX_DentaroUI();
  M5GFX_DentaroUI(M5GFX* _lcd);

  M5GFX* lcd; // M5GFXクラスへのポインタ//タッチポイントとれる

  LGFX_Sprite layoutSprite_list[BUF_PNG_NUM];

//メモリ確保のために一時的に消す

// const std::string kanalist[HENKAN_NUM][3] = {
//   {"あ","ぁ","＿"},
//   {"い","ぃ","ゐ"},
//   {"う","ぅ","＿"},
//   {"え","ぇ","ゑ"},
//   {"お","ぉ","＿"},
//   {"か","が","＿"},
//   {"き","ぎ","＿"},
//   {"く","ぐ","＿"},
//   {"け","げ","＿"},
//   {"こ","ご","＿"},

//   {"さ","ざ","＿"},
//   {"し","じ","＿"},
//   {"す","ず","＿"},
//   {"せ","ぜ","＿"},
//   {"そ","ぞ","＿"},
//   {"た","だ","＿"},
//   {"ち","ぢ","＿"},
//   {"つ","づ","っ"},
//   {"て","で","＿"},
//   {"と","ど","＿"},

//   {"は","ば","ぱ"},
//   {"ひ","び","ぴ"},
//   {"ふ","ぶ","ぷ"},
//   {"へ","べ","ぺ"},
//   {"ほ","ぼ","ぽ"},
//   {"や","ゃ","＿"},
//   {"ゆ","ゅ","＿"},
//   {"よ","ょ","＿"},
//   {"ー","～","＿"},

//   {"ア","ァ","＿"},
//   {"イ","ィ","ヰ"},
//   {"ウ","ゥ","＿"},
//   {"エ","ェ","ヱ"},
//   {"オ","ォ","＿"},
//   {"カ","ガ","＿"},
//   {"キ","ギ","＿"},
//   {"ク","グ","＿"},
//   {"ケ","ゲ","＿"},
//   {"コ","ゴ","＿"},

//   {"サ","ザ","＿"},
//   {"シ","ジ","＿"},
//   {"ス","ズ","＿"},
//   {"セ","ゼ","＿"},
//   {"ソ","ゾ","＿"},
//   {"タ","ダ","＿"},
//   {"チ","ヂ","＿"},
//   {"ツ","ヅ","ッ"},
//   {"テ","デ","＿"},
//   {"ト","ド","＿"},

//   {"ハ","バ","パ"},
//   {"ヒ","ビ","ピ"},
//   {"フ","ブ","プ"},
//   {"ヘ","ベ","ペ"},
//   {"ホ","ボ","ポ"},
//   {"ヤ","ャ","＿"},
//   {"ユ","ュ","＿"},
//   {"ヨ","ョ","＿"},
//   };

  //M5GFX* lgfx;//グラフィックのみ
  // LGFX_Sprite flickUiSprite;//フリック展開パネル用
  // inline void update( M5GFX& _lcd );
  inline void update2( M5GFX& _lcd );

  inline void begin(M5GFX& _lcd, int _colBit = 24, int _rotateNo = 0, bool _calibF = false);

  // void begin( M5GFX& _lcd, int _colBit, int _rotateNo);
  // void begin( M5GFX &_lcd, int _colBit, int _rotateNo, bool _calibF);

  // void begin( M5GFX& _lcd, std::string _host, int _shiftNum, int _colBit, int _rotateNo, bool _calibF );
  // void begin( M5GFX& _lcd, int _shiftNum, int _colBit, int _rotateNo, bool _calibF );
  // void begin(M5GFX& _lcd);
  // void begin();

  inline int getTouchZoom();
  inline void setTouchZoom(int _touchzoom);

  inline void touchCalibration (bool _calibUseF);
  inline bool isAvailable(int _btnID);

  inline void addHandler(int _uiId, int _panelID, int _btnNo, DelegateBase2* _func, uint16_t _runEventNo, int parentID = 0, bool _constantGetF = false);
  inline void circle(M5GFX* _lcd, uint16_t c, int fillF);
  inline void rect(M5GFX* _lcd, uint16_t c, int fillF);
  inline float getAngle(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b );
  inline float getDist(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b );
  inline uint8_t getVecNo();

  inline int getPreEvent();

  inline void updateSelectBtnID(int _selectBtnID);
  inline lgfx::v1::touch_point_t getStartPos();//タッチされたスタート地点を取得
  inline lgfx::v1::touch_point_t getPos();//タッチしている座標を取得
  inline void setPos(int _x, int _y);
  inline void setStartPos(int _x, int _y);

  DelegateBase2 *setBtnID_ret_DG = Delegate2<RetClass>::createDelegator2( &obj_ret, &RetClass::setBtnID_ret );//型式が違うプロトタイプ関数
  DelegateBase2 *setBtnNO_ret_DG = Delegate2<RetClass>::createDelegator2( &obj_ret, &RetClass::setBtnNO_ret );//型式が違うプロトタイプ関数
  
  inline void setBtnName(int _uiId, int _panelId, int _btnNo, std::string _btnName);
  inline void setBtnName(int _btnNo, std::string _btnName, std::string _btnNameFalse);//toggle用
  inline void setQWERTY(int _uiId, std::string _btnsString, LGFX_Sprite& _sprite);

  inline void createOBtns( int _x, int _y, int _r0,int _r1, int _a, int _n, int _eventNo);//円形に並ぶ
  inline void createOBtns( int _r0,int _r1, int _a, int _n, int _eventNo);//円形に並ぶ
  
  inline void createToggles( int _uiSprite_x, int _uiSprite_y, int _w,int _h, int _row, int _col, int _eventNo);//縦方向に並ぶ

  // void createFlicks( int _uiSprite_x, int _uiSprite_y, int _w,int _h, int _row, int _col, int _eventNo);//フリック生成
  inline void createFlick9Btns(LGFX_Sprite& _flickUiSprite);
  inline void drawFlickBtns( M5GFX& _lcd, int _btnID, int _btnNo, bool _visible, bool _available );
  // void setFlickPanel(int _flickPanelID, int _btnID, std::string _btnsString, int _btn_mode);
  inline void setFlickPanel( int _flickPanelID, int _btnID, std::string _btnsString );
  inline bool readFlickData(int _uiId,int startLine);
  // void readFlickDataline(int _uiId, int _btnID);
  inline void readFlickDataline(int _PRESET_BTN_NUM, int _uiId, int _btnID);

  inline std::string getFlickline(int flickDatalineNo);
  inline std::string getFlickChar(int _selectbtnID);//0~4
  inline std::string getFlickChar(int _PRESET_BTN_NUM, int _uiId, int _btnID);
  inline void setupFlickData(int _uiId);
  inline void flickShift();
  inline void kanaShift();
  inline void kanaShift(uint8_t _kanaShiftNo);
  inline void drawFlicks(int _uiId, M5GFX& _lcd);
  inline void drawBtns(int _uiId, M5GFX& _lcd);

  inline void drawBtns(M5GFX& _lcd);
  inline void drawBtns(int _uiId, M5GFX& _lcd, int _uiSprite_x, int _uiSprite_y);

  // void drawOBtns(int _uiId, M5GFX& _lcd, int _x, int _y);
  // void drawOBtns(int _uiId, M5GFX& _lcd, LGFX_Sprite& _uiSprite　);
  inline void drawOBtns(int _uiId, M5GFX& _lcd, int _uiSprite_x, int _uiSprite_y);

  inline void drawToggles(int _uiId, M5GFX& _lcd, int _uiSprite_x, int _uiSprite_y);
  inline void drawToggles(int _uiId, M5GFX& _lcd );

  inline void drawSliders(int _uiId, M5GFX& _lcd );
  // void drawTile(int _uiId, M5GFX* _lcd, LGFX_Sprite& _layoutSprite, uint8_t _bgColIndex, int _spriteNo);

  inline void drawSelectBtn(int _id);
  // void setPngTile(fs::FS &fs, std::string _m_url, int _spriteNo);
  // void setTilePos(int _id, int _x, int _y);
  // LGFX_Sprite getTileSprite(int _btnID);
  inline int getTouchBtnNo();//タッチされたボタンオブジェクトの番号を取得
  inline int getTouchBtnID();//タッチされたボタンオブジェクトのIDを取得
  inline int getEvent();
  inline int getFlickEvent();
  inline void showTouchEventInfo(LGFX_Sprite& _lcd, int _x, int _y);
  inline lgfx::v1::touch_point_t getTouchPoint(int _x, int _y);
  inline float getSliderVal( int _uiId, int _panel_no, int _btnNo );
  inline float getSliderVal(int _uiId, int _panel_no, int _btnNo, int _xy);
  inline Vec2 getSliderVec2( int _uiId, int _btnNo );

  //inline void setSliderVal(int _uiId, int _panel_no, int _btnNo, float _x, float _y);
  inline void setSliderVal(int _uiId, int _panel_no, int _btnNo, float _x, float _y);
  inline bool getToggleVal(int _uiId, int _btnNo);
  // bool getToggleVal(int _btnID);
  // bool getToggleVal2();

  inline void setAllBtnAvailableF(int _uiId, bool _available);
  inline void setAvailableF(int _uiId, int _btnID, bool _available);

  inline void setAllBtnVisibleF(int _uiId, bool _visible);
  inline void setVisibleF(int _uiId, int _btnID, bool _visible);

  inline void setCharMode(int _charMode);
  inline int getCharMode();

  inline std::string getFlickStr();

  inline int getflickPanelBtnNo(int uiID);

  inline int getUiFirstNo(int _uiId);
  inline int getUiBtnNum(int _uiId);
  inline int getUiBoxNum();
  inline int getAllBtnNum();
  inline int getParentID();
  inline int getUiID( const char* _uiLabel );
  inline void setBtnID(int _uiId, int _panelId, int _btnID);

  // void setShowFlickPanelNo(int _showFlickPanelNo);
  inline void setUiLabels(int _uiId, int _showFlickPanelNo);
  inline lgfx::v1::touch_point_t getBtnPos(int _btnID);

  inline void showInfo( LGFX_Sprite& _lcd, int _infox, int _infoy);

  // std::string getHenkanChar(int _henkanListNo, int _kanaShiftNo);
  inline void setFlick(int _uiId, int _panelId, int _FlickUiID, int _LeftBtnUiID, int _RightBtnUiID);//キーボード用プリセッ；
  inline void setFlick(int _uiId, int _panelId, int _charMode, int _FlickUiID, int _LeftBtnUiID, int _RightBtnUiID );
  inline std::string getInvisibleFlickStrings();
  inline std::string getFlickString();
  inline std::string getFlickString(bool _visibleMode);
  inline std::string getFlickline();
  // std::string getKana(int _panelID, int _rowID, int _colID, int _kanaShiftNo);

  inline const char* next_c_mb(const char* c);
  inline void ngetc(char* const dst,const char* src);
  inline bool nchr_cmp(const char* c1, const char* c2);
  inline std::vector<std::string> split_mb(const char* src, const char* del);

  inline std::string delEndChar(std::string _str, int _ByteNum);

  // void delChar();
  // std::string delChar(std::string _flickStr);
  inline std::string delChar(const std::string& _flickStr);
  inline void switchToggleVal();

  //---Map用関数

  inline void setDrawFinishF(int _objId, bool _drawFinishF);
  inline bool getDrawFinishF(int _objId);

  inline int get_gPosId();
  inline void set_gPosId(int _gPosId);


  inline void setAddX(int _objId,  int _xtileNo);
  inline void setAddY(int _objId,  int _ytileNo);
  inline void setPreAddX(int _objId,  int _xtileNo);
  inline void setPreAddY(int _objId,  int _ytileNo);

  inline int getPositionNo(int _addXTileNo, int _addYTileNo);

  inline void setXtileNo(int _objId,  int _xtileNo);
  inline void setYtileNo(int _objId,  int _ytileNo);
  inline void setTileNo(int _objId, int _xtileNo, int _ytileNo);

  inline void setPreYtileNo(int _objId,  int _preYtileNo);
  inline void setPreXtileNo(int _objId,  int _preXtileNo);

  inline void setXtilePos(int _objId,  int _xtilePos);
  inline void setYtilePos(int _objId,  int _ytilePos);

  inline int getAddX(int _spriteNo);
  inline int getAddY(int _spriteNo);
  inline int getPreAddX(int _spriteNo);
  inline int getPreAddY(int _spriteNo);

  inline std::string getPngUrl(int addNo);
  inline int getXtile();
  inline int getYtile();
  inline int getZtile();
  inline int getXtileNo();
  inline int getYtileNo();

  inline int getXtileNo(int _objNo);
  inline int getYtileNo(int _objNo);

  inline int getPreXtileNo(int _objNo);
  inline int getPreYtileNo(int _objNo);

  inline int getPreXtileNo();
  inline int getPreYtileNo();
  inline int getPreXtile();
  inline int getPreYtile();

  inline int getVx();
  inline int getVy();

  inline void setExistF(int _objId, bool _existF);
  inline bool getExistF(int _objId);

  inline int getDirID();
  inline int getPreDirID();
  inline void setPreDirID(int _dirID);

  inline void nowLoc(M5GFX& _lcd);

  inline void drawMaps(M5GFX& _lcd, double _walkLatPos, double _walkLonPos, int _tileZoom);

  // void setPngTile(fs::FS &fs, std::string _m_url, int _spriteNo);
  inline void setDownloadF(bool _b);
  inline bool getDownloadF();
  inline void task2_setPngTile(int _posId);

  inline void getTilePos(double lat, double lon, int zoom_level);

  inline void updateOBtnSlider(int uiID, int _x, int _y);

  inline int getCurrentAngle(int uiID);
  inline int getOBtnDiffAngle(int uiID);

  //イベントビットの操作
  inline bool getEventBit(int bitNo);
  inline void setEventBit(int bitNo, bool inputBit);
  inline void resetEventBits();

  //物理ボタン
  // void updatePhVols();
  // int getPhVol(int n);
  // int getPhVolDir(int n);
  // int getPhVolVec(int n1, int n2); 

  inline void setConstantGetF(bool _constantGetF);
  inline void drawPngFile(LGFX_Sprite sprite, fs::SPIFFSFS* _SPIFFS,const char * _path, int32_t _x, int32_t _y);

  inline void createPanel( 
    int _ui_no, int _btn_mode, 
    int _uiSprite_x, int _uiSprite_y, 
    int _w,int _h, int _row, int _col, 
    int _touchZoom, int _eventNo, int _xy_mode, int _uiContainerID);

  inline void createUIs(int uiContainernum);
  inline void deletePanels(int _uiContainerID, uint8_t _PRESET_BTN_NUM);
  inline void clearAddBtns();
  inline void showSavedCalData(M5GFX& _lcd);

};


inline void M5GFX_DentaroUI::setEventBit(int bitNo, bool inputBit)
{

  // if (inputBit == 1)
  // {
  //   bitSet(eventBits, bitNo - 1); // 右０ビット目を１（Set）に
  // }
  // else if (inputBit == 0)
  // {
  //   bitClear(eventBits, bitNo); // 右０ビット目を0（Clear）に
  // }

  if (inputBit == 1)
  {
  // bitSet(eventBits, 0);//右０ビット目を１（Set）に
    // 右0ビット目を1に設定する
    eventBits |= (1 << 0);
  }else if (inputBit == 0){
    // bitClear(eventBits, 0);//右０ビット目を0（Clear）に
    // 右0ビット目を0にクリアする
    eventBits &= ~(1 << 0);
  }

}

inline void M5GFX_DentaroUI::update2( M5GFX& _lcd )
{
  _lcd.setTextSize(1);
  _lcd.setFont(&lgfxJapanGothicP_8);

  lgfx::touch_point_t tp2[3];
  int nums = _lcd.getTouchRaw(tp2, 3);
  if (nums)
  {
    // for (int i = 0; i < nums; ++i)
    // {
    //   _lcd.setCursor(16, 16 + i * 24);
    //   _lcd.printf("Raw X:%03d  Y:%03d", tp2[i].x, tp2[i].y);
    // }

    _lcd.convertRawXY(tp2, nums);

    for (int i = 0; i < nums; ++i)
    {
      // _lcd.setCursor(16, 128 + i * 24);
      // _lcd.printf("Convert X:%03d  Y:%03d", tp2[i].x-40, tp2[i].y);
      tp.x = tp2[i].x-40;
      tp.y = tp2[i].y;
      
    }
    _lcd.display();
  }

  // _lcd.getTouch(&tp);
  
  for(int i = 0; i < 4; i++)
  {
    clist[i+1] = clist[i];
  }
  if(eventState != NO_EVENT) preEventState = eventState;//前のイベント状態を保持

  //以下イベント取得処理
  eventState = NO_EVENT;//イベント初期化
  flickState = NO_EVENT;//-1で初期化
  obj_ret.reset();
  
  //1ビット左にシフト
  touchState = touchState<<1;

  clist[0] = TFT_WHITE;
  
  if ( _lcd.getTouch( &tp ) )
  {
    // bitSet(touchState, 0);//右０ビット目を１（Set）に
    // 右0ビット目を1に設定する
    touchState |= (1 << 0);

  }else{
    // bitClear(touchState, 0);//右０ビット目を0（Clear）に
    // 右0ビット目を0にクリアする
    touchState &= ~(1 << 0);
  }
  //下位２ビットをみてイベントを振り分け
  touchState &= 0x00000011;//マスク
  if (touchState == B_TOUCH){
    sp = tp; touchStartTime =  micros();
    eventState = TOUCH;//7 
    setEventBit(TOUCH,1);
  }
  else if (touchState == B_MOVE) {
    clist[0] = TFT_GREEN;
    unsigned long holdingTime = micros() - touchStartTime;
    
    eventState = WAIT;
    setEventBit(WAIT,1);
    if(holdingTime > HOLDING_SEC){
      clist[0] = TFT_YELLOW;
      eventState = MOVE;//8
      setEventBit(MOVE,1);
    }
  }
  else if (touchState == B_RELEASE) 
  {
    clist[0] = TFT_BLUE;
    unsigned long holdingTime = micros() - touchStartTime;
    float dist = sqrt( pow((tp.x - sp.x),2) + pow((tp.y - sp.y), 2) );

    if(holdingTime > HOLDING_SEC - 10000 && holdingTime <= HOLDING_SEC){
      if(dist <= FLICK_DIST){
        eventState = WAIT;//5
        setEventBit(WAIT,1);
      }
    }
    else if(holdingTime > HOLDING_SEC)
    {
      eventState = DRAG;//2
      setEventBit(DRAG,1);
    }
    else if(dist <= FLICK_DIST)
    {
      float dist2 = sqrt( pow((tp.x - sp2.x),2) + pow((tp.y - sp2.y),2) );
      
      if(preTouchStartTime != 0)
      {
        unsigned long tappingTime  = touchStartTime - preTouchStartTime;
        if( tappingTime < TAP_TO_TAP_SEC ){//前とのタップ間の時間調整, 位置判定
          if(dist2 < FLICK_DIST) tapCount++;//短距離、短時間なら加算
        }
        else if( tappingTime >= TAP_TO_TAP_SEC )//時間が空いていたらリセット
        {

          lastTapCount = tapCount;
          if(lastTapCount>0){
            eventState = WTAPPED;
            setEventBit(WTAPPED, 1);
          }else if(lastTapCount==0){
            eventState = TAPPED;
            setEventBit(TAPPED, 1);
          }
          tapCount = 0;

        }
      }

      //tapCount++;//今回タップした分を足す
      lastTappedTime = micros();
      // //Serial.print(tapCount);
      // //Serial.print(":");
      // //Serial.print(micros());
      // //Serial.print(":");
      // //Serial.println(micros() - firstTappedTime);

      
      // if(endTapCount)

      if(tapCount == 0){
        firstTappedTime = touchStartTime;//tap番号0のタップスタート時間を格納
        eventState = TAPPED;
        setEventBit(TAPPED, 1);
      }

      if(tapCount == 1){
        eventState = WTAPPED;//4
        setEventBit(WTAPPED, 1);
      }
      

      sp2 = tp;
      preTouchStartTime = touchStartTime;
    }
    else if(dist > FLICK_DIST)
    {
       if(holdingTime <= HOLDING_SEC){
        float angle = getAngle(sp, tp);
  //      //Serial.println(angle);
          
          if(angle <= 22.5 || angle > 337.5){
            eventState = RIGHT_FLICK;//0
            setEventBit(RIGHT_FLICK, 1);
            flickState = RIGHT_FLICK;//0
          }else if(angle <= 67.5 && angle > 22.5){
            eventState = R_D_FLICK;//7
            setEventBit(R_D_FLICK, 1);
            flickState = R_D_FLICK;//7
          }else if(angle <= 112.5 && angle > 67.5){
            eventState = DOWN_FLICK;//6
            setEventBit(DOWN_FLICK, 1);
            flickState = DOWN_FLICK;//6
          }else if(angle <= 157.5 && angle > 112.5){
            eventState = D_L_FLICK;//5
            setEventBit(D_L_FLICK, 1);
            flickState = D_L_FLICK;//5
          }else if(angle <= 202.5 && angle > 157.5){
            eventState = LEFT_FLICK;//4
            setEventBit(LEFT_FLICK, 1);
            flickState = LEFT_FLICK;//4
          }else if(angle <= 247.5 && angle > 202.5){
            eventState = L_U_FLICK;//3
            setEventBit(L_U_FLICK, 1);
            flickState = L_U_FLICK;//3
          }else if(angle <= 292.5 && angle > 247.5){
            eventState = UP_FLICK;//2
            setEventBit(UP_FLICK, 1);
            flickState = UP_FLICK;//2
          }else if(angle <= 337.5 && angle > 292.5){
            eventState = U_R_FLICK;//1
            setEventBit(U_R_FLICK, 1);
            flickState = U_R_FLICK;//1
          }

       }
    }
    eventState = RELEASE;//10
    setEventBit(RELEASE,1);
  }
  else if (touchState == B_UNTOUCH)
  {
    clist[0] = TFT_DARKGREEN;

  // if(micros() - firstTappedTime < 200000 && jadgeF == false){
  //   jadgeF = true;
  // }

    // if(micros() - firstTappedTime > 200000 && jadgeF == true){
    //   eventState = TAPPED;//
    //   setEventBit(TAPPED, 1);
    //   jadgeF = false;
    // }
  }
else if (touchState == B_UNTOUCH)
  {
    clist[0] = TFT_DARKGREEN;
  }
  for (int k = 0; k < uis.size(); k++)
  {  // UIの数
    for (int i = 0; i < uis[k]->panels.size(); i++)
    { // パネルの数
      // //Serial.print(panels[i]->id);
      for (int j = 0; j < uis[k]->panels[i]->b_num; j++)
      { // パネル内のボタンの数
        addHandler(k,i, j, setBtnID_ret_DG, uis[k]->panels[i]->eventNo, uis[k]->panels[i]->parentID, constantGetF);
        addHandler(k,i, j, setBtnNO_ret_DG, uis[k]->panels[i]->eventNo, uis[k]->panels[i]->parentID, constantGetF);
      }
    }
  }

  if (constantGetF)
  {
    if (getEvent() == NO_EVENT)
    {
      obj_ret.setBtnID_ret(-1); // ‐１（ボタンが離れている状態）にクリア
    }
    
    // if(getEvent() != TOUCH)obj_ret.setBtnID_ret(-1);//‐１（ボタンが離れている状態）にクリア
  }


  // constantBtnID = obj_ret.btnID;
}


inline float M5GFX_DentaroUI::getAngle(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b)
{
  float r = atan2(b.y - a.y, b.x - a.x);
  if (r < 0)
  {
    r = r + 2 * M_PI;
  }
  return r * 360 / (2 * M_PI);
}


inline void M5GFX_DentaroUI::addHandler(int _uiId, int _panelNo, int _btnNo, DelegateBase2 *_func, uint16_t _runEventNo, int _parentID, bool _constantGetF)
{
  runEventNo = _runEventNo;
  _parentID = 0; // ベータ版ではとりあえず強制的にLCDのみのイベントをとる

  int _btnID = uis[_uiId]->panels[_panelNo]->touchBtns[_btnNo]->getBtnID();

  uis[_uiId]->panels[_panelNo]->touchBtns[_btnNo]->addHandler(_func);

  if (!_constantGetF)
  {
    uis[_uiId]->panels[_panelNo]->touchBtns[_btnNo]->run2(_btnID, _btnNo, sp, tp, eventState, runEventNo);
    //      if( parentID == PARENT_LCD )    panels[_panelNo]->touchBtns[_btnNo]->run2( _btnID, _btnNo, sp, tp, eventState, runEventNo);//ボタン内のイベントかチェック
    // else if( parentID == PARENT_SPRITE ) panels[_panelNo]->touchBtns[_btnNo]->run2( _btnID, _btnNo, sp, tp, eventState, runEventNo);//ボタン内のイベントかチェック
  }
  else if (_constantGetF)
  { // spにtpを入れて、常時取得モードにする
    uis[_uiId]->panels[_panelNo]->touchBtns[_btnNo]->run2(_btnID, _btnNo, tp, tp, eventState, runEventNo);
    //      if( parentID == PARENT_LCD )    panels[_panelNo]->touchBtns[_btnNo]->run2( _btnID, _btnNo, tp, tp, eventState, runEventNo);//ボタン内のイベントかチェック
    // else if( parentID == PARENT_SPRITE ) panels[_panelNo]->touchBtns[_btnNo]->run2( _btnID, _btnNo, tp, tp, eventState, runEventNo);//ボタン内のイベントかチェック
  }

  uis[_uiId]->panels[_panelNo]->touchBtns[_btnNo]->delHandlers2();
}



// inline void M5GFX_DentaroUI::update(M5GFX &_lcd)
// {
//   _lcd.getTouch(&tp);

//   for (int i = 0; i < 4; i++)
//   {
//     clist[i + 1] = clist[i];
//   }
//   if (eventState != NO_EVENT)
//     preEventState = eventState; // 前のイベント状態を保持

//   // 以下イベント取得処理
//   eventState = NO_EVENT; // イベント初期化
//   flickState = NO_EVENT; //-1で初期化
//   obj_ret.reset();

//   // 1ビット左にシフト
//   touchState = touchState << 1;

//   clist[0] = TFT_WHITE;

//   if (_lcd.getTouch(&tp))
//   {
//       // bitSet(touchState, 0);//右０ビット目を１（Set）に
//     // 右0ビット目を1に設定する
//     touchState |= (1 << 0);

//   }else{
//     // bitClear(touchState, 0);//右０ビット目を0（Clear）に
//     // 右0ビット目を0にクリアする
//     touchState &= ~(1 << 0);
//   }
//   // 下位２ビットをみてイベントを振り分け
//   touchState &= 0x00000011;
//   if (touchState == B_TOUCH)
//   {
//     sp = tp;
//     sTime = micros();
//     eventState = TOUCH; // 6
//   }
//   else if (touchState == B_MOVE)
//   {
//     clist[0] = TFT_GREEN;
//     unsigned long touchedTime = micros() - sTime;
//     eventState = WAIT;
//     if (touchedTime > 160000)
//     {
//       clist[0] = TFT_YELLOW;
//       eventState = MOVE; // 8
//     }
//   }
//   else if (touchState == B_RELEASE)
//   {
//     clist[0] = TFT_BLUE;
//     unsigned long touchedTime = micros() - sTime;
//     float dist = sqrt(pow((tp.x - sp.x), 2) + pow((tp.y - sp.y), 2));

//     if (touchedTime > 120000 && touchedTime <= 160000)
//     {
//       if (dist <= FLICK_DIST)
//       {
//         eventState = WAIT; // 5
//       }
//     }
//     else if (touchedTime > 160000)
//     {
//       eventState = DRAG; // 2
//     }
//     else if (dist <= FLICK_DIST)
//     {
//       eventState = TAPPED; // 4

//       float dist2 = sqrt(pow((tp.x - sp2.x), 2) + pow((tp.y - sp2.y), 2));
//       if (sTime - tappedTime < 200000 && dist2 < FLICK_DIST)
//       {                       // ダブルタップの間の時間調整, 位置判定
//         eventState = WTAPPED; // 5
//       }
//       tappedTime = micros();
//       sp2 = tp;
//     }
//     else if (dist > FLICK_DIST)
//     {
//       if (touchedTime <= 160000)
//       {
//         float angle = getAngle(sp, tp);

//         if (angle <= 22.5 || angle > 337.5)
//         {
//           eventState = RIGHT_FLICK; // 0
//           flickState = RIGHT_FLICK; // 0
//         }
//         else if (angle <= 67.5 && angle > 22.5)
//         {
//           eventState = R_D_FLICK; // 7
//           flickState = R_D_FLICK; // 7
//         }
//         else if (angle <= 112.5 && angle > 67.5)
//         {
//           eventState = DOWN_FLICK; // 6
//           flickState = DOWN_FLICK; // 6
//         }
//         else if (angle <= 157.5 && angle > 112.5)
//         {
//           eventState = D_L_FLICK; // 5
//           flickState = D_L_FLICK; // 5
//         }
//         else if (angle <= 202.5 && angle > 157.5)
//         {
//           eventState = LEFT_FLICK; // 4
//           flickState = LEFT_FLICK; // 4
//         }
//         else if (angle <= 247.5 && angle > 202.5)
//         {
//           eventState = L_U_FLICK; // 3
//           flickState = L_U_FLICK; // 3
//         }
//         else if (angle <= 292.5 && angle > 247.5)
//         {
//           eventState = UP_FLICK; // 2
//           flickState = UP_FLICK; // 2
//         }
//         else if (angle <= 337.5 && angle > 292.5)
//         {
//           eventState = U_R_FLICK; // 1
//           flickState = U_R_FLICK; // 1
//         }
//       }
//     }
//     eventState = RELEASE; // 9
//   }
//   else if (touchState == B_UNTOUCH)
//   {
//     clist[0] = TFT_DARKGREEN;
//   }
//   for (int k = 0; k < uis.size(); k++)
//   {  // UIの数
//     for (int i = 0; i < uis[k]->panels.size(); i++)
//     { // パネルの数
//       // //Serial.print(panels[i]->id);
//       for (int j = 0; j < uis[k]->panels[i]->b_num; j++)
//       { // パネル内のボタンの数
//         addHandler(k,i, j, setBtnID_ret_DG, uis[k]->panels[i]->eventNo, uis[k]->panels[i]->parentID, constantGetF);
//         addHandler(k,i, j, setBtnNO_ret_DG, uis[k]->panels[i]->eventNo, uis[k]->panels[i]->parentID, constantGetF);
//       }
//     }
//   }

//   if (constantGetF)
//   {
//     if (getEvent() == NO_EVENT)
//     {
//       obj_ret.setBtnID_ret(-1); // ‐１（ボタンが離れている状態）にクリア
//     }
//   }
// }

inline void M5GFX_DentaroUI::showSavedCalData(M5GFX &_lcd)
{
  // タッチが使用可能な場合のキャリブレーション値の可視化を行います。（省略可）
  if (touchCalibrationF == true)
  {
    std::ofstream file(CALIBRATION_FILE, std::ios::binary);
    if (file.is_open())
    {
      file.write((const char *)calData, sizeof(calData));
      file.close();
    }
    
    _lcd.fillRect(0, 0, 128, 128, TFT_GREEN);
    _lcd.setTextSize(1);
    _lcd.setTextColor(TFT_WHITE, TFT_BLUE);
    _lcd.setCursor(0, 0);
    for (int i = 0; i < 8; ++i)
    {
      _lcd.println(calData[i]);
    }
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 1000ミリ秒待機
  _lcd.setTouchCalibrate(calData);
}

// メソッドの実装（インライン関数として）
inline M5GFX_DentaroUI::M5GFX_DentaroUI() : lcd(nullptr) {}
inline M5GFX_DentaroUI::M5GFX_DentaroUI(M5GFX* _lcd) : lcd(_lcd) {}

inline void M5GFX_DentaroUI::begin(M5GFX& _lcd, int _colBit, int _rotateNo, bool _calibF) {
    lcd = &_lcd;
    // _lcd.init();
    // _lcd.begin();
    _lcd.startWrite(); // CSアサート開始
    _lcd.setColorDepth(_colBit);
    // touchCalibrationF = _calibF;
    // // begin(_lcd);
    _lcd.setRotation(_rotateNo);
    // showSavedCalData(_lcd); // タッチキャリブレーションの値を表示
}

inline int M5GFX_DentaroUI::getEvent()
{
  return eventState;
}
inline void M5GFX_DentaroUI::createUIs(int uiContainernum){
// Ui オブジェクトを事前に生成して uis ベクターに追加
        for (int i = 0; i < uiContainernum; ++i) {
            Ui* ui = new Ui();
            uis.push_back(ui);
        }
}

inline void M5GFX_DentaroUI::createPanel(
  int _ui_no,int _btn_mode, 
  int _x, int _y,
  int _w, int _h,
  int _row, int _col,
  int _touchZoom, int _eventNo, int _xy_mode, int _uiContainerID)
{
  touchZoom = _touchZoom;
  addBtnNum += _row * _col;

  Panel *panel = new Panel(); // 新しい Panel オブジェクトを動的に確保

  panel->btn_mode = _btn_mode;
  panel->ui_no = _ui_no;

  if (_btn_mode == TOUCH_BTN_MODE||_btn_mode == TOUCH_PRE_MODE)
  {
    panel->label = std::to_string(gPanelID);
    //Serial.println("BTN_" + std::to_string(gPanelID) + "=[" + std::to_string(gPanelID) + "]" + std::to_string(gBtnID) + "~");
  }
  else if (_btn_mode == TOUCH_SLIDER_MODE)
  {
    panel->label = std::to_string(gPanelID);
    //Serial.println("SLIDER_" + std::to_string(gPanelID) + "=[" + std::to_string(gPanelID) + "]" + std::to_string(gBtnID) + "~");
  }
  else if (_btn_mode == TOUCH_FLICK_MODE)
  {
    panel->label = std::to_string(gPanelID);
    //Serial.println("FLICK_" + std::to_string(gPanelID) + "=[" + std::to_string(gPanelID) + "]" + std::to_string(gBtnID) + "~");
  }

  // int _startId = gBtnID;
  panel->b_sNo = gBtnID;
  panel->id = gPanelID;
  panel->x = _x * _touchZoom;
  panel->y = _y * _touchZoom;
  panel->w = _w * _touchZoom;
  panel->h = _h * _touchZoom;
  panel->row = _row;
  panel->col = _col;
  panel->eventNo = _eventNo;

  int b_w = int(panel->w / _row);
  int b_h = int(panel->h / _col);

  for (int j = 0; j < _col; j++)
  {
    for (int i = 0; i < _row; i++)
    {
      // panel->addTouchBtn(gBtnID,0,0,50,50,_btn_mode,_eventNo,_xy_mode);
      panel->addTouchBtn(gBtnID, panel->x + i * b_w,
                         panel->y + j * b_h, b_w, b_h, _btn_mode, _eventNo, _xy_mode); // TouchBtn オブジェクトを Panel オブジェクトに追加
      panel->b_num++;                                                                  // パネルが持つボタンの数を記録
      gBtnID++;
    }
  }
  
  if (panel != nullptr) {
    uis[_uiContainerID]->panels.push_back(panel);
  }

  //Serial.println("e");
  gPanelID++;
}
inline void M5GFX_DentaroUI::setupFlickData(int _uiId) {
  uis[_uiId]->panels[1]->touchBtns[0]->namelabel = "Nxt";
  uis[_uiId]->panels[1]->touchBtns[1]->namelabel = "a/A";
  uis[_uiId]->panels[1]->touchBtns[2]->namelabel = "SFT";
  uis[_uiId]->panels[1]->touchBtns[3]->namelabel = "゛小";

  uis[_uiId]->panels[2]->touchBtns[0]->namelabel = "del";
  uis[_uiId]->panels[2]->touchBtns[1]->namelabel = "_";
  uis[_uiId]->panels[2]->touchBtns[2]->namelabel = "Clr";
  uis[_uiId]->panels[2]->touchBtns[3]->namelabel = "Ret";
}

// 経緯度からタイル座標を求める
inline void M5GFX_DentaroUI::getTilePos(double _lat, double _lon, int zoom_level)
{
  // setLatLonPos(lat, lon);
  //  _lat = lat;
  //  _lon = lon;

  // //座標を含むタイル番号を計算
  // double lat_rad = _lat * (M_PI/180);
  // double n = _lcdpow(2, zoom_level);
  // xtileNo = int((_lon + 180.0) / 360.0 * n);
  // ytileNo = int((1.0 - log(tan(lat_rad) + (1 / cos(lat_rad))) / M_PI) / 2.0 * n);
  // ztile = zoom_level;

  // 緯度経度からタイル座標に変換
  double x = (_lon / 180 + 1) * pow(2, zoom_level) / 2;
  xtile = int(x * 255);
  double y = ((-log(tan((45 + _lat / 2) * M_PI / 180)) + M_PI) * pow(2, zoom_level) / (2 * M_PI));
  ytile = int(y * 255);
  ztile = zoom_level;

  // //座標を含むタイル番号を計算
  // x = (_lon / 180 + 1) * pow(2, zoom_level-1) / 2;
  // xtileNo = int(x);
  // y = ((-log(tan((45 + _lat / 2) * M_PI / 180)) + M_PI) * pow(2, zoom_level-1) / (2 * M_PI));
  // ytileNo = int(y);

  // //現在地を含むタイル番号を計算
  double lat_rad = _lat * (M_PI / 180);
  double n = pow(2, zoom_level);
  xtileNo = int((_lon + 180.0) / 360.0 * n);
  ytileNo = int((1.0 - log(tan(lat_rad) + (1 / cos(lat_rad))) / M_PI) / 2.0 * n);
}

void M5GFX_DentaroUI::updateOBtnSlider(int _uiId, int _x, int _y)
{
  uis[_uiId]->panels[0]->c_d = sqrt((pow(tp.x - (_x + uis[_uiId]->panels[0]->r1), 2.0) + pow(tp.y - (_y + uis[_uiId]->panels[0]->r1), 2.0)));
  if (uis[_uiId]->panels[0]->c_d > uis[_uiId]->panels[0]->r0 && uis[_uiId]->panels[0]->c_d < uis[_uiId]->panels[0]->r1)
  {
    if (getEvent() == MOVE)
    {

      uis[_uiId]->panels[0]->c_a = int((atan2(tp.y - (_y + uis[_uiId]->panels[0]->r1),
                                     tp.x - (_x + uis[_uiId]->panels[0]->r1)) *
                               180 / M_PI)); // 現在のボタン中心の角度を登録

      if (uis[_uiId]->panels[0]->c_a < 0)
        uis[_uiId]->panels[0]->c_a = 180 + (180 + uis[_uiId]->panels[0]->c_a); // 0~360の値に収める

      // if( abs(uis[_uiId]->panels[0]->c_a - uis[_uiId]->panels[0]->p_a) > 0 ){

      uis[_uiId]->panels[0]->diff_a = uis[_uiId]->panels[0]->c_a - uis[_uiId]->panels[0]->p_a; // 差分を計算する

      // //Serial.println(uis[_uiId]->panels[0]->diff_a);

      // if( getEvent() == TOUCH ){
      // uis[_uiId]->panels[0]->p_a = uis[_uiId]->panels[0]->c_a;//タッチした時のボタン中心の角度を登録しておく
      // }

      if (uis[_uiId]->panels[0]->diff_a > 180)
      {
        uis[_uiId]->panels[0]->diff_a -= 360;
      }
      else if (uis[_uiId]->panels[0]->diff_a < -180)
      {
        uis[_uiId]->panels[0]->diff_a += 360;
      }

      // //Serial.println(uis[_uiId]->panels[0]->diff_a);

      uis[_uiId]->panels[0]->p_a = uis[_uiId]->panels[0]->c_a;
      // //Serial.print(uis[_uiId]->panels[0]->p_a);
      // //Serial.print(":");
    }
    else if (getEvent() == RELEASE)
    {
      uis[_uiId]->panels[0]->diff_a = 0;
    }
  }
}

inline int M5GFX_DentaroUI::getCurrentAngle(int _uiId)
{
  return uis[_uiId]->panels[0]->c_a;
}

inline int M5GFX_DentaroUI::getOBtnDiffAngle(int _uiId)
{
  return uis[_uiId]->panels[0]->diff_a;
}

inline bool M5GFX_DentaroUI::getEventBit(int bitNo)
{
  uint32_t b = eventBits; // コピー
  // uint32_t mb = 0b1;//マスク用
  bool rb = 0b1 & (b >> (bitNo - 1)); // ビットシフトして、マスクで１ビット目を抽出してboolにキャスト
  return rb;
}

inline void M5GFX_DentaroUI::resetEventBits()
{
  eventBits = 0b00000000000000000000000000000000;
}

inline void M5GFX_DentaroUI::clearAddBtns()
{
  for (int k = 0; k < uis.size(); k++){
  int count = uis[k]->panels.size();
  }
  int count2 = touch_btn_list.size();
}

inline void M5GFX_DentaroUI::touchCalibration(bool _calibF)
{
  touchCalibrationF = _calibF;
}

inline int M5GFX_DentaroUI::getTouchZoom()
{
  return touchZoom;
}

inline void M5GFX_DentaroUI::setTouchZoom(int _touchzoom)
{
  touchZoom = _touchzoom;
}

// 交点を求める関数
// lgfx::v1::touch_point_t findIntersection(double x, double y, double x2, double y2, int h) {
//     // 直線の傾きを計算
//     double m = (y2 - y) / (x2 - x);

//     // 直線のy切片を計算
//     double b = y - m * x;

//     // 高さ128との水平線との交点を計算
//     double intersection_x = (h - b) / m;
//     double intersection_y = h;
//     lgfx::v1::touch_point_t p;
//     p.x = intersection_x;
//     p.y = intersection_y;
//     return p;
// }

inline uint8_t M5GFX_DentaroUI::getVecNo()
{
  // lgfx::v1::touch_point_t ip = findIntersection(sp.x, sp.y+76, tp.x, tp.y, sp.y-114);//最後の高さ0との交点
  // uint8_t btn_w = 48;

  // if(ip.x >= 0 && ip.x < btn_w){
  //   return 0;
  // }else if(ip.x >= btn_w && ip.x < btn_w*2){
  //   return 1;
  // }else if(ip.x >= btn_w*2 && ip.x < btn_w*3){
  //   return 2;
  // }else if(ip.x >= btn_w*3 && ip.x < btn_w*4){
  //   return 3;
  // }else if(ip.x >= btn_w*4 && ip.x < btn_w*5){
  //   return 4;
  // }else{
  //   return 0;
  // }

    // float angle = getAngle(sp, tp);
          
    // if(angle <= 22.5 || angle > 337.5){
    //   eventState = RIGHT_FLICK;//0
    //   setEventBit(RIGHT_FLICK, 1);
    //   flickState = RIGHT_FLICK;//0
    // }else if(angle <= 67.5 && angle > 22.5){
    //   eventState = R_D_FLICK;//7
    //   setEventBit(R_D_FLICK, 1);
    //   flickState = R_D_FLICK;//7
    // }else if(angle <= 112.5 && angle > 67.5){
    //   eventState = DOWN_FLICK;//6
    //   setEventBit(DOWN_FLICK, 1);
    //   flickState = DOWN_FLICK;//6
    // }else if(angle <= 157.5 && angle > 112.5){
    //   eventState = D_L_FLICK;//5
    //   setEventBit(D_L_FLICK, 1);
    //   flickState = D_L_FLICK;//5
    // }else if(angle <= 202.5 && angle > 157.5){
    //   eventState = LEFT_FLICK;//4
    //   setEventBit(LEFT_FLICK, 1);
    //   flickState = LEFT_FLICK;//4
    // }else if(angle <= 247.5 && angle > 202.5){
    //   eventState = L_U_FLICK;//3
    //   setEventBit(L_U_FLICK, 1);
    //   flickState = L_U_FLICK;//3
    // }else if(angle <= 292.5 && angle > 247.5){
    //   eventState = UP_FLICK;//2
    //   setEventBit(UP_FLICK, 1);
    //   flickState = UP_FLICK;//2
    // }else if(angle <= 337.5 && angle > 292.5){
    //   eventState = U_R_FLICK;//1
    //   setEventBit(U_R_FLICK, 1);
    //   flickState = U_R_FLICK;//1
    // }

    // return flickState-11;
}

inline void M5GFX_DentaroUI::drawPngFile(LGFX_Sprite sprite, fs::SPIFFSFS* _SPIFFS,const char * _path, int32_t _x, int32_t _y){
  // sprite.drawPngFile(_SPIFFS, _path, _x, _y);
}

inline void M5GFX_DentaroUI::setConstantGetF(bool _constantGetF)
{
  constantGetF = _constantGetF;
}


    // uint8_t getCharSize(char c) {
    //     // 単純な実装例（UTF-8の場合）
    //     if ((c & 0b10000000) == 0) {
    //         // 1バイト文字
    //         return 1;
    //     } else if ((c & 0b11100000) == 0b11000000) {
    //         // 2バイト文字
    //         return 2;
    //     } else if ((c & 0b11110000) == 0b11100000) {
    //         // 3バイト文字
    //         return 3;
    //     } else {
    //         // その他の場合、1バイトとして処理
    //         return 1;
    //     }
    // }

inline std::string M5GFX_DentaroUI::delChar(const std::string& _flickStr) {
    // 文字列が空の場合はそのまま返す
    if (_flickStr.empty()) {
        return _flickStr;
    }

    // 最後の文字を取り除く
    std::string updatedStr = _flickStr.substr(0, _flickStr.size() - 1);

    // std::stringをstd::stringに変換して返す
    return updatedStr;
}

// std::string M5GFX_DentaroUI::delChar(std::string _flickStr) {
//   // 文字列が空の場合はそのまま返す
//   if (_flickStr.isEmpty()) {
//     return _flickStr;
//   }

//   // 最後の文字を取り除く
//   _flickStr.remove(_flickStr.length() - 1);

//   // 更新後の文字列を返す
//   return _flickStr;
// }


// 次のマルチバイト文字へのポインタを取得
inline const char *M5GFX_DentaroUI::next_c_mb(const char *c)
{
  int L = mblen(c, 10);
  return c + L;
}

// マルチバイト文字を一文字取り出す
inline void M5GFX_DentaroUI::ngetc(char *const dst, const char *src)
{
  int L = mblen(src, 10);
  memcpy(dst, src, L);
  dst[L] = '\0';
}

// マルチバイト文字を比較する
inline bool M5GFX_DentaroUI::nchr_cmp(const char *c1, const char *c2)
{
  int K = mblen(c1, 10);
  int L = mblen(c2, 10);

  if (K != L)
    return false;

  bool issame = (strncmp(c1, c2, K) == 0);
  return issame;
}

inline std::vector<std::string> M5GFX_DentaroUI::split_mb(const char *src, const char *del)
{

  char tmp[10];

  std::vector<std::string> result;

  std::string tmps;
  while (*src)
  {

    // デリミタを飛ばす
    //  const char* p = src;
    while (nchr_cmp(src, del) == true && *src != '\0')
      src = next_c_mb(src);

    // デリミタに遭遇するまで文字を追加し続ける
    while (nchr_cmp(src, del) != true && *src != '\0')
    {
      ngetc(tmp, src); // 一文字取り出す
      tmps += tmp;
      src = next_c_mb(src);
    }
    if (tmps.size())
    {
      result.push_back(tmps);
    }
    tmps.clear();
  }

  return result;
}

inline void M5GFX_DentaroUI::updateSelectBtnID(int _selectBtnID)
{
  selectBtnID = _selectBtnID;
}


inline void M5GFX_DentaroUI::createToggles(int _x, int _y, int _w, int _h, int _row, int _col, int _eventNo)
{
  toggle_mode = true;
  // createBtns(_x, _y, _w, _h, _row, _col, _eventNo);
  toggle_mode = false;
}

inline void M5GFX_DentaroUI::createFlick9Btns(LGFX_Sprite &_flickUiSprite)
{
  flickPanel.b_sNo = 0;
  flickPanel.id = 0;
  flickPanel.x = 0;
  flickPanel.y = 0;
  //  flickPanel.w   = 144;
  //  flickPanel.h   = 144;
  flickPanel.row = 3;
  flickPanel.col = 3;
  // flickPanel.eventNo = _eventNo;

  //  _lcd.setPsram( USE_PSRAM );
  // _lcd.setPsram(false);//UNUSE_PSRAM
  // _lcd.setColorDepth( COL_DEPTH );
  // _lcd.createSprite( 144, 144 );
  //  _lcd.createSprite( flickPanel.w, flickPanel.h );

  // btnNum = _row * _col ;
  int b_w = 48;
  int b_h = 48;

  for (int j = 0; j < 3; j++)
  {
    for (int i = 0; i < 3; i++)
    {
      // if( i*flickPanel.col + j > 0 ){
      //    flick_touch_btn_list.push_back( new TouchBtn() );
      // }
      // else
      // {
      //   flick_touch_btn_list[ i*flickPanel.col + j ] = NULL;
      //   flick_touch_btn_list[ 0 ] = new TouchBtn();
      // }

      flick_touch_btn_list[i * flickPanel.col + j] = NULL;
      flick_touch_btn_list[i * flickPanel.col + j] = new TouchBtn();

      flick_touch_btn_list[i * flickPanel.col + j]->initBtn(i * flickPanel.col + j, "a",
                                                            i * b_w,
                                                            j * b_h,
                                                            b_w,
                                                            b_h,
                                                            getTouchPoint(0, 0),
                                                            TOUCH_FLICK_MODE);
      // ベータ版は基本true
      flick_touch_btn_list[i * flickPanel.col + j]->setVisibleF(true);
      flick_touch_btn_list[i * flickPanel.col + j]->setAvailableF(true);
    }
  }
  flickPanel.b_num = 9; // UIのボタン数をセット
}

inline void M5GFX_DentaroUI::setFlickPanel(int _flickPanelID, int _btnID, std::string _btnsString)
{
  flickPanels[_flickPanelID]->new_text_list(_btnID, _btnsString);
}

inline std::string M5GFX_DentaroUI::getFlickStr()
{
  if (getTouchBtnID() != NO_EVENT)
  {
    // 押されたボタンのモードがFLICKだったら
    if (touch_btn_list[getTouchBtnID()]->getBtnMode() == TOUCH_FLICK_MODE)
    {

      std::string gettingText = flickPanels[showFlickPanelNo]->text_list[getTouchBtnID() - getUiFirstNo(FlickUiID)].c_str();
      std::string trimText = "";
      flickString = "";
      trimText = gettingText;

      float angle = getAngle(sp, tp);
      //      //Serial.println(angle);
      float dist = getDist(sp, tp);
      if (charMode == CHAR_3_BYTE)
      { // 日本語3ビットの場合
        // ５文字の場合
        if (dist > 24)
        {

          if (gettingText.size() == 15) // 5文字以下
          {
            charNumMode = CHAR_3_BYTE_5; // 5文字
            if (angle <= 22.5 || angle > 337.5)
            {
              flickString = trimText.substr(3 * 3, 3).c_str(); // え
              curKanaColNo = 3;
            }
            else if (angle <= 112.5 && angle > 67.5)
            {
              flickString = trimText.substr(4 * 3, 3).c_str(); // お
              curKanaColNo = 4;
            }
            else if (angle <= 202.5 && angle > 157.5)
            {
              flickString = trimText.substr(1 * 3, 3).c_str(); // い
              curKanaColNo = 1;
            }
            else if (angle <= 292.5 && angle > 247.5)
            {
              flickString = trimText.substr(2 * 3, 3).c_str(); // う
              curKanaColNo = 2;
            }
          }
          else if (gettingText.size() == 27) // 9文字
          {
            charNumMode = CHAR_3_BYTE_9; // 9文字
            if (angle <= 22.5 || angle > 337.5)
            {
              flickString = trimText.substr(5 * 3, 3).c_str(); // か
              curKanaColNo = 5;
            }
            else if (angle <= 112.5 && angle > 67.5)
            {
              flickString = trimText.substr(7 * 3, 3).c_str(); // く
              curKanaColNo = 7;
            }
            else if (angle <= 202.5 && angle > 157.5)
            {
              flickString = trimText.substr(1 * 3, 3).c_str(); // い
              curKanaColNo = 1;
            }
            else if (angle <= 292.5 && angle > 247.5)
            {
              flickString = trimText.substr(3 * 3, 3).c_str(); // え
              curKanaColNo = 3;
            }
            else if (angle <= 67.5 && angle > 22.5)
            {
              flickString = trimText.substr(6 * 3, 3).c_str(); // き
              curKanaColNo = 6;
            }
            else if (angle <= 157.5 && angle > 112.5)
            {
              flickString = trimText.substr(8 * 3, 3).c_str(); // け
              curKanaColNo = 8;
            }
            else if (angle <= 247.5 && angle > 202.5)
            {
              flickString = trimText.substr(2 * 3, 3).c_str(); // う
              curKanaColNo = 2;
            }
            else if (angle <= 337.5 && angle > 292.5)
            {
              flickString = trimText.substr(4 * 3, 3).c_str(); // お
              curKanaColNo = 4;
            }
          }
        }
        else if (dist <= 24)
        {
          flickString = trimText.substr(0, 3).c_str(); // あ
          curKanaColNo = 0;
        }
        return flickString;
      }
      else if (charMode == CHAR_1_BYTE)
      { // 英数１ビットの場合
        if (dist > 24)
        {

          if (gettingText.size() <= 5) // 1バイト文字5文字（英語）以下の場合
          {
            charNumMode = CHAR_1_BYTE_5; // 5文字
            flickString = "";
            if (angle <= 22.5 || angle > 337.5)
            {
              if (trimText.length() >= 4)
                flickString = trimText.substr(3, 1).c_str(); // D
            }

            if (angle <= 112.5 && angle > 67.5 && trimText.length() >= 5)
            {
              flickString = trimText.substr(4, 1).c_str(); // E
            }

            if (angle <= 202.5 && angle > 157.5 && trimText.length() >= 2)
            {
              flickString = trimText.substr(1, 1).c_str(); // B
            }

            if (angle <= 292.5 && angle > 247.5 && trimText.length() >= 3)
            {
              flickString = trimText.substr(2, 1).c_str(); // C
            }
          }

          if (gettingText.size() == 9) // 1バイト文字5文字（英語）以下の場合
          {
            charNumMode = CHAR_1_BYTE_9; // 9文字
            flickString = "";

            if (angle <= 22.5 || angle > 337.5)
            {
              flickString = trimText.substr(5, 1).c_str(); // かF
              // curKanaColNo = 5;
            }
            else if (angle <= 112.5 && angle > 67.5)
            {
              flickString = trimText.substr(7, 1).c_str(); // くH
              // curKanaColNo = 7;
            }
            else if (angle <= 202.5 && angle > 157.5)
            {
              flickString = trimText.substr(1, 1).c_str(); // いB
              // curKanaColNo = 1;
            }
            else if (angle <= 292.5 && angle > 247.5)
            {
              flickString = trimText.substr(3, 1).c_str(); // えD
              // curKanaColNo = 3;
            }
            else if (angle <= 67.5 && angle > 22.5)
            {
              flickString = trimText.substr(6, 1).c_str(); // きG
              // curKanaColNo = 6;
            }
            else if (angle <= 157.5 && angle > 112.5)
            {
              flickString = trimText.substr(8, 1).c_str(); // けI
              // curKanaColNo = 8;
            }
            else if (angle <= 247.5 && angle > 202.5)
            {
              flickString = trimText.substr(2, 1).c_str(); // うC
              // curKanaColNo = 2;
            }
            else if (angle <= 337.5 && angle > 292.5)
            {
              flickString = trimText.substr(4, 1).c_str(); // おE
              // curKanaColNo = 4;
            }
          }
        }
        else if (dist <= 24)
        {
          flickString = trimText.substr(0, 1).c_str(); // A
        }
        return flickString;
      }
    }
  }
  return "";
}

inline void M5GFX_DentaroUI::drawFlickBtns(M5GFX &_lcd, int _btnID, int _btnNo, bool _visible, bool _available)
{

  if (_btnID != NO_EVENT)
  {

    // 押されたボタンのモードがFLICKだったら
    if (touch_btn_list[getTouchBtnID()]->getBtnMode() == TOUCH_FLICK_MODE)
    {
      // if(_visible){
      //  _lcd.setPivot( 0, 0 );//setPivot()で回転する場合の原点を指定します。初期値は左上の(0, 0)だと思います

      // _lcd.fillScreen(TFT_RED);

      std::string panelText = "０１２３４５６７８"; // デフォルトの文字セット
      // std::string prePanelText ="";
      std::string gettingText = flickPanels[showFlickPanelNo]->text_list[_btnNo].c_str(); // Arduinoのstd::string型をc_str()で、std::string型に直してから、渡す
      std::string trimText = "";

      if (gettingText.size() <= 5) // 1バイト文字5文字（英語）以下の場合
      {
        panelText = "";
        for (int i = 0; i < 9; i++)
        {
          int swapArray[9] =
              {5, 2, 6,
               1, 0, 3,
               8, 4, 7};
          if (swapArray[i] < gettingText.size())
          {
            trimText = gettingText;
            panelText += trimText.substr(swapArray[i], 1);
            panelText += " ";
            panelText += " ";
          }
          else
          {
            panelText += "　";
          }
        }
      }
      else if (gettingText.size() == 9) // 1バイト文字9文字（英語）以下の場合
      {
        panelText = "";
        for (int i = 0; i < 9; i++)
        {
          int swapArray[9] =
              {2, 3, 4,
               1, 0, 5,
               8, 7, 6};
          if (swapArray[i] < gettingText.size())
          {
            trimText = gettingText;
            panelText += trimText.substr(swapArray[i], 1);
            panelText += " ";
            panelText += " ";
          }
          else
          {
            panelText += "　";
          }
        }
      }
      else if (gettingText.size() == 15) // ２バイト文字（日本語）5文字の場合
      {
        panelText = "";

        panelText += "　"; //"５";//gettingText.substr(5*3,3).c_str();か
        trimText = gettingText;
        panelText += trimText.substr(2 * 3, 3); // う
        panelText += "　";                      //"８";//gettingText.substr(8*3,3).c_str();け

        trimText = gettingText;
        panelText += trimText.substr(1 * 3, 3); // い
        trimText = gettingText;
        panelText += trimText.substr(0, 3); // あ
        trimText = gettingText;
        panelText += trimText.substr(3 * 3, 3); // え

        panelText += "　"; //"６";//gettingText.substr(6*3,3).c_str();き
        trimText = gettingText;
        panelText += trimText.substr(4 * 3, 3); // お
        panelText += "　";                      //"７";//gettingText.substr(7*3,3).c_str();く
      }
      else if (gettingText.size() == 27) // 9文字の場合
      {
        panelText = "";

        trimText = gettingText;
        panelText += trimText.substr(2 * 3, 3); // う
        trimText = gettingText;
        panelText += trimText.substr(3 * 3, 3); // え
        trimText = gettingText;
        panelText += trimText.substr(4 * 3, 3); // お

        trimText = gettingText;
        panelText += trimText.substr(1 * 3, 3); // い
        trimText = gettingText;
        panelText += trimText.substr(0, 3); // あ
        trimText = gettingText;
        panelText += gettingText.substr(5 * 3, 3); // か

        trimText = gettingText;
        panelText += gettingText.substr(8 * 3, 3); // け
        trimText = gettingText;
        panelText += gettingText.substr(7 * 3, 3); // く
        trimText = gettingText;
        panelText += gettingText.substr(6 * 3, 3); // き
      }

      // panelText.replace( 0, gettingText.size(), gettingText );//　"あああ"　がきたら　"あああ３４５６７８"　のように置き換える

      // 以下なぜか動かず、、、
      //  if(panelText.size()<27){//3バイト*9文字以下なら
      //    panelText.replace(0,gettingText.size(), gettingText);//　"あああ"　がきたら　"あああ３４５６７８"　のように置き換える
      //  }else{
      //    panelText.substr(0,27);//3バイト*9文字のみ切り出す
      //  }

      // フリックパネルの文字を描画
      for (int j = 0; j < 3; j++)
      {
        for (int i = 0; i < 3; i++)
        {
          _lcd.setTextSize(1);

          flick_touch_btn_list[j * 3 + i]->setBtnName(panelText.substr((j * 3 + i) * 3, 3).c_str()); // std::string型の状態で一文字切り出して、std::stringに渡す
          // flick_touch_btn_list[ j*3+i ]->setBtnName(panelText.substr(3,3).c_str());//std::string型の状態で一文字切り出して、std::stringに渡す

          // std::string btn_name = flickPanels[showFlickPanelNo]->btn_name.c_str();
          std::string btn_name = flick_touch_btn_list[j * 3 + i]->getBtnName();

          // //Serial.print(panelText.substr((j*3+i)*3,3).c_str());
          // //Serial.println(btn_name);

          _lcd.fillRoundRect(
              // flick_touch_btn_list[i]->getBtnPos().x,
              // flick_touch_btn_list[i]->getBtnPos().y,
              48 * i,
              48 * j,
              48, // flick_touch_btn_list[i]->getBtnSize().w,
              48, // flick_touch_btn_list[i]->getBtnSize().h,
              10, TFT_WHITE);

          if (btn_name.empty())
          {
            btn_name = std::to_string(j * 3 + i);
          }

          _lcd.setTextColor(TFT_BLACK);
          _lcd.setFont(&lgfxJapanGothicP_16);
          int b_hw = 48 / 2;
          int b_hh = 48 / 2;
          float s_str_hw = _lcd.textWidth(btn_name.c_str()) / 2;
          // _lcd.drawString( flick_touch_btn_list[ j*3+i ]->getBtnName(), 48*i + b_hw - s_str_hw , 48*j + b_hh - 4 );

          // std::string btn_name_headline = "";
          
          // if(btn_name == "A")btn_name_headline = "ABC";
          // else if(btn_name == "a")btn_name_headline = "abc";

          // _lcd.drawString( btn_name_headline, 48 * i + b_hw - s_str_hw, 48 * j + b_hh - 4);
          _lcd.drawString(btn_name.c_str(), 48 * i + b_hw - s_str_hw, 48 * j + b_hh - 4);
        }
      }
      // _lcd.fillRoundRect(0, 0, 50, 50, 10, TFT_WHITE);
      // _lcd.pushSprite(&_lcd, _x, _y);
      // }
      // else if(!_visible){
      //_lcd.fillRoundRect(flick_touch_btn_list[i]->, 0, 48, 48, 10, TFT_WHITE);
      // _lcd.pushSprite(&_lcd, getBtnPos(_btnID).x, getBtnPos(_btnID).y);
      // _lcd.pushSprite(&_lcd, getBtnPos(_btnID).x, getBtnPos(_btnID).y);
      // }
    }
  }
}

inline void M5GFX_DentaroUI::createOBtns(
    int _r0, int _r1, int _a, int _n, int _eventNo)
{
  int _x = 125; // 初期値を適当に入れておく
  int _y = 245; // 初期値を適当に入れておく
  createOBtns(_x, _y, _r0, _r1, _a, _n, _eventNo);
}

inline void M5GFX_DentaroUI::createOBtns(
    int _x, int _y, int _r0, int _r1, int _a, int _n, int _eventNo)
{ // 円形に並ぶ//中心位置XY、外半径r0、内半径r1、スタート角、分割数

  // int _x, int _y,
  // int _w,int _h,
  // int _row, int _col,
  // LGFX_Sprite& _uiSprite, int _eventNo)
  // panels.push_back(*new Panel);

  // panels_num++;
  // uiId++;
  // uis[_uiId]->panels[0]->label = "BTN_" + std::to_string(uiId);
  // //Serial.println("BTN_" + std::to_string(uiId) + "=[" + std::to_string(uiId) + "]" + std::to_string(gBtnID) + "~");

  // int _startId = gBtnID; // スタート時のボタンIDをセット
  // uis[_uiId]->panels[0]->b_sNo = gBtnID;
  // uis[_uiId]->panels[0]->id = uiId;
  // uis[_uiId]->panels[0]->x = _x; // - _r1;
  // uis[_uiId]->panels[0]->y = _y; // - _r1;

  // // uis[_uiId]->panels[0]->w   = _w;
  // // uis[_uiId]->panels[0]->h   = _h;
  // // uis[_uiId]->panels[0]->row = _row;
  // // uis[_uiId]->panels[0]->col = _col;

  // uis[_uiId]->panels[0]->r0 = _r0;
  // uis[_uiId]->panels[0]->r1 = _r1;
  // uis[_uiId]->panels[0]->a = _a;
  // uis[_uiId]->panels[0]->n = _n;

  // uis[_uiId]->panels[0]->eventNo = _eventNo;
  // uis[_uiId]->panels[0]->toggle_mode = toggle_mode;

  // // _uiSprite.setPsram(false);//UNUSE_PSRAM
  // // _uiSprite.setPsram(USE_PSRAM);
  // // _uiSprite.setColorDepth( COL_DEPTH );
  // // _uiSprite.createSprite( 2*uis[_uiId]->panels[0]->r1, 2*uis[_uiId]->panels[0]->r1 );
  // // _uiSprite.createSprite( 150, 150 );

  // int b_w = int(uis[_uiId]->panels[0]->r0 - uis[_uiId]->panels[0]->r1); // ボタン幅を計算

  // for (int i = 0; i < uis[_uiId]->panels[0]->n; i++)
  // {

  //   int p_btnID = _startId + i; // 事前に計算

  //   touch_btn_list[p_btnID] = NULL;
  //   touch_btn_list[p_btnID] = new TouchBtn();

  //   touch_btn_list[p_btnID]->initOBtn(
  //       p_btnID,
  //       std::to_string(p_btnID),
  //       i,
  //       uis[_uiId]->panels[0]->a, // 描画のスタート角（右から時計回り）
  //       uis[_uiId]->panels[0]->x,
  //       uis[_uiId]->panels[0]->y,
  //       uis[_uiId]->panels[0]->r1,
  //       uis[_uiId]->panels[0]->r0,
  //       uis[_uiId]->panels[0]->n, // 分割数
  //       std::to_string(p_btnID),
  //       // uiSpritePos,
  //       getTouchPoint(_x, _y),
  //       TOUCH_OBTN_MODE);

  //   touch_btn_list[p_btnID]->setVisibleF(true);
  //   touch_btn_list[p_btnID]->setAvailableF(true);

  //   gBtnID++; // ボタンを更新
  // }
  // uis[_uiId]->panels[0]->b_num = gBtnID - uis[_uiId]->panels[0]->b_sNo; // UIのボタン数をセット
}

inline void M5GFX_DentaroUI::deletePanels(int _uiContainerID, uint8_t _PRESET_BTN_NUM) {
  if(_uiContainerID==1){
    if (_uiContainerID < uis.size()) {
        for (auto panel : uis[_uiContainerID]->panels) {
            delete panel;
        }
        uis[_uiContainerID]->panels.clear();
        // panelsベクターも削除する
        uis[_uiContainerID]->panels.shrink_to_fit();
    }
    gBtnID = _PRESET_BTN_NUM;//再設定のため、ボタンIDを初期化
    gPanelID = 1;//再設定のため、UIIDを初期化
  }
}

inline void M5GFX_DentaroUI::setAvailableF(int _uiId, int _btnID, bool _available)
{
  int _startId = uis[_uiId]->panels[0]->b_sNo; // スタート時のボタンIDをセット
  touch_btn_list[_startId + _btnID]->setAvailableF(_available);
}

inline void M5GFX_DentaroUI::setAllBtnAvailableF(int _uiId, bool _available)
{
  int _startId = uis[_uiId]->panels[0]->b_sNo; // スタート時のボタンIDをセット
  for (int i = 0; i < uis[_uiId]->panels[0]->row; i++)
  {
    for (int j = 0; j < uis[_uiId]->panels[0]->col; j++)
    {
      touch_btn_list[_startId + j * uis[_uiId]->panels[0]->row + i]->setAvailableF(_available);
    }
  }
}

inline void M5GFX_DentaroUI::setVisibleF(int _uiId, int _btnID, bool _visible)
{
  int _startId = uis[_uiId]->panels[0]->b_sNo; // スタート時のボタンIDをセット
  touch_btn_list[_startId + _btnID]->setVisibleF(_visible);
}

inline void M5GFX_DentaroUI::setAllBtnVisibleF(int _uiId, bool _visible)
{
  int _startId = uis[_uiId]->panels[0]->b_sNo; // スタート時のボタンIDをセット
  for (int i = 0; i < uis[_uiId]->panels[0]->row; i++)
  {
    for (int j = 0; j < uis[_uiId]->panels[0]->col; j++)
    {
      touch_btn_list[_startId + j * uis[_uiId]->panels[0]->row + i]->setVisibleF(_visible);
    }
  }
}

inline void M5GFX_DentaroUI::setQWERTY(int _uiId, std::string _btnsString, LGFX_Sprite &_sprite)
{
  int charsNum = _btnsString.length();
  const char *btnChars = _btnsString.c_str();
  int _id = uis[_uiId]->panels[0]->b_sNo;

  for (int i = 0; i < charsNum; i++)
  {
    touch_btn_list[_id + i]->setVisibleF(true);
    // touch_btn_list[_id + i]->setAvailableF(true);
    touch_btn_list[_id + i]->setBtnName(std::to_string(btnChars[i]));
    // touch_btn_list[_id + i]->setColor(_bgColIndex);
  }
}

inline void M5GFX_DentaroUI::setBtnName(int _btnNo, std::string _btnName, std::string _btnNameFalse)
{
  int _btnId = _btnNo;
  touch_btn_list[_btnId]->setBtnName(_btnName);
  // touch_btn_list[_btnId]->setBtnNameFalse(_btnNameFalse); // トグルのfalse状態の時のラベルを設定
}

inline void M5GFX_DentaroUI::setBtnName(int _uiId, int _panelId, int _btnNo, std::string _btnName)
{
  int _btnId = _btnNo;
  // panels[_btnId]->setAvailableF(true);
  // panels[_btnId]->setVisibleF(true);
  // panels[_btnId]->setAvailableF(true);
  uis[_uiId]->panels[_panelId]->setBtnName(_btnName);
  // panels[_btnId]->setColor(_bgColIndex);
}


inline bool M5GFX_DentaroUI::readFlickData(int _uiId, int startLine) {
    std::ifstream file("/init/param/flickdata.csv");
    if (!file.is_open()) {
        std::cout << "Failed to open file" << std::endl;
        return false;
    }

    std::string line;
    int j = 0;
    int currentLine = 0;

    // 読み込み
    while (std::getline(file, line) && j < 12) {
        if (currentLine >= startLine) {
            // 13行目から処理を開始
            line.erase(0, line.find_first_not_of(" \t")); // 行の前のスペースを削除
            size_t commaIndex = line.find(',');
            if (commaIndex != std::string::npos) {
                std::string str = line.substr(0, commaIndex); // 先頭文字をラベルにする
                uis[_uiId]->panels[0]->touchBtns[j]->namelabel = str;
                j++;
            }
        }
        currentLine++;
    }

    file.close();
    return true;
}

// bool M5GFX_DentaroUI::readFlickData(int _uiId, int startLine) {

//     File fr = SPIFFS.open("/init/param/flickdata.csv", "r");
//     if (!fr) {
//         //Serial.println("Failed to open file");
//         return false;
//     }

//     std::string line;
//     int j = 0;
//     int currentLine = 0;

//     // 読み込み
//     while (fr.available() && j < 12) {
//         char c = fr.read();
//         if (c == '\n') {
//             if (currentLine >= startLine) {
//                 // 13行目から処理を開始
//                 line.trim(); // 文字列の前後のスペースを削除
//                 int commaIndex = line.indexOf(',');
//                 if (commaIndex != -1) {
//                     std::string stdstr = line.substring(commaIndex + 1).c_str();
//                     std::string str = line.substring(0, commaIndex).c_str(); // 先頭文字をラベルにする
//                     uis[_uiId]->panels[0]->touchBtns[j]->namelabel = str;
//                     j++;
//                 }
//             }
//             currentLine++;
//             line = ""; // 行をリセット
//         } else {
//             line += c;
//         }
//     }
//     fr.close();
//     return true;
// }

inline std::string M5GFX_DentaroUI::getFlickChar(int _selectbtnID){
  std::string c = uis[1]->panels[3]->touchBtns[_selectbtnID]->shift_btn_name;
  return c;
}

inline std::string M5GFX_DentaroUI::getFlickChar(int _PRESET_BTN_NUM, int _uiId, int _btnID){
  // std::string c = uis[_uiId]->panels[3]->touchBtns[_btnID - 20 - _PRESET_BTN_NUM]->namelabel;
  std::string c = uis[_uiId]->panels[3]->touchBtns[_btnID - 20 - _PRESET_BTN_NUM]->shift_btn_name;

  return c;
}


inline std::string M5GFX_DentaroUI::getFlickline(int flickDatalineNo) {
    std::ifstream file("/init/param/flickdata.csv");
    if (!file.is_open()) {
        std::cout << "Failed to open file" << std::endl;
        return "";
    }

    std::string line;
    int currentLine = 0;

    // ファイルを行単位で読み込み
    while (std::getline(file, line)) {
        if (currentLine == flickDatalineNo) {
            // 指定した行数に達したらその行を返す
            // 必要に応じて行の前後のスペースを削除するなどの処理を行うこともできます
            file.close();
            return line;
        }
        currentLine++;
    }

    // 指定された行数がファイル内に存在しない場合は空文字列を返す
    file.close();
    return "";
}
// std::string M5GFX_DentaroUI::getFlickline(int flickDatalineNo) {
//     File fr = SPIFFS.open("/init/param/flickdata.csv", "r");
//     if (!fr) {
//         //Serial.println("Failed to open file");
//         return "";
//     }

//     std::string line;
//     int currentLine = 0;

//     // 読み込み
//     while (fr.available() && currentLine <= flickDatalineNo) {
//         char c = fr.read();
//         if (c == '\n') {
//             if (currentLine == flickDatalineNo) {
//                 // flickDatalineNo 番目の行を取得
//                 line.trim(); // 文字列の前後のスペースを削除
//                 fr.close();
//                 return line;
//             }
//             currentLine++;
//             line = ""; // 行をリセット
//         } else {
//             line += c;
//         }
//     }
//     fr.close();
//     return "";
// }

inline void M5GFX_DentaroUI::readFlickDataline(int _PRESET_BTN_NUM, int _uiId, int _btnID) {
    int flickDatalineNo = flickShiftNo * 12 + _btnID - _PRESET_BTN_NUM;
    std::string linedata = getFlickline(flickDatalineNo);
    
    // カンマ区切りで文字列を分割してpanels[3]->touchBtns[i]->namelabelに格納
    size_t start = 0;
    size_t end = 0;
    for (int i = 0; i < 5; i++) {
        end = linedata.find(',', start); // カンマを検索
        if (end != std::string::npos) {
            // カンマが見つかった場合
            uis[_uiId]->panels[3]->touchBtns[i]->namelabel = linedata.substr(start, end - start);
            start = end + 1; // 次の部分文字列の開始位置を設定
        } else {
            // カンマが見つからない場合
            uis[_uiId]->panels[3]->touchBtns[i]->namelabel = linedata.substr(start);
            break;
        }
    }
}

// void M5GFX_DentaroUI::readFlickDataline(int _PRESET_BTN_NUM, int _uiId, int _btnID) {
//   int flickDatalineNo = flickShiftNo * 12 + _btnID - _PRESET_BTN_NUM;//プリセットボタンの数を引いて0~12の値にする
//   std::string linedata = getFlickline(flickDatalineNo);
//   // スプリットして panels[3]->touchBtns[i]->namelabel に格納
//   for (int i = 0; i < 5; i++) {
//       int commaIndex = linedata.indexOf(',');
//       if (commaIndex != -1) {
//           uis[_uiId]->panels[3]->touchBtns[i]->namelabel = linedata.substring(0, commaIndex).c_str();
//           linedata = linedata.substring(commaIndex + 1);
//       } else {
//           // カンマが見つからない場合
//           uis[_uiId]->panels[3]->touchBtns[i]->namelabel = linedata;
//           break;
//       }
//   }
// }

inline void M5GFX_DentaroUI::flickShift(){
  
  flickShiftNo++;
  flickShiftNo%=5;
  // preflickShiftNo = flickShiftNo;
  kanaShiftNo=0;
}

inline void M5GFX_DentaroUI::kanaShift(uint8_t _kanaShiftNo){
    kanaShiftNo = _kanaShiftNo;
}
inline void M5GFX_DentaroUI::kanaShift(){
  // if(flickShiftNo != preflickShiftNo)
  // {
  //   kanaShiftNo=0;
  // }else{
    kanaShiftNo++;
    kanaShiftNo %=3;
  // }
}

inline void M5GFX_DentaroUI::drawFlicks(int _uiId, M5GFX &_lcd){
  // ファイルからデータを読み込む
  // if(_btn_mode == TOUCH_FLICK_MODE){
    readFlickData(_uiId, flickShiftNo * 12);

    // ボタンの描画などの処理
    // int k=1;//カスタムUIコンテナを指定
      for (int i = 0; i < uis[_uiId]->panels.size(); i++) {
          for (int j = 0; j < uis[_uiId]->panels[i]->b_num; j++) {
              int x = uis[_uiId]->panels[i]->touchBtns[j]->s_x;
              int y = uis[_uiId]->panels[i]->touchBtns[j]->s_y;
              int w = uis[_uiId]->panels[i]->touchBtns[j]->s_w;
              int h = uis[_uiId]->panels[i]->touchBtns[j]->s_h;
              std::string namelabel = uis[_uiId]->panels[i]->touchBtns[j]->namelabel;
              // uis[_uiId]->panels[i]->touchBtns[j]->btnDraw(_lcd, x, y, w, h, namelabel, uis[_uiId]->panels[i]->touchBtns[j]->btnID, 0, kanaShiftNo);
              uis[_uiId]->panels[i]->panelbtnDraw(_lcd, x, y, w, h, namelabel, uis[_uiId]->panels[i]->touchBtns[j]->btnID, kanaShiftNo);
          }
      }
  // }
}

inline void M5GFX_DentaroUI::drawBtns(int _uiId, M5GFX &_lcd)
{
  if (getEvent() != NO_EVENT)
  {
    if (getEvent() == uis[_uiId]->panels[0]->eventNo || uis[_uiId]->panels[0]->eventNo == MULTI_EVENT)
    {
      for (int i = 0; i < uis[_uiId]->panels.size(); i++) {
        for (int j = 0; j < uis[_uiId]->panels[i]->b_num; j++) {
          int x = uis[_uiId]->panels[i]->touchBtns[j]->s_x;
          int y = uis[_uiId]->panels[i]->touchBtns[j]->s_y;
          int w = uis[_uiId]->panels[i]->touchBtns[j]->s_w;
          int h = uis[_uiId]->panels[i]->touchBtns[j]->s_h;

          std::string namelabel = uis[_uiId]->panels[i]->touchBtns[j]->label;
          uis[_uiId]->panels[i]->touchBtns[j]->btnDraw(_lcd);
          // uis[_uiId]->panels[i]->btnDraw(_lcd, x, y, w, h, namelabel);
          
          
        }
      }
    }
  }
}

inline void M5GFX_DentaroUI::drawSliders(int _uiId, M5GFX &_lcd)
{
  if (getEvent() != NO_EVENT)
  {
    if (getEvent() == uis[_uiId]->panels[0]->eventNo || uis[_uiId]->panels[0]->eventNo == MULTI_EVENT)
    {
      for (int i = 0; i < uis[_uiId]->panels.size(); i++) {
        for (int j = 0; j < uis[_uiId]->panels[i]->b_num; j++) {
          int x = uis[_uiId]->panels[i]->touchBtns[j]->s_x;
          int y = uis[_uiId]->panels[i]->touchBtns[j]->s_y;
          int w = uis[_uiId]->panels[i]->touchBtns[j]->s_w;
          int h = uis[_uiId]->panels[i]->touchBtns[j]->s_h;

          std::string label = uis[_uiId]->panels[i]->touchBtns[j]->label;
          uis[_uiId]->panels[i]->touchBtns[j]->sliderDraw(_lcd);
        }
      }
    }
  }
}

inline void M5GFX_DentaroUI::drawToggles(int _uiId, M5GFX &_lcd)
{
  if (getEvent() == TOUCH)
  {
    if (_uiId >= 0)
    { // NULLを除外
      switchToggleVal();
      drawToggles(_uiId, _lcd, uis[_uiId]->panels[_uiId]->x, uis[_uiId]->panels[_uiId]->y);
    }
    else
    {
    }
  }
}

inline void M5GFX_DentaroUI::drawOBtns(int _uiId, M5GFX &_lcd, int _x, int _y)
{
  if (getEvent() != NO_EVENT)
  {
    if (getEvent() == uis[_uiId]->panels[0]->eventNo || uis[_uiId]->panels[0]->eventNo == MULTI_EVENT)
    {
      int _id = uis[_uiId]->panels[0]->b_sNo;
      int _btnNum = uis[_uiId]->panels[0]->b_num;
      // _uiSprite.setPivot( 0, 0 );//setPivot()で回転する場合の原点を指定します。初期値は左上の(0, 0)だと思います
      

      for (int i = _id; i < _id + _btnNum; i++)
      {
        uis[_uiId]->panels[i]->setSelectBtnF(false);
        if (selectBtnID == i)
          uis[_uiId]->panels[i]->setSelectBtnF(true);
        else
          uis[_uiId]->panels[i]->setSelectBtnF(false);
          // uis[_uiId]->panels[i]->panelbtnDraw(_lcd, 0, 0, 38, 48,"", gBtnID,0); // ボタンエリアの半径
          // uis[_uiId]->panels[i]->panelbtnDraw(_lcd, 0, 0, 38, 48, "", gBtnID, 0, kanaShiftNo);
          // uis[_uiId]->panels[i]->btnDraw(_uiSprite, uis[_uiId]->panels[0]->x, uis[_uiId]->panels[0]->y); //スプライト経由で描画する
      }
    }

    int _id = uis[_uiId]->panels[0]->b_sNo;

    for (int i = 0; i < uis[_uiId]->panels[0]->n; i++)
    {
      int p_btnID = _id + i; // 事前に計算
      // タッチポイントをUI表示位置にずらす

      // panels[p_btnID]->setOBtnPos(
      //     _x + 180 / 2,
      //     _y + 180 / 2);
    }
  }
}

inline void M5GFX_DentaroUI::drawToggles(int _uiId, M5GFX &_lcd, int _x, int _y)
{

  // if(panels[_uiId]->toggle_mode  == true){
  //   drawBtns(_uiId, _lcd, _uiSprite, _x, _y);
  // }
  toggle_mode = true;
  // drawBtns(_uiId, _lcd, _x, _y);
  toggle_mode = false;
}

// void M5GFX_DentaroUI::createFlicks(int _x, int _y, int _w, int _h, int _row, int _col, int _eventNo)
// { // 縦方向に並ぶ
//   // panels.push_back(*new Panel);
//   panels_num++;
//   uiId++;
//   uis[uiId]->panels[0]->label = "FLICK_" + std::to_string(uiId);
//   //Serial.println("FLICK_" + std::to_string(uiId) + "=[" + std::to_string(uiId) + "]" + std::to_string(gBtnID) + "~");

//   int _startId = gBtnID; // スタート時のボタンIDをセット
//   uis[uiId]->panels[0]->b_sNo = gBtnID;
//   uis[uiId]->panels[0]->id = uiId;
//   uis[uiId]->panels[0]->x = _x;
//   uis[uiId]->panels[0]->y = _y;
//   uis[uiId]->panels[0]->w = _w;
//   uis[uiId]->panels[0]->h = _h;
//   uis[uiId]->panels[0]->row = _row;
//   uis[uiId]->panels[0]->col = _col;
//   uis[uiId]->panels[0]->eventNo = _eventNo;
//   uis[uiId]->panels[0]->toggle_mode = false;

//   int b_w = int(uis[uiId]->panels[0]->w / uis[uiId]->panels[0]->row);
//   int b_h = int(uis[uiId]->panels[0]->h / uis[uiId]->panels[0]->col);

//   for (int i = 0; i < uis[uiId]->panels[0]->row; i++)
//   {
//     for (int j = 0; j < uis[uiId]->panels[0]->col; j++)
//     {
//       int p_btnID = _startId + j * uis[uiId]->panels[0]->row + i;

//       touch_btn_list[p_btnID] = NULL;
//       touch_btn_list[p_btnID] = new TouchBtn();
//       touch_btn_list[p_btnID]->initBtn(p_btnID, std::to_string(p_btnID),
//                                        i * b_w,
//                                        j * b_h,
//                                        b_w,
//                                        b_h,
//                                        // "あ",//str, //先頭文字をラベルにする
//                                        // // uiSpritePos,
//                                        getTouchPoint(_x, _y),
//                                        TOUCH_FLICK_MODE);

//       // ベータ版は基本true
//       touch_btn_list[p_btnID]->setVisibleF(true);
//       touch_btn_list[p_btnID]->setAvailableF(true);

//       gBtnID++; // ボタンを更新
//     }
//   }
//   uis[uiId]->panels[0]->b_num = gBtnID - uis[uiId]->panels[0]->b_sNo; // UIのボタン数をセット
// }

inline bool M5GFX_DentaroUI::isAvailable(int _btnID)
{
  return touch_btn_list[_btnID]->getAvailableF();
}


inline float M5GFX_DentaroUI::getDist(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b)
{
  float d = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
  return d;
}

inline int M5GFX_DentaroUI::getPreEvent()
{
  return preEventState;
}

inline void M5GFX_DentaroUI::circle(M5GFX *_lcd, uint16_t c, int fillF)
{
  if (fillF)
  {
    _lcd->fillCircle(tp.x, tp.y, 10, c);
    _lcd->fillCircle(tp.x, tp.y, 10, c);
  }
  else
  {
    _lcd->drawCircle(tp.x, tp.y, 10, c);
    _lcd->drawCircle(tp.x, tp.y, 10, c);
  }
}

inline void M5GFX_DentaroUI::rect(M5GFX *_lcd, uint16_t c, int fillF)
{
  if (fillF)
  {
    _lcd->fillRect(tp.x, tp.y, 10, 10, c);
    _lcd->fillRect(tp.x, tp.y, 10, 10, c);
  }
  else
  {
    _lcd->drawRect(tp.x, tp.y, 10, 10, c);
    _lcd->drawRect(tp.x, tp.y, 10, 10, c);
  }
}

inline lgfx::v1::touch_point_t M5GFX_DentaroUI::getPos()
{
  return tp;
}

inline lgfx::v1::touch_point_t M5GFX_DentaroUI::getStartPos()
{
  return sp;
}

inline void M5GFX_DentaroUI::setPos(int _x, int _y)
{
  tp.x = _x;
  tp.y = _y;
}

inline void M5GFX_DentaroUI::setStartPos(int _x, int _y)
{
  sp.x = _x;
  sp.y = _y;
}

inline int M5GFX_DentaroUI::getTouchBtnID()
{
  return obj_ret.btnID;
}

inline int M5GFX_DentaroUI::getTouchBtnNo()
{
  return getTouchBtnID() - getUiFirstNo(FlickUiID);
}

// int M5GFX_DentaroUI::getEvent()
// {
//   return eventState;
// }

inline int M5GFX_DentaroUI::getFlickEvent()
{
  return flickState;
}

inline lgfx::v1::touch_point_t M5GFX_DentaroUI::getTouchPoint(int _x, int _y)
{
  lgfx::v1::touch_point_t tp;
  tp.x = _x;
  tp.y = _y;
  return tp;
}

inline float M5GFX_DentaroUI::getSliderVal(int _uiId, int _panel_no, int _btnNo)
{
  float retVal = 1.0;
  int _btnID = uis[_uiId]->panels[_panel_no]->b_sNo + _btnNo;
  if (touch_btn_list[_btnID]->get_xy_mode() == X_VAL)
  {
    retVal = touch_btn_list[_btnID]->getSliderValx();
  }
  else if (touch_btn_list[_btnID]->get_xy_mode() == Y_VAL)
  {
    retVal = touch_btn_list[_btnID]->getSliderValy();
  }
  return retVal;
}

inline float M5GFX_DentaroUI::getSliderVal(int _uiId, int _panel_no, int _btnNo, int _xy)
{
  float retVal = 1.0;
  int _btnID = uis[_uiId]->panels[_panel_no]->b_sNo + _btnNo;
  if (_xy == X_VAL)
  {
    retVal = uis[_uiId]->panels[_panel_no]->touchBtns[_btnNo]->getSliderValx();
  }
  else if (_xy == Y_VAL)
  {
    retVal = uis[_uiId]->panels[_panel_no]->touchBtns[_btnNo]->getSliderValy();
  }
  return retVal;
}

inline Vec2 M5GFX_DentaroUI::getSliderVec2(int _uiId, int _btnNo)
{ // XYを一気に取得
  Vec2 retPos;
  int _btnID = uis[_uiId]->panels[0]->b_sNo + _btnNo;
  if (uis[_uiId]->panels[0]->touchBtns[_btnNo]->get_xy_mode() == XY_VAL)
  {
    retPos.x = uis[_uiId]->panels[0]->touchBtns[_btnNo]->getSliderValx();
    retPos.y = uis[_uiId]->panels[0]->touchBtns[_btnNo]->getSliderValy();
  }
  return retPos;
}

inline void M5GFX_DentaroUI::setSliderVal(int _uiId,int _panel_no, int _btnNo, float _x, float _y)
{
  int _btnID = uis[_uiId]->panels[0]->b_sNo + _btnNo;
  // touch_btn_list[_btnID]->setSliderVal(_x, _y);
}

inline void M5GFX_DentaroUI::setCharMode(int _charMode)
{
  charMode = _charMode;
}

inline int M5GFX_DentaroUI::getCharMode()
{
  return charMode;
}

inline bool M5GFX_DentaroUI::getToggleVal(int _uiId, int _btnNo)
{
  int _btnID = uis[_uiId]->panels[0]->b_sNo + _btnNo;
  // touch_btn_list[_btnID]->switchToggleVal();//値をスイッチしておいて
  return touch_btn_list[_btnID]->getToggleVal(); // スイッチした値をリターンする
}

inline int M5GFX_DentaroUI::getUiFirstNo(int _uiId)
{
  return uis[_uiId]->panels[0]->b_sNo;
}

inline int M5GFX_DentaroUI::getflickPanelBtnNo(int _uiId)
{
  return flickPanels[_uiId]->b_No;
}

inline int M5GFX_DentaroUI::getUiBtnNum(int _uiId)
{
  return uis[_uiId]->panels[0]->b_num;
}

inline int M5GFX_DentaroUI::getUiBoxNum()
{
  //  return panels.size();
  return panels_num;
}

inline int M5GFX_DentaroUI::getAllBtnNum()
{
  return gBtnID;
}

inline int M5GFX_DentaroUI::getParentID()
{
  return parentID;
}

inline int M5GFX_DentaroUI::getUiID(const char *_uiLabel)
{
  for (int k = 0; k < uis.size(); k++){
    for (int i = 0; i < uis[k]->panels.size(); i++)
    { // ボタンの数分繰り返す
      if (uis[k]->panels[i]->btn_name != std::string(_uiLabel))
{
    return i;
}
    }
  }
  return -1;
}

inline void M5GFX_DentaroUI::setBtnID(int _uiId, int _panelId, int _btnID)
{
  obj_ret.setBtnID_ret(_btnID);
}

inline lgfx::v1::touch_point_t M5GFX_DentaroUI::getBtnPos(int _btnID)
{
  return getTouchPoint(touch_btn_list[_btnID]->getBtnPos().x, touch_btn_list[_btnID]->getBtnPos().y);
}

inline void M5GFX_DentaroUI::setUiLabels(int _uiId, int _showFlickPanelNo)
{
  showFlickPanelNo = _showFlickPanelNo;

  for (int i = 0; i < uis[_uiId]->panels[0]->row; i++)
  {
    for (int j = 0; j < uis[_uiId]->panels[0]->col; j++)
    {
      int p_btnID = uis[_uiId]->panels[0]->b_sNo + j * uis[_uiId]->panels[0]->row + i;
      //     std::string stdstr = flickPanels[showFlickPanelNo]->text_list[j * uis[_uiId]->panels[0]->row + i].c_str();
      //     std::string str = stdstr.substr(0, 3).c_str(); // 先頭文字をラベルにする
      //     panels[p_btnID]->setBtnName(str);
      // panels[p_btnID]->setBtnName("め");
    }
  }
}

inline void M5GFX_DentaroUI::showTouchEventInfo(LGFX_Sprite &_lcd, int _x, int _y)
{
  _lcd.fillRect(_x + 1, _y, 9, 9, clist[0]);
}

inline void M5GFX_DentaroUI::showInfo(LGFX_Sprite &_lcd, int _infox, int _infoy)
{
  // フレームレート情報などを表示します。
  _lcd.setTextSize(1);
  _lcd.setFont(&lgfxJapanGothicP_8);
  // _lcd.fillRect(0, 0, 128, 10, TFT_BLACK);
  _lcd.setTextColor(TFT_WHITE,TFT_BLACK);
  _lcd.setCursor(_infox, _infoy);
  _lcd.print(fps);
  _lcd.print(":");
  _lcd.print(std::to_string(getEvent()).c_str());
  _lcd.print("[");
  _lcd.print(std::to_string(getPreEvent()).c_str());
  _lcd.print("]:BTN");
  _lcd.print("[");
  _lcd.print(std::to_string(getFlickEvent()).c_str());
  _lcd.print("]:BTN");
  _lcd.print(std::to_string(getTouchBtnID()).c_str());
  _lcd.print(":");
  // _lcd.println( std::to_string( getToggleVal(1,0) ) );
  ++frame_count;
  unsigned long currentTimeMicros = micros();
  unsigned long sec = currentTimeMicros / 1000000;
  // sec = millis() / 1000;
  if (psec != sec)
  {
    psec = sec;
    fps = frame_count;
    frame_count = 0;
    _lcd.setAddrWindow(0, 0, _lcd.width(), _lcd.height());
  }
}

// std::string M5GFX_DentaroUI::getHenkanChar(int _henkanListNo, int _kanaShiftNo)
// {
//   return kanalist[_henkanListNo][_kanaShiftNo];
// }

inline void M5GFX_DentaroUI::setFlick(int _uiId, int _panelId, int _FlickUiID, int _LeftBtnUiID, int _RightBtnUiID)
{
  setFlick(_uiId, _panelId, CHAR_3_BYTE, _FlickUiID, _LeftBtnUiID, _RightBtnUiID);
}

inline void M5GFX_DentaroUI::setFlick(int _uiId, int _panelId, int _charMode, int _FlickUiID, int _LeftBtnUiID, int _RightBtnUiID)
{

  FlickUiID = _FlickUiID;
  LeftBtnUiID = _LeftBtnUiID;
  RightBtnUiID = _RightBtnUiID;

  setBtnName(_uiId,_panelId, uis[_uiId]->panels[LeftBtnUiID]->b_sNo, "Nxt");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[LeftBtnUiID]->b_sNo + 1, "a/A");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[LeftBtnUiID]->b_sNo + 2, "SFT");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[LeftBtnUiID]->b_sNo + 3, "゛小");

  setBtnName(_uiId,_panelId, uis[_uiId]->panels[RightBtnUiID]->b_sNo, "del");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[RightBtnUiID]->b_sNo + 1, "_");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[RightBtnUiID]->b_sNo + 2, "Clr");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[RightBtnUiID]->b_sNo + 3, "Ret");

  if (_charMode == CHAR_1_BYTE)
  {
    fpNo = 2; // 英語モードに切り替える
  }
  else if (_charMode == NUMERIC)
  {
    fpNo = 4; // 数値モードに切り替える
  }
  else
  {
    setCharMode(_charMode); // 指定のパネルモードに切り替える
  }
  setUiLabels(FlickUiID, fpNo);
}

inline std::string M5GFX_DentaroUI::getInvisibleFlickStrings()
{
  std::string invisibleStr = "";
  // 最後の文字のバイト数を判定する
  setlocale(LC_ALL, "");
  std::vector<std::string> ret = split_mb(flickStrDel.c_str(), "\n");

  if (ret.size() >= 1)
  {

    for (uint8_t i = 0; i < ret.size() - 1; i++)
    {
      invisibleStr += "*";
    }

    std::ostringstream oss;
    oss << ret[ret.size() - 1];
    invisibleStr += oss.str();

    // invisibleStr += std::to_string(ret[ret.size() - 1].c_str());
  }

  return invisibleStr;
}

inline std::string M5GFX_DentaroUI::getFlickString(bool _visibleMode)
{
  if (_visibleMode == INVISIBLE)
    return getInvisibleFlickStrings();
  else
    return flickStr;
}

inline std::string M5GFX_DentaroUI::getFlickString()
{
  return flickStr;
}

inline std::string M5GFX_DentaroUI::getFlickline()
{

  std::string RetChar = "";
  if (!selectModeF)
  {
    RetChar = preFlickChar;
  }
  else if (selectModeF)
  {
    RetChar = previewFlickChar;
  }
  return RetChar;
}

inline std::string M5GFX_DentaroUI::delEndChar(std::string _str, int _ByteNum)
{
  std::string stdFlickStr = _str.c_str();
  for (int i = 0; i < _ByteNum; i++)
  {
    stdFlickStr.erase(--stdFlickStr.end());
  }
  return stdFlickStr.c_str();
}

inline void M5GFX_DentaroUI::switchToggleVal()
{
  if (getTouchBtnID() >= 0)
  { // NULLを除外
    touch_btn_list[getTouchBtnID()]->switchToggleVal();
  }
  else
  {
  }
}

//---MAP用関数

inline void M5GFX_DentaroUI::nowLoc(M5GFX &_lcd)
{
  _lcd.fillTriangle(110, 106, 130, 106, 120, 120, TFT_RED);
  _lcd.fillCircle(120, 120 - 18, 10, TFT_RED);
  _lcd.fillCircle(120, 120 - 18, 6, TFT_WHITE);
}

inline int M5GFX_DentaroUI::getPositionNo(int _addXTileNo, int _addYTileNo) // スプライトに高速描画
{
  int _posNo = 0;
  if (_addXTileNo == 0 && _addYTileNo == 0)
  {
    _posNo = 0;
  }
  else if (_addXTileNo == 1 && _addYTileNo == 0)
  {
    _posNo = 1;
  }
  else if (_addXTileNo == 0 && _addYTileNo == 1)
  {
    _posNo = 2;
  }
  else if (_addXTileNo == -1 && _addYTileNo == 0)
  {
    _posNo = 3;
  }
  else if (_addXTileNo == 0 && _addYTileNo == -1)
  {
    _posNo = 4;
  }
  else if (_addXTileNo == 1 && _addYTileNo == -1)
  {
    _posNo = 5;
  }
  else if (_addXTileNo == 1 && _addYTileNo == 1)
  {
    _posNo = 6;
  }
  else if (_addXTileNo == -1 && _addYTileNo == 1)
  {
    _posNo = 7;
  }
  else if (_addXTileNo == -1 && _addYTileNo == -1)
  {
    _posNo = 8;
  }
  return _posNo;
}

inline int M5GFX_DentaroUI::getAddX(int _spriteNo)
{
  int addX = MapTiles[_spriteNo]->getAddX();
  return addX;
}

inline int M5GFX_DentaroUI::getAddY(int _spriteNo)
{
  int addY = MapTiles[_spriteNo]->getAddY();
  return addY;
}

inline void M5GFX_DentaroUI::setAddX(int _objId, int _xtileNo)
{
  MapTiles[_objId]->setAddX(_xtileNo);
}

inline void M5GFX_DentaroUI::setAddY(int _objId, int _ytileNo)
{
  MapTiles[_objId]->setAddY(_ytileNo);
}

//---------

inline int M5GFX_DentaroUI::getPreAddX(int _spriteNo)
{
  int addX = MapTiles[_spriteNo]->getPreAddX();
  return addX;
}

inline int M5GFX_DentaroUI::getPreAddY(int _spriteNo)
{
  int addY = MapTiles[_spriteNo]->getPreAddY();
  return addY;
}

inline void M5GFX_DentaroUI::setPreAddX(int _objId, int _xtileNo)
{
  MapTiles[_objId]->setPreAddX(_xtileNo);
}

inline void M5GFX_DentaroUI::setPreAddY(int _objId, int _ytileNo)
{
  MapTiles[_objId]->setPreAddY(_ytileNo);
}

//---------

inline int M5GFX_DentaroUI::get_gPosId()
{
  return gPosId;
}

inline void M5GFX_DentaroUI::set_gPosId(int _gPosId)
{
  gPosId = _gPosId;
}

//---------

inline void M5GFX_DentaroUI::setDrawFinishF(int _objId, bool _drawFinishF)
{
  touch_btn_list[_objId]->setDrawFinishF(_drawFinishF);
}

inline bool M5GFX_DentaroUI::getDrawFinishF(int _objId)
{
  return touch_btn_list[_objId]->getDrawFinishF();
}

//---------

inline void M5GFX_DentaroUI::setXtileNo(int _objId, int _xtileNo)
{
  MapTiles[_objId]->setXtileNo(_xtileNo);
}

inline void M5GFX_DentaroUI::setPreXtileNo(int _objId, int _preXtileNo)
{
  MapTiles[_objId]->setPreXtileNo(_preXtileNo);
}

inline void M5GFX_DentaroUI::setYtileNo(int _objId, int _ytileNo)
{
  MapTiles[_objId]->setYtileNo(_ytileNo);
}

inline void M5GFX_DentaroUI::setPreYtileNo(int _objId, int _preYtileNo)
{
  MapTiles[_objId]->setPreYtileNo(_preYtileNo);
}

inline void M5GFX_DentaroUI::setTileNo(int _objId, int _xtileNo, int _ytileNo)
{
  MapTiles[_objId]->setXtileNo(_xtileNo);
  MapTiles[_objId]->setYtileNo(_ytileNo);
}

inline void M5GFX_DentaroUI::setXtilePos(int _objId, int _xtilePos)
{
  MapTiles[_objId]->setXtilePos(_xtilePos);
}

inline void M5GFX_DentaroUI::setYtilePos(int _objId, int _ytilePos)
{
  MapTiles[_objId]->setYtilePos(_ytilePos);
}

inline int M5GFX_DentaroUI::getXtile()
{
  return xtile;
}

inline int M5GFX_DentaroUI::getYtile()
{
  return ytile;
}

inline int M5GFX_DentaroUI::getZtile()
{
  return ztile;
}

inline int M5GFX_DentaroUI::getXtileNo()
{
  return xtileNo;
}

inline int M5GFX_DentaroUI::getYtileNo()
{
  return ytileNo;
}

inline int M5GFX_DentaroUI::getXtileNo(int _objNo)
{
  return MapTiles[_objNo]->getXtileNo();
}

inline int M5GFX_DentaroUI::getYtileNo(int _objNo)
{
  return MapTiles[_objNo]->getYtileNo();
}

inline int M5GFX_DentaroUI::getPreXtileNo(int _objNo)
{
  return MapTiles[_objNo]->getPreXtileNo();
}

inline int M5GFX_DentaroUI::getPreYtileNo(int _objNo)
{
  return MapTiles[_objNo]->getPreYtileNo();
}

inline int M5GFX_DentaroUI::getPreXtileNo()
{
  return preXtileNo;
}

inline int M5GFX_DentaroUI::getPreYtileNo()
{
  return preYtileNo;
}

inline int M5GFX_DentaroUI::getPreXtile()
{
  return preXtile;
}

inline int M5GFX_DentaroUI::getPreYtile()
{
  return preYtile;
}

inline int M5GFX_DentaroUI::getPreDirID()
{
  return preDirID;
}

inline int M5GFX_DentaroUI::getVx()
{
  return vx;
}

inline int M5GFX_DentaroUI::getVy()
{
  return vy;
}

inline void M5GFX_DentaroUI::setExistF(int _objId, bool _existF)
{
  MapTiles[_objId]->setExistF(_existF);
}

inline bool M5GFX_DentaroUI::getExistF(int _objId)
{
  return MapTiles[_objId]->getExistF();
}

inline void M5GFX_DentaroUI::setPreDirID(int _dirID)
{
  preDirID = _dirID;
}

inline int M5GFX_DentaroUI::getDirID()
{

  vx = getXtile() - getPreXtile();
  vy = getYtile() - getPreYtile();

  float r = atan2(vy, vx);
  if (r < 0)
  {
    r = r + 2 * M_PI;
  }
  float vecAngle = r * 360 / (2 * M_PI);

  // preDirID = dirID;

  if (vecAngle < 15 || vecAngle >= 345)
  {
    dirID = 1;
  }
  else if (vecAngle >= 15 && vecAngle < 75)
  {
    dirID = 2;
  }
  else if (vecAngle >= 75 && vecAngle < 105)
  {
    dirID = 3;
  }
  else if (vecAngle >= 105 && vecAngle < 165)
  {
    dirID = 4;
  }
  else if (vecAngle >= 165 && vecAngle < 195)
  {
    dirID = 5;
  }
  else if (vecAngle >= 195 && vecAngle < 255)
  {
    dirID = 6;
  }
  else if (vecAngle >= 255 && vecAngle < 285)
  {
    dirID = 7;
  }
  else if (vecAngle >= 285 && vecAngle < 345)
  {
    dirID = 8;
  }

  return dirID;
}

inline bool M5GFX_DentaroUI::getDownloadF()
{
  return DownloadF;
}

inline void M5GFX_DentaroUI::setDownloadF(bool _b)
{
  DownloadF = _b;
}
inline void M5GFX_DentaroUI::task2_setPngTile(int _posId)
{
  // ROI_m_url = "/tokyo/" + std::to_string(getZtile()) + "/" + std::to_string(getXtileNo(_posId)) + "/" + std::to_string(getYtileNo(_posId)) + ".png";
  // setPngTile(SD, ROI_m_url, _posId); // SDからの地図の読み込み
}
#endif // M5GFX_DENTARO_UI_HPP
