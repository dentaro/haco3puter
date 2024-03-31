#include "runLuaGame.h"


#include <chrono> // std::chrono
#include <thread> // std::this_thread

extern uint8_t mapArray[16][20];
extern int gSpr8numX;
extern int gSpr8numY;
extern int gWx;
extern int gWy;
extern  int gSprw;
extern  int gSprh;
extern bool drawMap();
extern bool readRowFromBinary2(const char *filename);
extern int HACO3_C0;
extern int HACO3_C1;
extern int HACO3_C2;
extern int HACO3_C3;
extern int HACO3_C4;
extern int HACO3_C5;
extern int HACO3_C6;
extern int HACO3_C7;
extern int HACO3_C8;
extern int HACO3_C9;
extern int HACO3_C10;
extern int HACO3_C11;
extern int HACO3_C12;
extern int HACO3_C13;
extern int HACO3_C14;
extern int HACO3_C15;
extern MyTFT_eSprite tft;


extern uint8_t masterVol;

extern uint8_t toolNo;

extern uint8_t clist2[16][3];

// extern M5Canvas sprite64;
// extern M5Canvas sprite256[2][2];

extern uint8_t sprno;
extern uint8_t repeatnum;

extern int8_t soundState;

// extern std::vector<uint8_t> rowData;
std::vector<std::vector<uint8_t>> rowData(16);

extern M5Canvas sprite64;
extern M5Canvas sprite88_roi;
extern String appfileName;
extern void startWifiDebug(bool isSelf);
extern void setFileName(String s);
extern bool isWifiDebug();
extern void readMap();
extern void reboot();
extern void reboot(String _fileName, uint8_t _isEditMode);
// extern Tunes tunes;
extern int pressedBtnID;
// extern M5GFX screen;
// extern M5GFX_DentaroUI ui(&screen);
extern M5GFX_DentaroUI ui;
extern int outputMode;
extern int mapsprnos[16];
extern int8_t sprbits[128];//8*16
extern vector<string> split(string& input, char delimiter);
extern String appNameStr;
extern String mapFileName;
extern float sliderval[2];
extern bool optionuiflag;
extern int frame;
extern int boxzerox;
extern int boxzeroy;
extern int allAddUiNum;
extern bool toneflag;
extern bool firstLoopF;
extern float radone;
extern bool downloadF;
extern void setOpenConfig(String fileName, uint8_t _isEditMode);
extern void getOpenConfig(String _wrfile);
extern std::deque<int> buttonState;//ボタンの個数未定

extern void restart(String _fileName, int _isEditMode);
extern int getcno2tftc(uint8_t _cno);

extern void broadchat(String _text);

extern char keychar;
// extern Editor editor;

extern uint8_t mainVol;

extern String savedAppfileName;
extern bool difffileF;//前と違うファイルを開こうとしたときに立つフラグ

// M5_CARDPUTER M5Cardputer;
// Speaker_Class speaker;


extern std::vector<uint8_t> sprite64cnos_vector;
extern int gameState;
// extern void getOpenConfig(String _wrfile);
extern uint16_t gethaco3Col(uint8_t haco3ColNo);

extern std::vector<std::array<float, 2>> bsParamFloat;
extern std::vector<std::array<uint8_t, 1>> bsParamInt8t;
extern std::vector<std::array<uint16_t, 1>> bsParamInt16t;

  struct Obj {
    float x;
    float y;
    float z;
    float angle;
    float scale;
    int colangle;
    int h;
    int s;
    int v;
    int r;
    int g;
    int b;
    int width;
    int height;

    std::vector<Vector3<float>> op;
    std::vector<Vector3<float>> p;
    // コンストラクタで初期値を設定
    Obj()
        : x(0), y(0), z(0), angle(0), scale(1.0),colangle(120), width(10), height(10)
    {
    }
    };




// Obj 構造体の動的配列を作成

    std::vector<Obj> objects;
    int objno = 0;

// float bsParamFloat[BSTARBUFNUM][2];
// uint8_t bsParamInt8t[BSTARBUFNUM][1];
// uint16_t bsParamInt16t[BSTARBUFNUM][1];
// 空のベクターを生成
    

int cursor = 0;

uint16_t hacol0;
uint16_t hacol1;
uint16_t hacol2;
uint16_t hacol3;

std::vector<uint8_t> buffer;
int bytesRead;


// runLuaGame.cpp ファイル内に以下のように RGBValues 配列を宣言してください。
// const uint8_t RGBValues[][3] PROGMEM = {//24bit

//   { 0,0,0},//0: 黒色
//   { 27,42,86 },//1: 暗い青色
//   { 137,24,84 },//2: 暗い紫色
//   { 0,139,75 },//3: 暗い緑色
//   { 183,76,45 },//4: 茶色
//   { 97,87,78 },//5: 暗い灰色
//   { 194,195,199 },//6: 明るい灰色
//   { 255,241,231 },//7: 白色
//   { 255,0,70 },//8: 赤色
//   { 255,160,0 },//9: オレンジ
//   { 255,238,0 },//10: 黄色
//   { 0,234,0 },//11: 緑色
//   { 0,173,255 },//12: 水色
//   { 134,116,159 },//13: 藍色
//   { 255,107,169 },//14: ピンク
//   { 255,202,165}//15: 桃色

// };

// const uint8_t RGBValues[][3] PROGMEM = {//16bit用
//   {0, 0, 0},     // 0: 黒色=なし
//   {24, 40, 82},  // 1: 暗い青色
//   {140, 24, 82}, // 2: 暗い紫色
//   {0, 138, 74},  // 3: 暗い緑色
//   {181, 77, 41}, // 4: 茶色 
//   {99, 85, 74},  // 5: 暗い灰色
//   {198, 195, 198}, // 6: 明るい灰色
//   {255, 243, 231}, // 7: 白色
//   {255, 0, 66},  // 8: 赤色
//   {255, 162, 0}, // 9: オレンジ
//   {255, 239, 0}, // 10: 黄色
//   {0, 235, 0},   // 11: 緑色
//   {0, 174, 255}, // 12: 水色
//   {132, 117, 156}, // 13: 藍色
//   {255, 105, 173}, // 14: ピンク
//   {255, 203, 165}  // 15: 桃色
// };

    std::vector<Vector3<float>> opg;
    std::vector<Vector3<float>> pg;


extern "C" {
  void gprint(char* s){
    tft.setCursor(3, cursor);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.print(s);
    cursor += 4;
    if(cursor > 128){
      cursor = 0;
    }
  }

  const char *getF(lua_State *L, void *ud, size_t *size){
    struct LoadF *lf = (struct LoadF *)ud;
    (void)L; /* not used */
    char* ret = NULL;

    if(!lf->f.available()){
      *size = 0;
      return NULL;
    }

    lf->f.readStringUntil('\n').toCharArray(lf->buf, MAX_CHAR);
    ret = lf->buf;
    int len = strnlen(ret, MAX_CHAR);
    ret[len] = '\n'; // todo n, n+1 > MAX_CHAR ?
    ret[len + 1] = 0;

    *size = len + 1;
    // Serial.print("");
    // Serial.print(ret);
    // Serial.println(*size);
    return ret;
  }
}

int runLuaGame::loadSurface(File *fp, uint8_t* buf){
  uint8_t c;
  unsigned long offset;
  unsigned long width, height;
  unsigned long biSize;
  uint16_t bitCount;

  Serial.println("pre read");
  fp->read(&c, 1);
  Serial.println("read1");
  if(c != 'B'){
    printf("header error 0");
    // Serial.print(c);
    Serial.println("unknown header");
    return -1;
  }
  fp->read(&c, 1);
  Serial.println("read2");
  if(c != 'M'){
    printf("header error 1");
    return -1;
  }
  Serial.println("pre seek");
  fp->seek(4 + 2 + 2, SeekCur); // size, resv1, resv2
  fp->read((uint8_t*)&offset, 4);

  fp->read((uint8_t*)&biSize, 4);
  fp->read((uint8_t*)&width, 4);
  fp->read((uint8_t*)&height, 4);
  fp->seek(2, SeekCur); // skip biPlanes
  fp->read((uint8_t*)&bitCount, 2);

  Serial.println("pre check");
  if(width != 128){
    printf("invalid width:%d\n", width);
    return -1;
  }
  if(height != 128){
    printf("invalid height:%d\n", height);
    return -1;
  }
  if(bitCount != 8){
    printf("invalid bitCount:%x\n", bitCount);
    return -1;
  }

  fp->seek(biSize - (4 + 4 + 4 + 2 + 2), SeekCur);
  uint8_t r, g, b;
  for(unsigned int i = 0; i < 256; i ++){
    fp->read(&b, 1);
    fp->read(&g, 1);
    fp->read(&r, 1);
    fp->seek(1, SeekCur);
    // palette[i] = lua_rgb24to16(r, g, b);
    // Serial.print("palette");
    // Serial.println(i);
    // Serial.print(r);
    // Serial.print(g);
    // Serial.print(b);
  }

  Serial.println("pre seek");
  fp->seek(offset, SeekSet); // go to bmp data section

  for(unsigned int i = 0; i < width * height; i ++){
    uint8_t d;
    fp->read(&d, 1);
    *buf = d;
    buf ++;
  }
  return 0;
}

runLuaGame::runLuaGame(int _gameState, String _mn){
  gameState = _gameState;
  mapFileName = _mn;
}


int runLuaGame::l_fget(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int sprno = lua_tointeger(L, 1);
  int fbitno = lua_tointeger(L, 2);
  bool b = (sprbits[sprno]>>fbitno)&1;//fbitnoビットシフトしてから，最下位ビットのみを取り出す
  lua_pushboolean(L, b);
  return 1;
}


int runLuaGame::l_mget(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int celx = lua_tointeger(L, 1);
  int cely = lua_tointeger(L, 2);
  lua_pushinteger(L, mapArray[celx][cely]);
  return 1;
}


int runLuaGame::l_fset(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int8_t sprno = lua_tointeger(L, 1);
  int8_t fbitno = lua_tointeger(L, 2);
  int8_t val = lua_tointeger(L, 3);
  int8_t bitfilter = 0b00000001<<fbitno;
       if(val == 0)sprbits[sprno] &= ~bitfilter;//スプライト番号sprnoのfbitno番目を0に
  else if(val == 1)sprbits[sprno] |=  bitfilter;//スプライト番号sprnoのfbitno番目を1に
  return 0;
}
extern bool musicflag;
extern uint8_t musicNo;
// extern uint8_t musicSpeed;
extern uint8_t tickTime;//100がデフォルト

extern uint8_t loopStart;
extern uint8_t loopEnd;
extern uint8_t looplen;
extern uint8_t patternNo;
extern float bpm;

int runLuaGame::l_music(lua_State* L) {
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  musicNo = lua_tointeger(L, 1); // 未使用
  masterVol = lua_tointeger(L, 2);
  int _bpm = lua_tointeger(L, 3);
  musicflag = true;
  
  // Retrieve loop start and end values
  uint8_t _loopStart = lua_tointeger(L, 4);
  uint8_t _loopEnd = lua_tointeger(L, 5);
  
  // Set default values for loop start and end
  if (_loopStart == NULL) {
    _loopStart = 0;
  }
  if (_loopEnd == NULL) {
    _loopEnd = 63;
  }
  
  // Set tickTime to default value if not provided
  bpm = (_bpm != NULL) ? _bpm : 120;

  tickTime = (60000 / float(bpm))/8;//BPMから1拍あたりのミリ秒数を計算し、tickTimeに代入　8chで割る
  
  // Set patternNo and loop lengths
  patternNo = 0;
  loopStart = _loopStart;
  loopEnd = _loopEnd;
  looplen = (loopEnd - loopStart) + 1;
  

  return 0;
}

// int runLuaGame::l_music(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//   musicNo = lua_tointeger(L, 1);//今のところ使ってない
//   masterVol = lua_tointeger(L, 2);
//   int _bpm = lua_tointeger(L, 3);
//   uint8_t _loopStart = 0;
//   uint8_t _loopEnd = 63;
//   _loopStart = lua_tointeger(L, 4);
//   _loopEnd   = lua_tointeger(L, 5);

//   tickTime = 100;//まずデフォルト値を入れておく
//   patternNo = 0;

//   loopStart = _loopStart;
//   loopEnd = _loopEnd;
//   looplen = (loopEnd - loopStart)+1;

//   if(_bpm!=NULL){tickTime = _bpm;}

//   if(_loopStart!=NULL&&_loopEnd!=NULL){
//     loopStart = 0;
//     loopEnd = 63;
//     looplen = (loopEnd - loopStart)+1;
//   }
  

//   return 0;
// }

extern bool sfxflag;
extern uint8_t sfxNo;
extern uint8_t sfxChNo;
extern uint8_t sfxVol;
extern float sfxspeed;

int runLuaGame::l_sfx(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  uint8_t chno = lua_tointeger(L, 1);
  uint8_t sfxn = lua_tointeger(L, 2);
  uint8_t vol = lua_tointeger(L, 3);
  float spd = lua_tonumber(L, 4);
  sfxflag = true;
  sfxChNo = chno;
  sfxNo = chno;
  sfxVol = vol;
  sfxspeed = spd;

  return 0;
}
int runLuaGame::l_go2(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  appfileName = lua_tostring(L, 1);
  int gs = lua_tointeger(L, 2);
  gameState = 0;
  if(gs != NULL){
    gameState = gs;
  }
  // appfileName = (String)text;
  // setFileName(appfileName);
  // appfileName = text;
  // runFileName(appfileName);
  // mode = 1;//appfileNameのゲームを起動させるために1モードを1に
  // pressedBtnID = 100;

  setOpenConfig(appfileName,0);//configに書き込んでおく
  pressedBtnID = 9999;
  return 1;
}

int runLuaGame::l_gstat(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  lua_pushinteger(L, gameState);
  return 1;
}

int runLuaGame::l_vol(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int v = lua_tointeger(L, 1);//チャンネル?
  mainVol = v;
  return 0;
}


int runLuaGame::l_tone(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);//チャンネル?
  int f = lua_tointeger(L, 2);//周波数
  int sl = lua_tointeger(L, 3);//音の長さ

  if(sl!=0){
    M5Cardputer.Speaker.begin();
    M5Cardputer.Speaker.setVolume(127);
    M5Cardputer.Speaker.setChannelVolume(0, 127);
    M5Cardputer.Speaker.tone(f,sl);
    delay(sl);
    M5Cardputer.Speaker.stop();
  }
  ledcWriteTone(n, 0);    // 消音

  /// tone data (8bit unsigned wav)
  //ノイズ音

  // const uint8_t wavdata[64] PROGMEM = { 132,138,143,154,151,139,138,140,144,147,147,147,151,159,184,194,203,222,228,227,210,202,197,181,172,169,177,178,172,151,141,131,107,96,87,77,73,66,42,28,17,10,15,25,55,68,76,82,80,74,61,66,79,107,109,103,81,73,86,94,99,112,121,129 };
  //サイン波
//   const uint8_t wavdata[64] PROGMEM = {
//   128, 141, 153, 164, 175, 185, 194, 202, 209, 215, 220, 224, 227, 229, 230, 229, 
//   228, 225, 222, 217, 212, 206, 199, 192, 184, 176, 167, 157, 147, 137, 126, 115, 
//   104,  93,  82,  71,  60,  50,  40,  31,  23,  16,  10,   5,   2,   0,   0,   2,  
//     5,  10,  16,  23,  31,  40,  50,  60,  71,  82,  93, 104, 115, 126, 137, 147
// };


//三角波
//   const uint8_t wavdata[64] PROGMEM = {
//   128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224, 232, 240, 248, 
//   255, 247, 239, 231, 223, 215, 207, 199, 191, 183, 175, 167, 159, 151, 143, 135, 
//   128, 120, 112, 104,  96,  88,  80,  72,  64,  56,  48,  40,  32,  24,  16,   8,  
//     0,    8,  16,  24,  32,  40,  48,  56,  64,  72,  80,  88,  96, 104, 112, 120
// };

//矩形波
//   const uint8_t wavdata[64] PROGMEM = {
//     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
//     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
//     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
//   };

// speaker.setVolume(mainVol);//151が出力限界か？

// if(sl!=0){
//   if(sl!=0){
//   // speaker.tone(f, sl, 0, true, wavdata, sizeof(wavdata));
//   // speaker.tone(f, sl, 0);//チャンネル０
//   // speaker.tone(f/2, sl, 1);//チャンネル1
//   // speaker.tone(f*2, sl, 2);//チャンネル2
//   speaker.tone(f, sl, 0, true, wavdata, sizeof(wavdata));//チャンネル0
//   // speaker.tone(f*2, sl, 1, true, wavdata, sizeof(wavdata));//チャンネル1

//   while (speaker.isPlaying()) { delay(1); } // Wait for the output to finish.
//   speaker.stop();

//   }
// }


  // toneflag = true;

  // portENTER_CRITICAL(&Tunes::timerMux);
  // Tunes::d[n] = (uint16_t)(3.2768*f);
  // portEXIT_CRITICAL(&Tunes::timerMux);

  // portENTER_CRITICAL_ISR(&Tunes::timerMux);
  //   if(soundNo > -1)
  //   {
  //     // digitalWrite()//外部給電
  //     // ledcWrite()//パルス幅変調
  //     dacWrite(SPEAKER_PIN, (Wx[soundNo][floor(wcnt)]));//255が最高値　スピーカー無音が128で、上下に波形が出る、ブザーは0~255
  //     wcnt += soundSpeed;
  //     if(wcnt>256)wcnt=0;
  //   }
  // portEXIT_CRITICAL_ISR(&Tunes::timerMux);

  return 0;
}


int runLuaGame::l_print(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* text = lua_tostring(L, 1);
  int x = lua_tointeger(L, 2);
  int y = lua_tointeger(L, 3);
  int tsize = lua_tointeger(L, 4);
  int cn = lua_tointeger(L, 5);
  int cn2 = lua_tointeger(L, 6);

  if(cn != NULL){
    self->col[0] = clist2[cn][0]; // 5bit
    self->col[1] = clist2[cn][1]; // 6bit
    self->col[2] = clist2[cn][2]; 
  }
  if(cn2 != NULL){
    self->col2[0] = clist2[cn2][0]; // 5bit
    self->col2[1] = clist2[cn2][1]; // 6bit
    self->col2[2] = clist2[cn2][2]; 
  }

  if(tsize == NULL){
    tft.setTextSize(1);
    tft.setFont(&fonts::Font0);
  }else if(tsize==8){
    tft.setTextSize(1);
    tft.setCursor(x,y);
    // tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
    tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]),lua_rgb24to16(self->col2[0], self->col2[1], self->col2[2]));//文字背景を消せる
    tft.setFont(&lgfxJapanGothicP_8);
  }else if(tsize==16){
    tft.setTextSize(1);
    tft.setCursor(x,y);
    // tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
    tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]),lua_rgb24to16(self->col2[0], self->col2[1], self->col2[2]));//文字背景を消せる
    tft.setFont(&lgfxJapanGothicP_16);
  }

  if(x == NULL && y == NULL && tsize == NULL && cn == NULL){//位置指定なしの場合
    tft.setTextColor(TFT_WHITE,TFT_BLACK);//文字背景あり
    tft.setCursor(0,0);
    tft.setClipRect(0, 0, TFT_WIDTH, TFT_HEIGHT);
  }else if(x != NULL && y != NULL && tsize == NULL && cn == NULL){//位置指定ありの場合
    tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
    tft.setTextColor(TFT_WHITE,TFT_BLACK);//文字背景あり
    tft.setCursor(x,y);
    tft.setClipRect(x, y, TFT_WIDTH-x, TFT_HEIGHT-y);
  }else if(x != NULL && y != NULL && tsize != NULL && cn == NULL){//位置指定、、色指定ありの場合
    tft.setTextColor(TFT_WHITE,TFT_BLACK);//文字背景あり
    tft.setCursor(x,y);
    tft.setClipRect(x, y, TFT_WIDTH-x, TFT_HEIGHT-y);
  }else if(x != NULL && y != NULL && tsize != NULL && cn != NULL){//位置指定、、色指定ありの場合
    tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
    tft.setCursor(x,y);
    tft.setClipRect(x, y, TFT_WIDTH-x, TFT_HEIGHT-y);
  }else if(x != NULL && y != NULL && tsize != NULL && cn != NULL && cn2 != NULL){//位置指定、、色指定,背景色指定ありの場合
    tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]),lua_rgb24to16(self->col2[0], self->col2[1], self->col2[2]));//文字背景を消せる
    tft.setCursor(x,y);
    tft.setClipRect(x, y, TFT_WIDTH-x, TFT_HEIGHT-y);
  }
  tft.println(text);
  tft.clearClipRect();
  //描画後は設定をデフォルトに戻しておく
  tft.setTextColor(TFT_WHITE,TFT_BLACK);//文字背景あり
  return 0;
}

