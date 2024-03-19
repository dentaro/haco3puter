#include <LovyanGFX_DentaroUI.hpp>
static LGFX lcd;
LovyanGFX_DentaroUI ui;
//static LGFX_Sprite ui_sprite0( &lcd );//円形ボタン用
//static LGFX_Sprite ui_sprite1( &lcd );//円形ボタン用
int btn_No[4] = {0,2,6,8};//四隅のボタンを消す
String btn_name[9] = { "0", "A", "0", "B", "C", "D", "0", "E", "0" };
int w = 10;
int x,y = 3;
int a = 0;
int ui_x,ui_y = 0;
int c = TFT_GREEN;
int rotateNum = -999;

void setup() {
  Serial.begin( 115200 ); delay( 50 );  // Serial Init Wait
  ui.begin( lcd, 3, 16, 0, true );//lcd, 色深度,回転方向,タッチキャリブレーション
  //円形Uiを格納できる一番大きなスプライトを用意
//  ui_sprite0.setPsram(USE_PSRAM);
//  ui_sprite0.setColorDepth( COL_DEPTH );
//  ui_sprite0.createSprite( 150, 150 );
  //円形ボタンをセット
  ui.createOBtns( 25, 75, 0, 1, MOVE );//外半径r0、内半径r1、スタート角（右から時計回り）、分割数、再描画イベント(位置は描画の時に決める)
  ui.createOBtns( 1, 25, 0, 1, MOVE );
}
void loop( void )
{
  ui.update( lcd );
  if( ui.getEvent() != NO_EVENT ){//何かイベントがあれば

  //UI自体を動かす
    if(a<360){a++;}
    else{a=0;}
    ui_x = 45 + cos(a*PI/180)*45;
    // ui_x = 50;
    ui_y = 156;
  
  if(ui.getTouchBtnID() == 1){
    rotateNum = 0;
    }
  ui.updateOBtnSlider(ui.getUiID("BTN_0"), ui_x, ui_y );
  rotateNum += ui.getOBtnDiffAngle(ui.getUiID("BTN_0"));

  lcd.fillRect(0, 30, 150, 30, TFT_BLACK);
  lcd.setFont(&fonts::Font4);
  lcd.setTextSize(1.25);
  lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  lcd.setCursor(0, 30);
  lcd.print(String(rotateNum));

    //UIの両サイドを塗りなおす
//    lcd.fillRect(ui_x -  10, 170, 10, 150, TFT_BLACK);
//    lcd.fillRect(ui_x + 150, 170, 10, 150, TFT_BLACK);

    //ui_sprite0スプライトに描画
    lcd.setColor(TFT_BLUE);//円形ボタンは1分割の時だけボタンの色変えられます(分割すると白)
    lcd.setTextColor(TFT_BLACK);//ボタン名の色
    ui.drawOBtns( ui.getUiID("BTN_0"), lcd, ui_x, ui_y );

    lcd.setColor(TFT_RED);//円形ボタンは1分割の時だけボタンの色変えられます(分割すると白)
    lcd.setTextColor(TFT_BLACK);//ボタン名の色
    ui.drawOBtns( ui.getUiID("BTN_1"), lcd, ui_x, ui_y );

//    ui_sprite0.pushSprite( ui_x, ui_y );//一つのスプライトui_sprite0にまとめて出力
  }
  ui.showTouchEventInfo( lcd, lcd.width() - 50, 0 );//タッチイベントを視覚化する
  ui.showInfo( lcd ,0, 48 );//ボタン情報、フレームレート情報などを表示します。
  delay(1);
}
