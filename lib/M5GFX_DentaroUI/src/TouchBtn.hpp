#pragma once //インクルードガード

#define SDCARD_SS_PIN 5 //M5toughのSD_CSは4
#define SDCARD_SPI SPI

#include <M5GFX.h>
// #include <M5Unified.h>
using namespace std;

#include <list>

#define USE_PSRAM true

#define COL_DEPTH 16
#define TILE_COL_DEPTH 16
// #define TILE_CHILD_COL_DEPTH 8

#define TOUCH_NONE_MODE -1
// #define TOUCH_TILE_MODE 0
#define TOUCH_PRE_MODE 0
#define TOUCH_BTN_MODE 1
#define TOUCH_SLIDER_MODE 2
#define TOUCH_FLICK_MODE 3
#define TOUCH_TOGGLE_MODE 4
#define TOUCH_OBTN_MODE 5

#define TFT4_BLACK 0
#define TFT4_BLUE 1
#define TFT4_GREEN 2
#define TFT4_RED 3
#define TFT4_CYAN 4
#define TFT4_MAGENTA 5
#define TFT4_YELLOW 6
#define TFT4_ORANGE 7
#define TFT4_WHITE 15

#define USE_PSRAM true

#define B_UNTOUCH 0x00000000
#define B_TOUCH 0x00000001
#define B_MOVE 0x00000011
#define B_RELEASE 0x00000010

//eventState
#define NO_EVENT -1
#define UNTOUCH 0
#define DRAG 2
#define TAPPED 3
#define WTAPPED 4
#define FLICK 5 //フリックの中心、始まりのイベント
#define FLICKED 6 //フリック終わりのイベント

#define TOUCH 7
#define WAIT 8
#define MOVE 9
#define RELEASE 10

//flickState
#define RIGHT_FLICK 11
#define U_R_FLICK 12
#define UP_FLICK 13
#define L_U_FLICK 14
#define LEFT_FLICK 15
#define D_L_FLICK 16
#define DOWN_FLICK 17
#define R_D_FLICK 18

#define MULTI_EVENT 31

#define XY_NONE 0
#define X_VAL 1
#define Y_VAL 2
#define XY_VAL 3

class DelegateBase2 {
public:
  DelegateBase2(){};
  virtual ~DelegateBase2(){};
  virtual int operator()( int value0 ) = 0;  // 関数プロトタイプ「int xxx(int);」
};

template < class T >
class Delegate2 : public DelegateBase2 {
  public:
    Delegate2(){};
    virtual ~Delegate2(){};

    // オペレータ実装
    virtual int operator()( int value0 ) {   // 関数プロトタイプ「int xxx(int);」
      (m_obj->*m_func)(value0);   // ハンドラ実行：関数プロトタイプ「int xxx(int);」
      return ( value0 );
    }
    typedef int (T::*EventFunc)(int);    // 関数プロトタイプ「int xxx(int);」


    // オブジェクトとメソッドを登録
    void set( T* obj, EventFunc func ) {
      m_obj = obj;
      m_func = func;
    }

    // デリゲータ生成関数
    static DelegateBase2* createDelegator2( T* obj, EventFunc func ) {
      Delegate2* dg = new Delegate2;
      dg->set( obj, func );
      return dg;
    }

  protected:
    T* m_obj;               // オブジェクト
    EventFunc m_func;       // 関数ポインタ
};

class TouchBtn {
  private:
    std::list<DelegateBase2*> lim2;
    // int b_x = 0;
    // int b_y = 0;

    // int b_w = 0;
    // int b_h = 0;
    // int b_hw = 0;
    // int b_hh = 0;
    // int b_qw = 0;

    uint16_t b_a = 0;
    uint16_t b_a0 = 360;
    uint16_t b_a1 = 0;
    uint16_t b_r0 = 50;
    uint16_t b_r1 = 0;
    uint16_t b_n = 1;

    uint16_t btns_starAngle = 0;

    // float s_str_hw = 4;//文字列の半分の長さを計算

    uint16_t sliderPosx = 0;
    uint16_t sliderPosy = 0;
    uint8_t xy_mode = XY_VAL;

    uint8_t bgColorIndex;
    // LGFX_Sprite divSprite0;
    uint16_t color = TFT_WHITE;
    
    
    // bool selectBtnF = false;
    
    // std::string btn_nameFalse;

