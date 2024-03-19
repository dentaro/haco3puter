// #include "Tunes.h"

// #include "mymelo.h"

// volatile SemaphoreHandle_t Tunes::timerSemaphore;
// TaskHandle_t taskHandle;
// portMUX_TYPE Tunes::timerMux = portMUX_INITIALIZER_UNLOCKED;
// volatile uint32_t Tunes::isrCounter = 0;
// volatile uint32_t Tunes::lastIsrAt = 0;
// volatile uint16_t Tunes::osc1 = 0;
// volatile uint16_t Tunes::osc2 = 0;
// volatile uint16_t Tunes::osc3 = 0;
// volatile uint16_t Tunes::d[3] = {0,0,0};
// int Tunes::SineValues[256];
// int Tunes::SquareValues[256];
// hw_timer_t* Tunes::timer;

// extern int soundNo;
// extern double soundSpeed;
// extern int musicNo;
// extern bool musicflag;
// extern bool sfxflag;
// extern bool toneflag;

// // SQURE_WAVE ---------------------------
// #define  V1  0xffff
// #define V2  0x7fff
// #define V3  0x7fff
// // uint16_t v1,v2,v3, Va,Vb;             // 振幅とその合算値
// uint16_t c1,c2,c3, C1,C2,C3;
// uint8_t note1,note2,note3;            // 楽譜データDO|L4
// #define SFXNUM 8 //8個の効果音波形

// char Wx[SFXNUM][256];
// // boolean timer_flag = false;

// #define BUFFER_LEN    (256)        // バッファサイズ
// char Ws[] PROGMEM={0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,17,18,20,21,23,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,65,67,70,73,76,79,82,85,88,90,93,97,100,103,106,109,112,115,118,121,124,127,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,128,124,121,118,115,112,109,106,103,100,97,93,90,88,85,82,79,76,73,70,67,65,62,59,57,54,52,49,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0};
// // char Ws[] PROGMEM={0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,17,18,20,21,23,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,65,67,70,73,76,79,82,85,88,90,93,97,100,103,106,109,112,115,118,121,124,127,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,128,124,121,118,115,112,109,106,103,100,97,93,90,88,85,82,79,76,73,70,67,65,62,59,57,54,52,49,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0};
// // const char Ws[] PROGMEM={0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,17,18,20,21,23,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,65,67,70,73,76,79,82,85,88,90,93,97,100,103,106,109,112,115,118,121,124,127,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,128,124,121,118,115,112,109,106,103,100,97,93,90,88,85,82,79,76,73,70,67,65,62,59,57,54,52,49,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0};
// // 正弦波の波形テーブル(値:0～255, データ数:256)
// // char Ws[] PROGMEM={0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,17,18,20,21,23,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,65,67,70,73,76,79,82,85,88,90,93,97,100,103,106,109,112,115,118,121,124,127,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,128,124,121,118,115,112,109,106,103,100,97,93,90,88,85,82,79,76,73,70,67,65,62,59,57,54,52,49,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0};
// // 三角波の波形テーブル(値:0～255, データ数:256)...伴奏用
// // char Wt[] PROGMEM={0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,129,131,133,135,137,139,141,143,145,147,149,151,153,155,157,159,161,163,165,167,169,171,173,175,177,179,181,183,185,187,189,191,193,195,197,199,201,203,205,207,209,211,213,215,217,219,221,223,225,227,229,231,233,235,237,239,241,243,245,247,249,251,253,255,253,251,249,247,245,243,241,239,237,235,233,231,229,227,225,223,221,219,217,215,213,211,209,207,205,203,201,199,197,195,193,191,189,187,185,183,181,179,177,175,173,171,169,167,165,163,161,159,157,155,153,151,149,147,145,143,141,139,137,135,133,131,129,127,126,124,122,120,118,116,114,112,110,108,106,104,102,100,98,96,94,92,90,88,86,84,82,80,78,76,74,72,70,68,66,64,62,60,58,56,54,52,50,48,46,44,42,40,38,36,34,32,30,28,26,24,22,20,18,16,14,12,10,8,6,4,2};
// // uint16_t SW[]={0,0,15289,10204,9091,11454,12135,13621,30578,16198,18182,22908,20408,0};//ブザー
// // uint16_t SW[]={0,0,4388,6576,7380,5860,5532,4928,2196,4144,3692,2928,3288,0}; //スピーカー*4
// uint16_t SW[]={0,0,2194,3288,3690,2930,2766,2464,1098,2072,1846,1464,1644,0}; //スピーカー*2
// // uint16_t SW[]={0,0,1097,1644,1845,1465,1383,1232,549,1036,923,732,822,0}; //スピーカー

