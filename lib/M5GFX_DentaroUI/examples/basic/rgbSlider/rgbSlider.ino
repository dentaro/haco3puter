#include <LovyanGFX_DentaroUI.hpp>
static LGFX lcd;
LovyanGFX_DentaroUI ui;

int c_val[4] = { 127, 127, 127, 127 };
String btn_name[4] = { "Brightness", "RED", "GREEN", "BLUE" };
void setup() {
  Serial.begin( 115200 ); delay( 50 );  // Serial Init Wait
  ui.begin( lcd, 3, 24, 0, true );//lcd, 色深度,回転方向,タッチキャリブレーション
  ui.createSliders( 0,   140, 180, 180, 1, 4, X_VAL, MULTI_EVENT );
  ui.createToggles( 180, 140,  60, 180, 1, 4, TOUCH );
  for( int i=0; i < 4; i++ ){ ui.setBtnName( i, btn_name[i] ); ui.setBtnName( 4 + i, "MAX", "OFF" ); }
}
void loop( void ){
  ui.update( lcd );
  for( int i=0; i < 4; i++ ){
    if( ui.getTouchBtnID() == ui.getUiFirstNo( ui.getUiID("SLIDER_0") )+i  ){ c_val[i] = int( ui.getSliderVal( ui.getUiID("SLIDER_0"), i )*255 ); }
    if( ui.getToggleVal( ui.getUiID("BTN_1"), i ) == true){ui.setSliderVal( ui.getUiID("SLIDER_0"), i , 1.0, 0.5 ); c_val[i] = 255; }
  }
  if( ui.getEvent() != NO_EVENT ){ lcd.setBrightness( c_val[0] ); lcd.fillRect( 0, 0, 240, 140, lcd.color888( c_val[1], c_val[2], c_val[3] ) ); };
  ui.drawSliders( ui.getUiID("SLIDER_0"), lcd); 
  ui.drawToggles( ui.getUiID("BTN_1"),    lcd); delay(1);
}//24行で完成！
