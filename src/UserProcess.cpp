#include "Parameter.hpp"
#include "UserProcess.hpp"
#include <math.h>
namespace user {
  int i;
  int cycle;
  short ref[CONTROL_PER_PERIOD];

}
void UserProcess::init(char *IOmap) {
  user::i = 0;
  user::cycle = 0;
  *(unsigned short *)(IOmap+USER_IO_PWM_ON) = (1<<0);
  for (int i = 0; i < CONTROL_PER_PERIOD; i++) {
    user::ref[i] = (short)((CONTROL_CYCLE/2)*sin(2.0*M_PI*(double)i/CONTROL_PER_PERIOD));
  }
}

void UserProcess::run(char *IOmap) {
  short pwm = (short)((CONTROL_CYCLE/2)*sin(2.0*M_PI*(double)user::i/CONTROL_PER_PERIOD));
  if (user::cycle == 0) {
    pwm *= (double)user::i / CONTROL_PER_PERIOD;
  }
  pwm += (CONTROL_CYCLE>>1);
  *(unsigned short *)(IOmap+USER_IO_PWM0) = pwm;

  user::i++;
  if (user::i >= CONTROL_PER_PERIOD) {
    user::cycle++;
    user::i = 0;
  }
}