// #define CNT 8000000/256*0.5/8*120/TEMPO // テンポ120の四分音符(長さ8)を0.5秒とする
// uint16_t cntr=CNT;                    // 32us*(1953*120/Tempo)=62.5ms
// uint16_t p1=0, p2=0, p3=0;            // 楽譜part1～part3のポインタ(初期値は先頭)
// uint8_t len1=8, len2=8, len3=8;       // 初期ディレイ(62.5ms*8=.5s)
// uint8_t len1a=0,len2a=0,len3a=0;			// アタックタイム(32μs*128=4.096ms)
// uint8_t decayTimer=0;                 // 32μs*16=480μsごとに減衰させる
// uint8_t icntr;                        // チャタリング防止カウンタ(32*255=8.1ms)
// uint8_t nPlay=2;                      // 演奏回数(初回は2回演奏)

// // SINE_WAVE ---------------------------
// #define  Nc  256
// uint16_t E0,E1,E2;                     // 包絡
// uint16_t pW1,pW2,pW3, sw1,sw2,sw3;    // 波形ポインタとサンプリングステップ幅
// uint16_t v,v1,v2,v3,Va,Vb, u,u1,u2;          // 振幅とその合算値
// uint8_t x,y;
// double wcnt = 0;

// void IRAM_ATTR Tunes::onTimer(){
//   //mymelo----------------------------------------------------------------------------
// if(musicflag){
//   portENTER_CRITICAL_ISR(&Tunes::timerMux);

//   //   if(nPlay>0){
//   //   if(--cntr==0){                    // 32us*(1953*120/TEMPO)=62.5ms(32分音符長)
//   //     cntr=CNT;
//   //     if(--len1==0){
//   //       note1=pgm_read_byte(&part1[p1++]);  // part1の次のデータ
//   //       if(note1==0){                 // 0なら今回の演奏終了
//   //         nPlay--;                    // 演奏回数
//   //         p1=p2=p3=0;
//   //         len1=31;len2=len3=32;       // ポーズ(62.5ms*32=2s)
//   //         note1=note2=note3=0;
//   //       }else{
//   //         len1=len[note1>>5];         // 音符は上位3ビット
//   //         c1=0;C1=(C[note1&=0x1f]+64)>>7; // 音は下位5ビット
//   //         v1=V1;                      // 初めは最大振幅
//   //         decayTimer=0;
//   //       }
//   //     }
//   //     if(--len2==0){
//   //       note2=pgm_read_byte(&part2[p2++]);  // part2の次のデータ
//   //       if(note2==0)  p2--;           // 0ならPart2の演奏終了
//   //       else{
//   //         len2=len[note2>>5];         // 音符は上位3ビット
//   //         c2=0;C2=(C[note2&=0x1f]+64)>>7; // 音は下位5ビット
//   //         v2=V2;                      // 初めは最大振幅
//   //       }
//   //     }
//   //     if(--len3==0){
//   //       note3=pgm_read_byte(&part3[p3++]);  // part3の次のデータ
//   //       if(note3==0)  p3--;           // 0ならPart3の演奏終了
//   //       else{
//   //         len3=len[note3>>5];         // 音符は上位3ビット
//   //         c3=0;C3=(C[note3&=0x1f]+64)>>7; // 音は下位5ビット
//   //         v3=V3;                      // 初めは最大振幅
//   //       }
//   //     }
//   //   }
//   //   Vb=Va=0;
//   //   decayTimer++;
//   //   if(note1>1){                      // part1が休符でなければ
//   //     if(++c1==C1)  c1=0;             // 周期は32us*C1
//   //     if(c1<(C1>>1))  Va=v1;          // 周期の前半は振幅Va=v1
//   //     else if(!(decayTimer&0x0f)) v1-=(v1>>8);  // 32us*16=480us毎にv1の振幅減衰
//   //   }
//   //   if(note2>1){                      // part2が休符でなければ
//   //     if(++c2==C2)  c2=0;             // 周期は32us*C2
//   //     if(c2<(C2>>1))  Vb=v2;          // 周期の前半は振幅Vb=v2
//   //     else if(!(decayTimer&0x0f)) v2-=(v2>>8);  // 32us*16=480us毎にv2の振幅減衰
//   //   }
//   //   if(note3>1){                      // part3が休符でなければ
//   //     if(++c3==C3)  c3=0;             // 周期は32us*C3
//   //     if(c3<(C3>>1))  Vb+=v3;         // 周期の前半は振幅Vb+=v3
//   //     else if(!(decayTimer&0x0f)) v3-=(v3>>8);  // 32us*16=480us毎にv3の振幅減衰
//   //   }