int runLuaGame::l_pinw(lua_State* L){  
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int no = lua_tointeger(L, 1);
  if(no == 0)digitalWrite(OUTPIN_0, LOW);
  if(no == 1)digitalWrite(OUTPIN_0, HIGH);
  return 0;
}

int runLuaGame::l_pinr(lua_State* L){  
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int no = lua_tointeger(L, 1);
  if(no == 0)digitalWrite(INPIN_0, LOW);
  if(no == 1)digitalWrite(INPIN_0, HIGH);
  return 0;
}

int runLuaGame::l_tool(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int no = lua_tointeger(L, 1);
  toolNo = no;
  return 0; 
}

int runLuaGame::l_spr8(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

  int n = lua_tointeger(L, 1);
  int x = lua_tointeger(L, 2);
  int y = lua_tointeger(L, 3);
  int scalex = lua_tointeger(L, 4);
  int scaley = lua_tointeger(L, 5);
  int angle = lua_tointeger(L, 6);
  int tc = 0;//デフォルトの透明色は黒
  tc = lua_tointeger(L, 7);//透明色

  if( x>=0 && y>=0 && x<DISP_SPR_W_NUM*8 && y<DISP_SPR_H_NUM*8 ){//ディスプレイに入っているときのみ描画する

  uint8_t edgeno = lua_tointeger(L, 8);//縁取りの色

  x >> 3;  // xを8で割る（右シフト演算子を使用）
  y >> 3;  // yを8で割る

  if(y<DISP_SPR_H_NUM && x<DISP_SPR_W_NUM){
    mapArray[y][x] = uint8_t(n);//スプライト位置のマップ情報を書き換える
  }

  int w = 8;
  int h = 8;

  if(edgeno!=NULL){//エッジあり
     w = 10;
     h = 10;
  }

  sprite88_roi.clear();//指定の大きさにスプライトを作り直す
  sprite88_roi.createSprite(w,h);

  int spr8numX = 8;//スプライトシートに並ぶｘ、ｙの個数
  int spr8numY = 8;

  int sx = ((n-1)%spr8numX); //0~7
  int sy = ((n-1)/spr8numY); //整数の割り算はintにいれると切り捨てされる

  // for(int y=0;y<8;y++){
  //   for(int x=0;x<8;x++){
  //     uint8_t bit4;
  //     int sprpos;
  //     sprpos = (sy*8*PNG_SPRITE_WIDTH+sx*8 + y*PNG_SPRITE_WIDTH + x)/2;//４ビット二つで８ビットに入れてるので1/2に
  //     bit4 = sprite64cnos_vector[sprpos];
  //     if(x%2 == 1)bit4 = (bit4 & 0b00001111);
  //     if(x%2 == 0)bit4 = (bit4 >> 4);
  //     sprite88_roi.drawPixel(x,y, gethaco3Col(bit4));
  //   }
  // }

//エッジあり
  if(edgeno!=NULL){
//キャラスプライト
  for(int y=0;y<8;y++){
      for(int x=0;x<8;x++){
        uint8_t bit4;
        int sprpos;

        sprpos = (sy*8*PNG_SPRITE_WIDTH+sx*8 + y*PNG_SPRITE_WIDTH + x)/2;//４ビット二つで８ビットに入れてるので1/2に
        bit4 = sprite64cnos_vector[sprpos];
        if(x%2 == 1)bit4 = (bit4 & 0b00001111);
        if(x%2 == 0)bit4 = (bit4 >> 4);
        sprite88_roi.drawPixel(x+1,y+1, gethaco3Col(bit4));

      }
  }
  sprite88_roi.drawRect(0,0,10,10,gethaco3Col(0));

  bool upCol[10];
  bool preupCol[10];
  bool preCol = 1;
  bool nowCol = 1;
  bool nutta = false;

  uint8_t colValR;
  uint8_t colValG;
  uint8_t colValB;

  for(int y=0;y<10;y++){
    for(int x=0;x<10;x++){

      colValR = sprite88_roi.readPixelRGB(x,y).R8();
      colValG = sprite88_roi.readPixelRGB(x,y).G8();
      colValB = sprite88_roi.readPixelRGB(x,y).B8();

      if(colValR==0&&colValG==0&&colValB==0){//0: 黒色=なし
        upCol[x] = 0;
        nowCol = 0;
      }else{
        upCol[x] = 1;
        nowCol = 1;
        nutta=false;
      }

      if(preupCol[x] == 0&&nowCol == 1){//上が黒だったら
        sprite88_roi.drawPixel(x,y-1, gethaco3Col(edgeno));
      }

      if(preupCol[x] == 1&&nowCol == 0&&y!=0){//上が黒以外だったら
        sprite88_roi.drawPixel(x,y, gethaco3Col(edgeno));
        // nutta = true;
      }

      if(preCol==0&&nowCol == 1){//黒から黒以外になったら
        sprite88_roi.drawPixel(x-1,y, gethaco3Col(edgeno));
      }

      if(preCol==1&&nowCol == 0&& nutta == false){//黒以外から黒になったら
        sprite88_roi.drawPixel(x,y, gethaco3Col(edgeno));
        nutta = true;
      }
      preCol = nowCol;
    }
    memcpy(preupCol, upCol, sizeof(upCol));
  }
  }else{
    for(int y=0;y<8;y++){
      for(int x=0;x<8;x++){
        uint8_t bit4;
        int sprpos;
        //sprite64cnos[(sy*8) * PNG_SPRITE_WIDTH + (sx*8) 取得スタート位置
        //y*PNG_SPRITE_WIDTH + xスプライトのピクセル取得
        sprpos = (sy*8*PNG_SPRITE_WIDTH+sx*8 + y*PNG_SPRITE_WIDTH + x)/2;//４ビット二つで８ビットに入れてるので1/2に
        bit4 = sprite64cnos_vector[sprpos];
        if(x%2 == 1)bit4 = (bit4 & 0b00001111);
        if(x%2 == 0)bit4 = (bit4 >> 4);
        sprite88_roi.drawPixel(x,y, gethaco3Col(bit4));
      }
    }
  }

  sprite88_roi.setPivot(w/2.0, h/2.0);
  // sprite88_roi.setPivot(0, 0);
  if(scalex == NULL && scaley==NULL && angle == NULL){
    sprite88_roi.pushRotateZoom(&tft, x, y, 0, 1, 1, gethaco3Col(tc));
  }else  if(scalex != NULL && scaley!=NULL && angle == NULL){
    sprite88_roi.pushRotateZoom(&tft, x, y, 0, scalex, scaley, gethaco3Col(tc));
  }else  if(scalex != NULL && scaley!=NULL && angle != NULL){
    sprite88_roi.pushRotateZoom(&tft, x, y, angle, scalex, scaley, gethaco3Col(tc));
  }

  }
  return 0;
}


int runLuaGame::l_spr(lua_State* L){  
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int w = lua_tointeger(L, 3);
  int h = lua_tointeger(L, 4);
  int sx = lua_tointeger(L, 5);
  int sy = lua_tointeger(L, 6);

  sx  /= 8;
  sy  /= 8;
  // int n = lua_tointeger(L, 1);
  // int x = lua_tointeger(L, 2);
  // int y = lua_tointeger(L, 3);
  // int gw = lua_tointeger(L, 4);
  // int gh = lua_tointeger(L, 5);
  // int scalex = lua_tointeger(L, 6);
  // int scaley = lua_tointeger(L, 7);
  // int angle = lua_tointeger(L, 8);

  // sprite64.pushRotateZoom(&tft, x, y, 0, 1, 1, TFT_BLACK);
  // sprite64.pushSprite(&tft, x, y);
  
  sprite88_roi.clear();//指定の大きさにスプライトを作り直す
  sprite88_roi.createSprite(w,h);

  int spr8numX = 8;//スプライトシートに並ぶｘ、ｙの個数
  int spr8numY = 8;

//キャラスプライト
  for(int y=0;y<8;y++){
      for(int x=0;x<8;x++){
        uint8_t bit4;
        int sprpos;
        //sprite64cnos[(sy*8) * PNG_SPRITE_WIDTH + (sx*8) 取得スタート位置
        //y*PNG_SPRITE_WIDTH + xスプライトのピクセル取得
        sprpos = (sy*8*PNG_SPRITE_WIDTH+sx*8 + y*PNG_SPRITE_WIDTH + x)/2;//４ビット二つで８ビットに入れてるので1/2に
        bit4 = sprite64cnos_vector[sprpos];
        if(x%2 == 1)bit4 = (bit4 & 0b00001111);
        if(x%2 == 0)bit4 = (bit4 >> 4);
        sprite88_roi.drawPixel(x,y, getcno2tftc(bit4));
      }
  }

  // sprite88_roi.setPivot(w/2.0, h/2.0);
  // sprite88_roi.pushRotateZoom(&tft, x, y, 0, 1, 1, TFT_BLACK);
  sprite88_roi.pushSprite(&tft, x, y);//4ずれない
  // sprite88_roi.pushRotateZoom(&tft, roix+n*8+4, roiy+m*8+4, 0, 1, 1, TFT_BLACK);//なぜか４を足さないとずれる(setPivot?)


  // if(scalex == NULL && scaley==NULL && angle == NULL){
  //   sprite88_roi.pushRotateZoom(&tft, x, y, 0, 1, 1, TFT_BLACK);
  // }else  if(scalex != NULL && scaley!=NULL && angle == NULL){
  //   sprite88_roi.pushRotateZoom(&tft, x, y, 0, scalex, scaley, TFT_BLACK);
  // }else  if(scalex != NULL && scaley!=NULL && angle != NULL){
  //   sprite88_roi.pushRotateZoom(&tft, x, y, angle, scalex, scaley, TFT_BLACK);
  // }

  return 0;
}



int runLuaGame::l_scroll(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);

  tft.scroll(x, y);
  return 0;
}


int runLuaGame::l_win(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int state = lua_tointeger(L, 1);
  int x = lua_tointeger(L, 2);
  int y = lua_tointeger(L, 3);
  int row = lua_tointeger(L, 4);
  int col = lua_tointeger(L, 5);
  int charmax = lua_tointeger(L, 6);
  int winID = lua_tointeger(L, 7);
  const char* title = lua_tostring(L, 8);
  
  if(state == 1){//可視化状態
    tft.setFont(&lgfxJapanGothicP_8);
    tft.fillRoundRect(x,y,(charmax*row+row+1)*8,(col+2)*8,4,gethaco3Col(1));
    
    for(int j = 0; j<col; j++){
      for(int i = 0; i<row; i++){
        tft.setTextColor(gethaco3Col(7));

        if(winID==0){
          tft.setCursor(x+8+(i*(charmax+1)*8), y+8+(j*8));
          tft.print("");
          if(strcmp(title, "どうする？") == 0){
            tft.print("ステテコ");
            }
          else if(strcmp(title, "どうぐ") == 0){
            tft.print("みかわしのふく");
            }
          else if(strcmp(title, "むらびと") == 0){
            // tft.setFont(&lgfxJapanGothicP_16);
            // tft.print("曖昧模糊な薔薇の轟");
            // j++;//2倍角なのでjに1足して繰り返し回数を半分にする
            tft.setFont(&lgfxJapanGothicP_8);
            tft.print("画数が少ない文字であれば、8pxでも良");
            // tft.print("曖昧模糊な薔薇の轟曖昧模糊な薔薇の轟");
          }
        }else if(winID==HPMP){
          tft.setFont(&fonts::Font0);
          tft.setCursor(x+4+(i*(charmax+1)*8), y+12+(j*8));
          if(j==0){tft.print("HP");}
          else if(j==1){tft.print("MP");}
          tft.print("999");
        }

        // for(int n = 0; n<charmax; n++){
        //   // tft.drawRect(x+8+(n*8)+(i*(charmax+1)*8), y+8+(j*8), 8,8, gethaco3Col(7));
          // tft.setTextColor(gethaco3Col(7));
          // tft.setCursor(x+8+(n*8)+(i*(charmax+1)*8), y+8+(j*8));
          // tft.print("あ");
        // }
      }
    }
    //ウインドウタイトルを描画
    tft.drawRoundRect(x+1,y+1,(charmax*row+row+1)*8-2,(col+2)*8-2, 4,gethaco3Col(7));//白い枠線
    tft.setTextColor(gethaco3Col(7),gethaco3Col(1));//文字背景あり
    tft.setTextSize(1);
    tft.setFont(&lgfxJapanGothicP_8);
    tft.setCursor(x+4, y);
    tft.println(title);

    // if(strcmp(title, "むらびと") == 0){
    //   tft.setCursor(x+8, y+8);
    //   tft.setClipRect(x+8, y+8, 144, 40);//文字以外のすべての描画にかかわるので最後に指定
    //   tft.setFont(&lgfxJapanGothicP_16);
    //   tft.print("曖昧模糊な薔薇の轟19文字も書ける");
    // }
  }

  // gethaco3Col(1);//紺
  // gethaco3Col(7);//白

  // if(tsize == NULL){
  //   tft.setTextSize(1);
  //   tft.setFont(&fonts::Font0);
  // }else if(tsize==8){
  //   tft.setTextSize(1);
  //   tft.setCursor(x,y);
  //   // tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
  //   tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]),lua_rgb24to16(self->col2[0], self->col2[1], self->col2[2]));//文字背景を消せる
  //   tft.setFont(&lgfxJapanGothicP_8);
  // }else if(tsize==16){
  //   tft.setTextSize(1);
  //   tft.setCursor(x,y);
  //   // tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
  //   tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]),lua_rgb24to16(self->col2[0], self->col2[1], self->col2[2]));//文字背景を消せる
  //   tft.setFont(&lgfxJapanGothicP_16);
  // }

  // if(x == NULL && y == NULL && tsize == NULL && cn == NULL){//位置指定なしの場合
  //   tft.setTextColor(TFT_WHITE,TFT_BLACK);//文字背景あり
  //   tft.setCursor(0,0);
  //   tft.setClipRect(0, 0, TFT_WIDTH, TFT_HEIGHT);
  // }else if(x != NULL && y != NULL && tsize == NULL && cn == NULL){//位置指定ありの場合
  //   tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
  //   tft.setTextColor(TFT_WHITE,TFT_BLACK);//文字背景あり
  //   tft.setCursor(x,y);
  //   tft.setClipRect(x, y, TFT_WIDTH-x, TFT_HEIGHT-y);
  // }else if(x != NULL && y != NULL && tsize != NULL && cn == NULL){//位置指定、、色指定ありの場合
  //   tft.setTextColor(TFT_WHITE,TFT_BLACK);//文字背景あり
  //   tft.setCursor(x,y);
  //   tft.setClipRect(x, y, TFT_WIDTH-x, TFT_HEIGHT-y);
  // }else if(x != NULL && y != NULL && tsize != NULL && cn != NULL){//位置指定、、色指定ありの場合
  //   tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
  //   tft.setCursor(x,y);
  //   tft.setClipRect(x, y, TFT_WIDTH-x, TFT_HEIGHT-y);
  // }else if(x != NULL && y != NULL && tsize != NULL && cn != NULL && cn2 != NULL){//位置指定、、色指定,背景色指定ありの場合
  //   tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]),lua_rgb24to16(self->col2[0], self->col2[1], self->col2[2]));//文字背景を消せる
  //   tft.setCursor(x,y);
  //   tft.setClipRect(x, y, TFT_WIDTH-x, TFT_HEIGHT-y);
  // }
  // tft.println(text);
  // tft.clearClipRect();
  // //描画後は設定をデフォルトに戻しておく
  // tft.setTextColor(TFT_WHITE,TFT_BLACK);//文字背景あり
  return 0;
}


bool drawMap() {
  int ColCursor = 0;
  uint8_t sprno;
  uint8_t presprno = 0; // 初期値を設定
  uint8_t repeatnum;

  for (uint8_t k = 0; k < 16; k++) {
    int j = k;
    // int j = (k+16+targetDispRow)%16;
    for (uint8_t i = 0; i < rowData[k].size(); i += 2) {
      sprno = rowData[k][i];
      repeatnum = rowData[k][i + 1];

      if (sprno == 3 && repeatnum == 31) {
        repeatnum = 0;
        ColCursor = 0;
      } else {
        if (sprno == 7) {
          if (gWy >= 220 || (gWx >= 49 && gWy <= 97)) {
            sprno = 15;
          }
        }

        int sx = sprno % gSpr8numX;
        int sy = sprno / gSpr8numY;

        for (int y = 0; y < 8; y++) {
          for (int x = 0; x < 8; x++) {
            uint8_t bit4;
            int sprpos;
            sprpos = (sy * 8 * PNG_SPRITE_WIDTH + sx * 8 + y * PNG_SPRITE_WIDTH + x) / 2;
            bit4 = sprite64cnos_vector[sprpos];
            if (x % 2 == 1) bit4 = (bit4 & 0b00001111);
            if (x % 2 == 0) bit4 = (bit4 >> 4);
            sprite88_roi.drawPixel(x, y, gethaco3Col(bit4));
          }
        }

        for (int n = 0; n < repeatnum; n++) {
          int sprx = ColCursor + n;
          int displaysprx = sprx - gWx;

          if (sprx >= gWx && sprx < gWx + DISP_SPR_W_NUM) {
            mapArray[j][displaysprx] = sprno;

            // 例: 描画対象が同じ場合は描画をスキップ
            // if (presprno != sprno) {
            //   sprite88_roi.setPivot(4.0, 4.0);
            //   sprite88_roi.pushSprite(&tft, (ColCursor + n - gWx) * 8, j * 8);
            // }

            // sprite88_roi.setPivot(gSprw / 2.0, gSprh / 2.0);
            // sprite88_roi.pushSprite(&tft, (ColCursor + n - gWx) * 8, j * 8);

            sprite88_roi.setPivot(4.0, 4.0);
            sprite88_roi.pushSprite(&tft, (ColCursor + n - gWx) * 8, j * 8);
            // sprite88_roi.pushSprite(&tft, (ColCursor + n - gWx) << 3, j << 3);
            
            // 海岸線の場合は描写を足す
            if (displaysprx != 0) {
              if (presprno == 0 && sprno != 0) { // 海から陸ならば
                tft.fillRect((ColCursor + n - gWx - 1) * 8 + 7, j * 8, 1, 8, gethaco3Col(7));
              }
              if (presprno != 0 && sprno == 0) { // 陸から海ならば
                tft.fillRect((ColCursor + n - gWx) * 8, j * 8, 1, 8, gethaco3Col(7));
              }
            }

            if (j != 0) {
              if (sprno != 0 && mapArray[j - 1][displaysprx] == 0) { // 陸の時上が海か
                tft.fillRect((ColCursor + n - gWx) * 8, (j - 1) * 8 + 7, 8, 1, gethaco3Col(7));
              }

              if (sprno == 0 && mapArray[j - 1][displaysprx] != 0) { // 海の時上が陸か
                tft.fillRect((ColCursor + n - gWx) * 8, j * 8, 8, 1, gethaco3Col(7));
              }
            }

            presprno = sprno;
          }
        }

        ColCursor += repeatnum;
      }
    }
  }

  return true;
}

