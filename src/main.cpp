#include <stdio.h>
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
      mVin = 0;
      for (int i = 0; i < CONTROL_CYCLE; i++) {
        for (int j = 0; j < LOG_SLOT_SIZE; j++) {
          logdata[i][j] = 0;
        }
      }
    }
    void run() {
      mVin += TAU() * (mPWM[0]*V_DC() - mVin);
      logdata[mCycle][0] = mVin;
      mCycle++;
      if (mCycle >= CONTROL_CYCLE) {
        mCycle = 0;
        print();
      }
    }
    double sensor(int port) const {
      switch(port) {
        case 0:
          return mVin;
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
    int mCycle;
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
};

#include "common.cpp"
#include "UserProcess.cpp"

