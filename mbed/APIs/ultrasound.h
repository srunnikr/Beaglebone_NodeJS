#ifndef ULTRASOUND
#define ULTRASOUND

// Ultrasound class
// It supports both polled as well as interrupt driven IO using the same library.
class ultrasound {
    // Private variables in the class
    private:
        Timer t;
        float distance; 
        DigitalOut triggerPin;
        DigitalIn echoPinPolled;
        
    public:
        // Public elements of the class
        InterruptIn echoPin;
        ultrasound(PinName trigger, PinName echo);
        float get_ultrasound_distance_cm();
        float get_ultrasound_distance_cm_polled();
        void  echo_rise_handler(void);
        void  echo_fall_handler(void);
        void calculate_distance();
};

#endif