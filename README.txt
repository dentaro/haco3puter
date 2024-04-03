try the following steps, which might help you:

Open Visual Studio Code: To use PlatformIO, you need to open Visual Studio Code.

Set up PlatformIO:

Open Visual Studio Code and select Extensions.
In the search bar, type "PlatformIO" and find PlatformIO IDE.
Click the Install button to install the extension.
Once PlatformIO is installed successfully, restart Visual Studio Code.
Create a PlatformIO project:
In the VSCode terminal, execute pio pkg update to install packages to enable the use of the M5Stack StampS3 board.

Create a new project:
Click on the PlatformIO icon in the sidebar of Visual Studio Code and create a new project.
Press the Create New Project button and set the following:

Name: haco3puter
Board: M5Stack StampS3
Framework: Arduino

Build and upload test:
After preparing the source code, use PlatformIO's build feature to compile and write to the microcontroller to verify if the empty project compiles correctly.
Download the source code for haco3puter and overwrite the following in the empty project:

platformio.ini
lib
src
data (create this folder if it doesn't exist)
Install dependent libraries:
Two dependent libraries, M5GFX and M5unified, will be installed when you perform another write.

Upload the contents of the data folder to SPIFFS:
Under PROJECT TASKS, press Upload Filesystem Image to copy all contents of the data folder to the SPIFFS, which is the file system area.
The file structure within the data folder is listed in the cheat sheet.

By following these steps, you can create a new project with PlatformIO and start developing haco3.


The sample Lua program is located in data/appname/main.lua. By referencing the cheat sheet, you can edit main.lua to create or modify the game. Once you've finished coding, please upload it to SPIFFS again.

Additionally, you can use the built-in editor to edit Lua code and adjust parameters without needing a PC.

We're using Lua 5.3, so please refer to Lua's website for Lua syntax.

Press "~" (Aa+esc) to return to the menu.

Press "OK" to confirm the app selection.

Press "|" to toggle between the editor and execution modes.

In editor mode:

Press "fn" to switch, and the right bar will turn orange.

The mode changes, and you can use the cursor and other features.

Press "Aa" to switch, and the right bar will turn blue.

You can toggle between uppercase and lowercase and perform other tasks.


We achieve functionalities such as rendering 1000 stars, map deployment, and music playback entirely without utilizing PSRAM.

Please utilize the PSRAM area for modifications or other purposes.

For regular maps, draw them in 8 colors as PNG files and place them in /data/init/map, where they will be automatically generated.

After drawing a 256*256 world map in 8 colors, use the included processing application to export it as a binary file. Then, place it in /data/init/param/map to create your original map.

Enjoy!
--

コンパイル手順
 Visual Studio Codeを開く: PlatformIOを使用するためには、Visual Studio Codeを開きます。

1.PlatformIOを使えるようにしてください。
・Visual Studio Codeを開き、Extensions（拡張機能）を選択します。
・検索バーに「PlatformIO」と入力し、PlatformIO IDEを見つけます。
・インストールボタンをクリックして、拡張機能をインストールします。
・PlatformIOが正常にインストールされたら、Visual Studio Codeを再起動します。
 ・PlatformIOプロジェクトの作成手順： VScodeのターミナルで pio pkg update を実行して、M5Stack StampS3のボードが使えるようにパッケージをインストールしておいてください。

・新しいプロジェクトの作成: Visual Studio Codeのサイドバーから、PlatformIOのアイコンをクリックして、新しいプロジェクトを作成します。 Create New Projectボタンを押し

Name:haco3puter
Board:M5Stack StampS3
Framework:Arduino 

に設定してください。 

・ビルドと書き込みテスト: ソースコードが準備できたら、PlatformIOのビルド機能を使用してコンパイルし、マイクロコントローラに書き込み、空のプロジェクトが正しくコンパイルされるかを確かめてください。 
haco3puterのソースコードをダウンロードし、空のプロジェクトのうち、 以下を上書きしてください。 
platformio.ini
lib
src data 
dataフォルダは初めは存在しないのでコピーして追加してください。
再び書き込みを実行すると、

M5GFX
M5unified
 という２つの依存ライブラリがインストールされ、ライブラリの環境構築が行われます。

・dataフォルダの中身をSPIFFSにアップロードする PROJECT TASKS
の中にある
UploadFilesystem Image を押すと dataフォルダの全ての内容が、ファイルシステム領域であるSPIFFSにコピーされます。 dataフォルダの中のファイル構造はチートシートに記載してあります。

これらの手順に従うと、PlatformIOで新しいプロジェクトを作成し、haco3の開発を開始できます。

--

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

PSRAMをまったく使わずに、1000個の星やマップ展開、音楽再生などを実現しています。

PSRAM領域は改造などの際にお使い下さい

通常のマップは8色でpngを描き、/data/init/mapの中に置けば自動で生成してくれます。

256*256のワールドマップは、8色のpngを描いたあとで、同梱のprocessingアプリでbinを書き出し、/data/init/param/mapの中に置けば、オリジナルのマップが作れます。