//   //     dacWrite(SPEAKER_PIN,(Va+Vb+256)>>9);//和音
//   // }

// // 32us毎に
// 	if(nPlay>0){												// 演奏回数
// 		if(--cntr==0){
// 			cntr=CNT;												// 32us*(1953*120/Tempo)=62.5ms
// 			if(--len1==0){
// 				note1=pgm_read_byte(&part1[p1++]);	// part1の次のデータ
// 				if(note1==0){									// part1終了!!
// 					//nPlay--;										// 演奏回数
// 					p1=p2=0;										// ポインタを楽譜先頭
// 					note1=note2=0;
// 					len1=31; len2=32;						// 初期ディレイ(62.5ms*32=2s)
// 				}else{
// 					len1=len[note1>>5];					// 音符は上位3bit
// 					sw1=SW[note1&=0x1f];	pW1=0;// 音は下位5bit
// 					E1=0;												// 初めは0  32*64=2.048ms後に最大0x8000
// 					sw3=sw1+10;	pW3=0;					// わずか周波数をずらす(ビブラート効果)
// 				}
// 				len1a=128;										// アタックタイム(32*128=4.096ms)
// 			}
// 			if(--len2==0){
// 				note2=pgm_read_byte(&part2[p2++]);	// part2の次のデータ
// 				if(note2==0)	p2--;						// 0ならPart2の演奏終了
// 				else{
// 					len2=len[note2>>5];					// 音符は上位3bit
// 					sw2=SW[note2&=0x1f];	pW2=0;// 音は下位5bit
// 					E2=0;												// 初めは0  32*64=2.048ms後に最大0x8000
// 				}
// 				len2a=128;										// アタックタイム(32*128=4.096ms)
// 			}
// 		}
// 		u1=u2=0;	v1=v2=v3=0;
// 		if(note1>1){											// part1の振幅を加算
// 			if(len1a>0){len1a--;	E1+=256;}	// アタックタイム後にE1=0x8000
// 			E0=0x8000-E1;
// 			u1=(uint16_t)pgm_read_byte(&Ws[(uint8_t)(E0>>9)]);	u1-=(u1>>5);			// 0-96
// 			x=(pW1+=sw1)>>8;	y=(pW1+E0)>>8;
// 			v1=(uint16_t)pgm_read_byte(&Ws[x])+(uint16_t)pgm_read_byte(&Ws[y]);	// 0-510
// 			x=(pW3+=sw3)>>8;	y=(pW3+E0)>>8;
// 			v3=(uint16_t)pgm_read_byte(&Ws[x])+(uint16_t)pgm_read_byte(&Ws[y]);	// 0-510
// 		}else{	v1=v3=255;	u1=127;	}			// 休符
// 		if(note2>1){											// part2の振幅を加算
// 			if(len2a>0){len2a--;	E2+=256;}	// アタックタイム後にE2=0x8000
// 			E0=0x8000-E2;
// 			u2=(uint16_t)pgm_read_byte(&Ws[(uint8_t)(E0>>9)]);	u2-=(u2>>5);			// 0-96
// 			x=(pW2+=sw2)>>8;	y=(pW2+E0)>>8;
// #if defined(SINE_TRI_WAVE)						// 三角波で伴奏 ******
// 			v2=(uint16_t)pgm_read_byte(&Wt[x])+(uint16_t)pgm_read_byte(&Wt[y]);	// 0-510
// #else																	// 正弦波で伴奏 ******
// 			v2=(uint16_t)pgm_read_byte(&Ws[x])+(uint16_t)pgm_read_byte(&Ws[y]);	// 0-510
// #endif																// *******************
// 		}else{	v2=255;	u2=127;	}					// 休符
//     	v=v1+((v2+v3)>>1)+2;																									// 0-1020
//     	u=(u1<<1)+u2+u1;																											// 0-384
//     dacWrite(SPEAKER_PIN,(v-u)>>2);	
//     // dacWrite(SPEAKER_PIN,(1024-v-u)>>2);	
//     //OCR1B=(1024-v-u)>>2;																	// 0-255
// 		// OCR1A=(v1+1-(len1a>0?0:(u1<<1)))>>1;
// 		// dacWrite(SPEAKER_PIN,(1024-(v2+v3+1)-(len2a>0?0:u2)+(len1a>0?0:u1))>>2);	// 0-255
// 	}

