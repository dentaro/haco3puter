
#include "Editor.h"

// extern PS2Keyboard keyboard;
extern char keychar;
extern bool firstBootF;
extern uint8_t isEditMode;

extern String appfileName;//実行されるアプリ名
extern String savedAppfileName;//ファイルに書き込みした
extern bool difffileF;//前と違うファイルを開こうとしたときに立つフラグ

Editor::Editor() {
    // コンストラクタの実装
    // ...
}

void Editor::abAppend(struct abuf *ab, const char *s, int len) {
  char *newb = (char *)realloc(ab->b, ab->len + len);
  if (newb == NULL) return;
  memcpy(&newb[ab->len], s, len);
  ab->b = newb;
  ab->len += len;
}

// // カーソル位置(cx)を行の表示位置(rx)に変換する関数
int Editor::editorRowCxToRx(erow *row, int cx) {
  int rx = 0; // カーソル位置(cx)を示す変数(rx)を初期化

  // カーソル位置(cx)までの文字数を計算
  int j = 0;

  while (j <= cx) {
    if ((row->chars[j] & 0xC0) != 0x80) {
      j++;
      rx++;
    }else{//2バイト以上のもじ
      j+=3;
      rx+=2;
    }
  }
  return rx-1; // 行の表示位置(rx)を返す
}

void Editor::editorScroll() {
  if(firstBootF){
    E.rx = 0; // カーソル位置(rx)を初期化
  }

  // カーソル位置(cx)を行の表示位置(rx)に変換し、E.rxに格納
  if (E.cy < E.numrows) {
    E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
  }

  E. screenrow = getRx() - E.coloff; // カーソルの表示位置をスクリーン座標に変換
  E. screencol = getCy() - E.rowoff; // カーソルの表示位置をスクリーン座標に変換

  if (E. screenrow < 0) {
    E.coloff += E. screenrow; // カーソルが画面外に出た場合、coloffを調整してスクロール
  } else if (E. screenrow >= E.screencols) {
    E.coloff += E. screenrow - E.screencols + 1; // カーソルが画面外に出た場合、coloffを調整してスクロール
  }

  if (E. screencol < 0) {
    E.rowoff += E. screencol; // カーソルが画面外に出た場合、rowoffを調整してスクロール
  } else if (E. screencol >= E.screenrows) {
    E.rowoff += E. screencol - E.screenrows + 1; // カーソルが画面外に出た場合、rowoffを調整してスクロール
  }
}

int countThreeByteCharacters(const char *str) {
    int count = 0;
    while (*str != '\0') {
        if ((*str & 0xE0) == 0xE0) { // 3バイト文字の先頭バイトかどうかを判定
            count++;
            str += 2; // 3バイト文字なので2バイト進める
        } else {
            str++;
        }
    }
    return count;
}

void Editor::editorDrawRows(struct abuf *ab) {
  int y;
  for (y = 0; y < E.screenrows; y++) {
    int filerow = y + E.rowoff;
      if (filerow >= E.numrows) {
        if (E.numrows == 0 && y == E.screenrows / 3) {
          char welcome[80];
          int welcomelen = snprintf(welcome, sizeof(welcome),
            "Kilo editor -- version %s", KILO_VERSION);
          if (welcomelen > E.screencols) welcomelen = E.screencols;
          int padding = (E.screencols - welcomelen) / 2;
          if (padding) {
            abAppend(ab, "~", 1);
            padding--;
          }
          while (padding--) abAppend(ab, " ", 1);
          abAppend(ab, welcome, welcomelen);
        } else {
          abAppend(ab, "~", 1);
        }
      }  else {


    // 行の表示位置を計算
    int len = E.row[filerow].rsize - E.coloff;
    if (len < 0) len = 0;

    if (len >= E.screencols + countThreeByteCharacters(&E.row[filerow].render[E.coloff])) len = E.screencols - 1;

    // バッファに追加
    abAppend(ab, &E.row[filerow].render[E.coloff], len);
      }

    // 新しい行が挿入された場合、それ以降の行も再描画
    if (y < E.screenrows - 1 && filerow < E.numrows - 1) {
      abAppend(ab, "\r\n", 2);
    }

  }

  // 最終行の終端子を追加
  abAppend(ab, "\n", 1);
}


