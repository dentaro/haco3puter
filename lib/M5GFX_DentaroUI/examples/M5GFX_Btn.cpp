#include "M5GFX_Btn.hpp"

//#include "Arduino.h"
//#include <M5Stack.h>
//
//#include "LGFX_ESP32_ao.hpp"
//
//M5GFX lcd;
//
//enum
//{ btn_a
//, btn_b
//, btn_c
//, BTN_MAX
//};
//
//// ボタン状態のビット列を保持する型（ボタン数を９個以上に増やす場合は必要に応じてuint16_tやuint32_tに変更する）
//typedef std::uint8_t bits_btn_t;
//
//enum : bits_btn_t
//{ bit_btn_a = 1<<btn_a
//, bit_btn_b = 1<<btn_b
//, bit_btn_c = 1<<btn_c
//};
//
//struct command_holder
//{
//  const std::uint32_t value;   // ボタンコマンドヒット時の識別値
//  const std::uint_fast8_t len; // ボタンコマンド長
//  const bits_btn_t* inputs;    // ボタンコマンド配列
//};
//
//static constexpr std::size_t BTN_STACK_MAX = 8; // ボタン状態の入力履歴保持数 （コマンド列以上の値にすること）
//
//// 判定対象となるコマンド列の作成
//// 例: 0, bit_btn_b, 0, bit_btn_a, 0 
////  この例にヒットする入力は、 無入力 -> ボタンA -> 無入力 -> ボタンB -> 無入力。 （ボタンBが離された時点でヒット）
////  配列の順序は時系列と逆順になる点に注意
//static constexpr bits_btn_t btn_a_single[] = { 0, 0, bit_btn_a, 0, 0 };
//static constexpr bits_btn_t btn_b_single[] = { 0, 0, bit_btn_b, 0, 0 };
//static constexpr bits_btn_t btn_c_single[] = { 0, 0, bit_btn_c, 0, 0 };
//static constexpr bits_btn_t btn_a_hold[]   = { bit_btn_a, bit_btn_a, 0 };
//static constexpr bits_btn_t btn_b_hold[]   = { bit_btn_b, bit_btn_b, 0 };
//static constexpr bits_btn_t btn_c_hold[]   = { bit_btn_c, bit_btn_c, 0 };
//static constexpr bits_btn_t btn_a_double[] = { 0, bit_btn_a, 0, bit_btn_a, 0 };
//static constexpr bits_btn_t btn_b_double[] = { 0, bit_btn_b, 0, bit_btn_b, 0 };
//static constexpr bits_btn_t btn_c_double[] = { 0, bit_btn_c, 0, bit_btn_c, 0 };
//static constexpr bits_btn_t btn_ab_hold[]  = { bit_btn_a|bit_btn_b, bit_btn_a|bit_btn_b };
//static constexpr bits_btn_t btn_ac_hold[]  = { bit_btn_a|bit_btn_c, bit_btn_a|bit_btn_c };
//static constexpr bits_btn_t btn_bc_hold[]  = { bit_btn_b|bit_btn_c, bit_btn_b|bit_btn_c };
//static constexpr bits_btn_t btn_abc_hold[] = { bit_btn_a|bit_btn_b|bit_btn_c, bit_btn_a|bit_btn_b|bit_btn_c };
//static constexpr bits_btn_t btn_a_b_c[]    = { 0, bit_btn_c, 0, bit_btn_b, 0, bit_btn_a, 0 };
//
//// 上記で作成したコマンド列を使い、コマンドリストを作成（第2引数の数字は第3引数の配列長と同じにすること）
//static constexpr command_holder commands[] = 
//{ { 101, 5, btn_a_single } // A 短押し
//, { 102, 5, btn_b_single } // B 短押し
//, { 103, 5, btn_c_single } // C 短押し
//, { 111, 3, btn_a_hold   } // A 長押し
//, { 112, 3, btn_b_hold   } // B 長押し
//, { 113, 3, btn_c_hold   } // C 長押し
//, { 121, 5, btn_a_double } // A 二回押し
//, { 122, 5, btn_b_double } // B 二回押し
//, { 123, 5, btn_c_double } // C 二回押し
//, { 201, 2, btn_ab_hold  } // AB 同時押し
//, { 202, 2, btn_ac_hold  } // AC 同時押し
//, { 203, 2, btn_bc_hold  } // BC 同時押し
//, { 204, 2, btn_abc_hold } // ABC 同時押し
//, { 301, 7, btn_a_b_c    } // ABC 順番押し
//};
//
//
//
//static btn_manager bm;
//
//void println(const char* src)
//{
//  lcd.fillRect(0, 0, M5.Lcd.width(), 30, TFT_BLACK);
//  lcd.drawstd::to_string(src, 0, 0);
//  Serial.println(src);
//}