bool readRowFromBinary2(const char *filename) {
  int _startRow = gWy;
  int nnum = 0;

  File mfile = SPIFFS.open(filename, "rb");

  if (!mfile) {
    // Serial.println("Error opening file");
    return false;
  }

  std::vector<std::vector<uint8_t>> new_rowData(16, std::vector<uint8_t>());

  // buffer のクリアとリサイズ
  buffer.clear();
  buffer.resize(8);

  while (nnum < _startRow + DISP_SPR_H_NUM) {
    bytesRead = mfile.read(buffer.data(), buffer.size());

    if (bytesRead == 0) {
      break;
    }

    for (int i = 0; i < bytesRead; i++) {
      sprno = buffer[i] >> 5;
      repeatnum = buffer[i] & 0b00011111;

      if (nnum >= _startRow && nnum < _startRow + DISP_SPR_H_NUM) {
        if (sprno == 0 && repeatnum == 0) {
          repeatnum = 255;
        }

        new_rowData[nnum - _startRow].push_back(sprno);
        new_rowData[nnum - _startRow].push_back(repeatnum);
      }

      if (sprno == 3 && repeatnum == 31) {
        ++nnum;
      }
    }
  }

  mfile.close();

  // rowData を new_rowData に入れ替える
  rowData.swap(new_rowData);

  mfile = File();  // 不要な行の削除

  // drawWaitF = false;
  return true;
}
// extern M5Canvas sprite64;

int runLuaGame::l_map(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int mx = lua_tointeger(L, 1);
  int my = lua_tointeger(L, 2);
  int roix = lua_tointeger(L, 3);
  int roiy = lua_tointeger(L, 4);
  int roiW = lua_tointeger(L, 5);
  int roiH = lua_tointeger(L, 6);

  String fn = lua_tostring(L, 7);
  
  if(fn != NULL){
    mapFileName = fn;
  }

  sprite88_roi.clear();//指定の大きさにスプライトを作り直す
  sprite88_roi.createSprite(8,8);

  int spr8numX = 8;//スプライトシートに並ぶ８＊８スプライトの個数
  int spr8numY = 8;
  for(int m=0;m<roiH;m++){
      for(int n=0;n<roiW;n++){
          int sprno = mapArray[my+n][mx+m];
          
            int sx = ((sprno-1)%spr8numX); //0~7
            int sy = ((sprno-1)/spr8numY); //整数の割り算は自動で切り捨てされる
            // sprite64.pushSprite(&sprite88_roi, -8*(sx), -8*(sy));//128*128のpngデータを指定位置までずらすsprite64のスプライトデータ8*8で切り抜く

          for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
              uint8_t bit4;
              int sprpos;
              sprpos = (sy * 8 * PNG_SPRITE_WIDTH + sx * 8 + y * PNG_SPRITE_WIDTH + x) / 2;
              bit4 = sprite64cnos_vector[sprpos];
              if (x % 2 == 1) bit4 = (bit4 & 0b00001111);
              if (x % 2 == 0) bit4 = (bit4 >> 4);
              sprite88_roi.drawPixel(x, y, gethaco3Col(bit4));
            }
          }

            sprite88_roi.pushSprite(&tft, roix+n*8, roiy+m*8);//4ずれない
            // sprite88_roi.pushRotateZoom(&tft, roix+n*8+4, roiy+m*8+4, 0, 1, 1, TFT_BLACK);//なぜか４を足さないとずれる要修正
      }
  }
  return 0;
}

int runLuaGame::l_rmap(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  String fn = lua_tostring(L, 1);
  gWx = lua_tointeger(L, 2);
  gWy = lua_tointeger(L, 3);
  mapFileName = fn;
  sprite88_roi.clear(); // 指定の大きさにスプライトを作り直す
  sprite88_roi.createSprite(gSprw, gSprh);

  while(!readRowFromBinary2(fn.c_str())){
    // delay(1);//1/50秒表示
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // 20ミリ秒スリープ
  }
  
  while(!drawMap()){
    // delay(1);//1/50秒表示
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // 20ミリ秒スリープ
  }

  return 0;
}

int runLuaGame::l_pset(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  int cn = lua_tointeger(L, 3);
  int cn2 = lua_tointeger(L, 4);
  int cn3 = lua_tointeger(L, 5);
  
  if(cn2 == NULL){
    tft.writePixel(x, y, cn);
  }

  if(cn3 != NULL)
  {
    self->col[0] = cn; // 5bit
    self->col[1] = cn2; // 6bit
    self->col[2] = cn3; // 5bit
    tft.drawPixel(x, y, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  }
  return 0;
}

int runLuaGame::l_pget(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int32_t x = lua_tointeger(L, 1);
  int32_t y = lua_tointeger(L, 2);

  uint16_t c = tft.readPixel(x, y);

  uint8_t index = 0;
  // for(unsigned int pi = 0; pi < 256; pi ++){
  //   if(self->palette[pi] == c){
  //     index = pi;
  //     break;
  //   }
  // }
  uint8_t r = ((c >> 11) << 3); // 5bit
  uint8_t g = (((c >> 5) & 0b111111) << 2); // 6bit
  uint8_t b = ((c & 0b11111) << 3);       // 5bit

  lua_pushinteger(L, (lua_Integer)r);
  lua_pushinteger(L, (lua_Integer)g);
  lua_pushinteger(L, (lua_Integer)b);
  lua_pushinteger(L, (lua_Integer)index);
  return 4;
}


int numVertices;//すべての頂点数
float deg = 0;


  Vector3<float> myop;
  Vector3<float> myp;

// std::vector<Vector3<float>> opg;
// std::vector<Vector3<float>> pg;
// std::vector<Vector3<float>> op;
// std::vector<Vector3<float>> p;



// int runLuaGame::l_creobj(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  
//   objno = lua_tointeger(L, 1);//カメラのx軸角0~360

//   // Obj 構造体を作成し、値を設定し、動的配列に追加する
//   Obj obj;
//   obj.x = 0;
//   obj.y = 0;
//   obj.z = 0;

//   obj.scale = 1.0; // ここでサイズの初期値を設定します

//   obj.angle = 0; // ここで角度の初期値を設定します
//   obj.colangle = 120; // ここで色の初期値を設定します
//   obj.width = 10; // ここで幅の初期値を設定します
//   obj.height = 10; // ここで高さの初期値を設定します

//   // 動的配列に追加
//   objects.push_back(obj);
//   uint8_t lastno =objects.size() - 1;

//     int numCorners = 24;//輪切りにした時の頂点数
//     int numRings = numCorners / 2 + 1;//縦方向の頂点数
//     numVertices = numCorners * numRings;//すべての頂点数
//     float cdeg = 360.0f/ float(numCorners);
  
//     //球オブジェクトを生成
//     for (int j = 0; j < numRings; j++) //13
//     {
//       // float r = sin(cdeg * j * RAD_ONE);
//       float y = cos(cdeg * j * RAD_ONE);

//       for(int i=0;i<numCorners; i++){//24
//         int k = j * numCorners + i;//k番目の頂点
//         for(int n=0; n < 3; n++){//xyz分３を確保
//           //ベクター配列を初期化
//           Vector3<float> initp;
//           objects[lastno].op.push_back(initp);
//           objects[lastno].p.push_back(initp);
//         }

//         objects[lastno].op[k].set(obj.x+(sin(((i * cdeg)-45) * RAD_ONE) )*obj.scale,
//                                   obj.y+ y*obj.scale,
//                                   obj.z+(cos(((i * cdeg)-45) * RAD_ONE) )*obj.scale);
//       }
//     }

//     for(int i=0; i<BSTARBUFNUM; i++){//星の数だけ

//       Vector3<float> initp;
//       objects[0].op.push_back(initp);
//       objects[0].p.push_back(initp);
      
//       //星の３次元座標を求めてオリジナルポジションに入れる
//       objects[0].op[i].setX(cos(bsParamFloat[i][1]) * cos(bsParamFloat[i][0]));
//       objects[0].p[i].setZ(cos(bsParamFloat[i][1]) * sin(bsParamFloat[i][0]));
//       objects[0].p[i].setY(sin(bsParamFloat[i][1]));

//     }
  
//     return 0; // テーブルをスタックに返す
// }

int numCorners = 24;
int numRings = numCorners / 2 + 1;
// int numVertices = numCorners * numRings;
// float deg = 0;
float cdeg = 360.0f/ float(numCorners);

int runLuaGame::l_creobj(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

    //グリッド生成
    // for (int i = 0; i < numRings*numCorners; i++) //=numVertices
    // {   //ベクター配列を初期化
    //     Vector3<float> initp;
    //     opg.push_back(initp);
    //     pg.push_back(initp);

    // }

    //オブジェクトを生成
    for (int j = 0; j < numRings; j++) 
    {
      float r = sin(cdeg * j * RAD_ONE);
      float y = cos(cdeg * j * RAD_ONE);
      for(int i=0;i<numCorners; i++){

        int k = j * numCorners + i;

        //ベクター配列を初期化
        Vector3<float> initp;
        opg.push_back(initp);
        pg.push_back(initp);

        opg[k].setX(sin(i*cdeg*RAD_ONE) * r);
        opg[k].setZ(cos(i*cdeg*RAD_ONE) * r);
        opg[k].setY(y);
      }
    }

    
  //    Obj 構造体を作成し、値を設定し、動的配列に追加する
  
  Obj obj;
  obj.x = 0;
  obj.y = 0;
  obj.z = 0;
  obj.h = 255;
  obj.s = 255;
  obj.v = 255;

  obj.scale = 1; // ここでサイズの初期値を設定します

  obj.angle = 0; // ここで角度の初期値を設定します
  obj.colangle = 120; // ここで色の初期値を設定します
  obj.width = 10; // ここで幅の初期値を設定します
  obj.height = 10; // ここで高さの初期値を設定します
  objects.push_back(obj);

    for(int i=0; i<BSTARBUFNUM; i++){//星の数だけ

      Vector3<float> initp;
      objects[0].op.push_back(initp);
      objects[0].p.push_back(initp);
      
      //星の３次元座標を求めてオリジナルポジションに入れる
      objects[0].op[i].setX(cos(bsParamFloat[i][1]) * cos(bsParamFloat[i][0]));
      objects[0].op[i].setZ(cos(bsParamFloat[i][1]) * sin(bsParamFloat[i][0]));
      objects[0].op[i].setY(sin(bsParamFloat[i][1]));

    }

    // for(int i=0; i<numVertices; i++){//グリッドの数だけ

    //   Vector3<float> initp;
    //   opg.push_back(initp);
    //   pg.push_back(initp);
      
    //   //星の３次元座標を求めてオリジナルポジションに入れる
    //   opg[i].setX(cos(bsParamFloat[i][1]) * cos(bsParamFloat[i][0]));
    //   opg[i].setZ(cos(bsParamFloat[i][1]) * sin(bsParamFloat[i][0]));
    //   opg[i].setY(sin(bsParamFloat[i][1]));

    // }

    return 0; // テーブルをスタックに返す
}



// int runLuaGame::l_creobj(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  
//     グリッド生成
//     for (int i = 0; i < numRings*numCorners; i++) //=numVertices
//     {   //ベクター配列を初期化
//         Vector3<float> initp;
//         opg.push_back(initp);
//         pg.push_back(initp);

//     }

//       Obj 構造体を作成し、値を設定し、動的配列に追加する
//   Obj obj;
//   obj.x = objx;
//   obj.y = objy;
//   obj.z = objz;
//   obj.h = objh;
//   obj.s = objs;
//   obj.v = objv;

//   obj.scale = scale; // ここでサイズの初期値を設定します

//   obj.angle = 0; // ここで角度の初期値を設定します
//   obj.colangle = 120; // ここで色の初期値を設定します
//   obj.width = 10; // ここで幅の初期値を設定します
//   obj.height = 10; // ここで高さの初期値を設定します

//     objects.push_back(obj);
//     uint8_t lastno =objects.size() - 1;

//     int numCorners = 24;//輪切りにした時の頂点数
//     int numRings = numCorners / 2 + 1;//縦方向の頂点数
//     float cdeg = 360.0f/ float(numCorners);

//     //オブジェクトを生成
//     for (int j = 0; j < numRings; j++) 
//     {
//       float r = sin(cdeg * j * RAD_ONE);
//       float y = cos(cdeg * j * RAD_ONE);
//       for(int i=0;i<numCorners; i++){

//         int k = j * numCorners + i;

//         //ベクター配列を初期化
//         Vector3<float> initp;
//         opg.push_back(initp);
//         pg.push_back(initp);

//         oobjects[0].p[k].setX(sin(i*cdeg*RAD_ONE) * r);
//         oobjects[0].p[k].setZ(cos(i*cdeg*RAD_ONE) * r);
//         oobjects[0].p[k].setY(y);
//       }
//     }

//     for(int i=0; i<BSTARBUFNUM; i++){//星の数だけ

//       Vector3<float> initp;
//       objects[0].op.push_back(initp);
//       objects[0].p.push_back(initp);
      
//       //星の３次元座標を求めてオリジナルポジションに入れる
//       objects[0].op[i].setX(cos(bsParamFloat[i][1]) * cos(bsParamFloat[i][0]));
//       objects[0].p[i].setZ(cos(bsParamFloat[i][1]) * sin(bsParamFloat[i][0]));
//       objects[0].p[i].setY(sin(bsParamFloat[i][1]));

//     }


//     for(int i=0; i<numVertices; i++){//グリッドの数だけ

//       Vector3<float> initp;
//       opg.push_back(initp);
//       pg.push_back(initp);
      
//       //星の３次元座標を求めてオリジナルポジションに入れる
//       objects[0].op[i].setX(cos(bsParamFloat[i][1]) * cos(bsParamFloat[i][0]));
//       objects[0].op[i].setZ(cos(bsParamFloat[i][1]) * sin(bsParamFloat[i][0]));
//       objects[0].op[i].setY(sin(bsParamFloat[i][1]));

//     }

//     return 0; // テーブルをスタックに返す
// }


int runLuaGame::l_color(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int r,g,b;
  r = lua_tointeger(L, 1);
  g = lua_tointeger(L, 2);
  b = lua_tointeger(L, 3);
    //とにかく一回格納する
  self->col[0] = r;
  self->col[1] = g;
  self->col[2] = b;
  //色番号だったら上書き
  if(g == NULL && b == NULL){
    int cn = lua_tointeger(L, 1);
    self->col[0] = clist2[cn][0]; // 5bit
    self->col[1] = clist2[cn][1]; // 6bit
    self->col[2] = clist2[cn][2]; // 5bit
  }
  return 0;
}


// int runLuaGame::l_color(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//   int r,g,b;
//   r = lua_tointeger(L, 1);
//   g = lua_tointeger(L, 2);
//   b = lua_tointeger(L, 3);
//     //とにかく一回格納する
//   self->col[0] = r;
//   self->col[1] = g;
//   self->col[2] = b;
//   //色番号だったら上書き
//   if(g == NULL && b == NULL){
//     int cn = lua_tointeger(L, 1);
//     self->col[0] = clist2[cn][0]; // 5bit
//     self->col[1] = clist2[cn][1]; // 6bit
//     self->col[2] = clist2[cn][2]; // 5bit
//   }
//   return 0;
// }

int runLuaGame::l_text(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* text = lua_tostring(L, 1);
  int x = lua_tointeger(L, 2);
  int y = lua_tointeger(L, 3);
  tft.setCursor(x,y);
  tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));

  tft.setTextSize(1);//サイズ
  tft.setFont(&lgfxJapanGothic_12);//日本語可,等幅

  tft.print(text);
  return 0;
}




int runLuaGame::l_opmode(lua_State* L){//FAST,WIDE
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int _n = lua_tointeger(L, 1);
  outputMode = _n;
  return 0;
}