void Editor::abFree(struct abuf *ab) {
  free(ab->b);
}

void Editor::editorMoveCursor(int c) {
  erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  switch (c) {
    case PS2_LEFTARROW:
      if (E.cx != 0) {
        // カーソルが2バイト目または3バイト目にある場合、1バイト分戻る
        if (E.cx >= 3 && (row->chars[E.cx - 1] & 0xC0) == 0x80) {
          E.cx -= 3;
        } else {
          E.cx--;
        }
      } else if (E.cy > 0) {
        E.cy--;
        E.cx = E.row[E.cy].size;
      }
      break;
  case PS2_RIGHTARROW:
    if (row && E.cx < row->size) {
        if (E.cx <= row->size - 2 && (row->chars[E.cx + 1] & 0xC0) == 0x80) {
          E.cx += 3; // 3バイト文字の場合は3つ進む
        } else {
          E.cx++;
        }
      } else if (row && E.cx == row->size) {
      E.cy++;
      E.cx = 0;
    }
    break;
    case PS2_UPARROW:
      if (E.cy != 0) {
        E.cy--;
      }
      break;
    case PS2_DOWNARROW:
      if (E.cy < E.numrows) {
        E.cy++;
      }
      break;
  }
  
  row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  int rowlen = row ? row->size : 0;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }
}


// 既存の文字列を解放してから新しいメモリを確保する
// void Editor::editorRowInsertChar(erow *row, int at, int c) {
//   if (at < 0 || at > row->size) at = row->size;

//   // 既存のメモリを解放
//   free(row->chars);

//   // 新しいメモリを確保
//   row->chars = (char *)malloc(row->size + 2);

//   // メモリの確保が成功したかを確認
//   if (row->chars == NULL) {
//     // メモリの確保に失敗
//     exit(1); // ここでプログラムを終了させるか、適切なエラーハンドリングを行う
//   }

//   memmove(&row->chars[at + 1], &row->chars[at], row->size - at);
//   row->size++;
//   row->chars[at] = c;
//   row->chars[row->size] = '\0';
//   editorUpdateRow(row);
//   E.dirty++;
// }


void Editor::editorRowInsertChar(erow *row, int at, int c) {
  if (at < 0 || at > row->size) {
    at = row->size; // at が有効な範囲外の場合、末尾に挿入
  }

  if ((c & 0xC0) == 0x80) { // 3バイト文字の場合
    row->chars = (char *)realloc(row->chars, row->size + 3); // 3バイト追加
    memmove(&row->chars[at + 3], &row->chars[at], row->size - at + 1); // 3バイト分シフト
    row->chars[at] = 0xE0 | ((c >> 12) & 0x0F); // 3バイト目
    row->chars[at + 1] = 0x80 | ((c >> 6) & 0x3F); // 2バイト目
    row->chars[at + 2] = 0x80 | (c & 0x3F); // 1バイト目
    row->size += 3;
  } else { // 1バイト文字の場合
    row->chars = (char *)realloc(row->chars, row->size + 2); // 2バイト追加
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1); // 2バイト分シフト
    row->chars[at] = c; // 挿入
    row->size += 1;
  }

  editorUpdateRow(row);
  E.dirty++;
}

// void Editor::editorRowInsertChar(erow *row, int at, int c) {
//   if (at < 0 || at > row->size) at = row->size;
//   row->chars = (char *)realloc(row->chars, row->size + 2);
//   memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
//   row->size++;
//   row->chars[at] = c;
//   editorUpdateRow(row);
//   E.dirty++;
// }

//改行付きで送られてくるもの（空白行など含む生データ）
void Editor::editorInsertRow2(int at, const char *s, size_t len) {
  if (at < 0 || at > E.numrows) return;
  E.row = (erow *)realloc(E.row, sizeof(erow) * (E.numrows + 1));
  memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numrows - at));
  E.row[at].size = len;
  E.row[at].chars = (char *)malloc(len + 1);
  memcpy(E.row[at].chars, s, len);
  E.row[at].chars[len] = '\0';
  E.row[at].rsize = 0;
  E.row[at].render = NULL;
  editorUpdateRow(&E.row[at]);
  E.numrows++;
  E.dirty++;
}


