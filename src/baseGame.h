#ifndef BASE_GAME_H
#define BASE_GAME_H

#define CTRLBTNNUM 1//ゲームに使えるサイドボタン数を決めます9
#define CALIBRATION_FILE "/init/param/caldata.txt"
#define MAPSPRNOS_FILE "/init/param/mapsprnos.txt"//色番号に対応するスプライトを登録するファイル
#define WIFIPASS_FILE "/init/param/wifipass.txt"
#define SPRBITS_FILE "/init/param/sprbits.txt"
#define PAL_DIR "/init/param/pal"
#define FAST_MODE 1
#define WIDE_MODE 2 //倍率になっています。

#define BSTARBUFNUM 1000

#define TFT_OFFSET_X 60
#define TFT_OFFSET_Y 0
#define RAD_ONE PI/180

#define TFT_WIDTH 160
#define TFT_HEIGHT 128

#define DISP_SPR_W_NUM 20
#define DISP_SPR_H_NUM 16

#define PNG_SPRITE_WIDTH 64
#define PNG_SPRITE_HEIGHT 128

#define OUTPIN_0 16
#define INPIN_0  17

#define MAPW 20//読み込みマップのpixelサイズ
#define MAPH 16//マップのpixelサイズ

#define HPMP 1
class BaseGame
{
  public:
    bool wifiDebugRequest = false;//外部ファイルから書き換えてWifiモードにできる
    bool wifiDebugSelf = false;

    void setWifiDebugRequest(bool _b){wifiDebugRequest = _b;};//外部ファイルから書き換えてWifiモードにできる
    void setWifiDebugSelf(bool _b){wifiDebugSelf = _b;};

  BaseGame(){}
  virtual void init(){};
  virtual int run(int remainTime){return 0;};
  virtual void pause(){};
  virtual void resume(){};

};


#endif


