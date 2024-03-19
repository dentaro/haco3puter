#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FS.h>
#include <M5GFX_DentaroUI.hpp>


#include "SPIFFS.h"
#include "baseGame.h"

// #include "Editor.h"

#include <bitset>
#include <iostream>
#include <fstream>

#include <vector> 

// #include "Speaker_Class.hpp"

extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#ifndef RUN_LUA_GAME_H
#define RUN_LUA_GAME_H


#define MAX_CHAR 512//1024
// #define MAX_CHAR 2048

// #define LUA_BUFFERSIZE 1024
// #define LUA_BUFFERSIZE_PS (LUA_BUFFERSIZE * 2)

#define LUA_BUFSIZE 1024

struct LoadF {
  File f;
  char* buf; // ポインタを使用

  // コンストラクタでメモリ確保
  LoadF(size_t size) : buf(new char[size]) {}

  // デストラクタでメモリ解放
  ~LoadF() {
    delete[] buf; // ポインタを使用してメモリを解放
  }
};


inline uint16_t lua_rgb24to16(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t tmp = ((r>>3) << 11) | ((g>>2) << 5) | (b>>3);
  return tmp; //(tmp >> 8) | (tmp << 8);
}

class runLuaGame: public BaseGame
{//クラスはデフォルトではprivata

  public:
    enum WifiPhase{
      NONE,
      SELECT,
      SHOW,
      RUN
    };

    //  void fillFastTriangle(float x0, float y0, float x1, float y1, float x2, float y2, uint16_t c1);

    
  struct Rect2D {
    int x;
    int y;
    int w;
    int h;
    Rect2D(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) {}
  };

struct CameraObj {
    float x;
    float y;
    float z;
    float x2;
    float y2;
    float z2;
    float anglex;
    float angley;
    float anglez;
    float zoom;

    // コンストラクタで初期値を設定
    CameraObj()
        : x(0), y(0), z(0), x2(0), y2(0), z2(0), anglex(0), angley(0), anglez(0), zoom(1.0)
    {
    }
  };

  
  struct LightObj {
    float x;
    float y;
    float z;

    // コンストラクタで初期値を設定
    LightObj()
        : x(0), y(0), z(0)
    {
    }
  };

  struct CubeObj {
    float x;
    float y;
    float z;
    float angle;
    float size;
    int colangle;
    int width;
    int height;
    // コンストラクタで初期値を設定
    CubeObj()
        : x(0), y(0), z(0), angle(0), size(1.0),colangle(120), width(10), height(10)
    {
    }
};

    lua_State* L;
    luaL_Buffer b;
    uint8_t  col[3] = {0,0,0};

    // std::deque<int> buttonState;//ボタンの個数未定
    // int touchState;//タッチボタン
    // int tp[2] ={0,0};
    // uint16_t palette[256];

    // bool wifiDebugRequest = true;//外部ファイルから書き換えテifiモードにできる
    // bool wifiDebugSelf = false;

    WifiPhase wifiMode = NONE;
    int modeSelect = 0;
    bool exitRequest = false;
    bool runError = false;
    String errorString;
    // Vector3::Vector3 boxzero;
    int boxzerox = 60;
    int boxzeroy = 60;

    std::vector<String> fileNamelist;

    // int gameState = 0;

  runLuaGame(int _gameState, String _mn);//ゲームの状態を立ち上げ時に渡す

    int loadSurface(File* fp, uint8_t* buf);
    // static int l_tp(lua_State* L);
    // static int l_tstat(lua_State* L);
    static int l_vol(lua_State* L);
    static int l_pinw(lua_State* L);
    static int l_pinr(lua_State* L);
    static int l_tone(lua_State* L);
    static int l_spr8(lua_State* L);
    static int l_spr(lua_State* L);
    static int l_scroll(lua_State* L);
    static int l_rmap(lua_State* L);
    static int l_pset(lua_State* L);
    static int l_pget(lua_State* L);
    static int l_cls(lua_State* L);
    static int l_color(lua_State* L);
    static int l_text(lua_State* L);
    static int l_opmode(lua_State* L);
    static int l_drawrect(lua_State* L);
    static int l_fillrect(lua_State* L);
    static int l_rectfill(lua_State* L);//png配置できるrect
    static int l_fillp(lua_State* L);
    static int l_oval(lua_State* L);
    static int l_ovalfill(lua_State* L);
    static int l_fillpoly(lua_State* L);
    static int l_drawbox(lua_State* L);
    static int l_drawboxp(lua_State* L);
    static int l_fillcircle(lua_State* L);
    static int l_drawcircle(lua_State* L);
    static int l_drawtri(lua_State* L);
    static int l_filltri(lua_State* L);
    static int l_pal(lua_State* L);
    static int l_key(lua_State* L);
    static int l_btn(lua_State* L);
    static int l_touch(lua_State* L);
    static int l_btnp(lua_State* L);
    static int l_sldr(lua_State* L);
    
    // static int l_getip(lua_State* L);
    // static int l_iswifidebug(lua_State* L);
    // static int l_wifiserve(lua_State* L);
    static int l_initstars(lua_State* L);
    static int l_creobj(lua_State* L);
    static int l_creobj2(lua_State* L);
    static int l_rendr(lua_State* L);
    static int l_rendr2(lua_State* L);
    static int l_drawstars(lua_State* L);
    
    static int l_win(lua_State* L);
    static int l_run(lua_State* L);
    static int l_appmode(lua_State* L);
    static int l_appinfo(lua_State* L);
    static int l_editor(lua_State* L);
    static int l_line(lua_State* L);
    static int l_list(lua_State* L);
    // static int l_require(lua_State* L);
    // static int l_httpsget(lua_State* L);
    // static int l_httpsgetfile(lua_State* L);
    // static int l_savebmp(lua_State* L);
    static int l_reboot(lua_State* L);
    static int l_debug(lua_State* L);

    void fastPoint(const Vector3<float> v1, int starbrightness, int noInt);

    String getBitmapName(String s);
    String getPngName(String s);
    void hsbToRgb(float angle, float si, float br, int& r, int& g, int& b);
    void hsbToRgb2(float angle, float br, int& r, int& g, int& b);
    void fillFastTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c1);
    void line3D(const Vector3<float> v1,const Vector3<float> v2,uint16_t c1);
    void triangle3D(const Vector3<float> v1,const Vector3<float> v2,const Vector3<float> v3, int idx);
    void fill3DFastTriangle(int32_t x0, int32_t y0, int32_t z0,
                                        int32_t x1, int32_t y1, int32_t z1,
                                        int32_t x2, int32_t y2, int32_t z2,
                                        uint16_t c1);
float calculateLength(float x, float y, float z);
Vector3<float> normalize(float x, float y, float z);
float calculateDotProduct(const Vector3<float>& v1, const Vector3<float>& v2);
Vector3<float> calculateNormal(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3);
float calculateBrightness(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3, const LightObj& light);

    void resume();
    void init();
    int run(int _remainTime);
    void pause();

    // void fillFastTriangle(float x0, float y0, float x1, float y1, float x2, float y2, uint16_t c1);
  
    protected://継承先でも使えるもの
    
    
};

#endif