void Editor::editorInsertRow(int at, const char *s, size_t len) {
  if (at < 0 || at > E.numrows) return;  // 挿入位置が範囲外なら何もせずに関数を抜ける

  E.row = (erow *)realloc(E.row, sizeof(erow) * (E.numrows + 1));  // 行データのメモリ領域を拡張

  for (int i = E.numrows; i > at; i--) {
    E.row[i] = E.row[i - 1];  // 指定位置以降の行を1つずつ後ろにずらす
  }

  E.row[at].size = len;  // 新しい行のサイズを設定
  // E.row[at].size = len * 3;  // 新しい行のサイズを設定

  E.row[at].chars = new char[len + 1];  // 新しい行の文字列を格納するメモリを確保

  for (size_t i = 0; i < len; i++) {
    E.row[at].chars[i] = s[i];  // 渡された文字列を新しい行にコピー
  }

  E.row[at].chars[len] = '\0';  // 文字列の終端を設定

  E.row[at].rsize = 0;  // 行の描画サイズをリセット
  E.row[at].render = NULL;  // レンダリングデータをNULLに設定
  editorUpdateRow(&E.row[at]);  // 行データを更新

  E.numrows++;  // 行数を増やす
  E.dirty++;  // 変更フラグを立てる
}

// void Editor::editorInsertChar(int c) {
//   if (E.cy == E.numrows) {
//     editorInsertRow(E.numrows, "", 0);
//   }
//   editorRowInsertChar(&E.row[E.cy], E.cx, c);
//   E.cx++;
// }
void Editor::editorInsertChar(int c) {
  if (E.cy == E.numrows) {
    editorInsertRow(E.numrows, "", 0);
  }
  editorRowInsertChar(&E.row[E.cy], E.cx, c);
  E.cx++;
}

// カーソル位置が行の先頭の場合、新しい行を挿入
// それ以外の場合、現在の行を分割し、新しい行を挿入
// void Editor::editorInsertNewline() {
//   if (E.cx == 0) { // カーソルが行の先頭にある場合
//     editorInsertRow(E.cy, "", 0); // 空行を挿入
//   } else { // カーソルが行の途中にある場合
//     erow *row = &E.row[E.cy];
//     editorInsertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx); // 現在の行を分割し、新しい行を挿入
//     row = &E.row[E.cy];
//     row->size = E.cx; // 分割された行のサイズを更新
//     row->chars[row->size] = '\0'; // 行の終端に終端文字を追加
//     editorUpdateRow(row); // 行の更新を反映
//   }
//   E.cy++; // カーソルを次の行に移動
//   E.cx = 0; // カーソルを行の先頭に移動
// }

void Editor::editorInsertNewline() {
  if (E.cx == 0) { // カーソルが行の先頭にある場合
    editorInsertRow(E.cy, "", 0); // 空行を挿入
  } else { // カーソルが行の途中にある場合
    erow *row = &E.row[E.cy];
    // 新しい行に挿入する文字列のバイト数を計算
    size_t split_size = row->size - E.cx;
    editorInsertRow(E.cy + 1, &row->chars[E.cx], split_size);
    row = &E.row[E.cy];
    // 分割された行のサイズを更新
    row->size = E.cx;
    row->chars[row->size] = '\0';
    editorUpdateRow(row); // 行の更新を反映
  }
  E.cy++; // カーソルを次の行に移動
  E.cx = 0; // カーソルを行の先頭に移動
}

void Editor::editorRowDelChar(erow *row, int at) {
  if (at < 0 || at >= row->size) return;

  if ((row->chars[at] & 0xC0) == 0x80) {
    // 削除位置が3バイト文字の途中なら、3バイト目を削除
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    row->chars[row->size] = '\0';
  } else if (at > 0 && (row->chars[at - 1] & 0xC0) == 0xC0) {
    // 削除位置が2バイト文字の途中なら、2バイト目を削除
    memmove(&row->chars[at - 1], &row->chars[at], row->size - at);
    row->size--;
    row->chars[row->size] = '\0';
  } else {
    // 1バイト文字の場合
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    row->chars[row->size] = '\0';
  }

  editorUpdateRow(row);
  E.dirty++;
}

