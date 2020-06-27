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
      mVin = 0;
    }
    void run() {
      mVin += TAU() * (mPWM[0]*V_DC() - mVin);
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
    double logger(int port) const {
      switch(port) {
        case 0:
          return mVin;
        default:
          return 0;
      }
    }
  private:
    int mPWM[USER_IO_PWM_SIZE];
    unsigned short mDI;
    double mVin;
};

#include "common.cpp"
#include "UserProcess.cpp"

