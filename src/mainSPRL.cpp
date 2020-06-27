#include <stdio.h>
#include <math.h>
#include "UserProcess.hpp"
#include "Parameter.hpp"

class Circuit {
  public:
    void setPWM(int port, int ref) {
      mPWM[port] = ref;
    }
    void setDI(unsigned short in) {
      mDI = in;
    }
    void init() {
      mSrcCycle = 0;
      mVin = 0;
      mIout = 0;
      mDO = 0;
    }
    void run() {
      mVin = sqrt(2.0)*sin(2.0*M_PI*((double)mSrcCycle/(CONTROL_PER_PERIOD*CONTROL_CYCLE)));
      if (((mDO & (1<<0)) == 0) || (mPWM[0] == 0)) {
        mIout = 0;
      } else {
        mIout += CONNECT_RL() * ((mPWM[0]*V_DC() - mVin) - mIout);
      }
      if (mDI & (1<<0)) {
        mDO = (1<<0);
      } else if (mDO & (1<<0)) {
        if (fabs(mIout) < 0.01) {
          mDO = 0;
        }
      }
      mSrcCycle++;
      if (mSrcCycle >= (CONTROL_PER_PERIOD*CONTROL_CYCLE)) {
        mSrcCycle = 0;
      }
    }
    double sensor(int port) const {
      switch(port) {
        case 0:
          return mVin;
        case 1:
          return mIout * CONNECT_GAIN();
        default:
          return 0;
      }
    }
    unsigned short DO() const {
      return 0;
    }
    double logger(int port) const {
      switch(port) {
        case 0:
          return sensor(port);
        case 1:
          return sensor(port);
        default:
          return 0;
      }
    }
  private:
    int mPWM[USER_IO_PWM_SIZE];
    unsigned short mDI;
    unsigned short mDO;
    int mSrcCycle;
    double mVin;
    double mIout;
};

#include "common.cpp"
#include "UserProcessSPRL.cpp"

