#include "mbed.h"
#include "ultrasound.h"

ultrasound::ultrasound(PinName trigger, PinName echo): triggerPin(trigger), echoPinPolled(echo), echoPin(echo){
    // Intitalize class variables
    distance = 0;
    // Reset the timer when object instantiates
    t.stop();
    t.reset();   
}

float ultrasound::get_ultrasound_distance_cm_polled() {
    
    // Make distance 0 at start
    distance = 0;
    
    // Reset timer for distance measurement
    t.reset();
    
    // Make the trigger pin low for clear pulse transmission
    triggerPin = 0;
    wait_us(10);
    
    // Send the trigger pulse with 10us duration
    triggerPin = 1;
    wait_us(10);    
    triggerPin = 0;
    
    // Wait for the echo pin to go high
    while(!echoPinPolled) {
    }
    
    // Start timer and wait for echo to go low
    t.start();
    
    while(echoPinPolled) {
    }
    
    // Calculate the result
    t.stop();
    distance = (t.read_us() * .034) / (float)2;
    t.reset();
    
    // Return the calculated distance
    return distance;
    
}

float ultrasound::get_ultrasound_distance_cm() {
    
    // initiate the sensor calculation
    calculate_distance();
    
    // Once it is done, the distance would be logged in the class variable by the
    // interrupt handler function. so return that value
    return distance;
    
}

void ultrasound::calculate_distance() {
 
     // Attaching the rising and falling interrupts for echoPin
    echoPin.rise(this,&ultrasound::echo_rise_handler);
    echoPin.fall(this,&ultrasound::echo_fall_handler);
    
    // Sending the trigger pulse
    triggerPin = 1;
    wait_us(10);
    triggerPin = 0;  
    
}

void ultrasound::echo_rise_handler(void) {
    // On the rise start the timer
    t.start();
}

void ultrasound::echo_fall_handler(void) {
    // On the fall of echo pin, stop the timer, calculate the distance
    t.stop();
    distance = (t.read_us() * .034) / (float)2;
    t.reset();
}
 