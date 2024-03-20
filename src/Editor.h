#ifndef EDITOR_H
#define EDITOR_H

#include <Arduino.h>
#include "SPIFFS.h"

#include <FS.h>
#include <regex>
// #include <LovyanGFX.h>  // 必要なヘッダを追加
#include <M5Unified.h>
// #include "KbdRptParser.h"
#include <regex>
// #include <PS2Keyboard.h>

#define FORMAT_SPIFFS_IF_FAILED false
#define KILO_VERSION "0.0.1"
#define KILO_TAB_STOP 2
#define ABUF_INIT {NULL, 0}


// Every call to read() returns a single byte for each
// keystroke.  These configure what byte will be returned
// for each "special" key.  To ignore a key, use zero.
#define PS2_TAB				9
#define PS2_ENTER			13
#define PS2_BACKSPACE			127
#define PS2_ESC				27
#define PS2_INSERT			0
#define PS2_DELETE			127
#define PS2_HOME			36
#define PS2_END				35
#define PS2_PAGEUP			25
#define PS2_PAGEDOWN			26
#define PS2_UPARROW			11
#define PS2_LEFTARROW			8
#define PS2_DOWNARROW			10
#define PS2_RIGHTARROW			21
#define PS2_F1				0
#define PS2_F2				0
#define PS2_F3				0
#define PS2_F4				0
#define PS2_F5				0
#define PS2_F6				0
#define PS2_F7				0
#define PS2_F8				0
#define PS2_F9				0
#define PS2_F10				0
#define PS2_F11				0
#define PS2_F12				0
#define PS2_SCROLL			0
#define PS2_EURO_SIGN			0

#define PS2_INVERTED_EXCLAMATION	161 // ¡
#define PS2_CENT_SIGN			162 // ¢
#define PS2_POUND_SIGN			163 // £
#define PS2_CURRENCY_SIGN		164 // ¤
#define PS2_YEN_SIGN			165 // ¥
#define PS2_BROKEN_BAR                 	166 // ¦
#define PS2_SECTION_SIGN		167 // §
#define PS2_DIAERESIS			168 // ¨
#define PS2_COPYRIGHT_SIGN		169 // ©
#define PS2_FEMININE_ORDINAL		170 // ª
#define PS2_LEFT_DOUBLE_ANGLE_QUOTE	171 // «
#define PS2_NOT_SIGN			172 // ¬
#define PS2_HYPHEN			173
#define PS2_REGISTERED_SIGN		174 // ®
#define PS2_MACRON			175 // ¯
#define PS2_DEGREE_SIGN			176 // °
#define PS2_PLUS_MINUS_SIGN		177 // ±
#define PS2_SUPERSCRIPT_TWO		178 // ²
#define PS2_SUPERSCRIPT_THREE		179 // ³
#define PS2_ACUTE_ACCENT		180 // ´
#define PS2_MICRO_SIGN			181 // µ
#define PS2_PILCROW_SIGN		182 // ¶
#define PS2_MIDDLE_DOT			183 // ·
#define PS2_CEDILLA			184 // ¸
#define PS2_SUPERSCRIPT_ONE		185 // ¹
#define PS2_MASCULINE_ORDINAL		186 // º
#define PS2_RIGHT_DOUBLE_ANGLE_QUOTE	187 // »
#define PS2_FRACTION_ONE_QUARTER	188 // ¼
#define PS2_FRACTION_ONE_HALF		189 // ½
#define PS2_FRACTION_THREE_QUARTERS	190 // ¾
#define PS2_INVERTED_QUESTION_MARK	191 // ¿
#define PS2_A_GRAVE			192 // À
#define PS2_A_ACUTE			193 // Á
#define PS2_A_CIRCUMFLEX		194 // Â
#define PS2_A_TILDE			195 // Ã
#define PS2_A_DIAERESIS			196 // Ä
#define PS2_A_RING_ABOVE		197 // Å
#define PS2_AE				198 // Æ
#define PS2_C_CEDILLA			199 // Ç
#define PS2_E_GRAVE			200 // È
#define PS2_E_ACUTE			201 // É
#define PS2_E_CIRCUMFLEX		202 // Ê
#define PS2_E_DIAERESIS			203 // Ë
#define PS2_I_GRAVE			204 // Ì
#define PS2_I_ACUTE			205 // Í
#define PS2_I_CIRCUMFLEX		206 // Î
#define PS2_I_DIAERESIS			207 // Ï
#define PS2_ETH				208 // Ð
#define PS2_N_TILDE			209 // Ñ
#define PS2_O_GRAVE			210 // Ò
#define PS2_O_ACUTE			211 // Ó
#define PS2_O_CIRCUMFLEX		212 // Ô
#define PS2_O_TILDE			213 // Õ
#define PS2_O_DIAERESIS			214 // Ö
#define PS2_MULTIPLICATION		215 // ×
#define PS2_O_STROKE			216 // Ø
#define PS2_U_GRAVE			217 // Ù
#define PS2_U_ACUTE			218 // Ú
#define PS2_U_CIRCUMFLEX		219 // Û
#define PS2_U_DIAERESIS			220 // Ü
#define PS2_Y_ACUTE			221 // Ý
#define PS2_THORN			222 // Þ
#define PS2_SHARP_S			223 // ß
#define PS2_a_GRAVE			224 // à
#define PS2_a_ACUTE			225 // á
#define PS2_a_CIRCUMFLEX		226 // â
#define PS2_a_TILDE			227 // ã
#define PS2_a_DIAERESIS			228 // ä
#define PS2_a_RING_ABOVE		229 // å
#define PS2_ae				230 // æ
#define PS2_c_CEDILLA			231 // ç
#define PS2_e_GRAVE			232 // è
#define PS2_e_ACUTE			233 // é
#define PS2_e_CIRCUMFLEX		234 // ê
#define PS2_e_DIAERESIS			235 // ë
#define PS2_i_GRAVE			236 // ì
#define PS2_i_ACUTE			237 // í
#define PS2_i_CIRCUMFLEX		238 // î
#define PS2_i_DIAERESIS			239 // ï
#define PS2_eth				240 // ð
#define PS2_n_TILDE			241 // ñ
#define PS2_o_GRAVE			242 // ò
#define PS2_o_ACUTE			243 // ó
#define PS2_o_CIRCUMFLEX		244 // ô
#define PS2_o_TILDE			245 // õ
#define PS2_o_DIAERESIS			246 // ö
#define PS2_DIVISION			247 // ÷
#define PS2_o_STROKE			248 // ø
#define PS2_u_GRAVE			249 // ù
#define PS2_u_ACUTE			250 // ú
#define PS2_u_CIRCUMFLEX		251 // û
#define PS2_u_DIAERESIS			252 // ü
#define PS2_y_ACUTE			253 // ý
#define PS2_thorn			254 // þ
#define PS2_y_DIAERESIS			255 // ÿ