//   portEXIT_CRITICAL_ISR(&Tunes::timerMux);

// }

// if(toneflag){
// // Increment the counter and set the time of ISR--------------------------------------------
//   portENTER_CRITICAL_ISR(&Tunes::timerMux);
//   Tunes::isrCounter++;
//   Tunes::lastIsrAt = millis();
//   Tunes::osc1 += d[0];
//   Tunes::osc2 += d[1];
//   Tunes::osc3 += d[2];
  
//   portEXIT_CRITICAL_ISR(&Tunes::timerMux);
//   // Give a semaphore that we can check in the loop

//   // xSemaphoreGiveFromISR(Tunes::timerSemaphore, NULL);
//   int out = 0;
//   //out += (Tunes::osc1 >> 8);//((osc1 >> 15)&1 == 1)?0:200;
//   //out += (Tunes::osc2 >> 8);//((osc2 >> 15)&1 == 1)?0:200;
//   //out += (Tunes::osc3 >> 8);//((osc3 >> 15)&1 == 1)?0:200;
//   out += Tunes::SineValues[(osc1 >> 8)];//((osc1 >> 15)&1 == 1)?0:200;
//   out += Tunes::SineValues[(osc2 >> 8)];//((osc2 >> 15)&1 == 1)?0:200;
//   out += Tunes::SineValues[(osc3 >> 8)];//((osc3 >> 15)&1 == 1)?0:200;
//   //out += ((osc1 >> 15)&1 == 1)?0:200;
//   //out += ((osc2 >> 15)&1 == 1)?0:200;
//   //out += ((osc3 >> 15)&1 == 1)?0:200;

//   if(d[0] == 0 && d[1] == 0 && d[2] == 0){
//     digitalWrite(SPEAKER_PIN, LOW);
//   }else{
//     dacWrite(SPEAKER_PIN,(out/3+128));
//   }
// }

//     if(sfxflag){
// // -------------------------------------------------------------------------------------------------
//     portENTER_CRITICAL_ISR(&Tunes::timerMux);
//     if(soundNo > -1)
//     {
//       // digitalWrite()//外部給電
//       // ledcWrite()//パルス幅変調
//       dacWrite(SPEAKER_PIN, (Wx[soundNo][(int)floor(wcnt)]));//255が最高値　スピーカー無音が128で、上下に波形が出る、ブザーは0~255
//       wcnt += soundSpeed;

