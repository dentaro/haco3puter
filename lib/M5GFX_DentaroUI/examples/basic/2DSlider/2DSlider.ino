//LovyanGFXライブラリがないと動きません。
#include <M5GFX_DentaroUI.hpp>
#define SLIDER_NO_0 0
#define SLIDER_NO_1 1
static M5GFX lcd;
M5GFX_DentaroUI ui();
//static LGFX_Sprite ui_sprite0( &lcd );//スライダ用
void setup(){
  Serial.begin( 115200 ); delay( 50 );  // Serial Init Wait
  ui.begin( lcd, 3, 24, 0, true );//lcd, 色深度,回転方向,タッチキャリブレーション
  ui.createSliders( 0, 160, 240, 160, 2, 1, XY_VAL, MULTI_EVENT );
  ui.setBtnName( ui.getUiID("SLIDER_0"), "2DSlider0" );
  ui.setBtnName( ui.getUiID("SLIDER_0")+1, "2DSlider1" );
  lcd.setBrightness( 127 ); 
}
void loop( void ){
  ui.update( lcd );
  if( ui.getEvent() != NO_EVENT ){ 
    lcd.fillRect( 0, 0, 120, 160, TFT_BLACK);
    lcd.fillRect(
      ui.getSliderVal( ui.getUiID("SLIDER_0"), SLIDER_NO_0, X_VAL) * 120,
      ui.getSliderVal( ui.getUiID("SLIDER_0"), SLIDER_NO_0, Y_VAL) * 160,
      20,20,TFT_RED); 
    lcd.fillRect( 120, 0, 120, 160, TFT_BLACK);
    lcd.fillRect(
      ui.getSliderVec2( ui.getUiID("SLIDER_0"), SLIDER_NO_1).x * 120,
      ui.getSliderVec2( ui.getUiID("SLIDER_0"), SLIDER_NO_1).y * 160,
      20,20,TFT_BLUE); 
  }
  ui.drawSliders( ui.getUiID("SLIDER_0"), lcd);
  ui.showTouchEventInfo( lcd, lcd.width() - 100, 0 );//タッチイベントを視覚化する
  ui.showInfo( lcd ,0,0 );//ボタン情報、フレームレート情報などを表示します。
  delay(1); 
}