// void Editor::editorRowDelChar(erow *row, int at) {
//   if (at < 0 || at >= row->size) return;
//   memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
//   row->size--;
//   editorUpdateRow(row);
//   E.dirty++;
// }

void Editor::editorRowAppendString(erow *row, char *s, size_t len) {
  row->chars = (char *)realloc(row->chars, row->size + len + 1);
  memcpy(&row->chars[row->size], s, len);
  row->size += len;
  row->chars[row->size] = '\0';
  editorUpdateRow(row);
  E.dirty++;
}

void Editor::editorFreeRow(erow *row) {
  free(row->render);
  free(row->chars);
}

// void Editor::editorDelRow(int at) {
//   if (at < 0 || at >= E.numrows) return;
//   editorFreeRow(&E.row[at]);
//   memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));
//   E.numrows--;
//   E.dirty++;
// }
void Editor::editorDelRow(int at) {
  if (at < 0 || at >= E.numrows) return;
  editorFreeRow(&E.row[at]);  // 行を削除する前にメモリを解放
  memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));
  E.numrows--;
  E.dirty++;
}



void Editor::editorDelChar() {
  if (E.cy == E.numrows) return;
  if (E.cx == 0 && E.cy == 0) return;
  erow *row = &E.row[E.cy];
  if (E.cx > 0) {
    editorRowDelChar(row, E.cx - 1);
    E.cx--;
  } else {
    E.cx = E.row[E.cy - 1].size;
    editorRowAppendString(&E.row[E.cy - 1], row->chars, row->size);
    editorDelRow(E.cy);
    E.cy--;
  }
}

void Editor::editorDelChar3() {
  // if (E.cy == E.numrows) return;
  // if (E.cx == 0 && E.cy == 0) return;

  // erow *row = &E.row[E.cy];
  // if (E.cx > 0) {


  //     int at = E.cx - 3;
  //     if (at < 0 || at >= row->size) return;
  //     memmove(&row->chars[at], &row->chars[at + 3], row->size - at);
  //     row->size-=3;
  //     editorUpdateRow(row);
  //     E.dirty+=3;

  //     E.cx-=3;


  // } else {
  //   E.cx = E.row[E.cy - 1].size;
  //   editorRowAppendString(&E.row[E.cy - 1], row->chars, row->size);
  //   editorDelRow(E.cy);
  //   E.cy--;
  // }
}

char *Editor::editorRowsToString(int *buflen) {
  int totlen = 0;
  int j;
  for (j = 0; j < E.numrows; j++)
    totlen += E.row[j].size + 1;
  *buflen = totlen;
  char *buf = (char *)malloc(totlen);
  char *p = buf;
  for (j = 0; j < E.numrows; j++) {
    memcpy(p, E.row[j].chars, E.row[j].size);
    p += E.row[j].size;
    *p = '\r';
    p++;
  } *--p = '\0';
  return buf;
}


//----------------------------------------------------
bool Editor::getShiftF(){
  return shiftF;
}

void Editor::editorPageMove(char c) {
  if (c == PS2_PAGEUP&&getShiftF()) {
          E.cy = E.rowoff;
        } else if (c == PS2_PAGEDOWN&&getShiftF()) {
          E.cy = E.rowoff + E.screenrows - 1;
          if (E.cy > E.numrows) E.cy = E.numrows;
        }
        int times = E.screenrows;
        while (times--)
          editorMoveCursor(c == PS2_PAGEUP ? PS2_UPARROW : PS2_DOWNARROW);
}