//       if(wcnt>256)wcnt=0;
//     }
//     portEXIT_CRITICAL_ISR(&Tunes::timerMux);
//   }
  
// }


// // void Tunes::task1(void *pvParameters) {
// //   while (1) {
// //     xSemaphoreTake(timerSemaphore, portMAX_DELAY);
// //     Serial.println("int task1");
// //     delay(1);
// //   }
// // }


// void Tunes::init(){

//   pinMode(SPEAKER_PIN, OUTPUT);

//   File fr = SPIFFS.open("/init/sound/wave_sfx.txt", "r");// ⑩波形ファイル（256*256）を読み込みモードで開く
//   for (int j = 0; j < SFXNUM; j++) {
//     for (int i = 0; i < 256; i++) {
//       String _readStr = fr.readStringUntil(',');// カンマまで１行読み出
//       std::string _readstr = _readStr.c_str();
//         Wx[j][i] = atoi(_readstr.c_str());
//     }
//   }
//   fr.close();	// ⑫	ファイルを閉じる

//   fr = SPIFFS.open("/init/sound/wave.txt", "r");// ⑩波形ファイル（256*256）を読み込みモードで開く
//   for (int i = 0; i < 256; i++) {
//     String _readStr = fr.readStringUntil(',');// カンマまで１行読み出し
//     std::string _readstr = _readStr.c_str();
//     Ws[i] = atoi(_readstr.c_str());
//     // Serial.print(i);
//     // Serial.print(",");
    
//   }
//   fr.close();	// ⑫	ファイルを閉じる

//   float ConversionFactor=(2*PI)/256;        // convert my 0-255 bits in a circle to radians
//                                             // there are 2 x PI radians in a circle hence the 2*PI
//                                             // Then divide by 256 to get the value in radians
//                                             // for one of my 0-255 bits.
//   float RadAngle;                           // Angle in Radians
//   // calculate sine values
//   for(int MyAngle=0;MyAngle<256;MyAngle++) {
//     RadAngle=MyAngle*ConversionFactor;               // 8 bit angle converted to radians
//     Tunes::SineValues[MyAngle]=(sin(RadAngle)*128);  // get the sine of this angle and 'shift' up so
//                                             // there are no negative values in the data
//                                             // as the DAC does not understand them and would
//                                             // convert to positive values.
//     Tunes::SquareValues[MyAngle] = MyAngle<128?0:128;
//   }
  
//   Tunes::timerSemaphore = xSemaphoreCreateBinary();
//    // Core1の優先度1で通知受信用タスク起動
//   // xTaskCreateUniversal(
//   //   Tunes::task1,
//   //   "task1",
//   //   8192,
//   //   NULL,
//   //   1,
//   //   &taskHandle,
//   //   APP_CPU_NUM
//   // );
//   pinMode(SPEAKER_PIN, OUTPUT);
//   digitalWrite(SPEAKER_PIN, LOW);

//   ledcAttachPin(SPEAKER_PIN, BUZZER_CHANEL);

//   Tunes::timer = timerBegin(0, 80, true); // /80 prescale = 1us = 1/1000000s = 1MHz
//   timerAttachInterrupt(Tunes::timer, &Tunes::onTimer, true);
//   timerAlarmWrite(Tunes::timer, 50, true);
//   timerAlarmEnable(Tunes::timer);

//   //無音で立ち上げ
//   // d[0] = 0;
//   // d[1] = 0;
//   // d[2] = 0;
// }

// void Tunes::pause(){
//   timerAlarmDisable(Tunes::timer);
// }

// // extern bool firstBootF;
// void Tunes::resume(){
//   // if(firstBootF)
//   timerAlarmEnable(Tunes::timer);
// }

