#include "stepperMotor.h"
#include "mbed.h"
#include <stdio.h>

Serial s(USBTX,USBRX);
// Array for drive seconds
unsigned char stepperMotor::halfDriveValues_C[8] = {0x9, 0x8, 0xC, 0x4, 0x6, 0x2, 0x3, 0x1};
unsigned char stepperMotor::halfDriveValues_A[8] = {0x1, 0x3, 0x2, 0x6, 0x4, 0xC, 0x8, 0x9};
unsigned char stepperMotor::fullDriveValues_C[8] = {0x9, 0xC, 0x6, 0x3};
unsigned char stepperMotor::fullDriveValues_A[8] = {0x3, 0x6, 0xC, 0x9};

// Constructor
stepperMotor::stepperMotor(PinName a, PinName b, PinName c, PinName d):
 bus( a, b, c, d)

 {
    pin_a = a;
    pin_b = b;
    pin_c = c;
    pin_d = d;
    pps = DEFAULT_PPS;
    driveMode = HALF_STEP;
    direction = 'c';
}

// set the pps value here ( change the write interval for values )
void stepperMotor::set_pps(float ppsVal) {
    pps = ppsVal;
    s.printf("PPS SET");
}

// change the drive mode to full and half step
void stepperMotor::set_drive(int driveVal) {
    if ( driveVal == HALF_STEP || driveVal == FULL_STEP ) {
        driveMode =  driveVal;
    }    
}

// This write values for motor bus
void stepperMotor::write_value_motor(int val) {
    int pinArray[] = {pin_a, pin_b, pin_c, pin_d};
    bus = val;
}

// Clockwise rotation
void stepperMotor::shift_motor_clockwise(void){
    
    if (stepperMotor::driveMode == HALF_STEP)
    {
        write_value_motor(halfDriveValues_C[currentArrayPosition]);
        if (currentArrayPosition < 7 ){
            currentArrayPosition+=1;
        } 
        else
        {
        currentArrayPosition = 0;
        }
    }
    
    else if (stepperMotor::driveMode == FULL_STEP)
    {
        write_value_motor(fullDriveValues_C[currentArrayPosition]);

        if (currentArrayPosition < 3 ){
            currentArrayPosition+=1;
        } 
        else 
        {
        currentArrayPosition = 0;
        }
    }
}

// Anti clockwise rotation
void stepperMotor::shift_motor_anticlockwise(void){
    
    if (stepperMotor::driveMode == HALF_STEP)
    {
        write_value_motor(halfDriveValues_A[currentArrayPosition]);
        if (currentArrayPosition < 7 ){
            currentArrayPosition+=1;
        } 
        else
        {
        currentArrayPosition = 0;
        }
    }
    
    else if (stepperMotor::driveMode == FULL_STEP)
    {
        write_value_motor(fullDriveValues_A[currentArrayPosition]);

        if (currentArrayPosition < 3 ){
            currentArrayPosition+=1;
        } 
        else 
        {
        currentArrayPosition = 0;
        }
    }
}

// Move the motor!
void stepperMotor::move_motor(int numOfSteps, int stepMode){
    float ppsInterval = (1.0/(float)pps);
    int stepsMoved = 0;
    currentArrayPosition = 0;
    stepperMotor::driveMode = stepMode;   
    timer.detach();

    while(stepsMoved < numOfSteps){
        if( stepperMotor::direction == 'c'){
            timer.detach();
            timer.attach( this, &stepperMotor::shift_motor_clockwise , ppsInterval);
        }
        else if ( stepperMotor::direction == 'a'){
            timer.detach();
            timer.attach( this, &stepperMotor::shift_motor_anticlockwise , ppsInterval);
        }
        s.printf("Working! \r\n");
        ++stepsMoved;    
    }
    timer.detach();
}