    uint8_t eventState = -1;
    uint8_t eventNo = -1;
    uint16_t AngleCount = 0;
    uint8_t bgColIndex = 0;
    bool availableF = false;
    bool visibleF = false;

    uint8_t btn_mode = TOUCH_NONE_MODE;
    bool toggleVal = false;

    lgfx::v1::touch_point_t sp;
    lgfx::v1::touch_point_t tp;
    lgfx::v1::touch_point_t uiSpritePos;//ボタン用の位置
    lgfx::v1::touch_point_t tilePos;//basic_spriteの位置
    float sliderValx = 0.5;
    float sliderValy = 0.5;

    bool drawFinishF = false;

public:
    std::string label = "";//ボタン番号
    std::string btn_name = "";//ボタンの名前
    std::string namelabel = "あ";
    std::string shift_btn_name = "べ";
    uint8_t btnID = 0;//すべてのボタンの固有のID
    uint8_t btnNo = 0;//UI内での順番
    uint16_t s_x = 0;
    uint16_t s_y = 0;
    uint16_t s_w = 48;
    uint16_t s_h = 38;
    uint16_t s_hw = 0;
    uint16_t s_hh = 0;
    uint16_t s_qw = 0;

    TouchBtn()
    {
    }

    // void initBtn(int _btnID, std::string _label, int _b_x, int _b_y, int _b_w, int _b_h,
    // lgfx::v1::touch_point_t _uiSpritePos,
    // int _btn_mode);

    inline void initBtn(int _btnID, std::string _label, int _s_x, int _s_y, int _s_w, int _s_h,
    lgfx::v1::touch_point_t _uiSpritePos,
    int _btn_mode);

    inline void initFlick(int _btnID, std::string _label, int _b_x, int _b_y, int _b_w, int _b_h,
    lgfx::v1::touch_point_t _uiSpritePos,
    int _btn_mode);


    inline void setOBtnPos( int _b_x, int _b_y);
    inline void initOBtn(int _btnID, std::string _label, int _btnNo, int _btns_starAngle, int _b_x, int _b_y, int _b_r0, int _b_r1, int _b_n, std::string _btn_name,
    // lgfx::v1::touch_point_t _uiSpritePos,
    lgfx::v1::touch_point_t _uiSpritePos,
    int _btn_mode);

    inline void setuiSpritePos(lgfx::v1::touch_point_t _uiSpritePos);

    // void initSlider(int _btnID, std::string _label, int _b_x, int _b_y, int _b_w, int _b_h,
    // lgfx::v1::touch_point_t _uiSpritePos,
    // int _btn_mode);

    inline void initSlider(int _btnID, std::string _label, int _b_x, int _b_y, int _b_w, int _b_h,
      lgfx::v1::touch_point_t _uiSpritePos,
      int _btn_mode,
      int _xy_mode,
      int _eventNo);

    inline void setBtnNamelabel(std::string _btn_namelabel);
    
    // void initSlider( int _btnID, std::string _label, int _s_x, int _s_y, int _s_w, int _s_h,lgfx::v1::touch_point_t _uiSpritePos,int _visible_mode );

    // void initTile(int _btnID,std::string _btn_name, lgfx::v1::touch_point_t _uiSpritePos, int layoutSprite_w, int layoutSprite_h, LGFX_Sprite& _layoutSprite, LGFX_Sprite& _g_basic_sprite);
    // void setBtnName(std::string _btnName);
    // void setBtnNameFalse(std::string _btnNameFalse);
    inline void btnDraw(M5GFX& _lcd);

    inline void sliderDraw(M5GFX& _lcd);
    inline void tileDraw(M5GFX& _lgfx, LGFX_Sprite& _layoutSprite, lgfx::v1::touch_point_t _uiSpritePos, lgfx::v1::touch_point_t _sp, uint8_t _bgColIndex, LGFX_Sprite& _g_basic_sprite);
    inline void setAvailableF(bool _availableF);
    inline bool getAvailableF();
    inline void setVisibleF(bool _visibleF);
    inline bool getVisibleF();
    inline void addHandler(DelegateBase2* _func);

    inline void delHandlers2();
    inline void run2( int _btnID, int _btnNo, lgfx::v1::touch_point_t _sp, lgfx::v1::touch_point_t _tp, int _eventState, int _runEventNo);
    inline void run2( int _btnID, int _btnNo, int _sx, int _sy, int _tx, int _ty, int _eventState, int _runEventNo);

