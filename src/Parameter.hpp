#ifndef PARAMETER_HPP
#define PARAMETER_HPP
static const int CONTROL_CYCLE = 256;
static const int LOG_SLOT_SIZE = 3;
static const int CONTROL_PER_PERIOD = 128;
static const int SIMULATION_CYCLE = CONTROL_CYCLE*CONTROL_PER_PERIOD*100;

// UserProcess の IOmap について
// 大きさ USER_IO_SIZE byte のメモリとする。
// アドレスとデータ型は以下の定義の通り。
//
// ADC
// AD 変換は [-2:2) の範囲を [0:AD_RANGE) に変換する。
// USER_IO_AD0 から USER_IO_AD5
//
// PWM
// USER_IO_PWM_ON の i ビット目が 1 の時，
// PWMi が有効となる。
// USER_IO_PWM_ON の i ビット目が 0 の時，
// PWMi が無効となる。対応する回路のスイッチは OFF となる。
//
// PWM は [0:CONTROL_CYCLE) の入力を受け付ける。
// USER_IO_PWM0 から USER_IO_PWM3
// 入力された値に比例したパルス幅のパルスを出力する。
// 0 でパルス幅0，CONTROL_CYCL-1で連続パルスとなる。
// パルスが hi の時，回路側ではハイサイドのスイッチが ON する。
// パルスが low の時，回路側ではローサイドのスイッチが ON する。
//
// DIO
// USER_IO_DI の i ビット目は i 番目のデジタル入力値を示す。
// USER_IO_DO の i ビット目が i 番目のデジタル出力となる。
//
//
static const int USER_IO_SIZE = 256;
static const int USER_IO_PWM_SIZE = 4;
static const int USER_IO_ADC_SIZE = 6;
static const int USER_IO_AD0 = 0x00;// unsigned short
static const int USER_IO_AD1 = 0x02;// unsigned short
static const int USER_IO_AD2 = 0x04;// unsigned short
static const int USER_IO_AD3 = 0x06;// unsigned short
static const int USER_IO_AD4 = 0x08;// unsigned short
static const int USER_IO_AD5 = 0x0a;// unsigned short
static const int USER_IO_PWM0 = 0x10;// unsigned short
static const int USER_IO_PWM1 = 0x12;// unsigned short
static const int USER_IO_PWM2 = 0x14;// unsigned short
static const int USER_IO_PWM3 = 0x16;// unsigned short
static const int USER_IO_PWM_ON = 0x20;// unsigned short
static const int USER_IO_DI = 0x22;// unsigned short
static const int USER_IO_DO = 0x24;// unsigned short
static const short AD_RANGE = 1024;

static double TAU() {
  return 0.2/CONTROL_CYCLE;
}
static double CONNECT_RL() {
  return 1.0/CONTROL_CYCLE/CONTROL_PER_PERIOD;
}
static double CONNECT_GAIN() {
  return 20.0;
}

static double V_DC() {
  return 1.5;
}

#endif