int runLuaGame::l_drawrect(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  float w = lua_tonumber(L, 3);
  float h = lua_tonumber(L, 4);
  int cn = lua_tointeger(L, 5);
  
  if(cn != NULL)
  {
    self->col[0] = clist2[cn][0]; // 5bit
    self->col[1] = clist2[cn][1]; // 6bit
    self->col[2] = clist2[cn][2]; // 5bit
  }

  tft.drawRect(x, y, w, h, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int runLuaGame::l_drawbox(lua_State* L) {
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  
  float px = lua_tonumber(L, 1);
  float py = lua_tonumber(L, 2);
  float x = lua_tonumber(L, 3);
  float y = lua_tonumber(L, 4);
  float z = lua_tonumber(L, 5);
  int cn = lua_tointeger(L, 6);
  
  if (cn != 0) { // NULL ではなく 0 と比較する
    self->col[0] = clist2[cn][0]; // 5bit
    self->col[1] = clist2[cn][1]; // 6bit
    self->col[2] = clist2[cn][2]; // 5bit
  }

  self->boxzerox = px;
  self->boxzeroy = py;

  // float unit = 10;
  float rad = atan(0.5);
  const Vector3<float> a(9, 4.5, z);
  const Vector3<float> b(-9, 4.5, z);
  const Vector3<float> c(a.x + b.x, a.y + b.y, a.z + b.z); // ベクトル合成
  const Vector3<float> o(x * cos(rad) - y * cos(rad) + self->boxzerox, x * sin(rad) + y * sin(rad) + self->boxzeroy, z); // クオータービュー（x座標・y座標反転）

  // self->fillFastTriangle(o.x, o.y, o.x - a.x, o.y + a.y, o.x - c.x, o.y + c.y, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  // self->fillFastTriangle(o.x, o.y, o.x - b.x, o.y + b.y, o.x - c.x, o.y + c.y, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));

  self->fillFastTriangle(o.x, o.y-z, o.x - a.x, o.y + a.y-z, o.x - c.x, o.y + c.y-z, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  self->fillFastTriangle(o.x, o.y-z, o.x - b.x, o.y + b.y-z, o.x - c.x, o.y + c.y-z, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));

  self->fillFastTriangle(o.x - c.x, o.y + c.y-z,  o.x - a.x, o.y + a.y-z, o.x - c.x, o.y + c.y, lua_rgb24to16(50, 50, 100));
  self->fillFastTriangle(o.x - a.x, o.y + a.y-z, o.x - a.x, o.y + a.y   , o.x - c.x, o.y + c.y, lua_rgb24to16(50, 50, 100));

  self->fillFastTriangle(o.x - c.x, o.y + c.y-z, o.x - b.x, o.y + b.y-z, o.x - c.x, o.y + c.y,    lua_rgb24to16(100, 100, 100));
  self->fillFastTriangle(o.x - b.x, o.y + b.y,    o.x - c.x, o.y + c.y,    o.x - b.x, o.y + b.y-z, lua_rgb24to16(100, 100, 100));

  // tft.drawRect(x+50, y+50, w, h, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));

  return 0;
}

int runLuaGame::l_drawboxp(lua_State* L) {
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  
  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  float z = lua_tonumber(L, 3);

  float rad = atan(0.5);
  const Vector3<float> a(9, 4.5, z);
  const Vector3<float> b(-9, 4.5, z);
  const Vector3<float> c(a.x + b.x, a.y + b.y, a.z + b.z); // ベクトル合成
  const Vector3<float> o(x * cos(rad) - y * cos(rad) + self->boxzerox, x * sin(rad) + y * sin(rad) + self->boxzeroy, z); // クオータービュー（x座標・y座標反転）

  // self->fillFastTriangle(o.x, o.y-z, o.x - a.x, o.y + a.y-z, o.x - c.x, o.y + c.y-z, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  // self->fillFastTriangle(o.x, o.y-z, o.x - b.x, o.y + b.y-z, o.x - c.x, o.y + c.y-z, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  self->fillFastTriangle(o.x, o.y-z, o.x - a.x, o.y + a.y-z, o.x - c.x, o.y + c.y-z, lua_rgb24to16(0, 0, 50));
  self->fillFastTriangle(o.x, o.y-z, o.x - b.x, o.y + b.y-z, o.x - c.x, o.y + c.y-z, lua_rgb24to16(0, 0, 50));

  // Create a Lua table
lua_newtable(L);

// Set the values of the table
lua_pushnumber(L, o.x);
lua_setfield(L, -2, "x");

lua_pushnumber(L, o.y-z);
lua_setfield(L, -2, "y");

lua_pushnumber(L, o.z);
lua_setfield(L, -2, "z");

// テーブルのサイズをスタックに積む
lua_pushinteger(L, 3);
return 2;
}


int runLuaGame::l_oval(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int rx = lua_tointeger(L, 3);
  int ry = lua_tointeger(L, 4);
  int cn = lua_tointeger(L, 5);
  if(cn != NULL)
  {
    hacol0 =  gethaco3Col(cn);
  }
  tft.drawEllipse(x, y, rx, ry, hacol0);
  return 0;
}

extern bool fpsSetF;
extern int fps;

int runLuaGame::l_fps(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  fps = lua_tointeger(L, 1);
  return 0;
}

int runLuaGame::l_fillp(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int cn0 = lua_tointeger(L, 1);
  int cn1 = lua_tointeger(L, 2);
  int cn2 = lua_tointeger(L, 3);
  int cn3 = lua_tointeger(L, 4);

  hacol0 =  gethaco3Col(cn0);
  hacol1 =  gethaco3Col(cn1);

  if(cn2==NULL){
    hacol2 =  gethaco3Col(cn0);
    hacol3 =  gethaco3Col(cn1);
  }
  
  if(cn2!=NULL){
    hacol2 =  gethaco3Col(cn2);
  }
  if(cn3!=NULL){
    hacol3 =  gethaco3Col(cn3);
  }
  return 0;
}

int runLuaGame::l_ovalfill(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int rx = lua_tointeger(L, 3);
  int ry = lua_tointeger(L, 4);
  int cn0 = lua_tointeger(L, 5);
  int cn1 = lua_tointeger(L, 6);

  if(cn0 != NULL)
    {
      hacol0 = gethaco3Col(cn0);
      hacol1 = gethaco3Col(cn0);//仮に入れておく
      hacol2 = gethaco3Col(cn0);//仮に入れておく
      hacol3 = gethaco3Col(cn0);//仮に入れておく
    }
    
    if(cn1 != NULL)
    {
      hacol0 = gethaco3Col(cn0);
      hacol1 = gethaco3Col(cn1);
      hacol2 = gethaco3Col(cn0);
      hacol3 = gethaco3Col(cn1);
    }

    int32_t xt, yt, i;
    int32_t rx2 = rx * rx;
    int32_t ry2 = ry * ry;
    int32_t s;

    tft.startWrite();

    if(y%4==0)tft.setColor(hacol0);
    else if(y%4==1)tft.setColor(hacol1);
    else if(y%4==2)tft.setColor(hacol2);
    else if(y%4==3)tft.setColor(hacol3);
    tft.writeFastHLine(x - rx, y, (rx << 1) + 1);//まんなかの１本
    
    i = 0;
    yt = 0;
    xt = rx;
    s = (rx2 << 1) + ry2 * (1 - (rx << 1));

    do {
      while (s < 0) s += rx2 * ((++yt << 2) + 2);

      for (int32_t k = 0; k < (yt - i); k++) {

        if(((y - yt)+k)%4==0)tft.setColor(hacol0);
        else if(((y - yt)+k)%4==1)tft.setColor(hacol1);
        else if(((y - yt)+k)%4==2)tft.setColor(hacol2);
        else if(((y - yt)+k)%4==3)tft.setColor(hacol3);
        tft.writeFastHLine(x - xt, (y - yt)+k, (xt << 1) + 1);

        if(((y + i + 1)+k)%4==0)tft.setColor(hacol0);
        else if(((y + i + 1)+k)%4==1)tft.setColor(hacol1);
        else if(((y + i + 1)+k)%4==2)tft.setColor(hacol2);
        else if(((y + i + 1)+k)%4==3)tft.setColor(hacol3);
        tft.writeFastHLine(x - xt, (y + i + 1)+k, (xt << 1) + 1);
      }

      i = yt;
      s -= (--xt) * ry2 << 2;
    } while (rx2 * yt <= ry2 * xt);

    xt = 0;
    yt = ry;
    s = (ry2 << 1) + rx2 * (1 - (ry << 1));
    do {
      while (s < 0) s += ry2 * ((++xt << 2) + 2);
        if((y - yt)%4==0)tft.setColor(hacol0);
        else if((y - yt)%4==1)tft.setColor(hacol1);
        else if((y - yt)%4==2)tft.setColor(hacol2);
        else if((y - yt)%4==3)tft.setColor(hacol3);
        tft.writeFastHLine(x - xt, y - yt, (xt << 1) + 1);

        if((y + yt)%4==0)tft.setColor(hacol0);
        else if((y + yt)%4==1)tft.setColor(hacol1);
        else if((y + yt)%4==2)tft.setColor(hacol2);
        else if((y + yt)%4==3)tft.setColor(hacol3);
        tft.writeFastHLine(x - xt, y + yt, (xt << 1) + 1);
        s -= (--yt) * rx2 << 2;
    } while(ry2 * xt <= rx2 * yt);

    tft.endWrite();
  return 0;
}


int runLuaGame::l_fillpoly(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float x0 = lua_tonumber(L, 1);
  float y0 = lua_tonumber(L, 2);
  float x1 = lua_tonumber(L, 3);
  float y1 = lua_tonumber(L, 4);
  float x2 = lua_tonumber(L, 5);
  float y2 = lua_tonumber(L, 6);
  float x3 = lua_tonumber(L, 7);
  float y3 = lua_tonumber(L, 8);
  int cn = lua_tointeger(L, 9);

  self->col[0] = clist2[cn][0]; // 5bit
  self->col[1] = clist2[cn][1]; // 6bit
  self->col[2] = clist2[cn][2]; // 5bit

  self->fillFastTriangle(x0,y0,x1,y1,x2,y2,lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  self->fillFastTriangle(x2,y2,x3,y3,x0,y0,lua_rgb24to16(self->col[0], self->col[1], self->col[2]));

  // tft.fillTriangle( x0,y0,x1,y1,x2,y2);
  // tft.fillTriangle( x2,y2,x3,y3,x0,y0);
  return 0;

}

int runLuaGame::l_cls(lua_State* L) {
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);

  uint8_t r, g, b;
  if (n >= 0 && n < sizeof(clist2) / sizeof(clist2[0])) {
    r = clist2[n][0];
    g = clist2[n][1];
    b = clist2[n][2];
  } else {
    r = g = b = 0; // Default to black color if n is out of range
  }

  self->col[0] = r; // 5bit
  self->col[1] = g; // 6bit
  self->col[2] = b; // 5bit

  tft.fillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  // tft.fillScreen(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));

  return 0;
}

int runLuaGame::l_fillrect(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int w = lua_tointeger(L, 3);
  int h = lua_tointeger(L, 4);
  int cn = lua_tointeger(L, 5);


  
  if(cn != NULL)
  {
    // self->col[0] = clist2[cn][0]; // 5bit
    // self->col[1] = clist2[cn][1]; // 6bit
    // self->col[2] = clist2[cn][2]; // 5bit

    tft.fillRect(x, y, w, h, gethaco3Col(cn));
  }else{
    tft.fillRect(x, y, w, h, gethaco3Col(8));
    // tft.fillRect(x, y, w, h, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  }

  return 0;
}


extern bool enemyF;
extern uint8_t enemyX;
extern uint8_t enemyY;
extern uint8_t enemyTransCn;
extern String enemyPath;

int runLuaGame::l_rectfill(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int xa = lua_tointeger(L, 1);
  int ya = lua_tointeger(L, 2);
  int xb = lua_tointeger(L, 3);
  int yb = lua_tointeger(L, 4);
  int cn = lua_tointeger(L, 5);
  String fn = lua_tostring(L, 6);
  if(fn == NULL){
    if(cn != NULL)
    {
    self->col[0] = clist2[cn][0]; // 5bit
    self->col[1] = clist2[cn][1]; // 6bit
    self->col[2] = clist2[cn][2]; // 5bit
    }
    tft.fillRect(xa, ya, xb, yb, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  }else if(fn != NULL){//cnを透明対応色にしたい、、、
    enemyF = true;
    enemyPath = fn;
    enemyX = 0;
    enemyY = 32;
    enemyX = xa;
    enemyY = ya;
    enemyTransCn = cn;
  }
  return 0;
}

// int runLuaGame::l_fillrect(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//   float x = lua_tonumber(L, 1);
//   float y = lua_tonumber(L, 2);
//   float w = lua_tonumber(L, 3);
//   float h = lua_tonumber(L, 4);
//   int cn = lua_tointeger(L, 5);
//   int cn2 = lua_tointeger(L, 6);
//   int cn3 = lua_tointeger(L, 7);
//   int cmode = lua_tointeger(L, 8);

//   if(cn != NULL)
//   {
//     self->col[0] = clist2[cn][0]; // 5bit
//     self->col[1] = clist2[cn][1]; // 6bit
//     self->col[2] = clist2[cn][2]; // 5bit
//   }

//   if(cn3 != NULL)
//   {
//     self->col[0] = cn; // 5bit
//     self->col[1] = cn2; // 6bit
//     self->col[2] = cn3; // 5bit
//   }

//   if(cmode!=NULL||cmode==1){//cmode1のときはｈｓｂ
//     // HSBからRGBに変換
//     int r, g, b;
//     self->hsbToRgb(cn, cn2, cn3, r, g, b);

//     // RGB値を設定
//     self->col[0] = r; // Red
//     self->col[1] = g; // Green
//     self->col[2] = b; // Blue

//     tft.fillRect(x, y, w, h, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
//   }
//   else
//   {
//     tft.fillRect(x, y, w, h, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
//   }

//   return 0;
// }

int runLuaGame::l_drawtri(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x0 = lua_tointeger(L, 1);
  int y0 = lua_tointeger(L, 2);
  int x1 = lua_tointeger(L, 3);
  int y1 = lua_tointeger(L, 4);
  int x2 = lua_tointeger(L, 5);
  int y2 = lua_tointeger(L, 6);
  int cn = lua_tointeger(L, 7);
  int cn2 = lua_tointeger(L, 8);
  int cn3 = lua_tointeger(L, 9);
  if(cn != NULL)
  {
    self->col[0] = clist2[cn][0]; // 5bit
    self->col[1] = clist2[cn][1]; // 6bit
    self->col[2] = clist2[cn][2]; // 5bit
  }
  if(cn3 != NULL)
  {
    self->col[0] = cn; // 5bit
    self->col[1] = cn2; // 6bit
    self->col[2] = cn3; // 5bit
  }
  tft.drawTriangle(x0, y0, x1, y1, x2, y2, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

void runLuaGame::hsbToRgb2(float angle, float br, int& r, int& g, int& b) {
  if (angle != -1) {
    float hue = angle / 360.0f;  // 角度を0から1の範囲に正規化

    // 色相に応じて彩度を変化させる
    float si = 0.5f + 0.5f * cos(hue * 2 * M_PI);

    float hueNormalized = hue * 6.0f;
    int i = static_cast<int>(hueNormalized);
    float f = hueNormalized - i;

    float p = br * (1.0f - si);
    float q = br * (1.0f - si * f);
    float t = br * (1.0f - si * (1.0f - f));

    int br255 = static_cast<int>(br);
    int p255 = static_cast<int>(p);
    int q255 = static_cast<int>(q);
    int t255 = static_cast<int>(t);

    switch (i) {
      // ... 同じ
    }
  } else { // 無彩色指定の場合
      r = br;
      g = br;
      b = br;
  }
}

void runLuaGame::hsbToRgb(float angle, float si, float br, int& r, int& g, int& b) {

  if(angle!=-1){
  float hue = angle / 360.0f;  // 角度を0から1の範囲に正規化

  si = 1.0f;  // 彩度を常に最大値として扱う

  if (si == 0) {
    // S彩度が0の場合、色相に関係なく明度がそのままRGB値となる
    r = g = b = static_cast<int>(br);
    return;
  }

  float hueNormalized = hue * 6.0f;
  int i = static_cast<int>(hueNormalized);
  float f = hueNormalized - i;
  float p = br * (1.0f - si);
  float q = br * (1.0f - si * f);
  float t = br * (1.0f - si * (1.0f - f));

  int br255 = static_cast<int>(br);
  int p255 = static_cast<int>(p);
  int q255 = static_cast<int>(q);
  int t255 = static_cast<int>(t);

  switch (i) {
    case 0:
      r = br255;
      g = t255;
      b = p255;
      break;
    case 1:
      r = q255;
      g = br255;
      b = p255;
      break;
    case 2:
      r = p255;
      g = br255;
      b = t255;
      break;
    case 3:
      r = p255;
      g = q255;
      b = br255;
      break;
    case 4:
      r = t255;
      g = p255;
      b = br255;
      break;
    case 5:
      r = br255;
      g = p255;
      b = q255;
      break;
  }
  }else{//無彩色指定の場合
      r = br;
      g = br;
      b = br;
  }
}

void runLuaGame::fillFastTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c1){
// void runLuaGame::fillFastTriangle(float x0, float y0, float x1, float y1, float x2, float y2, uint16_t c1){

  tft.setColor(c1);
  int16_t a, b;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }
    if (y1 > y2) { std::swap(y2, y1); std::swap(x2, x1); }
    if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }

    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
      a = b = x0;
      if (x1 < a)      a = x1;
      else if (x1 > b) b = x1;
      if (x2 < a)      a = x2;
      else if (x2 > b) b = x2;
      tft.drawFastHLine(a, y0, b - a + 1);
      return;
    }
    if ((x1-x0) * (y2-y0) == (x2-x0) * (y1-y0)) {
      tft.drawLine(x0,y0,x2,y2);
      return;
    }

    int16_t dy1 = y1 - y0;
    int16_t dy2 = y2 - y0;
    bool change = ((x1 - x0) * dy2 > (x2 - x0) * dy1);
    int16_t dx1 = abs(x1 - x0);
    int16_t dx2 = abs(x2 - x0);
    int16_t xstep1 = x1 < x0 ? -1 : 1;
    int16_t xstep2 = x2 < x0 ? -1 : 1;
    a = b = x0;
    if (change) {
      std::swap(dx1, dx2);
      std::swap(dy1, dy2);
      std::swap(xstep1, xstep2);
    }
    int16_t err1 = (std::max(dx1, dy1) >> 1)
                 + (xstep1 < 0
                   ? std::min(dx1, dy1)
                   : dx1);
    int16_t err2 = (std::max(dx2, dy2) >> 1)
                 + (xstep2 > 0
                   ? std::min(dx2, dy2)
                   : dx2);
    tft.startWrite();
    if (y0 != y1) {
      do {
        err1 -= dx1;
        while (err1 < 0) { err1 += dy1; a += xstep1; }
        err2 -= dx2;
        while (err2 < 0) { err2 += dy2; b += xstep2; }
        // if (y0 % 2 != 0) { // 奇数の行のみを処理
          tft.writeFastHLine(a, y0, b - a + 1);
        // }
      } while (++y0 < y1);
    }

    if (change) {
      b = x1;
      xstep2 = x2 < x1 ? -1 : 1;
      dx2 = abs(x2 - x1);
      dy2 = y2 - y1;
      err2 = (std::max(dx2, dy2) >> 1)
           + (xstep2 > 0
             ? std::min(dx2, dy2)
             : dx2);
    } else {
      a = x1;
      dx1 = abs(x2 - x1);
      dy1 = y2 - y1;
      xstep1 = x2 < x1 ? -1 : 1;
      err1 = (std::max(dx1, dy1) >> 1)
           + (xstep1 < 0
             ? std::min(dx1, dy1)
             : dx1);
    }
    do {
      err1 -= dx1;
      while (err1 < 0) { err1 += dy1; if ((a += xstep1) == x2) break; }
      err2 -= dx2;
      while (err2 < 0) { err2 += dy2; if ((b += xstep2) == x2) break; }
      // if (y0 % 2 != 0) { // 奇数の行のみを処理
        tft.writeFastHLine(a, y0, b - a + 1);
      // }
    } while (++y0 <= y2);
    tft.endWrite();
}


int runLuaGame::l_filltri(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x0 = lua_tointeger(L, 1);
  int y0 = lua_tointeger(L, 2);
  int x1 = lua_tointeger(L, 3);
  int y1 = lua_tointeger(L, 4);
  int x2 = lua_tointeger(L, 5);
  int y2 = lua_tointeger(L, 6);
  int cn = lua_tointeger(L, 7);
  int cn2 = lua_tointeger(L, 8);
  int cn3 = lua_tointeger(L, 9);
  int cmode = lua_tointeger(L, 10);
  if(cn != NULL)
  {
    self->col[0] = clist2[cn][0]; // 5bit
    self->col[1] = clist2[cn][1]; // 6bit
    self->col[2] = clist2[cn][2]; // 5bit
  }

  if(cn3 != NULL)
  {
    self->col[0] = cn; // 5bit
    self->col[1] = cn2; // 6bit
    self->col[2] = cn3; // 5bit
  }

  
  if(cmode!=NULL||cmode==1){//cmode1のときはｈｓｂ
    // HSBからRGBに変換
    int r, g, b;
    self->hsbToRgb(cn, cn2, cn3, r, g, b);

    // RGB値を設定
    self->col[0] = r; // Red
    self->col[1] = g; // Green
    self->col[2] = b; // Blue

    self->fillFastTriangle(x0, y0, x1, y1, x2, y2, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  }
  else
  {
    self->fillFastTriangle(x0, y0, x1, y1, x2, y2, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  }
  
  return 0;
}

int runLuaGame::l_fillcircle(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  float r = lua_tonumber(L, 3);

  tft.fillCircle(x, y, r, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int runLuaGame::l_drawcircle(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  float r = lua_tonumber(L, 3);

  tft.drawCircle(x, y, r, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}


int runLuaGame::l_key(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

  lua_pushstring(L, &keychar);
  return 1;
}

int runLuaGame::l_bchat(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  String text = lua_tostring(L, 1);
  // broadchat(text);
  return 0;
}

int runLuaGame::l_btn(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);
  int n2 = lua_tointeger(L, 2);
  float addval = lua_tonumber(L, 3);
  float val = lua_tonumber(L, 4);

  if(n2 == NULL){
    lua_pushinteger(L, (lua_Integer)buttonState[n]);
  }else{
    if (buttonState[n] >= 2) {
      lua_pushnumber(L, val + addval);
    } 
    else if (buttonState[n2] >= 2) {
      lua_pushnumber(L, val - addval);
    }
  }
  return 1;
}

int runLuaGame::l_touch(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  // int n = lua_tointeger(L, 1);
  // lua_pushinteger(L, (lua_Integer)self->touchState);
  return 1;
}

void getPalValue(uint8_t palNo){
  //外部カラーパレットを読み込む
  File fr = SPIFFS.open("/init/param/pal/" + String(palNo) + ".txt", "r");
  String line;
  uint8_t j = 0;
  while (fr.available()) {
    line = fr.readStringUntil('\n');
    
    if (!line.isEmpty()) {
      int commaIndex = line.indexOf(',');
      if (commaIndex != -1) {
        String val = line.substring(0, commaIndex);
        clist2[j][0] = val.toInt();
        // Serial.print(val);
        // Serial.print(",");
          for (int i = 1; i < 3; i++) {
            int nextCommaIndex = line.indexOf(',', commaIndex + 1);
            if (nextCommaIndex != -1) {
              val = line.substring(commaIndex + 1, nextCommaIndex);
              // Serial.print(val);
              // Serial.print(",");
              clist2[j][i] = val.toInt(); // uint8_tに直接キャストする必要はありません
              commaIndex = nextCommaIndex;
            }
          }
      }
      j++;
      // Serial.println("");
    }
  }
  fr.close();
}

int runLuaGame::l_pal(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int palno = lua_tointeger(L, 1);
  getPalValue(palno);
  return 0;
}


int runLuaGame::l_btnp(lua_State* L)
{
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);

  if(buttonState[n] <= 4){
    if(buttonState[n]==1){lua_pushboolean(L, true);}//最初の１だけtrue
    else{lua_pushboolean(L, false);}
  }else{
    if(buttonState[n]%2 == 0){
      // Serial.println("定期的にtrue");
      lua_pushboolean(L, true);
    }else{
      lua_pushboolean(L, false);
    }
  }
  return 1;
}

int runLuaGame::l_sldr(lua_State* L)
{
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int xy = lua_tointeger(L, 1);
  
  // if(xy==0||xy==1){
    optionuiflag = true;
    lua_pushnumber(L, sliderval[xy]);
  // }
  return 1;
}

// int runLuaGame::l_iswifidebug(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

//   lua_pushboolean(L, isWifiDebug()?1:0);
//   return 1;
// }

// int runLuaGame::l_getip(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//   char buf[16];
//   IPAddress ip = WiFi.localIP();
//   if(self->wifiDebugSelf){
//     ip = WiFi.softAPIP();
//   }
//   sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3] );

//   lua_pushstring(L, buf);
//   return 1;
// }


// int runLuaGame::l_wifiserve(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//   self->wifiDebugSelf = false;
//   if(lua_gettop(L) == 1){
//     const char* text = lua_tostring(L, 1);
//     if(strncmp(text, "ap", 3) == 0){
//       self->wifiDebugSelf = true;
//     }
//   }
//   self->wifiDebugRequest = true;
//   return 0;
// }

String nowappfileName = "";
int runLuaGame::l_run(lua_State* L){//ファイル名を取得して、そのファイルを実行runする

  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  // const char* file = lua_tostring(L, 1);
  appfileName = lua_tostring(L, 1);
  Serial.println(appfileName);

  // setFileName(file);//次のゲームのパスをセット
  self->exitRequest = true;//次のゲームを立ち上げるフラグを立てる
  nowappfileName = appfileName;
  setOpenConfig(appfileName,0);//configに書き込んでおく
  return 0;
}

int runLuaGame::l_appmode(lua_State* L){//ファイル名を取得して、そのファイルを実行runする
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* file = lua_tostring(L, 1);
  const int modeno = lua_tointeger(L, 2);

  Serial.print(file);
  Serial.print("][");
  Serial.println(savedAppfileName);

  if(savedAppfileName != file){//違うゲームファイルを開こうとしていたら
    // editor.setCursor(0,0,0);//カーソルの座標をリセット
    // editor.setEditorConfig();
    difffileF = true;
  }

  // appfileName = file;//開くファイルを更新しておく

  if(modeno==2){
    setOpenConfig(file,2);
    delay(100); 
    reboot(file, modeno);
  }else if(modeno==3){
    setOpenConfig(file,3);
    delay(100); 
    reboot(file, modeno);
  }
  else {
    restart(file, modeno);
  }
  
  return 0;
}

int runLuaGame::l_appinfo(lua_State* L){//ファイル名を取得して、そのファイルを実行runする
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int infono = lua_tointeger(L, 1);
  if(infono == 0){
    appfileName = nowappfileName;
    // setOpenConfig(appfileName,1);//configに書き込んでおく
    const char *lineChar = appfileName.c_str();
    lua_pushstring(L, lineChar);//引数０の時は名前
  }
  return 1;
}


int runLuaGame::l_editor(lua_State* L){//ファイル名を取得して、そのファイルを実行runする
  return 0;
}



// 時分秒を度に変換する関数
float hmsToDegrees(int hours, int minutes, float seconds) {
    return (hours * 15.0 + minutes + seconds /3600); // 1時間 = 15度, 1分 = 0.25度, 1秒 = 0.00416667度
}

// 度分秒を度に変換する関数
float dmsToDegrees(int degrees, int minutes, float seconds) {
    return degrees + minutes / 60.0 + seconds / 3600.0; // 1度 = 60分 = 3600秒
}

// 時分秒を度に変換する関数
// float hmsToRadian(int hours, int minutes, float seconds) {
//     return (hours * 15.0 + minutes + seconds * 0.00416667) * (M_PI / 180.0); // 1時間 = 15度, 1分 = 0.25度, 1秒 = 0.00416667度
// }
float hmsToRadian(int hours, int minutes, float seconds) {
    return (hours * 15.0 + minutes + seconds /3600) * RAD_ONE; // 1時間 = 15度, 1分 = 0.25度, 1秒 = 0.00416667度
}

// 度分秒を度に変換する関数
float dmsToRadian(int degrees, int minutes, float seconds) {
    return (degrees + minutes / 60.0 + seconds / 3600.0) * RAD_ONE;// 1度 = 60分 = 3600秒
}

#include <vector>
#include <iostream>
#include <string>
int runLuaGame::l_initstars(lua_State* L) 
{
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float _asc_h = lua_tonumber(L, 1);
  float _asc_m = lua_tonumber(L, 2);
  float _asc_s = lua_tonumber(L, 3);
  float _dec_d  = lua_tonumber(L, 4);
  float _dec_m  = lua_tonumber(L, 5);
  float _dec_s  = lua_tonumber(L, 6);
  int zoom = lua_tointeger(L, 7);

  // 1000個の要素を事前に確保する
  // BSTARBUFNUM個の要素を追加
    bsParamFloat.resize(BSTARBUFNUM);
    bsParamInt8t.resize(BSTARBUFNUM);
    bsParamInt16t.resize(BSTARBUFNUM);

  // 赤経の中心と幅を指定
  float centerRightAscension = hmsToRadian( _asc_h, _asc_m, _asc_s ); // 中心の赤経（時分秒をラジアンに変換する関数）
  float centerDeclination    = dmsToRadian( _dec_d, _dec_m, _dec_s );// 中心の赤緯（度分秒をラジアンに変換する関数）
  
  float halfWidthRightAscension = (M_PI / zoom); // 赤経の半幅（ラジアン）
  float halfHeightDeclination = (M_PI / zoom);// 赤緯の半幅（ラジアン）

  if (SPIFFS.exists("/star/brightstar1000.csv")) {
    File fr = SPIFFS.open("/star/brightstar1000.csv", FILE_READ);
    String line;

    int currentIndex = 0;
    // 赤経と赤緯のラジアン変換
    float starRightAscension = 0;
    float starDeclination = 0;
    int starbrightness = 0;
    int colangle = -1;//無彩色を指定
    
    String noStr = "";
    String rightAscensionStr = "";
    String declinationStr = "";
    String brightnessStr = "";
    String spectraltypeStr = "";
    // String starData[6]; // 6つの要素を持つ配列Y

    int startLine = 1;//この行から読み込み描画を開始する
    int currentLine = 1;

    float startAscRadian = centerRightAscension + (M_PI/zoom);

    //赤経のラジアン角から読み込み描画のスタート行を決定する
         if(startAscRadian >=6.771955068 && startAscRadian <= 7.051396826){startLine = 9000;}
    else if(startAscRadian >=6.422898914 && startAscRadian < 6.771955068 ){startLine = 8750;}
    else if(startAscRadian >=6.195972174 && startAscRadian < 6.422898914 ){startLine = 8500;}
    else if(startAscRadian >=6.021395616 && startAscRadian < 6.195972174 ){startLine = 8250;}
    else if(startAscRadian >=5.881778972 && startAscRadian < 6.021395616 ){startLine = 8000;}
    else if(startAscRadian >=5.707464213 && startAscRadian < 5.881778972 ){startLine = 7750;}
    else if(startAscRadian >=5.567624555 && startAscRadian < 5.707464213 ){startLine = 7500;}
    else if(startAscRadian >=5.410525529 && startAscRadian < 5.567624555 ){startLine = 7250;}
    else if(startAscRadian >=5.253470137 && startAscRadian < 5.410525529 ){startLine = 7000;}
    else if(startAscRadian >=5.079169923 && startAscRadian < 5.253470137 ){startLine = 6750;}
    else if(startAscRadian >=4.921930301 && startAscRadian < 5.079169923 ){startLine = 6500;}
    else if(startAscRadian >=4.764840973 && startAscRadian < 4.921930301 ){startLine = 6250;}
    else if(startAscRadian >=4.590283807 && startAscRadian < 4.764840973 ){startLine = 6000;}
    else if(startAscRadian >=4.4157024   && startAscRadian < 4.590283807 ){startLine = 5750;}
    else if(startAscRadian >=4.223832538 && startAscRadian < 4.4157024   ){startLine = 5500;}
    else if(startAscRadian >=4.031904498 && startAscRadian < 4.223832538 ){startLine = 5250;}
    else if(startAscRadian >=3.839792228 && startAscRadian < 4.031904498 ){startLine = 5000;}
    else if(startAscRadian >=3.630415744 && startAscRadian < 3.839792228 ){startLine = 4750;}
    else if(startAscRadian >=3.438395589 && startAscRadian < 3.630415744 ){startLine = 4500;}
    else if(startAscRadian >=3.22900456  && startAscRadian < 3.438395589 ){startLine = 4250;}
    else if(startAscRadian >=3.01961838  && startAscRadian < 3.22900456  ){startLine = 4000;}
    else if(startAscRadian >=2.862533899 && startAscRadian < 3.01961838  ){startLine = 3750;}
    else if(startAscRadian >=2.705400937 && startAscRadian < 2.862533899 ){startLine = 3500;}
    else if(startAscRadian >=2.565638849 && startAscRadian < 2.705400937 ){startLine = 3250;}
    else if(startAscRadian >=2.426206434 && startAscRadian < 2.565638849 ){startLine = 3000;}
    else if(startAscRadian >=2.286449194 && startAscRadian < 2.426206434 ){startLine = 2750;}
    else if(startAscRadian >=2.146842246 && startAscRadian < 2.286449194 ){startLine = 2500;}
    else if(startAscRadian >=2.007303173 && startAscRadian < 2.146842246 ){startLine = 2250;}
    else if(startAscRadian >=1.885023466 && startAscRadian < 2.007303173 ){startLine = 2000;}
    else if(startAscRadian >=1.728065037 && startAscRadian < 1.885023466 ){startLine = 1750;}
    else if(startAscRadian >=1.571082367 && startAscRadian < 1.728065037 ){startLine = 1500;}
    else if(startAscRadian >=1.413774872 && startAscRadian < 1.571082367 ){startLine = 1250;}
    else if(startAscRadian >=1.239435872 && startAscRadian < 1.413774872 ){startLine = 1000;}
    else if(startAscRadian >=1.029865462 && startAscRadian < 1.239435872 ){startLine = 750;}
    else if(startAscRadian >=0.820416256 && startAscRadian < 1.029865462 ){startLine = 500;}
    else if(startAscRadian >=0.541212057 && startAscRadian < 0.820416256 ){startLine = 250;}
    else if(startAscRadian >=0 && startAscRadian < 0.541212057 ){startLine = 1;}

    // while (fr.available()) {
    // while (starData[1].toFloat() < centerRightAscension + (M_PI/zoom)) {//描画すべき赤経を超えるまで読み込む
    for(int i=0;i<BSTARBUFNUM;i++){
      line = fr.readStringUntil('\n');
        if (currentLine >= startLine) {
          if (!line.isEmpty()) {

            int currentIndex = 0;

            while (line.length() > 0) {
              int commaIndex = line.indexOf(',');
              if (commaIndex != -1) {

                String val = line.substring(0, commaIndex);
                // std::string val(str_val.begin(), str_val.end());

                if(currentIndex==0)bsParamFloat[i][0] = val.toFloat();
                else if(currentIndex==1)bsParamFloat[i][1] = val.toFloat();
                else if(currentIndex==2)bsParamInt8t[i][0] = val.toInt();
                else if(currentIndex==3)bsParamInt16t[i][0] = val.toInt();

                //      if (currentIndex == 0)
                //     bsParamFloat[i][0] = std::atof(val.c_str());
                // else if (currentIndex == 1)
                //     bsParamFloat[i][1] = std::atof(val.c_str());
                // else if (currentIndex == 2)
                //     bsParamInt8t[i][0] = std::atoi(val.c_str());
                // else if (currentIndex == 3)
                //     bsParamInt16t[i][0] = std::atoi(val.c_str());

    //                 if (currentIndex == 0) {
    //     if (bsParamFloat.size() <= i) // i番目の要素が存在しない場合
    //         bsParamFloat.resize(i + 1); // 要素を追加する
    //     bsParamFloat[i].push_back(std::stof(val)); // 要素に値を追加する
    // }
    // else if (currentIndex == 1) {
    //     if (bsParamFloat.size() <= i) // i番目の要素が存在しない場合
    //         bsParamFloat.resize(i + 1); // 要素を追加する
    //     bsParamFloat[i].push_back(std::stof(val)); // 要素に値を追加する
    // }
    // else if (currentIndex == 2) {
    //     if (bsParamInt8t.size() <= i) // i番目の要素が存在しない場合
    //         bsParamInt8t.resize(i + 1); // 要素を追加する
    //     bsParamInt8t[i].push_back(std::stoi(val)); // 要素に値を追加する
    // }
    // else if (currentIndex == 3) {
    //     if (bsParamInt16t.size() <= i) // i番目の要素が存在しない場合
    //         bsParamInt16t.resize(i + 1); // 要素を追加する
    //     bsParamInt16t[i].push_back(std::stoi(val)); // 要素に値を追加する
    // }

                currentIndex++;
                line = line.substring(commaIndex + 1);
              } else {
                // starData[currentIndex] = line;
                line = "";
              }
            }
            currentIndex++;
          }
        }
        currentLine++;
      }
    fr.close();

    
    
    
  }
  return 0;
}


int runLuaGame::l_line(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

  double xa = lua_tonumber(L, 1);
  double ya = lua_tonumber(L, 2);
  double xb = lua_tonumber(L, 3);
  double yb = lua_tonumber(L, 4);
  int    cn = lua_tointeger(L, 5);
  
  // int xa = lua_tointeger(L, 1);
  // int ya = lua_tointeger(L, 2);
  // int xb = lua_tointeger(L, 3);
  // int yb = lua_tointeger(L, 4);
  // int cn = lua_tointeger(L, 5);

  if(cn != NULL)
  {
    self->col[0] = clist2[cn][0]; // 5bit
    self->col[1] = clist2[cn][1]; // 6bit
    self->col[2] = clist2[cn][2]; // 5bit
  }
  tft.drawLine(xa,ya,xb,yb, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int runLuaGame::l_list(lua_State* L) {
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int modeno = lua_tointeger(L, 1);

  if(modeno == -1){
  File f;
  // lua_newtable(L);
  // ファイル数をあらかじめカウント
  File root = SPIFFS.open("/");
  f = root.openNextFile();
  int firstCountNo = 0;
  int fileCount = firstCountNo; // ファイル数をカウントするための変数を初期化

  while (f) {
    String filePath = f.path(); // ファイルパスを取得
    
    // フォルダ名を取得
    String folderName = filePath.substring(0, filePath.lastIndexOf('/'));
    // フォルダ名が "/init/" で始まる場合はスキップする
    if (folderName.startsWith("/init/")) {
      f = root.openNextFile();
      continue;
    }

    if (filePath.endsWith(".lua") || filePath.endsWith(".js") || filePath.endsWith("caldata.txt")) { // 拡張子が ".lua"" または ".js" または "caldata.txt" の場合のみ処理
      self->fileNamelist.push_back(filePath);
      // lua_pushstring(L, filePath.c_str()); // パスを文字列にしてリターン
      // lua_rawseti(L, -2, fileCount);
      fileCount++; // ファイル数をインクリメント

    }
    f = root.openNextFile();
  }
  f.close();
  root.close();

  lua_pushinteger(L, fileCount);//ファイル数を返す

  }else if(modeno >= 0){//モードが0以上の時
    // Serial.println(modeno);
    appfileName = self->fileNamelist[modeno];
    lua_pushstring(L, appfileName.c_str());
    
  }

  // テーブルはスタック上に残すため、ここでは解放しない
  return 1; // 1を返してテーブルの数を指定
}


// int runLuaGame::l_require(lua_State* L){
//   bool loadError = false;
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//   const char* fname = lua_tostring(L, 1);
//   File fp = SPIFFS.open(fname, FILE_READ);

//   struct LoadF lf;
//   lf.f = fp;
//   char cFileName[32];
//   appfileName.toCharArray(cFileName, 32);
//   if(lua_load(L, getF, &lf, cFileName, NULL)){
//     printf("error? %s\n", lua_tostring(L, -1));
//     Serial.printf("error? %s\n", lua_tostring(L, -1));
//     loadError = true;
//   }
//   fp.close();

//   if(loadError == false){
//     if(lua_pcall(L, 0, 1, 0)){
//       Serial.printf("init error? %s\n", lua_tostring(L, -1));

//     }
//   }

//   Serial.println("finish require");


//   return 1;
// }

// int runLuaGame::l_httpsget(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//   const char* host = lua_tostring(L, 1);
//   const char* path = lua_tostring(L, 2);
//   WiFiClientSecure client;
//   const int httpsPort = 443;
//   // Serial.println(host);
//   // Serial.println(path);
//   if(!client.connect(host, httpsPort)){
//     // connection failed
//     Serial.println("connect failed");
//   }
//   client.print(String("GET ") + path + " HTTP/1.1\r\n" +
//     "Host: " + host + "\r\n" +
//     "User-Agent: o-bako\r\n" +
//     "Connection: close\r\n\r\n"
//   );
//   String line;
//   while(client.connected()){
//     line = client.readStringUntil('\n');
//     if(line == "\r"){
//       // headers recieved
//       Serial.println("headers recieved");
//       break;
//     }
//   }
//   line = client.readString();
//   int lineLength = line.length();
//   const char *lineChar = line.c_str();

//   lua_pushstring(L, lineChar);
//   Serial.println("done");
//   return 1;
// }

// int runLuaGame::l_httpsgetfile(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//   const char* host = lua_tostring(L, 1);
//   const char* path = lua_tostring(L, 2);
//   const char* filePath = lua_tostring(L, 3);
//   WiFiClientSecure client;
//   const int httpsPort = 443;
//   if(!client.connect(host, httpsPort)){
//     // connection failed
//     Serial.println("connect failed");
//   }
//   client.print(String("GET ") + path + " HTTP/1.1\r\n" +
//     "Host: " + host + "\r\n" +
//     "User-Agent: o-bako\r\n" +
//     "Connection: close\r\n\r\n"
//   );
//   String line;
//   int len = 0;
//   while(client.connected()){
//     line = client.readStringUntil('\n');
//     if(line == "\r"){
//       // headers recieved
//       Serial.println("headers recieved");
//       break;
//     }
//     if(line.startsWith("Content-Length: ")){
//       len = line.substring(16).toInt();
//     }

//   }

//   tunes.pause();
//   File f = SPIFFS.open(filePath, FILE_WRITE);
//   while(client.available() && len > 0){
//     char c = client.read();
//     f.write(c);
//     len --;
//   }
//   f.close();
//   tunes.resume();
//   return 0;
// }

// int runLuaGame::l_savebmp(lua_State* L){
//   runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//   const char* path = lua_tostring(L, 1);
//   uint8_t buf[4];
//   long* ltmp = (long*) buf;
//   uint16_t* tmp2 = (uint16_t*)buf;
//   uint8_t tmp1;

//   tunes.pause();
//   File f = SPIFFS.open(path, FILE_WRITE);
//   f.write('B');
//   f.write('M');
//   *ltmp = 14 + 40 + 4 * 256;
//   f.write(buf, 4); // file size
//   *tmp2 = 0;
//   f.write(buf, 2); // reserved1
//   f.write(buf, 2); // reserved2
//   *ltmp = 14 + 40 + 4 * 256;
//   f.write(buf, 4); // header size

//   // BITMAPCOREHEADER
//   *ltmp = 40;
//   f.write(buf, 4); // bc size
//   *ltmp = 128;
//   f.write(buf, 4); // width
//   f.write(buf, 4); // height
//   *tmp2 = 1;
//   f.write(buf, 2); // planes
//   *tmp2 = 8;
//   f.write(buf, 2); // bitcount
//   *ltmp = 0;
//   f.write(buf,4); // compression
//   *ltmp = 0;
//   f.write(buf,4); // size image
//   *ltmp = 0;
//   f.write(buf,4); // horizon dot/m
//   *ltmp = 0;
//   f.write(buf,4); // vertical dot/m
//   *ltmp = 0;
//   f.write(buf,4); // cir used
//   *ltmp = 0;
//   f.write(buf,4); // cir important

//   uint8_t r,g,b;
//   for(unsigned int i = 0; i < 256; i ++){
//     r = ((self->palette[i] >> 11) << 3);
//     g = (((self->palette[i] >> 5) & 0b111111) << 2);
//     b = ((self->palette[i] & 0b11111) << 3);
//     f.write(b);
//     f.write(g);
//     f.write(r);
//     f.write(0); // reserved
//   }
//   int x = 0,y = 127;
//   for(unsigned int i = 0; i < 128*128; i ++){
//     uint16_t d = tft.readPixel(x, y);
//     uint8_t index = 0;
//     for(unsigned int pi = 0; pi < 256; pi ++){
//       if(self->palette[pi] == d){
//         index = pi;
//         break;
//       }
//     }
//     f.write(index);
//     x ++;
//     if(x == 128){
//       x = 0;
//       y --;
//     }
//   }
//   f.close();
//   tunes.resume();
//   return 0;
// }

int runLuaGame::l_reboot(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

  reboot();
  return 0;
}

int runLuaGame::l_debug(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* text = lua_tostring(L, 1);

  // Serial.println(text);
  return 0;
}

String runLuaGame::getBitmapName(String s){
  int p = s.lastIndexOf("/");
  if(p == -1){
    p = 0;
  }
  return s.substring(0, p) + "/sprite.bmp";
}

String runLuaGame::getPngName(String s){
  int p = s.lastIndexOf("/");
  if(p == -1){
    p = 0;
  }
  return s.substring(0, p) + "/initspr.png";
}

void runLuaGame::init(){
  this->resume();
}

void runLuaGame::pause(){
  lua_close(L);
}

extern bool firstBootF;

void runLuaGame::resume(){//ゲーム起動時のみ一回だけ走る処理（setupのようなもの)

// if(firstBootF){

L = luaL_newstate();
lua_setglobal(L, "PSRAM");

luaL_Buffer buff;
luaL_buffinit(L, &buff);

// ここでluaBufferに適切なデータを読み込む（例：ファイルからデータを読み込む）
char *luaBuffer = (char *)malloc(LUA_BUFSIZE); // メモリ確保を行う
// ... データをluaBufferに読み込む処理 ...

luaL_addlstring(&buff, luaBuffer, LUA_BUFSIZE);

free(luaBuffer); // メモリ解放は必要ならここで行う

luaL_openlibs(L);

// L = luaL_newstate();
// lua_setglobal(L, "PSRAM");

// char *luaBuffer = (char *)malloc(MAX_CHAR); // メモリ割り当てを行う
// luaL_Buffer buff;
// luaL_buffinit(L, &buff);
// luaL_buffinitsize(L, &buff, MAX_CHAR);
// luaL_addlstring(&buff, luaBuffer, MAX_CHAR);

// // free(luaBuffer); // メモリ解放を行う

//   luaL_openlibs(L);

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_tp, 1);
  // lua_setglobal(L, "tp");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_tstat, 1);
  // lua_setglobal(L, "tstat");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_vol, 1);
  lua_setglobal(L, "vol");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_print, 1);
  lua_setglobal(L, "print");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_pinw, 1);
  lua_setglobal(L, "pinw");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_pinr, 1);
  lua_setglobal(L, "pinr");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_tone, 1);
  lua_setglobal(L, "tone");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_tool, 1);
  lua_setglobal(L, "tool");


  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_spr8, 1);
  lua_setglobal(L, "spr8");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_spr, 1);
  lua_setglobal(L, "spr");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_scroll, 1);
  lua_setglobal(L, "scroll");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_map, 1);
  lua_setglobal(L, "map");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_mget, 1);
  lua_setglobal(L, "mget");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_rmap, 1);
  lua_setglobal(L, "rmap");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_rendr, 1);
  lua_setglobal(L, "rendr");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_rendr2, 1);
  lua_setglobal(L, "rendr2");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawstars, 1);
  lua_setglobal(L, "drawstars");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_pset, 1);
  lua_setglobal(L, "pset");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_pget, 1);
  lua_setglobal(L, "pget");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_cls, 1);
  lua_setglobal(L, "cls");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fget, 1);
  lua_setglobal(L, "fget");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fset, 1);
  lua_setglobal(L, "fset");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_music, 1);
  lua_setglobal(L, "music");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sfx, 1);
  lua_setglobal(L, "sfx");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_go2, 1);
  lua_setglobal(L, "go2");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_gstat, 1);
  lua_setglobal(L, "gstat");
  
  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_rnd, 1);
  lua_setglobal(L, "rnd");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_srnd, 1);
  lua_setglobal(L, "srnd");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sgn, 1);
  lua_setglobal(L, "sgn");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_shl, 1);
  lua_setglobal(L, "shl");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_shr, 1);
  lua_setglobal(L, "shr");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_flr, 1);
  lua_setglobal(L, "flr");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_min, 1);
  lua_setglobal(L, "min");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_mid, 1);
  lua_setglobal(L, "mid");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_max, 1);
  lua_setglobal(L, "max");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_min, 1);
  lua_setglobal(L, "min");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_abs, 1);
  lua_setglobal(L, "abs");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sqrt, 1);
  lua_setglobal(L, "sqrt");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_distance, 1);
  lua_setglobal(L, "distance");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sin, 1);
  lua_setglobal(L, "sin");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_gsin, 1);
  // lua_setglobal(L, "gsin");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_gcos, 1);
  // lua_setglobal(L, "gcos");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_cos, 1);
  lua_setglobal(L, "cos");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_atan2, 1);
  lua_setglobal(L, "atan2");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_band, 1);
  lua_setglobal(L, "band");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_bnot, 1);
  lua_setglobal(L, "bnot");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_bor, 1);
  lua_setglobal(L, "bor");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_bxor, 1);
  lua_setglobal(L, "bxor");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_creobj, 1);
  lua_setglobal(L, "creobj");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_ceil, 1);
  lua_setglobal(L, "celi");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_creobj2, 1);
  lua_setglobal(L, "creobj2");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_color, 1);
  lua_setglobal(L, "color");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_text, 1);
  lua_setglobal(L, "text");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_opmode, 1);
  lua_setglobal(L, "opmode");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawrect, 1);
  lua_setglobal(L, "drawrect");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fillrect, 1);
  lua_setglobal(L, "fillrect");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_rectfill, 1);
  lua_setglobal(L, "rectfill");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fillp, 1);
  lua_setglobal(L, "fillp");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_oval, 1);
  lua_setglobal(L, "oval");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_ovalfill, 1);
  lua_setglobal(L, "ovalfill");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fps, 1);
  lua_setglobal(L, "fps");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fillpoly, 1);
  lua_setglobal(L, "fillpoly");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawbox, 1);
  lua_setglobal(L, "drawbox");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawboxp, 1);
  lua_setglobal(L, "drawboxp");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawtri, 1);
  lua_setglobal(L, "drawtri");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_filltri, 1);
  lua_setglobal(L, "filltri");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fillcircle, 1);
  lua_setglobal(L, "fillcircle");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawcircle, 1);
  lua_setglobal(L, "drawcircle");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_pal, 1);
  lua_setglobal(L, "pal");
  
  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_key, 1);
  lua_setglobal(L, "key");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_bchat, 1);
  lua_setglobal(L, "bchat");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_btn, 1);
  lua_setglobal(L, "btn");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_touch, 1);
  lua_setglobal(L, "touch");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_btnp, 1);
  lua_setglobal(L, "btnp");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sldr, 1);
  lua_setglobal(L, "sldr");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_getip, 1);
  // lua_setglobal(L, "getip");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_iswifidebug, 1);
  // lua_setglobal(L, "iswifidebug");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_initstars, 1);
  lua_setglobal(L, "initstars");


  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_win, 1);
  lua_setglobal(L, "win");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_run, 1);
  lua_setglobal(L, "run");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_appmode, 1);
  lua_setglobal(L, "appmode");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_appinfo, 1);
  lua_setglobal(L, "appinfo");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_editor, 1);
  lua_setglobal(L, "editor");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_line, 1);
  lua_setglobal(L, "line");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_list, 1);
  lua_setglobal(L, "list");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_reboot, 1);
  lua_setglobal(L, "reboot");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_debug, 1);
  lua_setglobal(L, "debug");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_require, 1);
  // lua_setglobal(L, "require");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_httpsget, 1);
  // lua_setglobal(L, "httpsget");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_httpsgetfile, 1);
  // lua_setglobal(L, "httpsgetfile");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_savebmp, 1);
  // lua_setglobal(L, "savebmp");

  masterVol = 0;//起動時すぐには音が鳴らないようにする

  objects.clear();//オブジェクトを一度滑れ破棄してリセット

  // haco8resume();//派生クラスでのみこの位置で実行されるダミー関数

  File fr;

  fr = SPIFFS.open(SPRBITS_FILE, "r");
  for (int i = 0; i < 128; i++) {
    String _readStr = fr.readStringUntil(','); // ,まで１つ読み出し
    std::string _readstr = _readStr.c_str();

    // 改行を取り除く処理
    const char CR = '\r';
    const char LF = '\n';
    std::string destStr;
    for (std::string::const_iterator it = _readstr.begin(); it != _readstr.end(); ++it) {
      if (*it != CR && *it != LF && *it != '\0') {
        destStr += *it;
      }
    }

    _readstr = destStr;

    uint8_t bdata = 0b00000000;
    uint8_t bitfilter = 0b10000000; // 書き換え対象ビット指定用

    for (int j = 0; j < _readstr.length(); ++j) {
        char ch = _readstr[j];
        // Serial.print(ch);
        if (ch == '1') {
            bdata |= bitfilter; // 状態を重ね合わせて合成
        }
        bitfilter = bitfilter >> 1; // 書き換え対象ビットを一つずらす
    }

    // sprbits[i] = bdata;
    
    // Serial.print(":");
    // Serial.print(bdata); // 0～255
    // Serial.print(":");
    // Serial.println("end");
  }
  fr.close();
  //アプリのパスからアプリ名を取得
  string str1 = appfileName.c_str();
  int i=0;

  for (string s : split(str1,'/')) {
    if(i==1){
      appNameStr = s.c_str();
      fr = SPIFFS.open("/" + appNameStr + "/mapinfo.txt", "r");// ⑩ファイルを読み込みモードで開く
    }
     i++;
  }

  for(int i= 0;i<16;i++){//マップを描くときに使うスプライト番号リストを読み込む
    String _readStr = fr.readStringUntil(',');// ⑪,まで１つ読み出し
    mapsprnos[i] = atoi(_readStr.c_str());
  }

  String _readStr = fr.readStringUntil(',');// 最後はマップのパス
  mapFileName = "/init/map/"+_readStr;
  fr.close();	// ⑫	ファイルを閉じる
  readMap();

