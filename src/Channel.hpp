#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Speaker_Class.hpp"
// #include "CRingBuffur.hpp"
// #include "M5Unified.h"
// #include "M5Cardputer.h"

#define numPitches 12  // ドレミファソラシド（12音）// + レ（13音）
#define numOctaves 16

#define PATTERN_NUM 64//64パタン用意するとメモリが足らない
#define TONE_NUM 32
#define CHANNEL_NUM 8//4チャネルにするとうまくいかない


// 新しい構造体
struct ChannelData {
    bool onoffF;
    size_t loopStart;
    size_t loopEnd;
    size_t hz;
    size_t pitch;
    size_t octave;
    size_t sfxno;
    size_t instrument;
    size_t volume;
    size_t effectNo;
    size_t tickNo;
    size_t looplen;
};

// class Channel : public m5::Speaker_Class {//M5のスピーカークラス
class Channel : public Speaker_Class {//独自のスピーカークラス
public:
  ChannelData notedata[CHANNEL_NUM][TONE_NUM*2];  // 構造体をメンバ変数として持つ（バックバッファ2パタン8チャンネル分しかデータは持たない）
  size_t patterns[PATTERN_NUM][CHANNEL_NUM];//３２パターン（小節）8チャンネルにSfxNo0~63を３２ノート割り付けるための配列
  // size_t speakerEffectNo = 0;

  // 二次元浮動小数点数配列の宣言
  double hzList[numPitches][numOctaves];

  // CRingBuffur ringbuf;


  // コンストラクタ
  Channel();
  // デストラクタ
  virtual ~Channel();
  void setTones(size_t onoffF, size_t loopStart, size_t loopEnd, size_t instrument, size_t pitch, size_t octave, size_t sfxno, size_t volume, size_t effectNo, size_t tickNo, size_t _chno, size_t buffAreaNo);
  void setPatterns(size_t _patternNo, size_t _ch0, size_t _ch1, size_t _ch2, size_t _ch3, size_t _ch4, size_t _ch5, size_t _ch6, size_t _ch7);
  void setPatterns(size_t _patternNo, size_t _ch, size_t _patternID);
  size_t getPatternID(size_t _patternNo, size_t _chno);
  bool note(size_t channelno, size_t tick, size_t _patternNo);
  size_t gettick();
  void resetTones(size_t tickNo, size_t _sfxno, size_t _pitch, size_t _octave, size_t _volume, size_t _instrument, size_t _effectNo, size_t _buffAreaNo);

    // size_t* getSoundBuffer() const {
    //     return *sound_buf32;
    // }
  // size_t* getbuf(){ return sound_buf32_b; }//2147483647

  // protected:
  // uint8_t _tone_wav[8][16] = {0};
  // size_t speakerEffectNo = 0;

  protected:

//   size_t Speaker_Class::speakerEffectNo = 0;//実体を定義
// const uint8_t Speaker_Class::_default_tone_wav[16] = { 177, 219, 246, 255, 246, 219, 177, 128, 79, 37, 10, 1, 10, 37, 79, 128 }; // サイン波
// // const uint8_t Speaker_Class::_default_tone_wav[16] = 
// // {255, 223, 191, 159, 127, 95, 63, 31, 0, 31, 63, 95, 127, 159, 191, 223};//三角波２回

// const uint8_t Speaker_Class::_tone_wav[8][16] = {
//   { 177, 219, 246, 255, 246, 219, 177, 128, 79, 37, 10, 1, 10, 37, 79, 128 }, // サイン波
//   // { 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0 }, // 矩形波
//   { 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255}, // 長い矩形波
//   { 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0 }, // 短い矩形波
//   { 0, 32, 64, 96, 128, 160, 192, 224, 255, 223, 191, 159, 127, 95, 63, 31 }, // のこぎり波
//   {0, 21, 43, 64, 85, 107, 128, 149, 171, 192, 213, 235, 255, 235, 213, 192},// スムースのこぎり波
//   // {255, 223, 191, 159, 127, 95, 63, 31, 0, 31, 63, 95, 127, 159, 191, 223}//三角波２回
//   {255, 223, 191, 159, 127, 95, 63, 31, 0, 31, 63, 95, 127, 159, 191, 223},//三角波２回
//   {143, 247, 143, 247, 0, 123, 231, 99, 25, 95, 51, 214, 11, 237, 82, 160},//ノイズ
//   {0, 32, 64, 96, 128, 160, 192, 224, 255, 224, 192, 160, 128, 96, 64, 32}//三角波
// };


};

#endif // CHANNEL_HPP