// void Editor::editorProcessKeypress(int c, fs::FS &fs, fs::FS &SD) {
void Editor::editorProcessKeypress(int c, fs::FS &fs) {
  if(isEditMode){
    // Serial.println(c);
    
  // c = keychar;
  if (!c) return;

  switch (c) {
    case PS2_ENTER:
      editorInsertNewline();
      // editorSave(fs);
      break;
    // case PS2_HOME:
    //   E.cx = 0;
    //   break;
    // case PS2_END:
    //   if (E.cy < E.numrows)
    //     E.cx = E.row[E.cy].size;
    //   break;
    
    case PS2_DELETE:
      if((E.row[E.cy].chars[E.cx - 1] & 0xC0) == 0x80){
        editorDelChar();
        editorDelChar();
        editorDelChar();
      }else{
        editorDelChar();
      }
      break;
    // case PS2_BACKSPACE:
    //   if((E.row[E.cy].chars[E.cx - 1] & 0xC0) == 0x80){
    //     editorDelChar();
    //     editorDelChar();
    //     editorDelChar();
    //   }else{
    //     editorDelChar();
    //   }
    //   break;
    case PS2_ESC:
      editorSave(fs);
      delay(200);//ちょっと待つ
      // editorSaveSD(SD);//SDに同時にバックアップ保存する
      // delay(200);//ちょっと待つ
      break;  
    
    case PS2_PAGEUP:
    case PS2_PAGEDOWN:
      {
        // editorPageMove(c, Prs);
      //   if (c == PS2_PAGEUP) {
      //     E.cy = E.rowoff;
      //   } else if (c == PS2_PAGEDOWN) {
      //     E.cy = E.rowoff + E.screenrows - 1;
      //     if (E.cy > E.numrows) E.cy = E.numrows;
      //   }
      //   int times = E.screenrows;
      //   while (times--)
      //     editorMoveCursor(c == PS2_PAGEUP ? ARROW_UP : ARROW_DOWN);
      }
      break;
    case PS2_UPARROW:
      editorMoveCursor(c);
    case PS2_DOWNARROW:
      editorMoveCursor(c);
    case PS2_LEFTARROW:
      editorMoveCursor(c);
    case PS2_RIGHTARROW:
      editorMoveCursor(c);
    
      break;
    default:
      editorInsertChar(c);
      editorSetStatusMessage("");
      break;
  }
  }
}

void Editor::editorSetStatusMessage(const char *fmt, ...) {
  // va_list ap;
  // va_start(ap, fmt);
  // vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
  statusMessage = String(E.statusmsg);
  // va_end(ap);
  E.statusmsg_time = time(NULL);
}

void Editor::editorDrawStatusBar(LGFX_Sprite& tft, struct abuf *ab) {

  // tft.setTextSize(1);
  // tft.setFont(&lgfxJapanGothicP_8);
  // tft.setFont(&fonts::Font0);//6*8
  tft.setCursor(0, 115);
  tft.setTextColor(HACO3_C6);
  tft.print(String(E.filename));
  tft.println(" row "+ String(getCy())+"/"+String(E.numrows)+ " col " +String(getRx()));
  // E.preCy = getCy();
  // E.preCx = getCx();
  // E.preRx = getRx();

  // tft.println("srow["+ String(E.screencol)+ "] row[" +String(getCy())+"]");

  // tft.print("cx["+ String(E.cx)+ "] rx[" +String(E.rx)+"]");
  tft.print(statusMessage);


  // int codeunit = int(160/E.numrows);
  // int codelen = int(1600/E.numrows);
  
  // int curpos = int(codeunit*getCy());
  // int codepos = int(codeunit*(getCy() -E.screencol));
  
  // tft.fillRect(252,0,4,160,HACO3_C5);//コードの全体の長さを表示
  // tft.fillRect(252,codepos,4,codelen,HACO3_C6);//コードの位置と範囲を表示
  // tft.fillRect(251,curpos,2,codeunit,HACO3_C8);//コードの位置と範囲を表示
  
}

int Editor::getNumRows(){
  return E.numrows;
}

int Editor::getCy(){
  return E.cy;
}
int Editor::getCx(){
  return E.cx;
}

// int Editor::getPreCy(){
//   return E.preCy;
// }
// int Editor::getPreCx(){
//   return E.preCx;
// }

int Editor::getRx(){
  return E.rx;
}

// int Editor::getPreRx(){
//   return E.preRx;
// }

int Editor::getScreenCol(){
  return E.screenrow;
}
int Editor::getScreenRow(){
  return E.screencol;
}

