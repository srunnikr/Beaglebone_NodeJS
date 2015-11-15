#include "radar.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

Serial st(USBTX,USBRX);


class I2CPreInit : public I2C
{
public:
    I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl)
    {
        frequency(400000);
        start();
    };
};

I2CPreInit gI2C(p9,p10);


      
     radar_system::radar_system():motor(p21, p22, p23, p24), ultra(p30, p6), compass(p28,p27), oled(gI2C, p11) {
         
         angle_counter = 0.0;
         flag = 0;
         counter = 0;
         angle_counter_new=0;
         sweep_index_clockwise = 0; sweep_index_anticlockwise = 0;
         sweep_x_clockwise[0] = 0; sweep_x_clockwise[1] = 0; sweep_x_clockwise[2] = 0;
         sweep_x_clockwise[3] = 30; sweep_x_clockwise[4] = 60; sweep_x_clockwise[5] = 90;
         sweep_x_clockwise[6] = 127; sweep_x_clockwise[7] = 127; sweep_x_clockwise[8] = 127;
         
         sweep_y_clockwise[0] = 40; sweep_y_clockwise[1] = 25; sweep_y_clockwise[2] = 0;
         sweep_y_clockwise[3] = 0; sweep_y_clockwise[4] = 0; sweep_y_clockwise[5] = 0;
         sweep_y_clockwise[6] = 0; sweep_y_clockwise[7] = 25; sweep_y_clockwise[8] = 40;
        
         sweep_x_anticlockwise[0] = 127; sweep_x_anticlockwise[1] = 127; sweep_x_anticlockwise[2] = 127;
         sweep_x_anticlockwise[3] = 90; sweep_x_anticlockwise[4] = 60; sweep_x_anticlockwise[5] = 30;
         sweep_x_anticlockwise[6] = 0; sweep_x_anticlockwise[7] = 0; sweep_x_anticlockwise[8] = 0;
         
         sweep_y_anticlockwise[0] = 40; sweep_y_anticlockwise[1] = 25; sweep_y_anticlockwise[2] = 0;
         sweep_y_anticlockwise[3] = 0; sweep_y_anticlockwise[4] = 0; sweep_y_anticlockwise[5] = 0;
         sweep_y_anticlockwise[6] = 0; sweep_y_anticlockwise[7] = 25; sweep_y_anticlockwise[8] = 40;
         
         //Initialize the obstacle buffer
         clear_display_buffer();
    }

    void radar_system::clear_display_buffer() {
         
        for(int i=0; i<180; ++i) {
             x_buffer[i] = y_buffer[i] = 0;
        }
     
    }
     //************************************ MOTOR: 180 *********************************
     void radar_system::motor_task(void) {
        
        st.printf("In motor task\r\n");
        ultrasound_complete = false;
        magnetometer_complete = false;
        angle_counter++;      
        motor.move_motor(20,HALF_STEP); 
        if(angle_counter == 51.0 && motor.direction == 'c') {
            clear_display_buffer();
            motor.direction = 'a'; angle_counter = 0;
            st.printf("Motor direction was c now %c\r\n", motor.direction);
        } else if(angle_counter == 51.0 && motor.direction == 'a') {
            clear_display_buffer();
            motor.direction = 'c'; angle_counter = 0;
            st.printf("Motor direction was a now %c\r\n", motor.direction);
        }   
        st.printf("angle_counter : %f, direction : %c\r\n",angle_counter, motor.direction);
        
        ultrasound_complete = magnetometer_complete = false;
        
    } 
    
     //************************************ MOTOR:15 deg *********************************
     void radar_system::motor_task_new(int req_heading) {
         st.printf("in function");
         if ( !((curr_heading+2) > req_heading > (curr_heading-2)) && flag == 0 ){  // If not in this range
         motor.move_motor(20,HALF_STEP); 
            if (curr_heading > req_heading){
                motor.direction = 'a';
                }
            else{
                motor.direction = 'c';
                }                       
         }
         
         else if( (curr_heading+2) > req_heading &&  req_heading > (curr_heading-2) ){
             if(counter == 0) {
                 counter++;
                 if (motor.direction == 'c' ){  
                    motor.direction = 'a';     }   
                 else {
                    motor.direction = 'c';
                       }  
             }
             
             motor.move_motor(20,HALF_STEP);
             flag = 1;
             angle_counter_new++;
             if (curr_heading == 30 && motor.direction == 'a' ){  
                motor.direction = 'c';    
                angle_counter_new = 0;  }   
             else if (angle_counter_new == 30 && motor.direction == 'c'){ 
                motor.direction = 'a';
                 angle_counter_new = 0;  }  
             }
         ultrasound_complete = magnetometer_complete = false;
          
         }
         
     //*********************************** ULTRASOUND *******************************
     void radar_system::ultrasound_task(void) { 
 
        float distanceRange = 1500;  

        if(ultra.get_ultrasound_distance_cm() < distanceRange) 
            curr_distance = ultra.get_ultrasound_distance_cm();     
        st.printf("Distance via interrupt : %f cm\r\n",curr_distance);

        ultrasound_complete = true;
            
    }
    void radar_system::get_ultra_distance(void) {
        
        Timer t;
        float distance =0;
        DigitalOut triggerPin(p30);
        DigitalIn echoPin(p6);
        
        // make the pin stable and send a pulse
        triggerPin = 0;
        wait_us(10);
        // Send the trigger pulse with 10us duration
        triggerPin = 1;
        wait_us(10);    
        triggerPin = 0;
        
        while(!echoPin) {
        }
        
        t.start();
        
        while(echoPin) {
        }
        
        // Calculate the result
        t.stop();
        distance = (t.read_us() * .034) / (float)2;
        st.printf("Distance : %f\r\n",distance);
        t.reset();
        
        ultrasound_complete = true;
        curr_distance = distance;
        st.printf("curr distance : %f\r\n",radar_system::curr_distance);
 
    }
          
     //*********************************** MAGNETOMETER *******************************  
     void radar_system::magnetometer_task(void) { 
        
        vector<float> head;
        head.push_back(0);
        head.push_back(-1);
        head.push_back(0);
        st.printf("In the magnetometer task \r\n");
        curr_heading = compass.get_heading(head);
        st.printf("HEADING: %f N\r\n", curr_heading);
        
        magnetometer_complete = true;
        
    }
     
     //*********************************** LED_DISPLAY *******************************   
    void radar_system::setRowColumn(int row, int col) {
        // Col is 5px wide, row is 8 px wide 
        oled.setTextCursor( (col * 5), (row * 8) ); 
    }
        
    void radar_system::draw_radargrid() {
        
        // Display the radar grid
        oled.drawCircle(63, 127, 90, 1);
        oled.drawCircle(63, 127, 125, 1);
        
    }
    
    void radar_system::draw_directionbox() {
        // This draws the direction box on the top right corner of display
        oled.drawRect(99,0,27,10,1);
        setRowColumn(0,19);
        //setRowColumn(0,0);
        oled.printf("%d N",(int)curr_heading);    
    }
    
    void radar_system::ledDisplay_task(void) {
    
        //wait till the conversions are complete
        //while( !(ultrasound_complete == true && magnetometer_complete == true) ) {
        //}
        st.printf("Inside LED\r\n");
        // write grid info onto display buffer
        oled.clearDisplay();
        st.printf("Cleared LED\r\n");
        draw_radargrid();
        draw_directionbox();
        // calculate sweep line and write to buffer
        if(motor.direction=='c') {
            ++sweep_index_clockwise;
            oled.drawLine(63,63, sweep_x_clockwise[sweep_index_clockwise], sweep_y_clockwise[sweep_index_clockwise],1);
        } else {
            ++sweep_index_anticlockwise;
            oled.drawLine(63,63, sweep_x_anticlockwise[sweep_index_anticlockwise], sweep_y_anticlockwise[sweep_index_anticlockwise],1);     
        }
        
        // update the object buffer
        if(angle_counter/2 < 90) {
            // Scaling the display within the minimum bound
            // from center point the min distance to screen edges is 63 px
            x_obstacle = curr_distance * sin(angle_counter/2) * 0.63;
            y_obstacle = curr_distance * cos(angle_counter/2) * 0.63;
            st.printf("angle_counter : %f\r\n",angle_counter);
            st.printf("x_obs : %f y_obs : %f\r\n",x_obstacle,y_obstacle);
            y_obstacle = 63 - y_obstacle;
            x_obstacle = 63 - x_obstacle;
            st.printf("x_obs : %f y_obs : %f\r\n",x_obstacle,y_obstacle);
            x_buffer[(int)(angle_counter/2)] = x_obstacle;
            y_buffer[(int)(angle_counter/2)] = y_obstacle;
            //oled.fillCircle(x_obstacle, y_obstacle, 2, 1);
        } else if(angle_counter/2 == 90) {
            x_obstacle = 63;
            y_obstacle = curr_distance * 0.63;
            x_buffer[(int)(angle_counter/2)] = x_obstacle;
            y_buffer[(int)(angle_counter/2)] = y_obstacle;
            //oled.fillCircle(x_obstacle, y_obstacle, 2, 1);
        } else {
            x_obstacle = curr_distance * sin(180-(angle_counter/2)) * 0.63;
            y_obstacle = curr_distance * cos(180-(angle_counter/2)) * 0.63;
            y_obstacle = 63 - y_obstacle;
            x_obstacle = 63 + x_obstacle;
            if (angle_counter/2 <180) {
                x_buffer[(int)(angle_counter/2)] = x_obstacle;
                y_buffer[(int)(angle_counter/2)] = y_obstacle;
            }
            //oled.fillCircle(x_obstacle, y_obstacle, 2, 1);
        }
        
        if( (angle_counter/2) < 180 ) {
            for(int i=0; i<(angle_counter/2); ++i) {
                oled.fillCircle(x_buffer[i], y_buffer[i], 2, 1);    
            }
        }
        
        st.printf("Going to display\r\n");
        oled.display();
        st.printf("Displayed\r\n");
        
    }  
    
   string radar_system::returnBuffer() {
    //char A[];
    //char B[];
    std::stringstream out;
     string buffer ="";
     string insidebuf= "";
      for(int i=0; i<sizeof(x_buffer); ++i) {
        if( x_buffer[i] > 0 && x_buffer[i] < 5000 &&  y_buffer[i] > 0 && y_buffer[i] < 5000) {
            std::stringstream out1;
            std::stringstream out2;
            out1 << x_buffer[i];
            buffer += "X:";
            buffer += out1.str();
            buffer += " ";
            
            out2 << y_buffer[i];
            buffer += "Y:";
            buffer += out2.str();
            buffer += ", ";

            st.printf(" X : %d Y: %d\r\n",x_buffer[i],y_buffer[i]); 

        }
      }
      st.printf("buffer: %s\r\n", buffer);
      return buffer;      
       
    }
    
