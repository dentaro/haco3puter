#include "Channel.hpp"

// Channel クラスのコンストラクタの実装
Channel::Channel() {
    // ここで初期化処理を行う
    setupHz();
}

// Channel クラスのデストラクタの実装
Channel::~Channel() {
    // ここでクリーンアップ処理を行う
}

// void Channel::test();

extern uint8_t tick;
extern uint8_t tickTime;
// extern uint8_t gEfectNo;

// void Channel::setChannel(const ChannelData newData[32])
// {
//     // 配列をコピーしてメンバ変数にセット
//     for (uint8_t i = 0; i < 32; ++i) {
//         this->notedata[i] = newData[i];
//     }
// }

float Channel::calculateFrequency(int pitch, int octave) {
    // A4の周波数（Hz）
    float baseFrequency = 440.0;

    // 全音階に対応するピッチの変換
    switch (pitch) {
        case 0: pitch = 0; break;  // ファ
        case 1: pitch = 1; break;  // ファ# / ソ♭
        case 2: pitch = 2; break;  // ソ
        case 3: pitch = 3; break;  // ソ# / ラ♭
        case 4: pitch = 4; break;  // ラ
        case 5: pitch = 5; break;  // ラ# / シ♭
        case 6: pitch = 6; break;  // シ
        case 7: pitch = 7; break;  // ド
        case 8: pitch = 8; break;  // ド# / レ♭
        case 9: pitch = 9; break;  // レ
        case 10: pitch = 10; break; // レ# / ミ♭
        case 11: pitch = 11; break; // ミ
        // default: pitch = 0; break; // デフォルトは全音階に含まれないピッチ
    }

    // オクターブの増加に応じて周波数を調整
    float octaveFactor = pow(2.0, octave - 4);

    // ドレミの差に基づいて周波数を計算
    float frequency = baseFrequency * octaveFactor * pow(2.0, (pitch-4) / 12.0);
    return frequency;
}

float calculateFrequency_p(int pitch, int octave) {
    // A4の周波数（Hz）
    float baseFrequency = 440.0;

    // ペンタトニックスケールに対応するピッチの変換
    switch (pitch) {
        case 0: pitch = 0; break;  // ド
        case 1: pitch = 2; break;  // レ
        case 2: pitch = 4; break;  // ミ
        case 3: pitch = 7; break;  // ソ
        case 4: pitch = 9; break;  // ラ
        case 5: pitch = 0; octave++; break; // 次のオクターブのド
        case 6: pitch = 2; octave++; break; // 次のオクターブのレ
        case 7: pitch = 4; octave++; break; // 次のオクターブのミ
        case 8: pitch = 7; octave++; break; // 次のオクターブのソ
        case 9: pitch = 9; octave++; break; // 次のオクターブのラ
        case 10: pitch = 0; octave += 2; break; // 2オクターブ進むド
        case 11: pitch = 2; octave += 2; break; // 2オクターブ進むレ
        // default: pitch = 0; octave = 4; break; // デフォルトはペンタトニックスケールに含まれないピッチ
    }

    // オクターブの増加に応じて周波数を調整
    float octaveFactor = pow(2.0, octave - 4);

    // ドレミの差に基づいて周波数を計算
    float frequency = baseFrequency * octaveFactor * pow(2.0, (pitch-4) / 12.0);
    return frequency;
}

void Channel::setupHz(){
        // 周波数リストを計算する
    for (int p = 0; p < numPitches; ++p) {
        for (int o = 0; o < numOctaves; ++o) {
            hzList[p][o] = calculateFrequency(p, o);
            // Serial.print(hzList[p][o]);
            // Serial.print(":");
        }
        // Serial.println(" ");
    }
}


void Channel::setTones(uint8_t onoffF, uint8_t loopStart, uint8_t loopEnd, uint8_t instrument, uint8_t pitch, uint8_t octave, uint8_t sfxno, 
uint8_t volume, uint8_t effectNo, uint8_t tickNo, uint8_t _chno, uint8_t _patternNo) {
uint8_t bufNo = _patternNo%2;
    // メンバ変数の初期化
    this->notedata[_chno][tickNo+bufNo*32].onoffF = onoffF;
    this->notedata[_chno][tickNo+bufNo*32].hz = hzList[pitch][octave]; // pitchとoctaveをそのまま使用
    this->notedata[_chno][tickNo+bufNo*32].instrument = instrument;
    this->notedata[_chno][tickNo+bufNo*32].pitch = pitch;
    this->notedata[_chno][tickNo+bufNo*32].octave = octave;
    this->notedata[_chno][tickNo+bufNo*32].sfxno = sfxno;
    this->notedata[_chno][tickNo+bufNo*32].volume = volume;
    this->notedata[_chno][tickNo+bufNo*32].effectNo = effectNo;
}


void Channel::setPatterns(uint8_t _patternNo, uint8_t _ch0, uint8_t _ch1, uint8_t _ch2, uint8_t _ch3, uint8_t _ch4, uint8_t _ch5, uint8_t _ch6, uint8_t _ch7)
{
    // メンバ変数の初期化
    patterns[_patternNo][0] = _ch0;
    patterns[_patternNo][1] = _ch1;
    patterns[_patternNo][2] = _ch2;
    patterns[_patternNo][3] = _ch3;
    patterns[_patternNo][4] = _ch4;
    patterns[_patternNo][5] = _ch5;
    patterns[_patternNo][6] = _ch6;
    patterns[_patternNo][7] = _ch7;
}

void Channel::setPatterns(uint8_t _patternNo, uint8_t _ch, uint8_t _patternID)
{
    // メンバ変数の初期化
    patterns[_patternNo][_ch] = _patternID;
}

uint8_t Channel::getPatternID(uint8_t _patternNo, uint8_t _chno){
    return patterns[_patternNo][_chno];
}

void Channel::resetTones(uint8_t tickNo, uint8_t _sfxno, uint8_t _pitch, uint8_t _octave, uint8_t _volume, uint8_t _instrument, uint8_t _effectNo)
{
    this->notedata[_sfxno][tickNo].pitch = _pitch;
    this->notedata[_sfxno][tickNo].octave = _octave;
    this->notedata[_sfxno][tickNo].volume = _volume;
    this->notedata[_sfxno][tickNo].hz = calculateFrequency(_pitch, _octave);
    this->notedata[_sfxno][tickNo].instrument = _instrument;
    this->notedata[_sfxno][tickNo].effectNo = _effectNo;
}

bool Channel::note(uint8_t channelno, uint8_t toneNo, uint8_t _patternNo)
{
    uint8_t bufNo = _patternNo%2;
    // uint8_t targetChNo = patterns[_patternNo][channelno];
    
    speakerEffectNo = this->notedata[channelno][(toneNo%TONE_NUM)+bufNo*32].effectNo;//noteパラメータの情報でエフェクトをかける

    M5.Speaker.setChannelVolume(channelno, this->notedata[channelno][toneNo%32].volume*32);//BGM

    M5.Speaker.tone(
        this->notedata[channelno][(toneNo%TONE_NUM)+bufNo*32].hz, 
        tickTime, 
        channelno, 
        true,
        _tone_wav[this->notedata[channelno][(toneNo%TONE_NUM)+bufNo*32].instrument],
        16,
        false);
    
    vTaskDelay(tickTime/CHANNEL_NUM);

    return true;
}

uint8_t Channel::gettick(){
return tick;
}

    