    inline void setColor(uint16_t _color);
    inline void setBgColorIndex(uint16_t _bgColorIndex);
    inline int getBtnID();
    inline int getBtnMode();

    inline void setBtnName(std::string _btn_name);
    inline std::string getBtnName();

    // void setPngTile(fs::FS &fs, std::string _m_url, LGFX_Sprite& _g_basic_sprite);
    inline lgfx::v1::touch_point_t getTouchPoint(int _x, int _y);
    inline lgfx::v1::touch_point_t getBtnPos();

    // void setSelectBtnF(bool _selectBtnF);
    inline void switchToggleVal();
    //private対策
    inline float getSliderValx();
    inline float getSliderValy();
    inline void setSliderVal(float _x, float _y);
    inline bool getToggleVal();
    inline void setTilePos(lgfx::v1::touch_point_t _pos);
    inline int get_xy_mode();
    inline void setDrawFinishF(bool _drawFinishF);
    inline bool getDrawFinishF();

};

inline void TouchBtn::setuiSpritePos(lgfx::v1::touch_point_t _uiSpritePos)
{
  uiSpritePos = _uiSpritePos;
}

inline void TouchBtn::initBtn(int _btnID, std::string _label, int _s_x, int _s_y, int _s_w, int _s_h,
lgfx::v1::touch_point_t _uiSpritePos,
int _btn_mode)
{
  int _xy_mode = XY_NONE;
  int _eventNo = TOUCH;
  initSlider(_btnID, _label, _s_x, _s_y, _s_w, _s_h, _uiSpritePos, _btn_mode, _xy_mode, _eventNo);
}

inline void TouchBtn::setOBtnPos( int _s_x, int _s_y){
  s_x = _s_x;
  s_y = _s_y;
}

inline void TouchBtn::initOBtn(int _btnID, std::string _label, int _btnNo, int _btns_starAngle, int _s_x, int _s_y, int _s_r0, int _s_r1, int _s_n, std::string _btn_name,
lgfx::v1::touch_point_t _uiSpritePos,
int _btn_mode)
{
  btnID = _btnID;
  label = _label;
  btnNo = _btnNo;
  btns_starAngle = _btns_starAngle;
  uiSpritePos = _uiSpritePos;
  s_x = _s_x;
  s_y = _s_y;

  b_r0 = _s_r0;
  b_r1 = _s_r1;
  b_n = _s_n;

  b_a = 360/_s_n;

  b_a0 = (b_a * btnNo + btns_starAngle)%360;
  b_a1 = b_a0 + b_a;

  s_hw = s_w/2;
  s_hh = s_h/2;

  btn_name = _btn_name;
  btn_mode = _btn_mode;
  
}

inline void TouchBtn::btnDraw(M5GFX&  _lcd)
{
  _lcd.fillRoundRect(this->s_x, this->s_y, this->s_w, this->s_h, 10, TFT_BLACK);
  _lcd.drawRoundRect(this->s_x, this->s_y, this->s_w, this->s_h, 10, TFT_WHITE);
  _lcd.setTextSize(1);
  _lcd.setFont(&lgfxJapanGothicP_8);
  _lcd.setTextColor(TFT_RED);
  int8_t s_str_hw = floor(_lcd.textWidth( btn_name.c_str() ) / 2);
  _lcd.drawString( btn_name.c_str(), s_x + s_hw - s_str_hw , s_y + s_hh - 4 );
  
}

inline void TouchBtn::sliderDraw(M5GFX& _lcd)
{
  if( btn_mode == TOUCH_SLIDER_MODE )
  {
    if(xy_mode != XY_NONE)
    {
      // _lcd.fillRect(this->s_x, this->s_y, this->s_w, this->s_h, TFT_BLACK);
      _lcd.setTextSize(1);
      _lcd.setTextColor(TFT_WHITE);
      this->sliderPosx = this->s_x + sliderValx * this->s_w;
      this->sliderPosy = this->s_y + sliderValy * this->s_h;

      _lcd.fillCircle(this->sliderPosx,
                      this->sliderPosy, 5, TFT_WHITE);
      _lcd.drawRoundRect(this->s_x, this->s_y, this->s_w, this->s_h, 10, TFT_WHITE);
      
      // if( xy_mode == Y_VAL || xy_mode==XY_VAL ){
      //   //_lcd.draw Sstring(std::to_string(sliderValy), this->s_x + 4 , this->s_y + 16);
      //   _lcd.drawLine(this->s_x,
      //                 this->sliderPosy,
      //                 this->s_x + this->s_w,
      //                 this->sliderPosy, TFT_WHITE);
      // }

      // if( xy_mode == X_VAL || xy_mode==XY_VAL ){
      //   //_lcd.drawString(std::to_string(sliderValx), this->s_x + 4 , this->s_y + 4);
      //   _lcd.drawLine(sliderPosx,
      //                 this->s_y,
      //                 sliderPosx,
      //                 this->s_y + this->s_h , TFT_WHITE);
    }
  }
   //_lcd.drawLine(this->s_x, this->s_x, this->s_x + this->s_w, this->s_x, TFT_WHITE);

  // if( btn_mode == TOUCH_BTN_MODE ){
  //   s_str_hw = _lcd.textWidth(btn_name)/2;
  //   _lcd.drawString(btn_name, this->s_x + this->s_hw - s_str_hw , this->s_y + this->s_hh -4);
  // }
  // }
}