void Editor::editorDrawMessageBar(LGFX_Sprite& tft, struct abuf *ab) {
  tft.setTextColor(HACO3_C4);
  tft.print(E.statusmsg);
  tft.print(E.statusmsg_time);
  // int msglen = strlen(E.statusmsg);
  // if (msglen > E.screencols) msglen = E.screencols;
  // if (msglen && time(NULL) - E.statusmsg_time < 5)
  //   abAppend(ab, E.statusmsg, msglen);
}

// void Editor::editorRefreshScreen(LGFX_Sprite& tft) {
 
//   struct abuf ab = ABUF_INIT;
//   // screen.fillScreen(TFT_DARKGREEN);
//   tft.fillScreen(HACO3_C1);
//   tft.setTextSize(1);//サイズ
//   tft.setFont(&lgfxJapanGothic_12);//日本語可,等幅
//   tft.setCursor(0, 1);//位置
//   tft.setTextWrap(true);
//   tft.setTextColor( TFT_WHITE);
//   tft.fillScreen(HACO3_C1);//コンソール画面をリフレッシュ
//   editorScroll();
//   editorDrawRows(&ab);
//   //終端子をダミー文字ごと消す処理
//   if (ab.len > 0) {
//     ab.len--; // バッファの長さを1つ減らす
//     ab.b[ab.len] = '\0'; // 新しい終端文字を追加
//   }
//   tft.fillRect(getScreenCol() * 6, getScreenRow() * 13, 6, 13, HACO3_C8); // カーソルを表示

//   tft.setCursor(0, 0);
//   tft.setTextColor(HACO3_C7);
//   tft.println(ab.b);
//   editorDrawStatusBar(tft, &ab);

//   // editorDrawMessageBar(tft,&ab);
//   abFree(&ab);

// }

void Editor::editorRefreshScreen(LGFX_Sprite& tft) {
    struct abuf ab = ABUF_INIT;
    
    tft.fillScreen(HACO3_C0);
    for(int j = 0;j<7;j++){
      tft.fillRect( 0, j*16, 160, 8, HACO3_C1);
      }
    // tft.setTextSize(1);
    // tft.setFont(&lgfxJapanGothic_8);
    tft.setCursor(0, 1);
    tft.setTextWrap(true);
    tft.setTextColor(HACO3_C7);
    editorScroll();
    editorDrawRows(&ab);

    // 終端子をダミー文字ごと消す処理
    if (ab.len > 0) {
        ab.len--;
        ab.b[ab.len] = '\0';
    }

    // カーソルを表示
    // tft.fillRect(getScreenCol() * 6, getScreenRow() * 13, 6, 13, HACO3_C8);//12px
    tft.fillRect(getScreenCol() * 6, getScreenRow() * 8, 6, 8, HACO3_C8);//6-8px

    // スクリーンのリフレッシュ
    tft.setCursor(0, 0);
    tft.setTextColor(HACO3_C7);
    tft.println(ab.b);
    editorDrawStatusBar(tft, &ab);

    // editorDrawMessageBar(tft,&ab);
    abFree(&ab);
}


void Editor::editorUpdateRow(erow *row) {
  int tabs = 0; // タブ文字の数をカウントする変数
  int j;
  for (j = 0; j < row->size; j++) {
    if (row->chars[j] == '\t') tabs++; // タブ文字を発見したらカウントを増やす
  }
  
  free(row->render); // 以前の描画データを解放
  row->render = (char *)malloc(row->size + tabs*(KILO_TAB_STOP - 1) + 1); // 描画データのメモリを確保
  
  
  int idx = 0; // 描画データのインデックス
  
  for (j = 0; j < row->size; j++) {
    if (row->chars[j] == '\t') {
      row->render[idx++] = ' '; // タブ文字をスペースに置換
      
      while (idx % KILO_TAB_STOP != 0) row->render[idx++] = ' '; // タブ幅分のスペースを追加
    } else {
      row->render[idx++] = row->chars[j]; // 通常の文字はそのままコピー
    }
  }
  
  row->render[idx] = '\0'; // 描画データの終端を設定
  row->rsize = idx; // 描画データのサイズを更新
}