//-------------------------------------------

LoadF lf(MAX_CHAR); // 2048 バイトのメモリを確保する例
lf.f = SPIFFS.open(appfileName, FILE_READ);

char cFileName[32];
appfileName.toCharArray(cFileName, 32);//char変換

if (lua_load(L, getF, &lf, cFileName, NULL)){//Luaに渡してファイル読み込みに成功したかチェック（成功すると0）
    printf("error? %s\n", lua_tostring(L, -1));
    Serial.printf("error? %s\n", lua_tostring(L, -1));
    runError = true;//エラーが発生
    errorString = lua_tostring(L, -1);
}

if (!runError) {
    if (lua_pcall(L, 0, 0, 0)) {
        Serial.printf("init error? %s\n", lua_tostring(L, -1));
        runError = true;
        errorString = lua_tostring(L, -1);
    }
}



}

int runLuaGame::l_sin(lua_State* L) {
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  lua_pushnumber(L, sin(x));
  return 1;
}

int runLuaGame::l_cos(lua_State* L) {
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  lua_pushnumber(L, cos(x));
  return 1;
}


int runLuaGame::l_atan2(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float y = lua_tonumber(L, 1);//intに入れた時点でfloorされてる
  float x = lua_tonumber(L, 2);//intに入れた時点でfloorされてる
  lua_pushnumber(L, atan2(y, x));
  return 1;
}