// inline void TouchBtn::setSelectBtnF(bool _selectBtnF){
//   selectBtnF = _selectBtnF;
// }

inline void TouchBtn::tileDraw(M5GFX& _lgfx, LGFX_Sprite& _layoutSprite,
lgfx::v1::touch_point_t _uiSpritePos, 
lgfx::v1::touch_point_t _sp,
uint8_t _bgColIndex, LGFX_Sprite& _g_basic_sprite)
{
  // _g_basic_sprite.setPivot(_g_basic_sprite.width()/2.0, _g_basic_sprite.height()/2.0);//setPivot()で回転する場合の原点を指定します。初期値は左上の(0, 0)だと思います

  // _g_basic_sprite.pushSprite(&divSprite0, tilePos.x, tilePos.y);
  // divSprite0.pushSprite(&_layoutSprite, 0, 0);//親を指定して出力

  // _g_basic_sprite.pushSprite(&divSprite0, tilePos.x-s_qw, tilePos.y);
  // divSprite0.pushSprite(&_layoutSprite, s_qw, 0);//親を指定して出力

  // _g_basic_sprite.pushSprite(&divSprite0, tilePos.x-s_qw*2, tilePos.y);
  // divSprite0.pushSprite(&_layoutSprite, s_qw*2, 0);//親を指定して出力

  // _g_basic_sprite.pushSprite(&divSprite0, tilePos.x-s_qw*3, tilePos.y);
  // divSprite0.pushSprite(&_layoutSprite, s_qw*3, 0);//親を指定して出力
}


inline int TouchBtn::getBtnID(){
    return btnID;
}
inline void TouchBtn::addHandler(DelegateBase2* _func){
  lim2.push_back(_func);
}


inline void TouchBtn::initSlider(int _btnID, std::string _label, int _s_x, int _s_y, int _s_w, int _s_h,
lgfx::v1::touch_point_t _uiSpritePos,
int _btn_mode,
int _xy_mode,
int _eventNo)
{
  btnID = _btnID;
  label = _label;
  uiSpritePos = _uiSpritePos;
  s_x = _s_x;
  s_y = _s_y;
  s_w = _s_w;
  s_h = _s_h;
  s_hw = s_w/2;
  s_hh = s_h/2;
  
  if(_btn_mode==TOUCH_PRE_MODE)btn_name = "PRE_"+_label;
  else  if(_btn_mode==TOUCH_BTN_MODE)btn_name = "BTN_"+_label;
  else if(_btn_mode==TOUCH_SLIDER_MODE)btn_name = "SLIDER_"+_label;
  else if(_btn_mode==TOUCH_FLICK_MODE)btn_name = "FLICK_"+_label;
  // btn_name = _label;
  btn_mode = _btn_mode;
  xy_mode = _xy_mode;
  eventNo = _eventNo;

  sliderPosx = _s_x + (_s_w/2);
  sliderPosy = _s_y + (_s_h/2);
}


inline void TouchBtn::initFlick(int _btnID, std::string _label, int _s_x, int _s_y, int _s_w, int _s_h,
lgfx::v1::touch_point_t _uiSpritePos,
int _btn_mode)
{
  int _xy_mode = XY_NONE;
  int _eventNo = TOUCH;
  initSlider(_btnID, _label, _s_x, _s_y, _s_w, _s_h, _uiSpritePos, _btn_mode, _xy_mode, _eventNo);
}

