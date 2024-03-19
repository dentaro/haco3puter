0,

デバック用起動モードの設定です。（最後にカンマが必要）

0,通常起動モード（通常はこちらに戻しておいてください）
setFileName("/init/main.lua");

1,ASPモード：共有のWiFiに入るWiFi接続モード

wifiDebugRequest = true;
wifiDebugSelf = false;

2,APモード：ESP32自体がアクセスポイントになるWiFi接続モード

wifiDebugRequest = true;
wifiDebugSelf = true;

