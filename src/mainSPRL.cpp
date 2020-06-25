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
      mCycle = 0;
      for (int i = 0; i < CONTROL_CYCLE; i++) {
        for (int j = 0; j < LOG_SLOT_SIZE; j++) {
          logdata[i][j] = 0;
        }
      }
      mIntCount = 0;
      mVin = 0;
      mIout = 0;
      mDO = 0;
    }
    void run() {
      mVin = sqrt(2.0)*sin(2.0*M_PI*(double)(mCycle+mIntCount*CONTROL_CYCLE)/(CONTROL_PER_PERIOD*CONTROL_CYCLE));
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
      logdata[mCycle][0] = sensor(0);
      logdata[mCycle][1] = sensor(1);
      mCycle++;
      if (mCycle >= CONTROL_CYCLE) {
        mCycle = 0;
        print();
        mIntCount++;
        if (mIntCount >= CONTROL_PER_PERIOD) {
          mIntCount = 0;
        }
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
  private:
    int mPWM[USER_IO_PWM_SIZE];
    unsigned short mDI;
    unsigned short mDO;
    int mCycle;
    int mIntCount;
    double logdata[CONTROL_CYCLE][LOG_SLOT_SIZE];
    void print() const {
      for (int i = 0; i < CONTROL_CYCLE; i++) {
        for (int j = 0; j < LOG_SLOT_SIZE; j++) {
          printf("%lf ", logdata[i][j]);
        }
        printf("\n");
      }
    }
    double mVin;
    double mIout;
};

#include "common.cpp"
#include "UserProcessSPRL.cpp"