inline void TouchBtn::setAvailableF(bool _availableF){
  availableF = _availableF;
}

inline bool TouchBtn::getAvailableF(){
  return availableF;
}

inline void TouchBtn::setVisibleF(bool _visibleF){
  visibleF = _visibleF;
}

inline bool TouchBtn::getVisibleF(){
  return visibleF;
}

inline void TouchBtn::delHandlers2(){
      std::list<DelegateBase2*>::iterator ite2 =  lim2.begin();
      while( ite2 != lim2.end() )
      {
        // DelegateBase2 *ptr = (*ite2);
        lim2.pop_front();//
      }
}

inline void TouchBtn::run2(int _btnID, int _btnNo, lgfx::v1::touch_point_t _sp, lgfx::v1::touch_point_t _tp, int _eventState, int _runEventNo)
{
  run2(_btnID, _btnNo, _sp.x, _sp.y, _tp.x, _tp.y, _eventState, _runEventNo);
}

inline void TouchBtn::run2(int _btnID, int _btnNo, int _sx, int _sy, int _tx, int _ty, int _eventState, int _runEventNo)
{
      sp.x = _sx;
      sp.y = _sy;
      tp.x = _tx;
      tp.y = _ty;

      eventState = _eventState;
      eventNo = _runEventNo;

      //初期化処理
      // this->sliderPosx = 0;
      // this->sliderPosy = 0;

      // this->sliderValx = 0;
      // this->sliderValy = 0;

      std::list<DelegateBase2*>::iterator ite2 =  lim2.begin();
      while( ite2 != lim2.end() )
      {

        DelegateBase2 *ptr = (*ite2);
        if(btn_mode == TOUCH_BTN_MODE){
          if(sp.x > this->s_x
          && sp.x < this->s_x + this->s_w
          && sp.y > this->s_y
          && sp.y < this->s_y + this->s_h)
          {//ボタンの領域内に入っていれば
            (*ptr)( _btnID );// 関数を実行！
          }
        }
        else if(btn_mode == TOUCH_FLICK_MODE){
          if(sp.x > this->s_x
          && sp.x < this->s_x + this->s_w
          && sp.y > this->s_y
          && sp.y < this->s_y + this->s_h)
          {//ボタンの領域内に入っていれば
            (*ptr)( _btnID );// 関数を実行！
          }
        }
        else if(btn_mode == TOUCH_TOGGLE_MODE)
        {//普通のボタン用の判定
          if(sp.x > s_x     + uiSpritePos.x + this->uiSpritePos.x
          && sp.x < s_x+s_w + uiSpritePos.x + this->uiSpritePos.x
          && sp.y > s_y     + uiSpritePos.y + this->uiSpritePos.y
          && sp.y < s_y+s_h + uiSpritePos.y + this->uiSpritePos.y)
          {//ボタンの領域内に入っていれば
          if(availableF == true){
            // this->switchToggleVal();
            (*ptr)( _btnID );    // 関数を実行！
          }
            // if(eventState == runEventNo)this->switchToggleVal();
            // if(eventState == runEventNo && eventState != NO_EVENT)this->switchToggleVal();
          }
        }
        else if(btn_mode == TOUCH_OBTN_MODE)
        {//円形ボタンの領域判定
          // if(sp.x > s_x     + uiSpritePos.x + this->uiSpritePos.x
          // && sp.x < s_x+s_w + uiSpritePos.x + this->uiSpritePos.x
          // && sp.y > s_y     + uiSpritePos.y + this->uiSpritePos.y
          // && sp.y < s_y+s_h + uiSpritePos.y + this->uiSpritePos.y)

          int d = sqrt( ( pow( sp.x- this->s_x, 2.0 ) + pow( sp.y- this->s_y, 2.0 ) ));
          int a = int((atan2(sp.y- this->s_y, sp.x- this->s_x)*180/M_PI ));
          if(a<0) a += 360;//マイナス値をなくす
          int div_a = (this->b_a * this->btnNo);

          if(d > this->b_r1 && d < this->b_r0)
          {//円形ボタンの領域内に入っていれば
            if((360 + a -this->btns_starAngle)%360 > div_a && (360 + a -this->btns_starAngle)%360 < div_a + this->b_a)
            {//分割角と一致する角度であれば
            if(availableF == true){
              (*ptr)( _btnID );    // 関数を実行！
            }
          }
          }
        }
        // else if(btn_mode == TOUCH_FLICK_MODE)//FLICK_MODEもこちらで処理
        // {//普通のボタン用の判定
        //   // if(sp.x > s_x     + uiSpritePos.x + this->uiSpritePos.x
        //   // && sp.x < s_x+s_w + uiSpritePos.x + this->uiSpritePos.x
        //   // && sp.y > s_y     + uiSpritePos.y + this->uiSpritePos.y
        //   // && sp.y < s_y+s_h + uiSpritePos.y + this->uiSpritePos.y)
        //   // {//ボタンの領域内に入っていれば
        //   if(sp.x > this->s_x
        //   && sp.x < this->s_x + this->s_w
        //   && sp.y > this->s_y
        //   && sp.y < this->s_y + this->s_h)
        //   {
        //   // if(availableF == true){
        //     (*ptr)( _btnID );    // 関数を実行！
        //   // }
        //     // if(eventState == runEventNo)this->switchToggleVal();
        //     // if(eventState == runEventNo && eventState != NO_EVENT)this->switchToggleVal();
        //   }
        // }
        else if(btn_mode == TOUCH_SLIDER_MODE)
        {//スライダボタン用の判定
          if(sp.x > this->s_x
          && sp.x < this->s_x + this->s_w
          && sp.y > this->s_y
          && sp.y < this->s_y + this->s_h)
          {//ボタンの領域内に入っていれば
            (*ptr)( _btnID );// 関数を実行！
            if( eventState == eventNo||eventNo == MULTI_EVENT ){
                this->sliderPosx = tp.x;
                this->sliderPosy = tp.y;

                this->sliderValx = float( tp.x - s_x)/s_w;
                this->sliderValy = float( tp.y - s_y)/s_h;
            }
            if( (tp.x) >= s_x && (tp.x) <= s_x + s_w ){
              if( (tp.y) >= s_y && (tp.y) <= s_y + s_h ){
                this->sliderPosx = tp.x;
                this->sliderPosy = tp.y;

                this->sliderValx = float(tp.x - s_x)/s_w;
                this->sliderValy = float(tp.y - s_y)/s_h;
              }
            }
          
          }else{
            // this->sliderPosx = 0;
            // this->sliderPosy = 0;

            // this->sliderValx = 0.5;
            // this->sliderValy = 0.5;
          }
        }
        ite2++;
        lim2.pop_front();//メソッドを実行したら削除
      }

}

