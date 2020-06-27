#include "Parameter.hpp"
#include "UserProcess.hpp"
#include <math.h>
namespace user {
  int i;
  int cycle;
  short ref[CONTROL_PER_PERIOD];
  double lpf = 0;

}
void UserProcess::init(char *IOmap) {
  user::i = 0;
  user::cycle = 0;
  *(unsigned short *)(IOmap+USER_IO_PWM_ON) = (1<<0);
  *(unsigned short *)(IOmap+USER_IO_DO) = 0;
  for (int i = 0; i < CONTROL_PER_PERIOD; i++) {
    user::ref[i] = (short)((CONTROL_CYCLE/2)*sin(2.0*M_PI*(double)i/CONTROL_PER_PERIOD));
  }
}

void UserProcess::run(char *IOmap) {
  short vin = *(unsigned short *)(IOmap+USER_IO_AD0) - (AD_RANGE>>1);
  double iin = *(unsigned short *)(IOmap+USER_IO_AD1) - (AD_RANGE>>1);
  double gain = 1.0;
  if (user::cycle == 1) {
    *(unsigned short *)(IOmap+USER_IO_DO) = (1<<0);
    gain = (double)user::i / CONTROL_PER_PERIOD;
  }
  double diff = 0;
  if (user::cycle > 0) {
    user::lpf += (iin - user::lpf)/5.0;
    diff = (-(double)vin - user::lpf);
  }
  double pwm = (CONTROL_CYCLE / V_DC()) * (2.0 / AD_RANGE) * ((double)vin + gain * 5.0 * (diff));

  if (pwm >= (CONTROL_CYCLE>>1)) {
    pwm = (CONTROL_CYCLE>>1) -1;
  } else if (pwm < -(CONTROL_CYCLE>>1)) {
    pwm = -(CONTROL_CYCLE>>1);
  }
  pwm += (CONTROL_CYCLE>>1);
  *(unsigned short *)(IOmap+USER_IO_PWM0) = pwm;

  user::i++;
  if (user::i >= CONTROL_PER_PERIOD) {
    user::cycle++;
    user::i = 0;
  }
}
