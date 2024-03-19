//別途LovyanGFXライブラリが必要です。サンプル内の「tokyo」フォルダをディレクトリごとSDカードにコピーしておいてください。
#include <LovyanGFX_DentaroUI.hpp>

LGFX lcd;
LovyanGFX_DentaroUI ui( &lcd );
//あらかじめ必要な領域のタイルマップをSDカードに読み込んでおいてください。"/tokyo/15/29099/12901.png"といったパスになります。
String host ="tokyo";//ルートディレクトリ名です。
double latPos = 35.68999241096032;//新宿駅の座標
double lonPos = 139.70054064148115;
int tileZoom = 15; //座標計算用
double walkLatPos = 0.0;
double walkLonPos = 0.0;
double walkAngle = 0.0;

void task2(void *pvParameters)
{
  while (1) {
    if(ui.getDownloadF() == true){
      ui.task2_setPngTile(ui.get_gPosId());
    }
    ui.setDownloadF(false);
    delay(10);
  }
}

void setup() 
{
  Serial.begin( 115200 ); delay( 50 );  // Serial Init Wait
  xTaskCreateUniversal(//マルチタスクでバックグラウンド読み込み処理(RTOS)
    task2,        //pxTaskCode  作成するタスク関数
    "task2",      //pcName  表示用タスク名
    8192,         //usStackDepth スタックメモリ量
    NULL,         //pvParameters 起動パラメータ
    1,            //uxPriority  優先度
    NULL,         //pxCreatedTask タスクハンドル
    PRO_CPU_NUM   //xCoreID 実行するコア
  );
  ui.begin( &lcd, host, SHIFT_NUM, TILE_COL_DEPTH, 3, true );//lcd, ホスト名, shiftの数, 色深度16, 回転方向,タッチキャリブレーション
}
void loop( void )
{
  ui.update( &lcd );
    lcd.setTextSize(1);
    lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    lcd.setCursor(0,20);
    lcd.setFont(&lgfxJapanGothicP_20);
    lcd.setTextWrap(true);
    lcd.setTextScroll(true);
    //-------指定座標（新宿駅）の周辺をぐるぐる回るための処理
    walkAngle += 0.1;
    float ZoomValue =  pow(2, ui.getZtile())*0.00625;
    if( walkAngle > TWO_PI) walkAngle -= TWO_PI;
    walkLatPos = latPos + sin(walkAngle)*1/ZoomValue;//ZoomValue＝15の時204.8;
    walkLonPos = lonPos + cos(walkAngle)*1/ZoomValue;//ZoomValue＝15の時204.8;
    //-------
//    ui.drawMaps(&lcd, walkLatPos, walkLonPos, tileZoom);//経緯度とズーム情報からタイル座標を計算して表示
    ui.nowLoc(&lcd);//現在位置を表示
    ui.setPreDirID(ui.getDirID());//過去の方向を検出
    lcd.fillRect(120 - ui.getVx(), 120 - ui.getVy(), 10, 10, TFT_GREEN);//過去の方向を表示
    ui.showTouchEventInfo( &lcd, lcd.width() - 50, 0 );//タッチイベントを視覚化する
    ui.showInfo( &lcd , 0, 48 );//ボタン情報、フレームレート情報などを表示します。
    delay(1);
}
