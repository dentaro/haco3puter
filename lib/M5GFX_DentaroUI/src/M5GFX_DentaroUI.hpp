#ifndef M5GFX_DENTARO_UI_HPP
#define M5GFX_DENTARO_UI_HPP
#define FLICK_DIST 4
#define HOLDING_SEC 160000
#define TAP_TO_TAP_SEC 220000
#define FORMAT_SPIFFS_IF_FAILED true
#pragma once //インクルードガード

#include <M5GFX.h>
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

#define USE_PSRAM true

#define COL_DEPTH 16
#define TILE_COL_DEPTH 16

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

struct Vec2{
  double x;
  double y;

  Vec2() = default; // デフォルトコンストラクタ
  Vec2(double _x, double _y) // コンストラクタ
  : x(_x)
  , y(_y) {}
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

    unsigned long touchStartTime = 0;
    unsigned long preTouchStartTime = 0;
    unsigned long lastTappedTime = 0;
    unsigned long firstTappedTime = 0;

    unsigned long sTime = 0;
    unsigned long tappedTime = 0;
    
    int dist_thre = 40;

    int parentID = 0;//初期値0 =　PARENT_LCD

    int timeCnt = 0;
    uint16_t clist[5] = {0,0,0,0,0};

    int AngleCount = 0;
    int panels_num = 0;

    std::string m_url = "";

    int sec, psec;
    int fps = 0;
    int frame_count = 0;

    int fpNo = 0;


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

public:

  M5GFX_DentaroUI();
  M5GFX_DentaroUI(M5GFX* _lcd);

  M5GFX* lcd; // M5GFXクラスへのポインタ//タッチポイントとれる

  LGFX_Sprite layoutSprite_list[BUF_PNG_NUM];

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
  
  inline void circle(M5GFX* _lcd, uint16_t c, int fillF);
  inline void rect(M5GFX* _lcd, uint16_t c, int fillF);

  inline uint8_t getVecNo();

  inline int getPreEvent();

  inline void showInfo( LGFX_Sprite& _lcd, int _infox, int _infoy);

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

  // inline void drawMaps(M5GFX& _lcd, double _walkLatPos, double _walkLonPos, int _tileZoom);

  // void setPngTile(fs::FS &fs, std::string _m_url, int _spriteNo);
  inline void setDownloadF(bool _b);
  inline bool getDownloadF();
  inline void task2_setPngTile(int _posId);

  inline void getTilePos(double lat, double lon, int zoom_level);

  inline void drawPngFile(LGFX_Sprite sprite, fs::SPIFFSFS* _SPIFFS,const char * _path, int32_t _x, int32_t _y);

};


inline void M5GFX_DentaroUI::update2( M5GFX& _lcd )
{
  _lcd.setTextSize(1);
  _lcd.setFont(&lgfxJapanGothicP_8);

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

inline void M5GFX_DentaroUI::drawPngFile(LGFX_Sprite sprite, fs::SPIFFSFS* _SPIFFS,const char * _path, int32_t _x, int32_t _y){
  // sprite.drawPngFile(_SPIFFS, _path, _x, _y);
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
