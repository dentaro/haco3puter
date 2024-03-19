#include <cstdint>
#include <chrono>
#include <algorithm>

#pragma once //インクルードガード

enum
{ btn_a
, btn_b
, btn_c
, BTN_MAX
};

// ボタン状態のビット列を保持する型（ボタン数を９個以上に増やす場合は必要に応じてuint16_tやuint32_tに変更する）
typedef std::uint8_t bits_btn_t;

enum : bits_btn_t
{ bit_btn_a = 1<<btn_a
, bit_btn_b = 1<<btn_b
, bit_btn_c = 1<<btn_c
};

struct command_holder
{
  const std::uint32_t value;   // ボタンコマンドヒット時の識別値
  const std::uint_fast8_t len; // ボタンコマンド長
  const bits_btn_t* inputs;    // ボタンコマンド配列
};

static constexpr std::uint8_t BTN_STACK_MAX = 8; // ボタン状態の入力履歴保持数 （コマンド列以上の値にすること）

// 判定対象となるコマンド列の作成
// 例: 0, bit_btn_b, 0, bit_btn_a, 0 
//  この例にヒットする入力は、 無入力 -> ボタンA -> 無入力 -> ボタンB -> 無入力。 （ボタンBが離された時点でヒット）
//  配列の順序は時系列と逆順になる点に注意
static constexpr bits_btn_t btn_a_single[] = { 0, 0, bit_btn_a, 0, 0 };
static constexpr bits_btn_t btn_b_single[] = { 0, 0, bit_btn_b, 0, 0 };
static constexpr bits_btn_t btn_c_single[] = { 0, 0, bit_btn_c, 0, 0 };
static constexpr bits_btn_t btn_a_hold[]   = { bit_btn_a, bit_btn_a, 0 };
static constexpr bits_btn_t btn_b_hold[]   = { bit_btn_b, bit_btn_b, 0 };
static constexpr bits_btn_t btn_c_hold[]   = { bit_btn_c, bit_btn_c, 0 };
static constexpr bits_btn_t btn_a_double[] = { 0, bit_btn_a, 0, bit_btn_a, 0 };
static constexpr bits_btn_t btn_b_double[] = { 0, bit_btn_b, 0, bit_btn_b, 0 };
static constexpr bits_btn_t btn_c_double[] = { 0, bit_btn_c, 0, bit_btn_c, 0 };
static constexpr bits_btn_t btn_ab_hold[]  = { bit_btn_a|bit_btn_b, bit_btn_a|bit_btn_b };
static constexpr bits_btn_t btn_ac_hold[]  = { bit_btn_a|bit_btn_c, bit_btn_a|bit_btn_c };
static constexpr bits_btn_t btn_bc_hold[]  = { bit_btn_b|bit_btn_c, bit_btn_b|bit_btn_c };
static constexpr bits_btn_t btn_abc_hold[] = { bit_btn_a|bit_btn_b|bit_btn_c, bit_btn_a|bit_btn_b|bit_btn_c };
static constexpr bits_btn_t btn_a_b_c[]    = { 0, bit_btn_c, 0, bit_btn_b, 0, bit_btn_a, 0 };

// 上記で作成したコマンド列を使い、コマンドリストを作成（第2引数の数字は第3引数の配列長と同じにすること）
static constexpr command_holder commands[] = 
{ { 101, 5, btn_a_single } // A 短押し
, { 102, 5, btn_b_single } // B 短押し
, { 103, 5, btn_c_single } // C 短押し
, { 111, 3, btn_a_hold   } // A 長押し
, { 112, 3, btn_b_hold   } // B 長押し
, { 113, 3, btn_c_hold   } // C 長押し
, { 121, 5, btn_a_double } // A 二回押し
, { 122, 5, btn_b_double } // B 二回押し
, { 123, 5, btn_c_double } // C 二回押し
, { 201, 2, btn_ab_hold  } // AB 同時押し
, { 202, 2, btn_ac_hold  } // AC 同時押し
, { 203, 2, btn_bc_hold  } // BC 同時押し
, { 204, 2, btn_abc_hold } // ABC 同時押し
, { 301, 7, btn_a_b_c    } // ABC 順番押し
};

