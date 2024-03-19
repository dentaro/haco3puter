#include <LovyanGFX_DentaroUI.hpp>
static LGFX lcd;
LovyanGFX_DentaroUI ui;
static LGFX_Sprite ui_sprite0( &lcd );//スライダ用
static LGFX_Sprite output_sprite( &lcd );//テキスト用
int btnNo = 0;
void setup() {
  Serial.begin( 115200 ); delay( 50 );  // Serial Init Wait
  ui.begin( lcd, 3, 24, 3, true );//lcd, 色深度,回転方向,タッチキャリブレーション
  ui.createBtns( 45, 170,  150, 150, 3, 3, ui_sprite0, TOUCH );
  output_sprite.createSprite(240, 64);
}
void loop( void )
{
  ui.update( lcd );
  if( ui.getEvent() != NO_EVENT ){//何かイベントがあれば
    if(ui.getTouchBtnID() == 0) btnNo = 0;
    else if(ui.getTouchBtnID() == 1) btnNo = 1;
    else if(ui.getTouchBtnID() == 2) btnNo = 2;
    else if(ui.getTouchBtnID() == 3) btnNo = 3;
    else if(ui.getTouchBtnID() == 4) btnNo = 4;
    else if(ui.getTouchBtnID() == 5) btnNo = 5;
    else if(ui.getTouchBtnID() == 6) btnNo = 6;
    else if(ui.getTouchBtnID() == 7) btnNo = 7;
    else if(ui.getTouchBtnID() == 8) btnNo = 8;
    ui.drawBtns( ui.getUiID("BTN_0"), lcd, ui_sprite0 ); 
    if( ui.getEvent() == TOUCH ){//TOUCHの時だけ
      output_sprite.setTextSize(2, 2);
      output_sprite.setTextColor(0xFFFFFFU, 0);
      output_sprite.setTextWrap(true);
      output_sprite.setTextScroll(true);
      output_sprite.printf("%d ", btnNo);
      output_sprite.pushSprite(0, 30);
    }
  }  
  ui.showTouchEventInfo( lcd, lcd.width() - 100, 0 );//タッチイベントを視覚化する
  ui.showInfo( lcd, 0, 0 );//ボタン情報、フレームレート情報などを表示します。
  delay(1);
}
