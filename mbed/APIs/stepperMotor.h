#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include "mbed.h"

#define DEFAULT_PPS 100
#define HALF_STEP 0
#define FULL_STEP 1
#define MAX_PPS 500

class stepperMotor {
    
    private:
        BusOut bus;
        float pps;
        Ticker timer;
        int driveMode;
        int currentArrayPosition;
        static unsigned char halfDriveValues_C[8];
        static unsigned char halfDriveValues_A[8];
        static unsigned char fullDriveValues_C[8];
        static unsigned char fullDriveValues_A[8];
        
    public :
        char direction; 
        PinName pin_a, pin_b, pin_c, pin_d;
        stepperMotor(PinName a, PinName b, PinName c, PinName d);
        void set_pps(float ppsVal);
        void set_drive(int driveVal);
        void write_value_motor(int val);
        void move_motor(int numOfSteps, int stepMode);
        void shift_motor_clockwise(void);
        void shift_motor_anticlockwise(void);
        void change_direction(char direction);
        
};

#endif
