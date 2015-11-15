#include "mbed.h"
#include "ultrasound.h"
#include "stepperMotor.h"
#include "lm303dlhc.h" 
#include <queue>
#include "Adafruit_SSD1306.h"
#include <vector>
#include <string>

class radar_system {
           
    public: 
     int flag;
     int angle_counter_new;
     float curr_distance;
     float curr_heading;
     float angle_counter;
     int counter;
     int sweep_index_clockwise, sweep_index_anticlockwise;
     int sweep_x_clockwise[9], sweep_y_clockwise[9];
     int sweep_x_anticlockwise[9], sweep_y_anticlockwise[9];
     int x_obstacle, y_obstacle;
     int x_buffer[180], y_buffer[180];
     bool ultrasound_complete, magnetometer_complete;
     stepperMotor motor;
     ultrasound ultra;
     lm303dlhc compass;
     Adafruit_SSD1306_I2c oled; 
           
    radar_system();

    void clear_display_buffer();
     
    void motor_task(void);
    void motor_task_new(int req_heading);   
     //*********************************** ULTRASOUND *******************************
    void ultrasound_task(void);
      
    void get_ultra_distance(void);
          
     //*********************************** MAGNETOMETER *******************************  
    void magnetometer_task(void);
     
     //*********************************** LED_DISPLAY *******************************   
    void setRowColumn(int row, int col);
        
    void draw_radargrid();
    
    void draw_directionbox() ;
    
    void ledDisplay_task(void);
    
    string returnBuffer();    
};