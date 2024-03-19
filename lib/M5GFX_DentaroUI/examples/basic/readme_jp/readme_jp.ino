//LovyanGFXの設定です。2021年11月時点での動作を確認しています。

//ESP32などの自作デバイス派の人は、TouchBtn.hppを開き、コメントアウトし直してください。
//AUTODETECTを使用せず、LGFX_ESP32_custom.hppにピンアサインなどの設定を書き込むことで使えるようになります。

//でんたろう自作UIライブラリを読み込みます。LovyanGFXがないと動きません。
#include <LovyanGFX_DentaroUI.hpp>

//利用のながれ
//create系の関数で、UIコンテナというボタンのはいる箱を作り、その中を行と列で区切ることで、たくさんのボタンを一気に作ります。

//create時UIコンテナに自動でIDが貼られ以下のようなシリアル出力があるので、UIのID番号情報を確認してからloop内で
//ui.getUiID("SLIDER_0")などのdraw作業をしてください。（基本的に上から0,1,2と順番にふられます）
//These UI IDs are generated automatically. Check out the output of Serial when you create a UIContainer.
//[UI_ID information]
//SLIDER_0=[0]
//BTN_1=[1]

LGFX lcd;// LGFXのインスタンスを作成
LovyanGFX_DentaroUI ui;//lcdを入れる
static std::uint32_t sec, psec;
static std::uint32_t fps = 0, frame_count = 0;
int v,r,g,b = 127;
void setup() {
  Serial.begin( 115200 ); delay( 50 );  // Serial Init Wait
  //lcd.init();lcd.begin();lcd.setRotation(1);lcd.setColorDepth(24);//ここでlcd関係を宣言するとなぜかタッチパネルキャリブレーションができます。
  ui.begin( lcd, 5, 24, 0, true );//lcd, 色深度,回転方向,タッチキャリブレーションの有無//uiを使うための準備
  ui.createSliders( 0,   140, 180, 180, 1, 4, X_VAL, MULTI_EVENT);
  //スライダ用の箱を用意(x,y,w,h)し、x方向1列、y方向4行に区切ったスライダを生成します。
  //X方向の値（X_VAL）を利用します。
  //このスライダはMULTI_EVENT（何かのイベントがあれば）のときに反応するように設定しています。
  
  ui.createToggles( 180, 140,  60, 180, 1, 4, TOUCH );
  //トグル用の箱を用意(x,y,w,h)し、x方向1列、y方向4行に区切ったトグルボタンを生成します。
  //このスライダはMULTI_EVENT（何かのイベントがあれば）のときに反応するように設定しています。
  //toggleはTOUCH,WTAPPED,TAPPEDイベントで利用してください。
  
  ui.setBtnName( 0, "Brightness"  ); 
  ui.setBtnName( 1, "RED"        );
  ui.setBtnName( 2, "GREEN"       ); 
  ui.setBtnName( 3, "BLUE"       );
  //スライダボタンのID番号を指定して、スライダに名前をつけます。
  
  ui.setBtnName( 4, "MAX", "OFF"  ); 
  ui.setBtnName( 5, "MAX", "OFF" );
  ui.setBtnName( 6, "MAX", "OFF"  ); 
  ui.setBtnName( 7, "MAX", "OFF" );
  //トグルボタンのID番号を指定して、オン状態、オフ状態の順で名前をつけます。

  //初期描画
  //スライダの値を入れておく
  ui.setSliderVal( ui.getUiID("SLIDER_0"), 0 , 1.0, 0.5 ); 
  v = 255;
  //スライダボタンを描画します。
  ui.drawSliders( ui.getUiID("SLIDER_0"), lcd );
  //トグルボタンを描画します。
  ui.drawToggles( ui.getUiID("BTN_1"),    lcd );

}
void loop( void ){
  ui.update( lcd );//この一行でタッチボタンイベントを取得します。

  //タッチされたボタン（スライダ）のID番号を取得し、UIコンテナ内のボタン番号と一致したら、スライダの値を変数に格納します。
  if( ui.getTouchBtnID() == ui.getUiFirstNo( ui.getUiID("SLIDER_0") )   ){ v = int( ui.getSliderVal( ui.getUiID("SLIDER_0"), 0 )*255 ); }
  if( ui.getTouchBtnID() == ui.getUiFirstNo( ui.getUiID("SLIDER_0") )+1 ){ r = int( ui.getSliderVal( ui.getUiID("SLIDER_0"), 1 )*255 ); }
  if( ui.getTouchBtnID() == ui.getUiFirstNo( ui.getUiID("SLIDER_0") )+2 ){ g = int( ui.getSliderVal( ui.getUiID("SLIDER_0"), 2 )*255 ); }
  if( ui.getTouchBtnID() == ui.getUiFirstNo( ui.getUiID("SLIDER_0") )+3 ){ b = int( ui.getSliderVal( ui.getUiID("SLIDER_0"), 3 )*255 ); }
  
  //タッチされたトグルボタンの値がtrueだったら、スライダのX値を1.0に、Y値を0.5に強制的に置き換えます。
  if( ui.getToggleVal( ui.getUiID("BTN_1"), 0 ) == true){ui.setSliderVal( ui.getUiID("SLIDER_0"), 0 , 1.0, 0.5 ); v = 255;}
  if( ui.getToggleVal( ui.getUiID("BTN_1"), 1 ) == true){ui.setSliderVal( ui.getUiID("SLIDER_0"), 1 , 1.0, 0.5 ); r = 255;}
  if( ui.getToggleVal( ui.getUiID("BTN_1"), 2 ) == true){ui.setSliderVal( ui.getUiID("SLIDER_0"), 2 , 1.0, 0.5 ); g = 255;}
  if( ui.getToggleVal( ui.getUiID("BTN_1"), 3 ) == true){ui.setSliderVal( ui.getUiID("SLIDER_0"), 3 , 1.0, 0.5 ); b = 255;}

  // //何かしらのイベントが発生したときだけ、矩形を描画し、画面輝度を変えます。
  if( ui.getEvent() != NO_EVENT ){
    lcd.fillRect( 0, 40, 240, 100, lcd.color888( r,g,b ) ); lcd.setBrightness( v );
  //   //スライダボタンを描画します。
    ui.drawSliders( ui.getUiID("SLIDER_0"), lcd );
  //   //トグルボタンを描画します。
    ui.drawToggles( ui.getUiID("BTN_1"),    lcd );
  }

  ui.showTouchEventInfo( lcd, lcd.width() - 80, 0 );//タッチイベントを視覚化する
  ui.showInfo( lcd ,0, 48);//ボタン情報、フレームレート情報などを表示します。
  //WDT対策
  delay(1);
}