// // float i = 0.0;
// void Tunes::run(){
//   // get semaphore to use GPIO registers (maybe?
//   if (xSemaphoreTake(Tunes::timerSemaphore, 0) == pdTRUE){
//     uint32_t isrCount = 0, isrTime = 0;
//     portENTER_CRITICAL(&Tunes::timerMux);
//     // ==== critical section begin ====
//     isrCount = Tunes::isrCounter;
//     isrTime = Tunes::lastIsrAt;
//     // ==== critical section end ====
//     portEXIT_CRITICAL(&Tunes::timerMux);
//   }

//   // below script is safe?

//   // if(timer_flag){
//     // portENTER_CRITICAL(&Tunes::timerMux);
//     // Serial.println("semaphore");delay(1);

//     // dacWrite(25,128+128*sin(i));
//     //   i += 0.1;
//     //   if(i >= 6.28){
//     //     i = 0;
//     //   }

//     // ledcWriteTone(BUZZER_CHANEL, 440);
    
//     // delay(100);
//     // ledcWriteTone(BUZZER_CHANEL, 0);    // ならしたら最後に消音

//   //   if(nPlay>0){
//   //   if(--cntr==0){                    // 32us*(1953*120/TEMPO)=62.5ms(32分音符長)
//   //     cntr=CNT;
//   //     if(--len1==0){
//   //       note1=pgm_read_byte(&part1[p1++]);  // part1の次のデータ
//   //       if(note1==0){                 // 0なら今回の演奏終了
//   //         nPlay--;                    // 演奏回数
//   //         p1=p2=p3=0;
//   //         len1=31;len2=len3=32;       // ポーズ(62.5ms*32=2s)
//   //         note1=note2=note3=0;
//   //       }else{
//   //         len1=len[note1>>5];         // 音符は上位3ビット
//   //         c1=0;C1=(C[note1&=0x1f]+64)>>7; // 音は下位5ビット
//   //         v1=V1;                      // 初めは最大振幅
//   //         decayTimer=0;
//   //       }
//   //     }
//   //     if(--len2==0){
//   //       note2=pgm_read_byte(&part2[p2++]);  // part2の次のデータ
//   //       if(note2==0)  p2--;           // 0ならPart2の演奏終了
//   //       else{
//   //         len2=len[note2>>5];         // 音符は上位3ビット
//   //         c2=0;C2=(C[note2&=0x1f]+64)>>7; // 音は下位5ビット
//   //         v2=V2;                      // 初めは最大振幅
//   //       }
//   //     }
//   //     if(--len3==0){
//   //       note3=pgm_read_byte(&part3[p3++]);  // part3の次のデータ
//   //       if(note3==0)  p3--;           // 0ならPart3の演奏終了
//   //       else{
//   //         len3=len[note3>>5];         // 音符は上位3ビット
//   //         c3=0;C3=(C[note3&=0x1f]+64)>>7; // 音は下位5ビット
//   //         v3=V3;                      // 初めは最大振幅
//   //       }
//   //     }
//   //   }
//   //   Vb=Va=0;
//   //   decayTimer++;
//   //   if(note1>1){                      // part1が休符でなければ
//   //     if(++c1==C1)  c1=0;             // 周期は32us*C1
//   //     if(c1<(C1>>1))  Va=v1;          // 周期の前半は振幅Va=v1
//   //     else if(!(decayTimer&0x0f)) v1-=(v1>>8);  // 32us*16=480us毎にv1の振幅減衰
//   //   }
//   //   if(note2>1){                      // part2が休符でなければ
//   //     if(++c2==C2)  c2=0;             // 周期は32us*C2
//   //     if(c2<(C2>>1))  Vb=v2;          // 周期の前半は振幅Vb=v2
//   //     else if(!(decayTimer&0x0f)) v2-=(v2>>8);  // 32us*16=480us毎にv2の振幅減衰
//   //   }
//   //   if(note3>1){                      // part3が休符でなければ
//   //     if(++c3==C3)  c3=0;             // 周期は32us*C3
//   //     if(c3<(C3>>1))  Vb+=v3;         // 周期の前半は振幅Vb+=v3
//   //     else if(!(decayTimer&0x0f)) v3-=(v3>>8);  // 32us*16=480us毎にv3の振幅減衰
//   //   }