class M5GFX_Btn
{
public:
  std::uint32_t msec_debounce = 50;  // デバウンス処理の待ち時間
  std::uint32_t msec_hold = 250;     // 長押し判定の待ち時間

  void setup(void)
  {
    // _gpio[btn_a] = 36;
    // _gpio[btn_b] = 35;
    // _gpio[btn_c] = 39;

    // for (auto pin : _gpio) { pinMode(pin, INPUT); }
  }

  std::uint32_t hitcheck(void)
  {
    std::uint32_t hitres = 0;
    std::uint_fast8_t hitlen = 0;

    // 登録されているコマンドと一致するものを探すループ
    for (const auto& cmd : commands)
    {
      // 既にヒットしたものより短いものは除外
      if (hitlen >= cmd.len) continue;

      // 履歴と一致しないものは除外
      if (memcmp(_btns_stack, cmd.inputs, sizeof(bits_btn_t) * cmd.len)) continue;

      // ヒットしたものを記録しておく
      hitres = cmd.value;
      hitlen = cmd.len;
    }
    return hitres;
  }

  // ボタン状態更新処理
  // 戻り値：true=状態変化あり / false=状態変化なし
  bool loop(void)
  {
    // 現在のミリ秒時間を取得
    // 現在のミリ秒時間を取得
    std::uint32_t msec = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()).count();

    // ボタンのGPIOの状態をbtnsにビット列として取得
    std::uint32_t btns = 0;
    // for (int i = 0; i < BTN_MAX; ++i) { if (_gpio[i] >= 0 && !digitalRead(_gpio[i])) btns |= 1 << i; }

    // 記録しているボタンの状態と比較
    if (_btns_before_db != btns)
    {
      // デバウンス処理前の入力状態を更新
      _btns_before_db = btns;
      _msec_changed = msec;
      return false;
    }

    // 入力変化後、デバウンス処理時間が経過していることを確認
    if (_btns_after_db != btns && msec - _msec_changed > msec_debounce)
    {
      // デバウンス処理後の入力状態を更新
      _btns_after_db = btns;
//      _msec_after_db = msec;

      // 履歴を一つ追加
      memmove(&_btns_stack[1], _btns_stack, (BTN_STACK_MAX - 1) * sizeof(bits_btn_t));
      // std::copy(_btns_stack, _btns_stack + BTN_STACK_MAX - 1, _btns_stack + 1);

      _btns_stack[0] = btns;

      // 履歴とコマンドの比較処理を行う
      _hitvalue = hitcheck();
      return true;
    }

    // 入力無変化での長押し時間経過を確認（無操作での時間経過時も含む）
    if (_btns_stack[1] != btns && msec - _msec_changed > msec_hold)
    {
      // 履歴を一つ追加（長押し時間が経過した場合、履歴の今回値と前回値が同じになる）
      memmove(&_btns_stack[1], _btns_stack, (BTN_STACK_MAX - 1) * sizeof(bits_btn_t));
      // std::copy(_btns_stack, _btns_stack + BTN_STACK_MAX - 1, _btns_stack + 1);

      // 履歴とコマンドの比較処理を行う
      _hitvalue = hitcheck();
      return true;
    }

    // 無入力状態が長押し時間２回分経過した場合は履歴をクリア
    if (!btns && msec - _msec_changed > (msec_hold<<1))
    {
      memset(_btns_stack, 0, BTN_STACK_MAX * sizeof(bits_btn_t));
      _msec_changed = msec;
      _hitvalue = 0;
      return true;
    }
    return false;
  }

  // コマンド判定後の値を取得する
  std::uint32_t getValue(void) const { return _hitvalue; }

  // ボタン入力履歴の配列を取得する
  const bits_btn_t* getStack(void) const { return _btns_stack; }

private:
  std::uint32_t _hitvalue;           // コマンド判定でヒットした値
  std::int32_t _gpio[BTN_MAX];       // ボタンのGPIOリスト
  std::uint32_t _msec_changed = 0;   // 入力変化時点の時間記録
  bits_btn_t _btns_before_db = 0;    // デバウンス処理前のボタン状態記録
  bits_btn_t _btns_after_db = 0;     // デバウンス処理後のボタン状態記録
  bits_btn_t _btns_stack[BTN_STACK_MAX]; // ボタン状態の履歴
};