int runLuaGame::l_rnd(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);
  n =  rand() % n;
  lua_pushinteger(L, n);
  return 1;
}

int runLuaGame::l_srnd(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int s = lua_tointeger(L, 1);
  srand(s);
  // lua_pushinteger(L, 0);
  return 0;
}

int runLuaGame::l_sgn(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  if (x > 0) {
    lua_pushinteger(L, 1);
  } else if (x < 0) {
    lua_pushinteger(L, -1);
  } else {
    lua_pushinteger(L, 0);
  }
  return 1;
}

int runLuaGame::l_shl(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  lua_pushinteger(L, x << y);
  return 1;
}

int runLuaGame::l_shr(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  lua_pushinteger(L, x >> y);
  return 1;
}

int runLuaGame::l_band(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  lua_pushinteger(L, x & y);
  return 1;
}

int runLuaGame::l_bnot(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  lua_pushinteger(L, ~x);
  return 1;
}

int runLuaGame::l_bor(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  lua_pushinteger(L, x | y);
  return 1;
}

int runLuaGame::l_bxor(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  lua_pushinteger(L, x ^ y);
  return 1;
}


int runLuaGame::l_ceil(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float n = lua_tonumber(L, 1);//
  int in =  (int)ceil(n);
  lua_pushinteger(L, in);
  return 1;
}

int runLuaGame::l_flr(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tonumber(L, 1);//intに入れた時点でfloorされてる
  // n =  (int)floor(n);
  lua_pushinteger(L, n);
  return 1;
}

int runLuaGame::l_max(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float a = lua_tonumber(L, 1);
  float b = lua_tonumber(L, 2);
  if (a > b){
    lua_pushnumber(L, a);
  }else{
    lua_pushnumber(L, b);
  }
  return 1;
}

int runLuaGame::l_mid(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  float z = lua_tonumber(L, 3);

      if ((x <= y && y <= z) || (z <= y && y <= x)) {
          lua_pushnumber(L, y);
      }
      else if ((y <= x && x <= z) || (z <= x && x <= y)) {
          lua_pushnumber(L, x);
      }
      else {
          lua_pushnumber(L, z);
      }
  return 1;
}

int runLuaGame::l_min(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float a = lua_tonumber(L, 1);
  float b = lua_tonumber(L, 2);
  if (a < b){
    lua_pushnumber(L, a);
  }else{
    lua_pushnumber(L, b);
  }
  return 1;
}


int runLuaGame::l_abs(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  if (x < 0) {
        lua_pushnumber(L, -x);
    } else {
        lua_pushnumber(L, x);
    }
  return 1;
}

int runLuaGame::l_sqrt(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  lua_pushnumber(L, sqrt(x));
  return 1;
}

int runLuaGame::l_distance(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  float x1 = lua_tonumber(L, 1);
  float y1 = lua_tonumber(L, 2);
  float x2 = lua_tonumber(L, 3);
  float y2 = lua_tonumber(L, 4);

  float dx = x2 - x1;
  float dy = y2 - y1;
  lua_pushnumber(L, sqrt(dx*dx + dy*dy));
  return 1;
} 

#define TFT_WIDTH_HALF 80
#define TFT_HEIGHT_HALF 64

void runLuaGame::fastPoint(const Vector3<float> v1, int starbrightness, int noInt) 
{

  uint16_t c1 = lua_rgb24to16(starbrightness, starbrightness, starbrightness);
  

  if(starbrightness>70 && starbrightness<=110){//110=４等級
    tft.startWrite();
    tft.writePixel(static_cast<int32_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF), 
      TFT_HEIGHT-static_cast<int32_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF),
      c1);
    tft.endWrite();
  }
  else if(starbrightness>110 && starbrightness<184){
    tft.startWrite();
    tft.writePixel(static_cast<int32_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF), 
      TFT_HEIGHT-static_cast<int32_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF),
      c1);
    tft.endWrite();
    
    // tft.fillCircle(static_cast<int32_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF), 
    // TFT_HEIGHT-static_cast<int32_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF), 1, c1);
  }
  else if(starbrightness>=184){
    tft.fillCircle(static_cast<int32_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF), 
    TFT_HEIGHT-static_cast<int32_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF), 1, c1); 
    
  }

  tft.setTextWrap(false);
  tft.setCursor(static_cast<int32_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF)+4, 
  TFT_HEIGHT-static_cast<int32_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF)+4);
        if(noInt==2061){tft.print("ベテルギウス");}
    else if(noInt==1713){tft.print("リゲル");}
    else if(noInt==5191){tft.print("アルカイド");}
    else if(noInt==4763){tft.print("ガクルックス");}
    else if(noInt==4357){tft.print("ゾズマ");}
    else if(noInt==264){tft.print("カシオペヤ");}
    else if(noInt==2491){tft.print("シリウス");}
    else if(noInt==2827){tft.print("アルドラ");}
    else if(noInt==3165){tft.print("ナオス");}
    else if(noInt==2990){tft.print("ポルックス");}
    else if(noInt==5054){tft.print("ミザール");}
    else if(noInt==5793){tft.print("アルフェッカ");}
    else if(noInt==6527){tft.print("シャウラ");}
    else if(noInt==7796){tft.print("サドル");}
    else if(noInt==7924){tft.print("デネブ");}
    else if(noInt==8162){tft.print("アルデラミン");}
    else if(noInt==8775){tft.print("シェアト");}
    else if(noInt==8308){tft.print("エニフ");}
    else if(noInt==8728){tft.print("フォーマルハウト");}
    else if(noInt==4301){tft.print("ドゥベー");}
    else if(noInt==4905){tft.print("アリオト");}
    else if(noInt==3982){tft.print("レグルス");}
    else if(noInt==4534){tft.print("デネボラ");}
    else if(noInt==4554){tft.print("フェクダ");}
    else if(noInt==7001){tft.print("ベガ");}
    else if(noInt==6705){tft.print("エルタニン");}
    else if(noInt==3748){tft.print("アルファルド");}
    else if(noInt==5056){tft.print("スピカ");}
    else if(noInt==3634){tft.print("スハイル");}
    else if(noInt==4853){tft.print("ミモザ");}
    else if(noInt==1903){tft.print("アルニラム");}
    else if(noInt==603){tft.print("アルマク");}
    else if(noInt==168){tft.print("シェダル");}
    else if(noInt==617){tft.print("ハマル");}
    else if(noInt==936){tft.print("アルゴル");}
    else if(noInt==1017){tft.print("ミルファク");}
    else if(noInt==1457){tft.print("アルデバラン");}
    else if(noInt==424){tft.print("ポラリス");}
    else if(noInt==2326){tft.print("カノープス");}
    else if(noInt==3685){tft.print("ミアプラキドゥス");}
    else if(noInt==2943){tft.print("プロキオン");}
    else if(noInt==99){tft.print("アンカー");}
    else if(noInt==472){tft.print("アケルナル");}
    else if(noInt==2249){tft.print("ミルザム");}
    else if(noInt==1948){tft.print("アルニタク");}
    else if(noInt==1790){tft.print("ベラトリクス");}//Bellatrix
    else if(noInt==1791){tft.print("エルナト");}//Elnath
    else if(noInt==2088){tft.print("メンカリナン");}//Menkalinan
    else if(noInt==5953){tft.print("ジュバ");}//Dschubba
    else if(noInt==6378){tft.print("サビク");}//Sabik
    else if(noInt==6217){tft.print("アトリア");}//Atria
    else if(noInt==6134){tft.print("アンタレス");}
    else if(noInt==5459){tft.print("Rigil Kentaurus");}
    else if(noInt==7790){tft.print("ピーコック");}
}


// テーブル内の要素のインデックス
const int VRTCS_TABLE_INDEX = 1;

// テーブル内の要素のキー
const char* V1_KEY = "v1";
const char* V2_KEY = "v2";
const char* V3_KEY = "v3";

inline float clamp(float value, float min, float max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}
static runLuaGame::CameraObj camera;
static runLuaGame::LightObj light;
static runLuaGame::CubeObj plyObj;

// ベクトルの長さを計算する関数
float runLuaGame::calculateLength(float x, float y, float z) {
  return std::sqrt(x * x + y * y + z * z);
}

// ベクトルの正規化を行う関数
Vector3<float> runLuaGame::normalize(float x, float y, float z) {
  float length = calculateLength(x, y, z);
  return {x / length, y / length, z / length};
}