//   //     dacWrite(SPEAKER_PIN,(Va+Vb+256)>>9);//和音
//   // }
//   // portEXIT_CRITICAL(&Tunes::timerMux);
//   // timer_flag = false;
  
//   // }//timer
// }



// //   //正弦波
// // //---------------------------------------------------------
// //     if(nPlay>0){
// //     if(--cntr==0){
// //       cntr=CNT;                       // 32us*(1953*120/Tempo)=62.5ms
// //       if(--len1==0){//大きい値から処理していく
// //         note1=pgm_read_byte(&part1[p1++]);  //mymelo.hにある楽譜データpart1のnoteデータを取得　p1で順次noteデータを参照していく
        
// //         if(note1==0){                 // part1終了!!
// //           // nPlay--;                    // 演奏回数
// //           p1=p2=0;                    // 楽譜先頭
// //           note1=note2=0;
// //           len1=31; len2=32;           // 初期ディレイ(62.5ms*32=2s)
// //         }else{//波形生成用のデータを抽出
// //           len1=len[note1>>5];         // 音符は上位3ビット 1110 0000 -> 0000 0111
// //           sw1=SW[note1&=0x1f];  pW1=0;// 音は下位5ビット 0x1f= 0001 1111 
// //           E1=Nc<<7;                   // 初めは最大振幅(128)
// //           sw3=sw1+10; pW3=0;          // わずか周波数をずらす
// //         }
// //       }
      
// //       //-----------------------------------------
// //       if(--len2==0){
// //         note2=pgm_read_byte(&part2[p2++]);  // part2の次のデータ
// //         if(note2==0)  p2--;           // 0ならPart2の演奏終了
// //         else{
// //           len2=len[note2>>5];         // 音符は上位3ビット
// //           sw2=SW[note2&=0x1f];  pW2=0;// 音は下位5ビット
// //           E2=Nc<<7;                   // 初めは最大振幅
// //         }
// //       }
// //       //-----------------------------------------
// //     }
// //     //u1=0; v1=v3=0;
// //     u1=u2=0;  v1=v2=v3=0;             //初期化
// //     if(note1>1){                      // part1の振幅を加算
// //       E=(Nc<<7)-E1+128;
// //       u1=(uint16_t)pgm_read_byte(&Ws[(uint8_t)(E>>9)]);  u1-=(u1>>5);
// //       x=((pW1+=sw1)+128)>>8;  y=(pW1+E)>>8;
// //       v1=(uint16_t)pgm_read_byte(&Ws[x])+(uint16_t)pgm_read_byte(&Ws[y]);
// //       x=((pW3+=sw3)+128)>>8;  y=(pW3+E)>>8;
// //       v3=(uint16_t)pgm_read_byte(&Ws[x])+(uint16_t)pgm_read_byte(&Ws[y]);
// //     }else{  v1=v3=255;  u1=127; }     // 休符
// // ////-----------------------------------------
// //     if(note2>1){                      // part2の振幅を加算
// //       E=(Nc<<7)-E2+128;
// //       u2=(uint16_t)pgm_read_byte(&Ws[(uint8_t)(E>>9)]);  u2-=(u2>>5);
// //       x=(pW2+=sw2)>>8;  y=(pW2+E)>>8;
// //       v2=(uint16_t)pgm_read_byte(&Ws[x])+(uint16_t)pgm_read_byte(&Ws[y]);
// //     }else{  v2=255; u2=127; }         // 休符
// //     v=v1+((v2+v3)>>1)+2;  u=(u1<<1)+u2+u1;
// // //-----------------------------------------
// //     dacWrite(25,(v-u)>>2);
// //   }
// //   if(icntr>0)  icntr--;
// //   if(--decayTimer==0) E1-=(E1>>5);    // 32us*256=8.192ms毎に包絡減衰

