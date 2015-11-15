#include "radar.h"
#include <string>

#define MOTOR_TASK 1
#define ULTRASOUND_TASK 2
#define MAGNETOMETER_TASK 3
#define DISPLAY_TASK 4
#define MAGNETOMETER_TASK_NEW 5
#define MOTOR_TASK_NEW 6
#define MAGNETOMETER_TASK_NEW_NEW 7
#define DISPLAY_TASK_NEW 8


using namespace std;

Serial serl(p13, p14);
Serial xt(USBTX, USBRX);

bool serialInterruptReceived = false;
char serialChar;
string degree;
int degreeVal;
string buffer;
bool degreeStart = false;

void killScheduler();
void restartScheduler();

class scheduler {
    
    private:

    radar_system rsys;
    queue<int> task_queue;

public:

    scheduler() {
    }

    void enqueue_task(int task_id) {

        task_queue.push(task_id);

    }
    
    void killScheduler() {
        while(!task_queue.empty()) {
            task_queue.pop();
        }
    }
    
    void restartScheduler() {
        xt.printf("In restart function\r\n");
        enqueue_task(MOTOR_TASK);
    }

    void run_scheduler() {

        while(1) {
            
            if (serialInterruptReceived == true && serialChar == 'A') {
                xt.printf("Inside A handler in scheduler\r\n");
                serialInterruptReceived = false;
                serialChar = 'x';
                restartScheduler();
            }
            
            if (serialInterruptReceived == true && serialChar == 'C') {
                xt.printf("Inside C handler in scheduler\r\n");
                serialInterruptReceived = false;
                serialChar = 'x';
                enqueue_task(MAGNETOMETER_TASK_NEW);
            }
            
            if (serialInterruptReceived == true && serialChar == 'D') {
                xt.printf("Inside D handler in scheduler\r\n");
                serialInterruptReceived = false;
                serialChar = 'x';
                buffer=rsys.returnBuffer();
                xt.printf("buffer: %s\r\n", buffer);
                serl.printf("%s\n", buffer);
            }


            while(!task_queue.empty()) {

                int task = task_queue.front();

                switch(task) {
                    case MOTOR_TASK:
                        xt.printf("Inside motor task in scheduler");
                        rsys.motor_task();
                        task_queue.push(ULTRASOUND_TASK); 
                        task_queue.push(MAGNETOMETER_TASK);
                        break;
                    case ULTRASOUND_TASK:
                        xt.printf("Inside ultrasound task in scheduler");
                        rsys.ultrasound_task();
                        break;
                    case MAGNETOMETER_TASK:
                        xt.printf("Inside magnetometer task in scheduler");
                        rsys.magnetometer_task();
                        task_queue.push(DISPLAY_TASK);
                        break;
                    case DISPLAY_TASK:
                        xt.printf("Inside display task in scheduler");
                        rsys.ledDisplay_task();
                        task_queue.push(MOTOR_TASK);
                        break;
                        
                    //Scan at heading scenario    
                    case MAGNETOMETER_TASK_NEW:
                    xt.printf("Inside mag new task in scheduler");
                        rsys.magnetometer_task();
                        task_queue.push(MOTOR_TASK_NEW);
                        break;
                    case MOTOR_TASK_NEW:
                     xt.printf("Inside motor new task in scheduler");
                        rsys.motor_task_new(degreeVal);  
                        task_queue.push(ULTRASOUND_TASK); 
                        task_queue.push(MAGNETOMETER_TASK_NEW_NEW);
                        break;  
                    case MAGNETOMETER_TASK_NEW_NEW:
                     xt.printf("Inside mag new new task in scheduler");
                        rsys.magnetometer_task();
                        task_queue.push(DISPLAY_TASK_NEW);
                        break;    
                    case DISPLAY_TASK_NEW:
                     xt.printf("Inside displ  new task in scheduler");
                        rsys.ledDisplay_task();
                        task_queue.push(MOTOR_TASK_NEW);
                        break;    
                    default:
                        break;
                }

                task_queue.pop();
                
                // Check for serial interrupt and handle the same
                if(serialInterruptReceived) {
                    xt.printf("Inside serial interrupt handler\r\n");
                    if (serialChar == 'B') {
                        // Remove the scheduler queue scheduler tasks
                        serialInterruptReceived = false;
                        serialChar = 'x';
                        killScheduler();
                        //Clear the serial interrupt and set default value for serialChar
                    }
                }
            }
        }
    }

};
scheduler sh;

void callback()
{  
    if(serl.readable()) {
        serialChar = serl.getc();
        if ( serialChar == 'C' ) {
            degreeStart = true;
        }
        // Getting heading
        if(degreeStart && serialChar != 'E' && serialChar != 'C' && serialChar != 'A' && serialChar != 'B' && serialChar != 'D' ) {
            degree += serialChar; 
            printf("degree: %d\r\n", degree);   
        }
        if (serialChar == 'E') {
            xt.printf("%s\r\n",degree);
            degreeVal = atoi(degree.c_str());
            xt.printf("Integer %d\r\n", degreeVal);
            serialInterruptReceived = true;
            degreeStart = false;
            degree = "";
            serialChar = 'C';
        }
        
        if (serialChar == 'A' || serialChar == 'B' || serialChar == 'D'){ 
        serialInterruptReceived = true;}
        
    }
    xt.printf("Received %c from serial input via BBB\r\n", serialChar);
}


int main() {
    serl.attach(&callback);
    sh.enqueue_task(MOTOR_TASK);
    sh.run_scheduler(); 
    while(1) {
    }
}