// 2つのベクトルの内積を計算する関数
float runLuaGame::calculateDotProduct(const Vector3<float>& v1, const Vector3<float>& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 3つの頂点から法線ベクトルを計算する関数
Vector3<float> runLuaGame::calculateNormal(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3) {
  float mx = v2.x - v1.x;
  float my = v2.y - v1.y;
  float uz = v2.z - v1.z;
  float vx = v3.x - v1.x;
  float vy = v3.y - v1.y;
  float vz = v3.z - v1.z;

  float nx = my * vz - uz * vy;
  float ny = uz * vx - mx * vz;
  float nz = mx * vy - my * vx;

  return normalize(nx, ny, nz);
}

// ポリゴンの明るさを計算する関数
// float runLuaGame::calculateBrightness(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3, const runLuaGame::LightObj& light) {
//   // ポリゴンの法線ベクトルを計算
//   Vector3<float> normal = calculateNormal(v1, v2, v3);
//   // 光源ベクトルを計算
//   Vector3<float> lightVector = normalize(light.x, light.y, light.z);
//   // ポリゴンの法線ベクトルと光源ベクトルの角度を計算
//   float dotProduct = calculateDotProduct(normal, lightVector);
//   // 明るさを0~1の範囲に正規化
//   float brightness = (dotProduct + 1) / 2;
//   return brightness;
// }

// ポリゴンの明るさを計算する関数
float runLuaGame::calculateBrightness(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3, const runLuaGame::LightObj& light) {
  // ポリゴンの法線ベクトルを計算
  Vector3<float> normal = calculateNormal(v1, v2, v3);
  // 光源ベクトルを計算して正規化
  Vector3<float> lightVector = normalize(light.x, light.y, light.z);
  
  // ポリゴンの法線ベクトルと光源ベクトルの角度を計算
  float dotProduct = calculateDotProduct(normal, lightVector);
  
  // 明るさを0~1の範囲に正規化（ここが修正点）
  // dotProductの範囲は-1から1ですが、角度の余弦の性質から0から1に正規化する必要があります
  float normalizedBrightness = (dotProduct + 1.0f) / 2.0f;
  
  return normalizedBrightness;
}


void runLuaGame::line3D(const Vector3<float> v1,const Vector3<float> v2,uint16_t c1) {
   Vector3<int32_t> p1,p2;

  p1.set(static_cast<int32_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF),
          TFT_HEIGHT-static_cast<int32_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF),
          v1.getZ());

  p2.set(static_cast<int32_t>((v2.getX() + 1.0f) * TFT_WIDTH_HALF),
          TFT_HEIGHT-static_cast<int32_t>((v2.getY() + 1.0f) * TFT_HEIGHT_HALF),
          v2.getZ());

  tft.drawLine(p1.getX(), p1.getY(), p2.getX(), p2.getY(), c1);
}

 void runLuaGame::fill3DFastTriangle(int32_t x0, int32_t y0, int32_t z0,
                                        int32_t x1, int32_t y1, int32_t z1,
                                        int32_t x2, int32_t y2, int32_t z2,
                                        uint16_t c1) {
  tft.setColor(c1);
  int32_t a, b;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }
    if (y1 > y2) { std::swap(y2, y1); std::swap(x2, x1); }
    if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }

    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
      a = b = x0;
      if (x1 < a)      a = x1;
      else if (x1 > b) b = x1;
      if (x2 < a)      a = x2;
      else if (x2 > b) b = x2;
      tft.drawFastHLine(a, y0, b - a + 1);
      return;
    }

    if ((x1-x0) * (y2-y0) == (x2-x0) * (y1-y0)) {
      tft.drawLine(x0,y0,x2,y2);
      return;
    }

    int32_t dy1 = y1 - y0;
    int32_t dy2 = y2 - y0;
    bool change = ((x1 - x0) * dy2 > (x2 - x0) * dy1);
    int32_t dx1 = abs(x1 - x0);
    int32_t dx2 = abs(x2 - x0);
    int32_t xstep1 = x1 < x0 ? -1 : 1;
    int32_t xstep2 = x2 < x0 ? -1 : 1;
    a = b = x0;
    if (change) {
      std::swap(dx1, dx2);
      std::swap(dy1, dy2);
      std::swap(xstep1, xstep2);
    }
    int32_t err1 = (std::max(dx1, dy1) >> 1)
                 + (xstep1 < 0
                   ? std::min(dx1, dy1)
                   : dx1);
    int32_t err2 = (std::max(dx2, dy2) >> 1)
                 + (xstep2 > 0
                   ? std::min(dx2, dy2)
                   : dx2);

    if (z0 < 0 || z0 > 1 || z1 < 0 || z1 > 1 || z2 < 0 || z2 > 1) {
        return;
    }

    tft.startWrite();
    if (y0 != y1) {
      do {
        err1 -= dx1;
        while (err1 < 0) { err1 += dy1; a += xstep1; }
        err2 -= dx2;
        while (err2 < 0) { err2 += dy2; b += xstep2; }
        // if (y0 % 2 != 0) { // 奇数の行のみを処理
        // Zが０～１の時のみを処理
          if (z0 >= 0 && z0 <= 1 && z1 >= 0 && z1 <= 1 && z2 >= 0 && z2 <= 1) {
            tft.writeFastHLine(a, y0, b - a + 1);
          }
        // }
      } while (++y0 < y1);
    }

    if (change) {
      b = x1;
      xstep2 = x2 < x1 ? -1 : 1;
      dx2 = abs(x2 - x1);
      dy2 = y2 - y1;
      err2 = (std::max(dx2, dy2) >> 1)
           + (xstep2 > 0
             ? std::min(dx2, dy2)
             : dx2);
    } else {
      a = x1;
      dx1 = abs(x2 - x1);
      dy1 = y2 - y1;
      xstep1 = x2 < x1 ? -1 : 1;
      err1 = (std::max(dx1, dy1) >> 1)
           + (xstep1 < 0
             ? std::min(dx1, dy1)
             : dx1);
    }
    do {
      err1 -= dx1;
      while (err1 < 0) { err1 += dy1; if ((a += xstep1) == x2) break; }
      err2 -= dx2;
      while (err2 < 0) { err2 += dy2; if ((b += xstep2) == x2) break; }
      // if (y0 % 2 != 0) { // 奇数の行のみを処理
        tft.writeFastHLine(a, y0, b - a + 1);
      // }
    } while (++y0 <= y2);
    tft.endWrite();
}


void runLuaGame::triangle3D(const Vector3<float> v1,const Vector3<float> v2,const Vector3<float> v3, int idx) {
  // Vector3<int32_t> p1,p2,p3;

  // p1.set(static_cast<int32_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF),
  //         TFT_HEIGHT-static_cast<int32_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF),
  //         v1.getZ());

  // p2.set(static_cast<int32_t>((v2.getX() + 1.0f) * TFT_WIDTH_HALF),
  //         TFT_HEIGHT-static_cast<int32_t>((v2.getY() + 1.0f) * TFT_HEIGHT_HALF),
  //         v2.getZ());

  // p3.set(static_cast<int32_t>((v3.getX() + 1.0f) * TFT_WIDTH_HALF),
  //         TFT_HEIGHT-static_cast<int32_t>((v3.getY() + 1.0f) * TFT_HEIGHT_HALF),
  //         v3.getZ());


        // カメラ座標系での座標変換
        Vector3<float> normal = calculateNormal(v1, v2, v3);

        Vector3<float> viewVector(
          camera.x2 - camera.x, 
          camera.y2 - camera.y, 
          camera.z2 - camera.z);

        viewVector = viewVector.normalize();
        float len = viewVector.length();

        // ポリゴンの法線ベクトルと視線ベクトルの角度を計算
        float dotProduct = calculateDotProduct(normal, viewVector);
        float angle1 = degrees(std::acos(dotProduct));

        // ポリゴンの明るさを計算
        float brightness = static_cast<float>(calculateBrightness(v1, v2, v3, light));

        // 明るさに基づいてポリゴンの色を設定（0~255の範囲に収める）（最低値を0にせずまっ黒にならないように調整している）
        int color = static_cast<int>(brightness * 200.0f + 55);
        color = clamp(color, 0, 255);

        // 角度が180度未満のポリゴンのみ描画
        if (angle1 < 180.0f) {
          // 陰面消去を実施
          Vector3<int32_t> p1,p2,p3;

          //画面に拡大表示
          p1.set(static_cast<int32_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF),
                  TFT_HEIGHT-static_cast<int32_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF),
                  v1.getZ());

          p2.set(static_cast<int32_t>((v2.getX() + 1.0f) * TFT_WIDTH_HALF),
                  TFT_HEIGHT-static_cast<int32_t>((v2.getY() + 1.0f) * TFT_HEIGHT_HALF),
                  v2.getZ());

          p3.set(static_cast<int32_t>((v3.getX() + 1.0f) * TFT_WIDTH_HALF),
                  TFT_HEIGHT-static_cast<int32_t>((v3.getY() + 1.0f) * TFT_HEIGHT_HALF),
                  v3.getZ());

          // ポリゴンの面積を計算
          float area = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);

          // 面積が正の場合はポリゴンを描画
          if (area > 0.0f) {
                // HSBからRGBに変換
            int r, g, b;
            // hsbToRgb(plyObj.colangle, 127, color, r, g, b);
            // hsbToRgb(objects[idx].h, objects[idx].s, objects[idx].v, r, g, b);//面の明るさを一定に
            hsbToRgb(objects[idx].h, objects[idx].s, color, r, g, b);

            byte col[3] = {0,0,0};

            // RGB値を設定
            col[0] = r; // Red
            col[1] = g; // Green
            col[2] = b; // Blue

            // gameInstance.fillFastTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, lua_rgb24to16(col[0], col[1], col[2]));
            fill3DFastTriangle(
              p1.getX(), p1.getY(), p1.getZ(), 
              p2.getX(), p2.getY(), p2.getZ(), 
              p3.getX(), p3.getY(), p3.getZ(), 
              lua_rgb24to16(col[0], col[1], col[2]));
            
            // Vector3<float> cp= {float(p1.getX()), float(p1.getY()), float(p1.getZ())};

            // point3D(cp,TFT_WHITE);
          }
        }
  
    // fill3DFastTriangle(
    //   p1.getX(), p1.getY(), p1.getZ(), 
    //   p2.getX(), p2.getY(), p2.getZ(), 
    //   p3.getX(), p3.getY(), p3.getZ(), 
    //   TFT_WHITE);

}

int langle = 0;
int sw = 2;
int runLuaGame::l_rendr(lua_State* L) {
    runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
    // float deg1 = lua_tonumber(L, 1);
    // float deg2 = lua_tonumber(L, 2);
    int deg1 = lua_tointeger(L, 1);
    int deg2 = lua_tointeger(L, 2);
    int sw = lua_tointeger(L, 3);
    float zoom = lua_tonumber(L, 4);
    camera.zoom = zoom;

    if (deg1 > 360 || deg1 <= 0 ) {
        // deg1 = fmod(deg1 , 360);
        deg1%=360;
    }

    if (deg2 > 360 || deg2 < 0 ) {
        // deg2 = fmod(deg2 , 360);
        deg2%=360;
    }

    // float c = gcos(deg1);
    // float s = gsin(deg1);

    // float c2 = gcos(deg2);
    // float s2 = gsin(deg2);

    float c = cos(deg1*RAD_ONE);
    float s = sin(deg1*RAD_ONE);

    float c2 = cos(deg2*RAD_ONE);
    float s2 = sin(deg2*RAD_ONE);

    langle += 3;
    langle %= 360;
    //ライトのxyzは位置ではなくベクトル成分。多分。
    light.x = sin(langle*RAD_ONE);//cos(langle*RAD_ONE);
    light.y = 1;//＋が上のよう
    light.z = cos(langle*RAD_ONE);//手前奥

    for(int i =0;i < numVertices; i++)
    // for(int i = 0; i < BSTARBUFNUM; i++)
    {
      //world座標変換
      
      objects[0].p[i].set(objects[0].op[i].getX(), 
                          objects[0].op[i].getY(), 
                          objects[0].op[i].getZ());

      //world座標変換
      // if(sw == 0){
      //   p[i].setX(c * op[i].getX() + -s * op[i].getY());
      //   p[i].setY(s * op[i].getX() +  c * op[i].getY());
      //   p[i].setZ(op[i].getZ());
      // }
      // else if(sw == 1){
      //   p[i].setX(c * op[i].getX() + -s * op[i].getZ());
      //   p[i].setY(op[i].getY());
      //   p[i].setZ(s * op[i].getX() +  c * op[i].getZ());
      // }
      // else if(sw == 2){
      //   p[i].setX(op[i].getX());
      //   p[i].setY(c2 * op[i].getY() + -s2 * op[i].getZ());
      //   p[i].setZ(s2 * op[i].getY() +  c2 * op[i].getZ());
      // }

      // カメラの移動
      objects[0].p[i].setZ(-objects[0].p[i].getZ() - 1);//カメラの位置0だと表示されない
      objects[0].p[i].setZ(-objects[0].p[i].getZ());//記号を反転させる
      objects[0].p[i].setX(objects[0].p[i].getX() / objects[0].p[i].getZ()/camera.zoom);
      objects[0].p[i].setY(objects[0].p[i].getY() / objects[0].p[i].getZ()/camera.zoom);
      objects[0].p[i].setZ(objects[0].p[i].getZ() / 5);

    }

    // self->triangle3D(p[0], p[1], p[2]);
    // self->triangle3D(p[0], p[2], p[3]);
    // self->triangle3D(p[0], p[2], p[3]);

    int numCorners = 24;//輪切りにした時の頂点数

      for(int i =0;i < numVertices; i++)
      // for(int i = 0; i < BSTARBUFNUM; i++)
      {
        int j = i + 1;
        if(j % numCorners == 0) j -= numCorners;

        int k = i + numCorners;
        int l = j + numCorners;

        // if(i < numVertices - numCorners){
          self->triangle3D(objects[0].p[i], objects[0].p[j], objects[0].p[k],0);
          self->triangle3D(objects[0].p[j], objects[0].p[l], objects[0].p[k],0);
          // self->fastPoint(p[i]);

          int starbrightness = bsParamInt8t[i][0];
          int noInt = bsParamInt16t[i][0];

          // self->fastPoint(p[i], starbrightness, noInt);
                  
        // }

        //線を書く

        // self->line3D(p[i], p[j], TFT_CYAN);
        // if(i < numVertices - numCorners){
        //   j = i + numCorners;
        //   self->line3D(p[i], p[j], TFT_BLUE);

        // self->line3D(p[i], p[j], TFT_CYAN);


        // if( i < numVertices - numCorners){//赤経をひく
        //   if( 0 == i % numCorners){//0度
        //     self->line3D(p[i], p[k], TFT_RED);
        //   }else{
        //     self->line3D(p[i], p[j], TFT_BLUE);
        //   }
        // }

        // if( i < numVertices - 1){//赤緯をひく
        //   if( i >= numCorners*(numRings/2) && i < numCorners*(numRings/2+1) ){
        //     self->line3D(p[i], p[j], TFT_RED);//0度
        //   }else{
        //     self->line3D(p[i], p[j], TFT_BLUE);
        //   }
        // }
        
        //点を書く

        // if( 3 >= i % numCorners){
        //   if( i <= numCorners*4){
          // self->point3D(p[i],TFT_WHITE);

          // self->fastPoint(p[i],TFT_WHITE);
        //   }
        // }
      }
      return 0;

}

// int runLuaGame::l_drawstars(lua_State* L) {
//     runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//     // float deg1 = lua_tonumber(L, 1);
//     // float deg2 = lua_tonumber(L, 2);
//     int deg1 = lua_tointeger(L, 1);
//     int deg2 = lua_tointeger(L, 2);
//     int sw = lua_tointeger(L, 3);
//     float zoom = lua_tonumber(L, 4);
//     camera.zoom = zoom;

//     if (deg1 > 360 || deg1 <= 0 ) {
//         // deg1 = fmod(deg1 , 360);
//         deg1%=360;
//     }

//     if (deg2 > 360 || deg2 < 0 ) {
//         // deg2 = fmod(deg2 , 360);
//         deg2%=360;
//     }

//     // float c = gcos(deg1);
//     // float s = gsin(deg1);

//     // float c2 = gcos(deg2);
//     // float s2 = gsin(deg2);

//     float c = cos(deg1*RAD_ONE);
//     float s = sin(deg1*RAD_ONE);

//     float c2 = cos(deg2*RAD_ONE);
//     float s2 = sin(deg2*RAD_ONE);

//     // langle += 3;
//     // langle %= 360;
//     //ライトのxyzは位置ではなくベクトル成分。多分。
//     // light.x = gsin(langle);//cos(langle*RAD_ONE);
//     // light.y = 1;//＋が上のよう
//     // light.z = gcos(langle);//手前奥

  
//     // for(int i =0;i < numVertices; i++)
//     for(int i = 0; i < BSTARBUFNUM; i++)
//     {
//       //world座標変換
      
//       // p[i].set( op[i].getX(), 
//       //           op[i].getY(), 
//       //           op[i].getZ());

//       //world座標変換
//       if(sw == 0){
//         p[i].setX(c * op[i].getX() + -s * op[i].getY());
//         p[i].setY(s * op[i].getX() +  c * op[i].getY());
//         p[i].setZ(op[i].getZ());
//       }
//       else if(sw == 1){
//         p[i].setX(c * op[i].getX() + -s * op[i].getZ());
//         p[i].setY(op[i].getY());
//         p[i].setZ(s * op[i].getX() +  c * op[i].getZ());
//       }
//       else if(sw == 2){
//         p[i].setX(op[i].getX());
//         p[i].setY(c2 * op[i].getY() + -s2 * op[i].getZ());
//         p[i].setZ(s2 * op[i].getY() +  c2 * op[i].getZ());
//       }

//       // カメラの移動
//       p[i].setZ(-p[i].getZ() - 1);//カメラの位置0だと表示されない
//       p[i].setZ(-p[i].getZ());//記号を反転させる
//       p[i].setX(p[i].getX() / p[i].getZ()/camera.zoom);
//       p[i].setY(p[i].getY() / p[i].getZ()/camera.zoom);
//       p[i].setZ(p[i].getZ() / 5);

//     }

//     // self->triangle3D(p[0], p[1], p[2]);
//     // self->triangle3D(p[0], p[2], p[3]);
//     // self->triangle3D(p[0], p[2], p[3]);

//       // for(int i =0;i < numVertices; i++)
//       for(int i = 0; i < BSTARBUFNUM; i++)
//       {
//         int j = i + 1;
//         if(j % numCorners == 0) j -= numCorners;

//         int k = i + numCorners;
//         int l = j + numCorners;

//         // if(i < numVertices - numCorners){
//           // self->triangle3D(p[i], p[j], p[k]);
//           // self->triangle3D(p[j], p[l], p[k]);
//           // self->fastPoint(p[i]);

//           int starbrightness = bsParamInt8t[i][0];
//           int noInt = bsParamInt16t[i][0];

//           self->fastPoint(p[i], starbrightness, noInt);
                  
//         // }

//         //線を書く

//         self->line3D(p[i], p[j], TFT_CYAN);
//         if(i < numVertices - numCorners){
//           j = i + numCorners;
//           self->line3D(p[i], p[j], TFT_BLUE);

//         self->line3D(p[i], p[j], TFT_CYAN);


//         if( i < numVertices - numCorners){//赤経をひく
//           if( 0 == i % numCorners){//0度
//             self->line3D(p[i], p[k], TFT_RED);
//           }else{
//             self->line3D(p[i], p[j], TFT_BLUE);
//           }
//         }

//         if( i < numVertices - 1){//赤緯をひく
//           if( i >= numCorners*(numRings/2) && i < numCorners*(numRings/2+1) ){
//             self->line3D(p[i], p[j], TFT_RED);//0度
//           }else{
//             self->line3D(p[i], p[j], TFT_BLUE);
//           }
//         }
        
//         //点を書く

//         // if( 3 >= i % numCorners){
//         //   if( i <= numCorners*4){
//           // self->point3D(p[i],TFT_WHITE);

//           // self->fastPoint(p[i],TFT_WHITE);
//         //   }
//         // }
//       }

   
// }
//  return 0;
// }