void Editor::getCursorConfig(String _wrfile) {
  File fr = SPIFFS.open(_wrfile, "r");
  String line;
  while (fr.available()) {
    line = fr.readStringUntil('\n');
    if (!line.isEmpty()) {

        int commaIndex = line.indexOf(',');
        String val = line.substring(0, commaIndex);

        if(val.toInt() != NULL)
        {
          E.cx =  val.toInt();
        }else {
          E.cx =  0;//configファイルが壊れていても強制的に値を入れて立ち上げる
        }

        int nextCommaIndex = line.indexOf(',', commaIndex + 1);//一つ先のカンマ区切りの値に進める
        if (nextCommaIndex != -1) {
          val = line.substring(commaIndex + 1, nextCommaIndex);
          if(val.toInt() != NULL){
            E.cy =  val.toInt();
          }else{
            E.cy = 0;//configファイルが壊れていても強制的に値を入れて立ち上げる
          }
        }

        int nextCommaIndex2 = line.indexOf(',', nextCommaIndex + 1);//一つ先のカンマ区切りの値に進める
        if (nextCommaIndex2 != -1) {
          val = line.substring(nextCommaIndex + 1, nextCommaIndex2);
          if(val.toInt() != NULL){
            E.rx =  val.toInt();
          }else{
            E.rx = 0;//configファイルが壊れていても強制的に値を入れて立ち上げる
          }
        }
  Serial.print("GET");
  Serial.print(E.cx);
  Serial.print(":");
  Serial.print(E.cy);
  Serial.print(":");
  Serial.println(E.rx);
    }
  }
  fr.close();
}

//強制的にカーソル位置をセットする関数
void Editor::setCursorConfig(int _cx, int _cy, int _rx) {
  E.cx = _cx;
  E.cy = _cy;
  E.rx = _rx;
  char numStr[100];//9999,999,9999,まで対応
  sprintf(numStr, "%d,%d,%d,", 
    E.cx, E.cy, E.rx
  );

  Serial.print("SET");
  Serial.print(E.cx);
  Serial.print(":");
  Serial.print(E.cy);
  Serial.print(":");
  Serial.println(E.rx);
  
  String writeStr = numStr;  // 書き込み文字列を設定
  File fw = SPIFFS.open("/init/param/editor.txt", "w"); // ファイルを書き込みモードで開く
  fw.println(writeStr);  // ファイルに書き込み
  delay(50);
  fw.close(); // ファイルを閉じる
}

//現在のカーソルの位置をセットする関数
void Editor::setCursorConfig() {
  char numStr[100];//9999,999,9999,まで対応
  sprintf(numStr, "%d,%d,%d,", 
    E.cx, E.cy, E.rx
  );

  Serial.print("SET");
  Serial.print(E.cx);
  Serial.print(":");
  Serial.print(E.cy);
  Serial.print(":");
  Serial.println(E.rx);
  
  String writeStr = numStr;  // 書き込み文字列を設定
  File fw = SPIFFS.open("/init/param/editor.txt", "w"); // ファイルを書き込みモードで開く
  fw.println(writeStr);  // ファイルに書き込み
  delay(50);
  fw.close(); // ファイルを閉じる
}

void Editor::setCursor(int _cx,int _cy,int _rx) {
    E.cx = _cx;
    E.cy = _cy;
    E.rx = _rx;
    // setCursorConfig();//外部ファイルに保存
}

void Editor::initEditor(LGFX_Sprite& tft) {
  
  if(!difffileF){
    getCursorConfig("/init/param/editor.txt");//カーソル位置を取得
    delay(50);
  }else{
    E.cx = 0;
    E.cy = 0;
    E.rx = 0;
  }

  E.rowoff = 0;
  E.coloff = 0;
  E.screenrows = EDITOR_ROWS;//11;
  E.screencols = EDITOR_COLS;//26;
  E.numrows = 0;
  E.row = NULL;
  E.dirty = 0;
  E.filename = NULL;
  E.statusmsg[0] = '\0';
  E.statusmsg_time = 0;
  E.screenrows -= 2;

  // screen.fillScreen(TFT_DARKGREEN);
  tft.fillScreen(HACO3_C1);
  // tft.setTextSize(1);//サイズ
  // tft.setFont(&lgfxJapanGothic_12);//日本語可,等幅
  // tft.setFont(&lgfxJapanGothic_8);//日本語可,等幅
  // tft.setTextSize(1, 1);
  tft.setFont(&fonts::Font0);//6*8
  tft.setCursor(0,1);//位置
  tft.setTextWrap(true);
  // screen.setTextColor(0x00FFFFU, 0x000033U);
  tft.setTextColor( TFT_WHITE);
  // tft.setTextColor( TFT_WHITE , TFT_BLACK );
}

