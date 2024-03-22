#include "Channel.hpp"

// Channel クラスのコンストラクタの実装
Channel::Channel() {
    // ここで初期化処理を行う
}

// Channel クラスのデストラクタの実装
Channel::~Channel() {
    // ここでクリーンアップ処理を行う
}

// void Channel::test();

extern int tick;
// extern size_t gEfectNo;

// void Channel::setChannel(const ChannelData newData[32])
// {
//     // 配列をコピーしてメンバ変数にセット
//     for (size_t i = 0; i < 32; ++i) {
//         this->notedata[i] = newData[i];
//     }
// }

double calculateFrequency(int pitch, int octave) {
    // A4の周波数（Hz）
    double baseFrequency = 440.0;

    switch (pitch){//ペンタトニックスケールにする
        case 0:pitch = 0;octave = 4;break;
        case 1:pitch = 2;octave = 4;break;
        case 2:pitch = 4;octave = 4;break;
        case 3:pitch = 7;octave = 4;break;
        case 4:pitch = 9;octave = 4;break;
        case 5:pitch =11;octave = 4;break;
        case 6:pitch = 0;octave = 5;break;
        case 7:pitch = 2;octave = 5;break;
        case 8:pitch = 4;octave = 5;break;
        case 9:pitch = 7;octave = 5;break;
        case 10:pitch= 9;octave = 5;break;
        case 11:pitch=11;octave = 5;break;
        // default:pitch= 0;octave = 5;break;
    }

    // ドレミの差に基づいて周波数を計算
    double frequency = baseFrequency * pow(2.0, (pitch / 12.0 + octave - 4));
    // double frequency = baseFrequency * pow(2.0, (pitch / 12.0 + octave - 5));

//     double frequency;
// if (octave == 4) {
//     frequency = baseFrequency * pow(2.0, ((pitch + 12 * (octave - 4)) / 12.0));
// } else if (octave == 5) {
//     frequency = baseFrequency * pow(2.0, ((pitch + 12 * (octave - 5)) / 12.0));
// }

    return frequency;
}

extern int tickTime;
void Channel::setTones( size_t onoffF, 
size_t loopStart, size_t loopEnd, size_t instrument, size_t pitch, size_t octave, size_t sfxno, size_t volume, size_t effectNo, size_t tickNo, 
size_t _chno, size_t _buffAreaNo)
{
    // instrument = pitch;//移しておく

    for (int pitch = 0; pitch < numPitches; ++pitch) {
        for (int octave = 0; octave < numOctaves; ++octave) {
            hzList[pitch][octave] = calculateFrequency(pitch, octave);
        }
    }
    // メンバ変数の初期化
    // for (int _chno = 0; _chno < 8; ++_chno) {//同じものを４つ作る
        this->notedata[_chno][tickNo+_buffAreaNo*32].onoffF = onoffF;
        this->notedata[_chno][tickNo+_buffAreaNo*32].loopStart = loopStart;
        this->notedata[_chno][tickNo+_buffAreaNo*32].loopEnd = loopEnd;
        this->notedata[_chno][tickNo+_buffAreaNo*32].hz = hzList[pitch][octave];
        this->notedata[_chno][tickNo+_buffAreaNo*32].instrument = instrument;
        this->notedata[_chno][tickNo+_buffAreaNo*32].pitch = pitch;
        this->notedata[_chno][tickNo+_buffAreaNo*32].octave = octave;
        this->notedata[_chno][tickNo+_buffAreaNo*32].sfxno = sfxno;
        this->notedata[_chno][tickNo+_buffAreaNo*32].volume = volume;
        this->notedata[_chno][tickNo+_buffAreaNo*32].effectNo = effectNo;
        this->notedata[_chno][tickNo+_buffAreaNo*32].looplen = loopEnd - loopStart; 
    // }
}

void Channel::setPatterns(size_t _patternNo, size_t _ch0, size_t _ch1, size_t _ch2, size_t _ch3, size_t _ch4, size_t _ch5, size_t _ch6, size_t _ch7)
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

void Channel::setPatterns(size_t _patternNo, size_t _ch, size_t _patternID)
{
    // メンバ変数の初期化
    patterns[_patternNo][_ch] = _patternID;
}

size_t Channel::getPatternID(size_t _patternNo, size_t _chno){
    return patterns[_patternNo][_chno];
}

void Channel::resetTones(size_t tickNo, size_t _sfxno, size_t _pitch, size_t _octave, size_t _volume, size_t _instrument, size_t _effectNo, size_t _buffAreaNo)
{
    this->notedata[_sfxno][tickNo+_buffAreaNo*32].pitch = _pitch;
    this->notedata[_sfxno][tickNo+_buffAreaNo*32].octave = _octave;
    this->notedata[_sfxno][tickNo+_buffAreaNo*32].volume = _volume;
    this->notedata[_sfxno][tickNo+_buffAreaNo*32].hz = calculateFrequency(_pitch, _octave);
    this->notedata[_sfxno][tickNo+_buffAreaNo*32].instrument = _instrument;
    this->notedata[_sfxno][tickNo+_buffAreaNo*32].effectNo = _effectNo;
}

bool Channel::note(size_t channelno, size_t tick, size_t _patternNo){
    size_t _buffAreaNo = _patternNo;
    // setChannelVolume(channelno, this->notedata[channelno][tick].volume*16);//BGM
    // tone(this->notedata[channelno][tick%32].hz, tickTime, channelno, true, _tone_wav[instrument], 16, false);
    
    size_t targetChNo = patterns[_patternNo][channelno];

    // size_t sfxNo = this->notedata[sfxNo][tick%32].sfxno;//noteパラメータの情報でエフェクトをかける
    speakerEffectNo = this->notedata[targetChNo][tick%32].effectNo;//noteパラメータの情報でエフェクトをかける
    setChannelVolume(targetChNo, this->notedata[targetChNo][tick%32].volume*16);//BGM


    tone(
        this->notedata[targetChNo][(tick+_buffAreaNo*32)%TONE_NUM].hz, 
        tickTime, 
        targetChNo, 
        true,
        _tone_wav[this->notedata[targetChNo][(tick+_buffAreaNo*32)%TONE_NUM].instrument],
        16,
        false);

    // tone(
    //     this->notedata[patternNo][tick%32].hz, 
    //     tickTime, 
    //     patternNo, 
    //     true,
    //     _tone_wav[_instrument],
    //     16,
    //     false);
    
    vTaskDelay(tickTime/CHANNEL_NUM);

    return true;
}

size_t Channel::gettick(){
return tick;
}

    