int runLuaGame::l_drawstars(lua_State* L) {
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int cam_angleX = lua_tointeger(L, 1);
  int cam_angleY = lua_tointeger(L, 2);
  int cam_angleZ = 0;
  float zoom = lua_tonumber(L, 3);
  if(camera.zoom<=0.2){camera.zoom = 0.2;}
  camera.zoom = zoom;
  int drawmode = lua_tointeger(L, 4);

  if (cam_angleX > 360 || cam_angleX <= 0 ) {
      // deg1 = fmod(deg1 , 360);
      cam_angleX+=360;
      cam_angleX%=360;
  }

  if (cam_angleY > 360 || cam_angleY < 0 ) {
      // deg2 = fmod(deg2 , 360);
      cam_angleY+=360;
      cam_angleY%=360;
  }

  if (cam_angleZ > 360 || cam_angleZ < 0 ) {
      cam_angleZ+=360;
      cam_angleZ%=360;
  }

  // 与えられた角度をラジアンに変換
  float angleX = cam_angleX * RAD_ONE;
  float angleY = cam_angleY * RAD_ONE;

  // 各軸周りの回転行列を計算
  float cosX = cos(angleX);
  float sinX = sin(angleX);
  float cosY = cos(angleY);
  float sinY = sin(angleY);

    // int deg1 = lua_tointeger(L, 1);
    // int deg2 = lua_tointeger(L, 2);
    // int sw = lua_tointeger(L, 3);
    // float zoom = lua_tonumber(L, 4);
    // camera.zoom = zoom;

    // langle += 3;
    // langle %= 360;
    // // ライトのxyzは位置ではなくベクトル成分。多分。
    // light.x = gsin(langle);//cos(langle*RAD_ONE);
    // light.y = 1;//＋が上のよう
    // light.z = gcos(langle);//手前奥

    if(drawmode == 1||drawmode == 2){
  
    //グリッド線を引く
    for(int i =0;i < numVertices; i++)
    {

      float x = opg[i].getX();
      float y = opg[i].getY();
      float z = opg[i].getZ();

      
      // y軸周りの回転
      float tempX1 = x * cosY - z * sinY;
      float tempZ1 = x * sinY + z * cosY;

      // x軸周りの回転
      pg[i].setX(tempX1);
      pg[i].setY(y * cosX + tempZ1 * sinX);
      pg[i].setZ(-y * sinX + tempZ1 * cosX);

      //world座標変換
      // if(sw == 0){
      //   objects[0].p[i].setX(cosX * objects[0].op[i].getX() + -sinX * objects[0].op[i].getY());
      //   objects[0].p[i].setY(sinX * objects[0].op[i].getX() +  cosX * objects[0].op[i].getY());
      //   objects[0].p[i].setZ(objects[0].op[i].getZ());
      // }
      // else if(sw == 1){
      //   objects[0].p[i].setX(cosX * objects[0].op[i].getX() + -sinX * objects[0].op[i].getZ());
      //   objects[0].p[i].setY(objects[0].op[i].getY());
      //   objects[0].p[i].setZ(sinX * objects[0].op[i].getX() +  cosX * objects[0].op[i].getZ());
      // }
      // else if(sw == 2){
      //   objects[0].p[i].setX(objects[0].op[i].getX());
      //   objects[0].p[i].setY(cosY * objects[0].op[i].getY() + -sinY * objects[0].op[i].getZ());
      //   objects[0].p[i].setZ(sinY * objects[0].op[i].getY() +  cosY * objects[0].op[i].getZ());
      // }

      // カメラの移動
      pg[i].setZ(-pg[i].getZ() - 1);//カメラの位置0だと表示されない
      pg[i].setZ(-pg[i].getZ());//記号を反転させる
      pg[i].setX(pg[i].getX() / pg[i].getZ()/camera.zoom);
      pg[i].setY(pg[i].getY() / pg[i].getZ()/camera.zoom);
      pg[i].setZ(pg[i].getZ() / 5);
    }
    

    int numCorners = 24;//輪切りにした時の頂点数
    int numRings = numCorners / 2 + 1;//縦方向の頂点数

    for(int i =0;i < numVertices; i++)
    {

    //   // self->triangle3D(pg[0], pg[1], pg[2]);
    //   // self->triangle3D(pg[0], pg[2], pg[3]);
    //   // self->triangle3D(pg[0], pg[2], pg[3]);

    //   //線を書く

    //   // self->line3D(p[i], objects[0].p[j], TFT_CYAN);
    //   // if(i < numVertices - numCorners){
    //   //   j = i + numCorners;
    //   //   self->line3D(objects[0].p[i], objects[0].p[j], TFT_BLUE);

    //   //self->line3D(objects[0].p[i], objects[0].p[j], TFT_CYAN);

      int j = i + 1;
    //   if(j % numCorners == 0) j -= numCorners;//一周したらリセット

      int k = i + numCorners;
      int l = j + numCorners;


      // k-l
      // |\|
      // i-j

      // ポリゴン生成
      // if(i < numVertices - numCorners){
      //   self->triangle3D(objects[0].p[i], objects[0].p[j], objects[0].p[k]);
      //   self->triangle3D(objects[0].p[j], pg[l], objects[0].p[k]);
      // }

      if( i < numVertices - numCorners){//赤経をひく
        if( 0 == i % numCorners){//0度
          self->line3D(pg[i], pg[k], HACO3_C8);
        }
        else{
          self->line3D(pg[i], pg[k], HACO3_C1);
        }
      }

      if( i < numVertices - 1){//赤緯をひく
        if( numCorners-1 == i % numCorners){
          j = i-(numCorners-1);
        }

        if( i >= numCorners*(numRings/2) && i < numCorners*(numRings/2+1) ){
        // if(i <= numCorners-1){
          self->line3D(pg[i], pg[j], HACO3_C8);//0度
        }else{
          self->line3D(pg[i], pg[j], HACO3_C1);
        }
      }
      
    //   //点を書く

    //   // if( 3 >= i % numCorners){
    //   //   if( i <= numCorners*4){
    //     // self->point3D(objects[0].p[i],TFT_WHITE);

    //     // self->fastPoint(objects[0].p[i],TFT_WHITE);
    //   //   }
    //   // }
    }

    }

    if(drawmode == 0||drawmode == 1){

    // 座標変換
  for (int i = 0; i < BSTARBUFNUM; i++) {
      float x = objects[0].op[i].getX();
      float y = objects[0].op[i].getY();
      float z = objects[0].op[i].getZ();

      // y軸周りの回転
      float tempX1 = x * cosY - z * sinY;
      float tempZ1 = x * sinY + z * cosY;

      // x軸周りの回転
      objects[0].p[i].setX(tempX1);
      objects[0].p[i].setY(y * cosX + tempZ1 * sinX);
      objects[0].p[i].setZ(-y * sinX + tempZ1 * cosX);

      //world座標変換
      // if(sw == 0){
      //   p[i].setX(cosX * op[i].getX() + -sinX * op[i].getY());
      //   p[i].setY(sinX * op[i].getX() +  cosX * op[i].getY());
      //   p[i].setZ(op[i].getZ());
      // }
      // else if(sw == 1){
      //   p[i].setX(cosX * op[i].getX() + -sinX * op[i].getZ());
      //   p[i].setY(op[i].getY());
      //   p[i].setZ(sinX * op[i].getX() +  cosX * op[i].getZ());
      // }
      // else if(sw == 2){
      //   p[i].setX(op[i].getX());
      //   p[i].setY(cosY * op[i].getY() + -sinY * op[i].getZ());
      //   p[i].setZ(sinY * op[i].getY() +  cosY * op[i].getZ());
      // }

      // カメラの移動
      objects[0].p[i].setZ(-objects[0].p[i].getZ() - 1);//カメラの位置0だと表示されない
      objects[0].p[i].setZ(-objects[0].p[i].getZ());//記号を反転させる
      objects[0].p[i].setX(objects[0].p[i].getX() / objects[0].p[i].getZ()/camera.zoom);
      objects[0].p[i].setY(objects[0].p[i].getY() / objects[0].p[i].getZ()/camera.zoom);
      objects[0].p[i].setZ(objects[0].p[i].getZ() / 5);
  }

//星を描画
    for(int i =0;i < BSTARBUFNUM; i++)
    {

      int starbrightness = bsParamInt8t[i][0];
      int noInt = bsParamInt16t[i][0];
      self->fastPoint(objects[0].p[i], starbrightness, noInt);
                  
    }
    }


    return 0;
}

int runLuaGame::l_creobj2(lua_State* L){
  runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  
  objno = lua_tointeger(L, 1);//カメラのx軸角0~360

  float objx = lua_tonumber(L, 2);//オブジェ位置
  float objy = lua_tonumber(L, 3);
  float objz = lua_tonumber(L, 4);

  int objh = lua_tointeger(L, 5);//オブジェ色
  int objs = lua_tointeger(L, 6);
  int objv = lua_tointeger(L, 7);

  float scale = lua_tonumber(L, 8);
  // Obj 構造体を作成し、値を設定し、動的配列に追加する
  Obj obj;
  obj.x = objx;
  obj.y = objy;
  obj.z = objz;
  obj.h = objh;
  obj.s = objs;
  obj.v = objv;

  obj.scale = scale; // ここでサイズの初期値を設定します

  obj.angle = 0; // ここで角度の初期値を設定します
  obj.colangle = 120; // ここで色の初期値を設定します
  obj.width = 10; // ここで幅の初期値を設定します
  obj.height = 10; // ここで高さの初期値を設定します

  // 動的配列に追加
  objects.push_back(obj);
  uint8_t lastno =objects.size() - 1;

  if(objno==0)
  {
    int numCorners = 24;//輪切りにした時の頂点数
    int numRings = numCorners / 2 + 1;//縦方向の頂点数
    numVertices = numCorners * numRings;//すべての頂点数
    float cdeg = 360.0f/ float(numCorners);
  
    //球オブジェクトを生成
    for (int j = 0; j < numRings; j++) //13
    {
      // float r = sin(cdeg * j * RAD_ONE);
      float y = cos(cdeg * j * RAD_ONE);

      for(int i=0;i<numCorners; i++){//24
        int k = j * numCorners + i;//k番目の頂点
        for(int n=0; n < 3; n++){//xyz分３を確保
          //ベクター配列を初期化
          Vector3<float> initp;
          objects[lastno].op.push_back(initp);
          objects[lastno].p.push_back(initp);
        }

        // objects[lastno].op[k].setX((sin(i*cdeg*RAD_ONE) )*obj.scale);//輪切り状態のX
        // objects[lastno].op[k].setZ((cos(i*cdeg*RAD_ONE) )*obj.scale);//輪切り状態のY
        // objects[lastno].op[k].setY(y*obj.scale);//高さ方向

        objects[lastno].op[k].set(obj.x+(sin(((i * cdeg)-45) * RAD_ONE) )*obj.scale,
                                  obj.y+ y*obj.scale,
                                  obj.z+(cos(((i * cdeg)-45) * RAD_ONE) )*obj.scale);
      }
    }
  }
  else if(objno==1){//立方体オブジェクトを生成

    for (int i = 0; i < 8; i++)
    {
      for(int n = 0; n < 3; n++)
      {
          // ベクター配列を初期化
          Vector3<float> initp;
          objects[lastno].op.push_back(initp);
          objects[lastno].p.push_back(initp);
      }
    }

    int numCorners = 4; // 輪切りにした時の頂点数
    int numRings = 2; // 縦方向の頂点数
    numVertices = numCorners * numRings; // すべての頂点数
    float cdeg = (360.0f) / float(numCorners);
  
    // 立方体オブジェクトを生成
    for (int j = 0; j < numRings; j++) {
        for(int i = 0; i < numCorners; i++) {
            int k = j * numCorners + i; // k番目の頂点

            for(int n = 0; n < 3; n++) {
                // ベクター配列を初期化
                Vector3<float> initp;
                objects[lastno].op.push_back(initp);
                objects[lastno].p.push_back(initp);
            }

            float y = cos((cdeg * j) * RAD_ONE)*2;

            objects[lastno].op[k].set(obj.x + sin(((i * cdeg)-45) * RAD_ONE)*obj.scale,
                                      obj.y + y*obj.scale,
                                      obj.z + cos(((i * cdeg)-45) * RAD_ONE)*obj.scale);

        }
    }
  }
    return 0; // テーブルをスタックに返す
}


int runLuaGame::l_rendr2(lua_State* L)
{
    runLuaGame* self = (runLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
    float camX = lua_tonumber(L, 1);//カメラのx軸角0~360
    float camY = lua_tonumber(L, 2);//カメラのy軸角0~360
    float camZ = lua_tonumber(L, 3);//カメラのy軸角0~360
    int camXangle = lua_tointeger(L, 4);//カメラのx軸角0~360
    int camYangle = lua_tointeger(L, 5);//カメラのy軸角0~360
    int sw = lua_tointeger(L, 6);
    float zoom = lua_tonumber(L, 7);
    camera.zoom = zoom;

    if (camXangle > 360 || camXangle <= 0 ) {
        // camXangle = fmod(camXangle , 360);
        camXangle%=360;
    }

    if (camYangle > 360 || camYangle < 0 ) {
        // camYangle = fmod(camYangle , 360);
        camYangle%=360;
    }

    float c = cos(camXangle*RAD_ONE);
    float s = sin(camXangle*RAD_ONE);

    float c2 = cos(camYangle*RAD_ONE);
    float s2 = sin(camYangle*RAD_ONE);

    langle += 3;
    langle %= 360;
    //ライトのxyzは位置ではなくベクトル成分。多分。
    light.x = sin(langle*RAD_ONE);//cos(langle*RAD_ONE);
    light.y = 1;//＋が上のよう
    light.z = cos(langle*RAD_ONE);//手前奥

    for (size_t idx = 0; idx < objects.size(); ++idx) {

    for(int i =0;i < numVertices; i++)
    {
      //world座標変換
      objects[idx].p[i].set(objects[idx].op[i].getX()+camX, 
                            objects[idx].op[i].getY()+camY, 
                            objects[idx].op[i].getZ()+camZ);

      //world座標変換
      // if(sw == 0){
      //   p[i].setX(c * op[i].getX() + -s * op[i].getY());
      //   p[i].setY(s * op[i].getX() +  c * op[i].getY());
      //   p[i].setZ(op[i].getZ());
      // }
      // else if(sw == 1){
        // p[i].setX((c * op[i].getX() + -s * op[i].getZ()));
        // p[i].setY((op[i].getY()));
        // p[i].setZ((s * op[i].getX() +  c * op[i].getZ()));
      // }
      // else if(sw == 2){
      //   p[i].setX(op[i].getX());
      //   p[i].setY(c2 * op[i].getY() + -s2 * op[i].getZ());
      //   p[i].setZ(s2 * op[i].getY() +  c2 * op[i].getZ());
      // }

      // カメラの移動
      objects[idx].p[i].setZ(-objects[idx].p[i].getZ() - 1);//カメラの位置0だと表示されない
      objects[idx].p[i].setZ(-objects[idx].p[i].getZ());//記号を反転させる
      objects[idx].p[i].setX(objects[idx].p[i].getX() / objects[idx].p[i].getZ()/camera.zoom);
      objects[idx].p[i].setY(objects[idx].p[i].getY() / objects[idx].p[i].getZ()/camera.zoom);
      objects[idx].p[i].setZ(objects[idx].p[i].getZ() / 5);
  }

  if(objno == 0){
    int numCorners = 24;//輪切りにした時の頂点数
    int numRings = numCorners / 2 + 1;//縦方向の頂点数

    for(int i =0;i < numVertices; i++)
    // for(int i = 0; i < BSTARBUFNUM; i++)
    {
        int j = i + 1;
        if(j % numCorners == 0) j -= numCorners;

        int k = i + numCorners;
        int l = j + numCorners;

        // if(i < numVertices - numCorners){
        self->triangle3D(objects[idx].p[i], 
                         objects[idx].p[j], 
                         objects[idx].p[k],idx);
        self->triangle3D(objects[idx].p[j], 
                         objects[idx].p[l],
                         objects[idx].p[k],idx);
        // self->fastPoint(p[i]);

        // int starbrightness = bsParamInt8t[i][0];
        // int noInt = bsParamInt16t[i][0];
      }
    }
    else if(objno==1) {

//       self->triangle3D(objects[0].p[0], objects[0].p[1], objects[0].p[2]);//上面
//       self->triangle3D(objects[0].p[0], objects[0].p[2], objects[0].p[3]);

//       self->triangle3D(objects[0].p[4], objects[0].p[6], objects[0].p[5]);//下面
//       self->triangle3D(objects[0].p[4], objects[0].p[7], objects[0].p[6]);

// //後２面
//       self->triangle3D(objects[0].p[0], objects[0].p[3], objects[0].p[7]);
//       self->triangle3D(objects[0].p[0], objects[0].p[7], objects[0].p[4]);

//       self->triangle3D(objects[0].p[1], objects[0].p[4], objects[0].p[5]);//右
//       self->triangle3D(objects[0].p[1], objects[0].p[0], objects[0].p[4]);

// //前２面
//       self->triangle3D(objects[0].p[3], objects[0].p[6], objects[0].p[7]);
//       self->triangle3D(objects[0].p[3], objects[0].p[2], objects[0].p[6]);

//       self->triangle3D(objects[0].p[2], objects[0].p[5], objects[0].p[6]);//右
//       self->triangle3D(objects[0].p[2], objects[0].p[1], objects[0].p[5]);

      // すべてのオブジェクトに対してループ
      
          self->triangle3D(objects[idx].p[0], objects[idx].p[1], objects[idx].p[2],idx);//上面
          self->triangle3D(objects[idx].p[0], objects[idx].p[2], objects[idx].p[3],idx);

          self->triangle3D(objects[idx].p[4], objects[idx].p[6], objects[idx].p[5],idx);//下面
          self->triangle3D(objects[idx].p[4], objects[idx].p[7], objects[idx].p[6],idx);

          // 後２面
          self->triangle3D(objects[idx].p[0], objects[idx].p[3], objects[idx].p[7],idx);
          self->triangle3D(objects[idx].p[0], objects[idx].p[7], objects[idx].p[4],idx);

          self->triangle3D(objects[idx].p[1], objects[idx].p[4], objects[idx].p[5],idx);//右
          self->triangle3D(objects[idx].p[1], objects[idx].p[0], objects[idx].p[4],idx);

          // 前２面
          self->triangle3D(objects[idx].p[3], objects[idx].p[6], objects[idx].p[7],idx);
          self->triangle3D(objects[idx].p[3], objects[idx].p[2], objects[idx].p[6],idx);

          self->triangle3D(objects[idx].p[2], objects[idx].p[5], objects[idx].p[6],idx);//右
          self->triangle3D(objects[idx].p[2], objects[idx].p[1], objects[idx].p[5],idx);
      }
    }
  return 0;
}


int runLuaGame::run(int _remainTime){

  if(exitRequest){//次のゲームを起動するフラグがたったら
    exitRequest = false;//フラグをリセットして、
    return 1; // exit(1をリターンすることで、main.cppの変数modeを１にする)
  }

  // // for(int i = 0; i < buttonState.size(); i ++){
  //   for(int i = 0; i < 7; i ++){
  //   if(pressedBtnID == -1){
  //     buttonState[i] = 0;
  //   }else if(pressedBtnID == i){//押されたものだけの値をあげる
  //     buttonState[i] ++;
  //   }
  // }
  
  // // for(int i = 0; i < buttonState.size(); i ++){
  for(int i = 0; i < 7; i ++){
    if(pressedBtnID == -1){
      buttonState[i] = 0;
    }else if(pressedBtnID == i){//押されたものだけの値をあげる
      buttonState[i] ++;
    }
  }

  if(runError){
      tft.setTextSize(1);
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
      tft.setCursor(0, 0);
      tft.setTextWrap(true);
      tft.print(errorString);
      tft.setTextWrap(false);
    }else{
        if (firstLoopF) {
          luaL_dostring(L, "_init()");
          firstLoopF = false;
        }
        luaL_dostring(L, "_update()");

        if(luaL_dostring(L, "_draw()")){
          lua_pop(L, 1);//最後に入れないとダメ
        }
    }

  //show FPS:ms
  // tft.setTextColor(TFT_WHITE, TFT_BLUE);
  // tft.setCursor(90, 127 - 16);
  // tft.print(String(1000/_remainTime) + "FPS");
  // tft.setCursor(90, 127 - 8);
  // tft.print(String(_remainTime) + "ms");

  return 0;
}