void Editor::readFile(fs::FS &fs, const char * path) {
   File file = fs.open(path);
   while(file.available()) file.read();
}


void Editor::editorOpen(fs::FS &fs, const char *filename) {
  E.filename = strdup(filename);
  if (fs.exists(filename)) {
    File file = fs.open(filename, FILE_READ);
    if (!file) {
      Serial.println("Failed to open file for reading");
      return;
    }

    String line;
    bool wasLastCharNewline = true; // 最初は改行として扱う

    while (file.available()) {
      char c = file.read();
      if (c == '\n' || c == '\r') {
        if (!wasLastCharNewline) {
          editorInsertRow2(E.numrows, line.c_str(), line.length());
          wasLastCharNewline = true;
        }
        line = "";
      } else {
        line += c;
        wasLastCharNewline = false;
      }
    }

    if (line.length() > 0) {
      editorInsertRow2(E.numrows, line.c_str(), line.length());
    }

    file.close();
  }

}


void Editor::editorSave(fs::FS &fs) {
  if (E.filename == NULL) return;
  File fp = fs.open(E.filename, FILE_WRITE);
  
  if (!fp) {
    editorSetStatusMessage("Failed to open file");
    return;
  }

  for (int i = 0; i < E.numrows; i++) {
    // 行末の改行を削除してから書き出す
    int len = strlen(E.row[i].chars);
    if (len > 0 && E.row[i].chars[len - 1] == '\n') {
      E.row[i].chars[len - 1] = '\0';
    }

    // ファイルに書き出す際にプラットフォームに合わせた改行を使用
    // 例: Windowsの場合はCRLF、他の場合はLF
    #ifdef _WIN32
      fp.print(E.row[i].chars); // WindowsではCRLFを使う
    #else
      fp.println(E.row[i].chars); // 他のプラットフォームではLFを使う
    #endif

    if (fp) {
      editorSetStatusMessage("File written");
    } else {
      editorSetStatusMessage("Failed to write file!");
      fp.close();
      return;
    }
  }

  E.dirty = 0;
  fp.close();
}

void Editor::writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

// void Editor::editorSaveSD(fs::FS &fs) {//SDに保存する
//   if (E.filename == NULL) return;
//   // ファイルパスをコピーして変更可能にする
//   char filePath[strlen(E.filename) + 1];
//   strcpy(filePath, E.filename);

//   // ファイルパスをスラッシュで分割する
//   char *folderName = strtok(filePath, "/");
//   char *fileName = strtok(NULL, "/");

//   // フォルダを作成
//   String folderPath = "/" + String(folderName);
//   if (!fs.exists(folderPath)) {
//     fs.mkdir(folderPath);
//   }

//   // ファイルパスを結合
//   String filePathWithFolder = folderPath + "/" + String(fileName);

//   // 編集中のデータを文字列に変換
//   int len;
//   char *buf = editorRowsToString(&len);

//   // ファイルを保存
//   writeFile(fs, filePathWithFolder.c_str(), buf);

//   free(buf);
// }

// int Editor::getTargetTime() {
//   return targetTime;
// }

// void Editor::setTargetTime(int _targetTime) {
//     targetTime = _targetTime;
// }
// int getKeyDownMillis(){
//   if (keyboard.available()){
//     return millis();
//   }
// }

// void Editor::update(LGFX_Sprite& tft, fs::FS &fs, fs::FS &SD, char _keychar) { 

//     editorProcessKeypress(_keychar, fs, SD);
//     editorRefreshScreen(tft);

// }

void Editor::update(LGFX_Sprite& tft, fs::FS &fs, int _keychar) { 

    editorProcessKeypress(_keychar, fs);
    // editorRefreshScreen(tft);

}

