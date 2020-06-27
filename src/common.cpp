#include <stdio.h>
#include <ctime>
#include "UserProcess.hpp"
#include "Parameter.hpp"

class Timer {
  public:
    Timer() :mTotalTime(0) {
    }
    void start() {
      mStartTime = ::std::clock();
    }
    void stop() {
      mTotalTime += (::std::clock() - mStartTime);
    }
    void print() const {
      fprintf(stderr,"# Simulation TIme[s]: %lf\n",(double)(mTotalTime) / CLOCKS_PER_SEC);
    }
  private:
    std::clock_t mStartTime;
    long mTotalTime;
};
class Logger {
  public:
    void init() {
      mLogCycle = 0;
      for (int i = 0; i < CONTROL_CYCLE; i++) {
        for (int j = 0; j < LOG_SLOT_SIZE; j++) {
          mLogData[i][j] = 0;
        }
      }
    }
    void run(const Circuit &circuit) {
      for (int j = 0; j < LOG_SLOT_SIZE; j++) {
        mLogData[mLogCycle][j] = circuit.logger(j);
      }
      mLogCycle++;
      if (mLogCycle >= CONTROL_CYCLE) {
        mLogCycle = 0;
        for (int i = 0; i < CONTROL_CYCLE; i++) {
          for (int j = 0; j < LOG_SLOT_SIZE; j++) {
            printf("%lf ", mLogData[i][j]);
          }
          printf("\n");
        }
      }
    }
    void flush() {
      for (int i = 0; i < mLogCycle; i++) {
        for (int j = 0; j < LOG_SLOT_SIZE; j++) {
          printf("%lf ", mLogData[i][j]);
        }
        printf("\n");
      }
      mLogCycle = 0;
    }
  private:
    int mLogCycle;
    double mLogData[CONTROL_CYCLE][LOG_SLOT_SIZE];
};
class Controller {
  public:
    void init() {
      mCycle = CONTROL_CYCLE;
      for (int i = 0; i < USER_IO_SIZE; i++) {
        mUserIO[i] = 0;
      }
      mUserProcess.init(mUserIO);
    }
    void run(const Circuit &circuit) {
      if (mCycle >= CONTROL_CYCLE) {
        mCycle = 0;
        unsigned short enable = *((unsigned short *)(mUserIO+USER_IO_PWM_ON));
        mDO = *((unsigned short *)(mUserIO+USER_IO_DO));
        for (int i = 0; i < USER_IO_PWM_SIZE; i++) {
          mPWM_enable[i] = enable & (1<<i);
          mPWM_ref[i] = *((unsigned short *)(mUserIO+USER_IO_PWM0+i*2));
        }
        for (int i = 0; i < USER_IO_ADC_SIZE; i++) {
          *((unsigned short *)(mUserIO+USER_IO_AD0+2*i)) = ADC(circuit.sensor(i));
        }
        *((unsigned short *)(mUserIO+USER_IO_DI)) = circuit.DO();
        timer.start();
        mUserProcess.run(mUserIO);
        timer.stop();
      }
      for (int i = 0; i < USER_IO_PWM_SIZE; i++) {
        if (mPWM_enable[i]) {
          if (mPWM_ref[i] >= mCycle) {
            mPWM_out[i] = 1;
          } else {
            mPWM_out[i] = -1;
          }
        } else {
          mPWM_out[i] = 0;
        }
      }
      mCycle++;
    }
    int PWM(int i) {
      return mPWM_out[i];
    }
    int DO() {
      return mDO;
    }
    void print() const {
      timer.print();
    }
  private:
    Timer timer;
    char mUserIO[USER_IO_SIZE];
    UserProcess mUserProcess;

    int mCycle;
    int mPWM_enable[USER_IO_PWM_SIZE];
    unsigned short mPWM_ref[USER_IO_PWM_SIZE];
    int mPWM_out[USER_IO_PWM_SIZE];
    unsigned short mDO;

    unsigned short ADC(double input) {
      short data = (short)(input/2.0*(AD_RANGE/2))+(AD_RANGE/2);
      if (data < 0) {
        data = 0;
      }
      if (data >= AD_RANGE) {
        data = AD_RANGE-1;
      }
      return (unsigned short)data;
    }
};
class Simulator {
  public:
    void run() {
      circuit.init();
      logger.init();
      controller.init();
      for (int i = 0; i < SIMULATION_CYCLE; i++) {
        circuit.run();
        logger.run(circuit);
        controller.run(circuit);
        for (int j = 0; j < USER_IO_PWM_SIZE; j++) {
          circuit.setPWM(j,controller.PWM(j));
        }
        circuit.setDI(controller.DO());
      }
      logger.flush();
      controller.print();
    }
  private:
    Circuit circuit;
    Logger logger;
    Controller controller;
};
int main(int argc, const char **argv) {
  Simulator sim;
  sim.run();
}

