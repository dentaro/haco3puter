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
void setup() {
  Serial.begin( 115200 ); delay( 50 );  // Serial Init Wait
  ui.begin( lcd, 3, 16, 0, true );//lcd, 色深度,回転方向,タッチキャリブレーション
  //円形Uiを格納できる一番大きなスプライトを用意
//  ui_sprite0.setPsram(USE_PSRAM);
//  ui_sprite0.setColorDepth( COL_DEPTH );
//  ui_sprite0.createSprite( 150, 150 );
  //円形ボタンをセット
  //  ui.createOBtns( 25, 75, 22, 8, TOUCH );//外半径r0、内半径r1、スタート角（右から時計回り）、分割数、イベント(位置は描画の時に決める)
//  ui.createOBtns( 1, 25, 22, 1, TOUCH );
  ui.createOBtns( 25, 75, 22, 8, MOVE );//外半径r0、内半径r1、スタート角（右から時計回り）、分割数、イベント(位置は描画の時に決める)
  ui.createOBtns( 1, 25, 22, 1, MOVE );
}
void loop( void )
{
  ui.update( lcd );
  
  if( ui.getEvent() != NO_EVENT ){//何かイベントがあれば
    
    lcd.fillRect(0, 30, 240, 140, TFT_BLUE);
    c = TFT_GREEN;
    if(ui.getTouchBtnID() == 0)     {x++;y++;}
    else if(ui.getTouchBtnID() == 1){    y++;}
    else if(ui.getTouchBtnID() == 2){x--;y++;}
    else if(ui.getTouchBtnID() == 3){x--;    }
    else if(ui.getTouchBtnID() == 4){x--;y--;}
    else if(ui.getTouchBtnID() == 5){    y--;}
    else if(ui.getTouchBtnID() == 6){x++;y--;}
    else if(ui.getTouchBtnID() == 7){x++;    }
    if(ui.getTouchBtnID() == 8)c = TFT_RED;
    if(x > 24)x = 0;if(x < 0)x = 24;
    if(y > 16 )y = 3;if(y < 3)y = 16;
    lcd.fillRect(x*w, y*w, w, w, c);
    
    //UI自体を動かす
    if(a<360){a++;}
    else{a=0;}
    ui_x = 45 + cos(a*PI/180)*45;
    ui_y = 156;

    //UIの両サイドを塗りなおす
//    lcd.fillRect(ui_x-10, 170, 10, 150, TFT_BLACK);
//    lcd.fillRect(ui_x+150, 170, 10, 150, TFT_BLACK);

    //ui_sprite0スプライトに描画
//    ui_sprite0.setColor(TFT_BLUE);//円形ボタンは1分割の時だけボタンの色変えられます(分割すると白)
//    ui_sprite0.setTextColor(TFT_BLACK);//ボタン名の色
    ui.drawOBtns( ui.getUiID("BTN_0"), lcd, ui_x, ui_y );

//    ui_sprite0.setColor(TFT_RED);//円形ボタンは1分割の時だけボタンの色変えられます(分割すると白)
//    ui_sprite0.setTextColor(TFT_BLACK);//ボタン名の色
    ui.drawOBtns( ui.getUiID("BTN_1"), lcd, ui_x, ui_y );

//    ui_sprite0.pushSprite( ui_x, ui_y );//一つのスプライトui_sprite0にまとめて出力
  }
  ui.showTouchEventInfo( lcd, lcd.width() - 50, 0 );//タッチイベントを視覚化する
  ui.showInfo( lcd ,0, 48 );//ボタン情報、フレームレート情報などを表示します。
  delay(1);
}
