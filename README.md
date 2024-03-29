Haco3をM5Cardputerで動くように改造しました！

haco3はpico8ライクな自作ゲームエンジンで、安価で気軽に、ビジュアルプログラミングやメディアデザインを学ぶ環境を提供するためにソフトウエア開発をつづけています。

PlatformIOのソースコードです。

src,lib,data,iniファイルをコピーしてコンパイルしてください。

dataフォルダをSPIFFSにアップロードしないと動きませんのでご注意ください。

dataフォルダの中にluaのサンプルコードがたくさん入っています。

main.luaを編集することで、ゲームを作ったり編集することができます。

また、内蔵エディタをつかってPCなしでLuaコードを編集して、ちょっとしたパラメーターの調整ができます。

Lua5.3を使用していますので、Luaの表記法はそちらを参照して下さい。

「〜」(Aa+esc)でメニューに戻る

「リターンOK」でアプリ決定

「｜」縦棒で、エディタと実行モード切り替え

エディタモードでは、
「fn」で切り替えすると右のバーがオレンジに。

モードが変わり、カーソルなどが使えるようになる

「Aa」で切り替えすると右のバーが青に。

大文字小文字の切り替えなどができる。

チートシートを作りました。改善していきますが、間違いが結構あるとおもいますのでその時はコードを参照して下さい。

treemap.txtにアプリのファイル構成図を書いてあります。

PSRAMをまったく使わずに、1000個の星やマップ展開、音楽再生などを実現しています。

PSRAM領域は改造などの際にお使い下さい