#define PS2_KEYMAP_SIZE 136

//メモリが足りない場合に、エディタの表示文字列を少なくする
// #define EDITOR_ROWS 11
// #define EDITOR_COLS 20
#define EDITOR_ROWS 16
#define EDITOR_COLS 26

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

class Editor {

  private:
  String statusMessage = "";

  bool shiftF = false;

  int keyMillis = 0;
  int keywaitTime = 200;

  struct abuf {
    char *b;
    int len;
  };

  typedef struct erow {
    int size;
    int rsize;
    char *chars;
    char *render;
  } erow;

  struct editorConfig {
    int cx, cy;
    int preCx, preCy;
    int rx;
    int preRx;
    int rowoff;
    int coloff;
    int screenrows;
    int screencols;
    int screenrow;
    int screencol;
    int dirty;
    int numrows;
    erow *row;
    char *filename;
    char statusmsg[80];
    time_t statusmsg_time;
  }; struct editorConfig E;


  public:
    Editor();
    void editorDrawMessageBar(M5Canvas& tft, struct abuf *ab);
    void editorDrawStatusBar(M5Canvas& tft, struct abuf *ab);
    void initEditor(M5Canvas& tft);
    void readFile(fs::FS &fs, const char * path);
    void editorOpen(fs::FS &fs, const char *filename);
    void editorUpdateRow(erow *row);
    void editorInsertRow(int at, const char *s, size_t len);
    void editorInsertRow2(int at, const char *s, size_t len);//改行付き
    void editorRefreshScreen(M5Canvas& tft);
    void editorScroll();
    int editorRowCxToRx(erow *row, int cx);
    void editorDrawRows(struct abuf *ab);
    void abAppend(struct abuf *ab, const char *s, int len);
    void abFree(struct abuf *ab);

    void editorProcessKeypress(int c, fs::FS &fs);
    void editorInsertChar(int c);
    void editorMoveCursor(int c);
    void editorRowInsertChar(erow *row, int at, int c);
    void editorSetStatusMessage(const char *fmt, ...);

    void editorInsertNewline();
    void editorDelChar();
    void editorDelChar3() ;
    void editorRowDelChar(erow *row, int at);
    void editorRowAppendString(erow *row, char *s, size_t len);
    void editorDelRow(int at);
    void editorFreeRow(erow *row);
    void editorSave(fs::FS &fs);
    // void editorSaveSD(fs::FS &fs);

    char *editorRowsToString(int *buflen);
    int getTargetTime();
    void setTargetTime(int _targetTime);

    int getNumRows();
    int getCy();
    int getCx();
    int getRx();
    int getPreCy();
    int getPreCx();
    int getPreRx();

    int getScreenCol();
    int getScreenRow();

    bool getShiftF();
    // void editorPageMove(int c, KbdRptParser &Prs);
    void editorPageMove(char c);

    int utf8_char_width(char c);
    // void update(M5Canvas& tft,fs::FS &fs, fs::FS &SD, KbdRptParser &Prs);
    // void update(M5Canvas& tft, fs::FS &fs, fs::FS &SD, char _keychar);
    void update(M5Canvas& tft, fs::FS &fs, int _keychar);
    void writeFile(fs::FS &fs, const char * path, const char * message);
    void appendFile(fs::FS &fs, const char * path, const char * message);

    void getCursorConfig(String _wrfile);
    void setCursorConfig();
    void setCursorConfig(int _cx, int _cy, int _rx);

    void setCursor(int _cx,int _cy,int _rx);
};

#endif
