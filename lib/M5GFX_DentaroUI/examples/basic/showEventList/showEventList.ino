// #include <LovyanGFX_DentaroUI.hpp>
// LGFX lcd;
// LovyanGFX_DentaroUI ui;

// static LGFX_Sprite flickUiSprite;//フリックパネル用
// uint16_t clist[3] = {TFT_RED, TFT_GREEN, TFT_BLUE};
// uint8_t cNo = 0;
// uint8_t addx = 50;
// uint8_t addy = 16;
// void setup() 
// {
//   Serial.begin( 115200 ); delay( 50 );  // Serial Init Wait
//   ui.begin( lcd, SHIFT_NUM, 16, 0, true );//lcd, フリックパネル, shiftの数, 色深度, 回転方向,タッチキャリブレーション//アドレス渡し
//   ui.flickSetup();//flickでBTN_4まで使います。//アドレス渡し
//   //これ以降、ボタンを配置する場合はBTN_5となります。
// }
// void loop( void ){
//   ui.update( lcd );//参照渡し
//   //ui.flickUpdate( lcd, layoutSprite, ui_sprite0, ui_sprite1, ui_sprite2, ui_sprite3 ,flickUiSprite);//参照渡し
//   for(int i = 0; i<32; i++){
//     if(ui.getEventBit(i)){
//       lcd.fillRect(20, addy + 8*i, 8,8, clist[cNo]);
//     }
//   }

//   lcd.setFont(&fonts::Font0);
//   lcd.setTextColor(TFT_WHITE, TFT_BLACK);
//   lcd.setCursor(30, addy + 8 * DRAG);lcd.print("DRAG");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * TAPPED);lcd.print("TAPPED");//イベント定義を描画
//     // lcd.fillRect(30+addx, addy + 8 * TAPPED,10,10,TFT_RED);
//   lcd.setCursor(30, addy + 8 * WTAPPED);lcd.print("WTAPPED");//イベント定義を描画
//     // lcd.fillRect(30+addx, addy + 8 * WTAPPED,10,10,TFT_YELLOW);
//   lcd.setCursor(30, addy + 8 * FLICK);lcd.print("FLICK");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * FLICKED);lcd.print("FLICKED");//イベント定義を描画

//   lcd.setCursor(30, addy + 8 * TOUCH);lcd.print("TOUCH");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * WAIT);lcd.print("WAIT");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * MOVE);lcd.print("MOVE");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * RELEASE);lcd.print("RELEASE");//イベント定義を描画

//   lcd.setCursor(30, addy + 8 * RIGHT_FLICK);lcd.print("RIGHT_FLICK");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * U_R_FLICK);lcd.print("U_R_FLICK");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * UP_FLICK);lcd.print("UP_FLICK");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * L_U_FLICK);lcd.print("L_U_FLICK");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * LEFT_FLICK);lcd.print("LEFT_FLICK");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * D_L_FLICK);lcd.print("D_L_FLICK");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * DOWN_FLICK);lcd.print("DOWN_FLICK");//イベント定義を描画
//   lcd.setCursor(30, addy + 8 * R_D_FLICK);lcd.print("R_D_FLICK");//イベント定義を描画

//   if(ui.getEvent() != NO_EVENT){

//     lcd.fillRect(30+addx, addy + 8 * TAPPED,10,10,TFT_BLUE);
//     lcd.fillRect(30+addx, addy + 8 * WTAPPED,10,10,TFT_YELLOW);

//     if( ui.getEventBit(TAPPED) )lcd.fillRect(100, addy, 100,100, TFT_BLUE);
//     else if( ui.getEventBit(WTAPPED) )lcd.fillRect(100, addy, 100,100, TFT_YELLOW);

//     lcd.fillRect(140-20, addy+40-20, 60, 60, TFT_BLACK);//塗りなおし
//     lcd.fillRect(140, addy+40, 20, 20, TFT_GREEN);//中心
//     if( ui.getEventBit(RIGHT_FLICK) )lcd.fillRect(140+20, addy+40,    20, 20, TFT_RED);
//     if( ui.getEventBit(U_R_FLICK) )  lcd.fillRect(140+20, addy+40-20, 20, 20, TFT_RED);
//     if( ui.getEventBit(UP_FLICK) )   lcd.fillRect(140,    addy+40-20, 20, 20, TFT_RED);
//     if( ui.getEventBit(L_U_FLICK) )  lcd.fillRect(140-20, addy+40-20, 20, 20, TFT_RED);
//     if( ui.getEventBit(LEFT_FLICK) ) lcd.fillRect(140-20, addy+40,    20, 20, TFT_RED);
//     if( ui.getEventBit(D_L_FLICK) )  lcd.fillRect(140-20, addy+40+20, 20, 20, TFT_RED);
//     if( ui.getEventBit(DOWN_FLICK) ) lcd.fillRect(140,    addy+40+20, 20, 20, TFT_RED);
//     if( ui.getEventBit(R_D_FLICK) )  lcd.fillRect(140+20, addy+40+20, 20, 20, TFT_RED);

//     for(int i = 0; i<3; i++){
//       for(int j = 0; j<3; j++){
//         lcd.drawRect(120+i*20, addy+20+j*20, 20, 20, TFT_WHITE);//枠線
//       }
//     }
//   }

//   if( ui.getEvent() == RELEASE ){
//     cNo++;
//     if(cNo==3)cNo = 0;
//     ui.resetEventBits();
    
//     for(int i = 0; i<32; i++){
//       lcd.fillRect(20, addy + 8*i, 8,8, TFT_BLACK);
//       lcd.drawRect(20, addy + 8*i, 8,8, TFT_WHITE);
//     }
//   }

   
//  ui.showTouchEventInfo( lcd, lcd.width() - 50, 0 );//タッチイベントを視覚化する//ポインタ渡し
//  ui.showInfo( lcd , 0, 48 );//ボタン情報、フレームレート情報などを表示します。//ポインタ渡し
//   delay(1);
// }
