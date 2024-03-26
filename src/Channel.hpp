#ifndef CHANNEL_HPP
#define CHANNEL_HPP

// #include "Speaker_Class.hpp"
// #include "CRingBuffur.hpp"
#include "M5Unified.h"
// #include "M5Cardputer.h"

#define numPitches 12  // ドレミファソラシド（12音）// + レ（13音）
#define numOctaves 16

#define PATTERN_NUM 64//64パタン用意するとメモリが足らない
#define TONE_NUM 32
#define CHANNEL_NUM 8//


// 新しい構造体
struct ChannelData {
    bool onoffF;
    uint8_t loopStart;
    uint8_t loopEnd;
    float hz;
    uint8_t pitch;
    uint8_t octave;
    uint8_t sfxno;
    uint8_t instrument;
    uint8_t volume;
    uint8_t effectNo;
    uint8_t tickNo;
    uint8_t looplen;
};

class Channel : public m5::Speaker_Class {//M5のスピーカークラス
// class Channel : public Speaker_Class {//独自のスピーカークラス
public:
  ChannelData notedata[CHANNEL_NUM][TONE_NUM*2];  // 構造体をメンバ変数として持つ（バックバッファ2パタン8チャンネル分しかデータは持たない）
  uint8_t patterns[PATTERN_NUM][CHANNEL_NUM];//３２パターン（小節）8チャンネルにSfxNo0~63を３２ノート割り付けるための配列
  // uint8_t speakerEffectNo = 0;

  // 二次元浮動小数点数配列の宣言
  double hzList[numPitches][numOctaves];

  // CRingBuffur ringbuf;


  // コンストラクタ
  Channel();
  // デストラクタ
  virtual ~Channel();
  void setTones(uint8_t onoffF, uint8_t loopStart, uint8_t loopEnd, uint8_t instrument, uint8_t pitch, uint8_t octave, uint8_t sfxno, uint8_t volume, 
  uint8_t effectNo, uint8_t tickNo, uint8_t _chno, uint8_t _patternNo);
  void setPatterns(uint8_t _patternNo, uint8_t _ch0, uint8_t _ch1, uint8_t _ch2, uint8_t _ch3, uint8_t _ch4, uint8_t _ch5, uint8_t _ch6, uint8_t _ch7);
  void setPatterns(uint8_t _patternNo, uint8_t _ch, uint8_t _patternID);
  uint8_t getPatternID(uint8_t _patternNo, uint8_t _chno);
  bool note(uint8_t channelno, uint8_t tick, uint8_t _patternNo);
  uint8_t gettick();
  void resetTones(uint8_t tickNo, uint8_t _sfxno, uint8_t _pitch, uint8_t _octave, uint8_t _volume, uint8_t _instrument, uint8_t _effectNo);
  void setupHz();
    // uint8_t* getSoundBuffer() const {
    //     return *sound_buf32;
    // }
  // uint8_t* getbuf(){ return sound_buf32_b; }//2147483647

  // protected:
  // uint8_t _tone_wav[8][16] = {0};
  // uint8_t speakerEffectNo = 0;

  protected:

  uint8_t speakerEffectNo = 0;//実体を定義
const uint8_t _default_tone_wav[16] = { 177, 219, 246, 255, 246, 219, 177, 128, 79, 37, 10, 1, 10, 37, 79, 128 }; // サイン波
// const uint8_t Speaker_Class::_default_tone_wav[16] = 
// {255, 223, 191, 159, 127, 95, 63, 31, 0, 31, 63, 95, 127, 159, 191, 223};//三角波２回

const uint8_t _tone_wav[8][16] = {
  { 177, 219, 246, 255, 246, 219, 177, 128, 79, 37, 10, 1, 10, 37, 79, 128 }, // サイン波
  // { 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0 }, // 矩形波
  { 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255}, // 長い矩形波
  { 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0 }, // 短い矩形波
  { 0, 32, 64, 96, 128, 160, 192, 224, 255, 223, 191, 159, 127, 95, 63, 31 }, // のこぎり波
  {0, 21, 43, 64, 85, 107, 128, 149, 171, 192, 213, 235, 255, 235, 213, 192},// スムースのこぎり波
  // {255, 223, 191, 159, 127, 95, 63, 31, 0, 31, 63, 95, 127, 159, 191, 223}//三角波２回
  {255, 223, 191, 159, 127, 95, 63, 31, 0, 31, 63, 95, 127, 159, 191, 223},//三角波２回
  {143, 247, 143, 247, 0, 123, 231, 99, 25, 95, 51, 214, 11, 237, 82, 160},//ノイズ
  {0, 32, 64, 96, 128, 160, 192, 224, 255, 224, 192, 160, 128, 96, 64, 32}//三角波
};

};

#endif // CHANNEL_HPP