//-----------------

inline int TouchBtn::getBtnMode(){
  return btn_mode;
}
inline void TouchBtn::setBtnNamelabel(std::string _namelabel){
    namelabel = _namelabel;
}
inline void TouchBtn::setColor(uint16_t _color){
  color = _color;
}
inline void TouchBtn::setBgColorIndex(uint16_t _bgColorIndex){
  bgColorIndex = _bgColorIndex;
}
inline lgfx::v1::touch_point_t TouchBtn::getTouchPoint(int _x, int _y){
  lgfx::v1::touch_point_t l_tp;
  l_tp.x = _x;
  l_tp.y = _y;
  return l_tp;
}
inline void TouchBtn::switchToggleVal(){
  if(this->toggleVal == true)this->toggleVal = false;
  else if(this->toggleVal == false)this->toggleVal = true;
  else ;
}
inline float TouchBtn::getSliderValx(){
  return this->sliderValx;
}
inline float TouchBtn::getSliderValy(){
  return this->sliderValy;
}
inline void TouchBtn::setSliderVal(float _x, float _y){
  this->sliderValx = _x;
  this->sliderValy = _y;
}
inline bool TouchBtn::getToggleVal(){
  return this->toggleVal;
}

// inline lgfx::v1::touch_point_t TouchBtn::getTilePos(){
//   return tilePos;
// }

inline void TouchBtn::setTilePos(lgfx::v1::touch_point_t _pos){
  tilePos = _pos;
}
inline int TouchBtn::get_xy_mode(){
  return xy_mode;
}
inline lgfx::v1::touch_point_t TouchBtn::getBtnPos(){
  return getTouchPoint(this->s_x, this->s_y);
}
inline void TouchBtn::setBtnName(std::string _btn_name){
  btn_name = _btn_name;
}
inline std::string TouchBtn::getBtnName()
{
  return btn_name;
}
inline void TouchBtn::setDrawFinishF(bool _drawFinishF){
  drawFinishF = _drawFinishF;
}
inline bool TouchBtn::getDrawFinishF(){
  return drawFinishF;
}