// // -------------------------------------------------------------------------------------
// //  if(icntr>0) icntr--;
// //  if(--decayTimer==0){ E1-=(E1>>5); E2-=(E2>>5);} // 32us*256=8.192ms毎に包絡減衰
// //  dacWrite(25,32*F_CPU/8/1000000);
// //  dacWrite(26,32*F_CPU/8/1000000);

//   //矩形波
//   // if(nPlay>0){
//   //   if(--cntr==0){                    // 32us*(1953*120/TEMPO)=62.5ms(32分音符長)
//   //     cntr=CNT;
//   //     if(--len1==0){
//   //       note1=pgm_read_byte(&part1[p1++]);  // part1の次のデータ
//   //       if(note1==0){                 // 0なら今回の演奏終了

//   //         //nPlay--;                    // 演奏回数(コメントアウトで無限ループ)

//   //         p1=p2=p3=0;
//   //         len1=31;len2=len3=32;       // ポーズ(62.5ms(32分音符長)*32=2s)
//   //         note1=note2=note3=0;
//   //       }else{
//   //         len1=len[note1>>5];         // 音符は上位3ビット
//   //         c1=0;C1=(C[note1&=0x1f]+64)>>7; // 音は下位5ビット
//   //         v1=V1;                      // 初めは最大振幅
//   //         decayTimer=0;
//   //       }
//   //     }
//   //     if(--len2==0){
//   //       note2=pgm_read_byte(&part2[p2++]);  // part2の次のデータ
//   //       if(note2==0)  p2--;           // 0ならPart2の演奏終了
//   //       else{
//   //         len2=len[note2>>5];         // 音符は上位3ビット
//   //         c2=0;C2=(C[note2&=0x1f]+64)>>7; // 音は下位5ビット
//   //         v2=V2;                      // 初めは最大振幅
//   //       }
//   //     }
//   //     if(--len3==0){
//   //       note3=pgm_read_byte(&part3[p3++]);  // part3の次のデータ
//   //       if(note3==0)  p3--;           // 0ならPart3の演奏終了
//   //       else{
//   //         len3=len[note3>>5];         // 音符は上位3ビット
//   //         c3=0;C3=(C[note3&=0x1f]+64)>>7; // 音は下位5ビット
//   //         v3=V3;                      // 初めは最大振幅
//   //       }
//   //     }
//   //   }
//   //   Vb=Va=0;
//   //   decayTimer++;
//   //   if(note1>1){                      // part1が休符でなければ
//   //     if(++c1==C1)  c1=0;             // 周期は32us*C1
//   //     if(c1<(C1>>1))  Va=v1;          // 周期の前半は振幅Va=v1
//   //     else if(!(decayTimer&0x0f)) v1-=(v1>>8);  // 32us*16=480us毎にv1の振幅減衰
//   //   }
//   //   if(note2>1){                      // part2が休符でなければ
//   //     if(++c2==C2)  c2=0;             // 周期は32us*C2
//   //     if(c2<(C2>>1))  Vb=v2;          // 周期の前半は振幅Vb=v2
//   //     else if(!(decayTimer&0x0f)) v2-=(v2>>8);  // 32us*16=480us毎にv2の振幅減衰
//   //   }
//   //   if(note3>1){                      // part3が休符でなければ
//   //     if(++c3==C3)  c3=0;             // 周期は32us*C3
//   //     if(c3<(C3>>1))  Vb+=v3;         // 周期の前半は振幅Vb+=v3
//   //     else if(!(decayTimer&0x0f)) v3-=(v3>>8);  // 32us*16=480us毎にv3の振幅減衰
//   //   }

//   //   // if(d[0] == 0 && d[1] == 0 && d[2] == 0){
//   //   //   digitalWrite(SPEAKER_PIN, LOW);
//   //   // }else{
//   //   //   dacWrite(SPEAKER_PIN,(Va+Vb+256)>>9);//和音
//   //   }

//   //     dacWrite(SPEAKER_PIN,(Va+Vb+256)>>9);//和音
  
  