#include "mbed.h"
#include "lm303dlhc.h"

Serial ser(USBTX,USBRX);

lm303dlhc::lm303dlhc(PinName sda, PinName scl): sensor(sda, scl) {
    
    // Initial sensor configuration done here
    // I2C write function returns 0 on aknowledgement
    
    sensor.frequency(400000);
    
    char cmd[2];
    cmd[0] = CTRL_REG1_A;
    cmd[1] = CNTRL_REG1_LOW_POW_5_37KHZ_NORM_1_34KHZ;
    if(sensor.write(ADDR_ACC_WRITE, cmd, 2))
        ser.printf("Not acknowledged\r\n");
    
    cmd[0] = CTRL_REG4_A;
    cmd[1] = CNTRL_REG4_LSBLOWER_4G_HIGHRES;
    if(sensor.write(ADDR_ACC_WRITE, cmd, 2))
    ser.printf("Not acknowledged\r\n"); 
    
    // initialize mag
    //Configuring the Mag sensor
    cmd[0] =  CRA_REG_M;
    cmd[1] = 0x0c; // 15Hz 
    if(sensor.write(addr_mag_write, cmd, 2)) 
        ser.printf("Not acknowledged\r\n");
    
    cmd[0] = CRB_REG_M;
    cmd[1] = 0xe0;      // +- 8
    if(sensor.write(addr_mag_write, cmd, 2))
        ser.printf("Not acknowledged\r\n");
    
    cmd[0] = MR_REG_M;
    cmd[1] = 0x00;       // constinuous mode 
    if(sensor.write(addr_mag_write, cmd, 2))
        ser.printf("Not acknowledged\r\n"); 
      
}

float lm303dlhc::get_acc_x_reading() {
    
    char data_lsb = 0x00, data_msb = 0x00;
    float x_reading = 0;
    
    char subadd[1] = {OUT_X_L_A};
    sensor.write(ADDR_ACC_WRITE, subadd, 1, true);
    if(sensor.read(ADDR_ACC_READ, &data_lsb, 1))
        ser.printf("read not acknowledged\r\n");
    
    subadd[0] = OUT_X_H_A;    
    sensor.write(ADDR_ACC_WRITE, subadd, 1, true);
    if(sensor.read(ADDR_ACC_READ, &data_msb, 1))
        ser.printf("read not acknowledged\r\n");
        
    x_reading = float( data_msb<<8 | data_lsb );
    
    return x_reading;
    
}

float lm303dlhc::get_acc_y_reading() {
    
    char data_lsb = 0x00, data_msb = 0x00;
    float y_reading = 0;
    
    char subadd[1] = {OUT_Y_L_A};
    sensor.write(ADDR_ACC_WRITE, subadd, 1, true);
    if(sensor.read(ADDR_ACC_READ, &data_lsb, 1))
        ser.printf("read not acknowledged\r\n");
    
    subadd[0] = OUT_Y_H_A;    
    sensor.write(ADDR_ACC_WRITE, subadd, 1, true);
    if(sensor.read(ADDR_ACC_READ, &data_msb, 1))
        ser.printf("read not acknowledged\r\n");
        
    y_reading = float( data_msb<<8 | data_lsb );
    
    return y_reading;
    
}

float lm303dlhc::get_acc_z_reading() {
 
    char data_lsb = 0x00, data_msb = 0x00;
    float z_reading = 0;
    
    char subadd[1] = {OUT_Z_L_A};
    sensor.write(ADDR_ACC_WRITE, subadd, 1, true);
    if(sensor.read(ADDR_ACC_READ, &data_lsb, 1))
        ser.printf("read not acknowledged\r\n");
    
    subadd[0] = OUT_Z_H_A;    
    sensor.write(ADDR_ACC_WRITE, subadd, 1, true);
    if(sensor.read(ADDR_ACC_READ, &data_msb, 1))
        ser.printf("read not acknowledged\r\n");
        
    z_reading = float( data_msb<<8 | data_lsb );
    
    return z_reading; 
   
}

vector<float> lm303dlhc::get_acc_reading() {

    vector<float> reading;
    
    reading.push_back(get_acc_x_reading());
    reading.push_back(get_acc_y_reading());
    reading.push_back(get_acc_z_reading());
    
    return reading;
    
}

vector<float> lm303dlhc::get_mag_reading() {
    
    char data_x_lsb = 0x04, data_x_msb = 0x03;
    char data_y_lsb = 0x08, data_y_msb = 0x07;
    char data_z_lsb = 0x06, data_z_msb = 0x05;
    
    float x_reading = 0;
    float y_reading = 0;
    float z_reading = 0;
    
    char subaddx[1] = {OUT_X_L_M};
    char subaddy[1] = {OUT_Y_L_M};
    char subaddz[1] = {OUT_Z_L_M};
    
    sensor.write(addr_mag_write, subaddx, 1, true);
    if(sensor.read(addr_mag_read, &data_x_lsb, 1))
        ser.printf("read not acknowledged\r\n");
        
    sensor.write(addr_mag_write, subaddy, 1, true);
    if(sensor.read(addr_mag_read, &data_y_lsb, 1))
        ser.printf("read not acknowledged\r\n");
        
    sensor.write(addr_mag_write, subaddz, 1, true);
    if(sensor.read(addr_mag_read, &data_z_lsb, 1))
        ser.printf("read not acknowledged\r\n");   
        
    subaddx[0] = OUT_X_H_M;
    subaddy[0] = OUT_Y_H_M;
    subaddz[0] = OUT_Z_H_M;
    
    sensor.write(addr_mag_write, subaddx, 1, true);
    if(sensor.read(addr_mag_read, &data_x_msb, 1))
        ser.printf("read not acknowledged\r\n");
        
    sensor.write(addr_mag_write, subaddy, 1, true);
    if(sensor.read(addr_mag_read, &data_y_msb, 1))
        ser.printf("read not acknowledged\r\n");
        
    sensor.write(addr_mag_write, subaddz, 1, true);
    if(sensor.read(addr_mag_read, &data_z_msb, 1))
        ser.printf("read not acknowledged\r\n");   
        
    
    
    vector<float> reading;
        
    x_reading = float( data_x_msb<<8 | data_x_lsb );
    y_reading = float( data_y_msb<<8 | data_y_lsb );
    z_reading = float( data_z_msb<<8 | data_z_lsb );
    
    //s.printf("X : %f Y : %f Z : %f\r\n", x_reading, y_reading, z_reading );

    reading.push_back(x_reading);
    reading.push_back(y_reading);
    reading.push_back(z_reading);
    
    return reading;
    
}

float lm303dlhc::vector_dot_product(vector<float>a ,vector<float> b) {

    float dot_result = 0;
    
    std::vector<float>::const_iterator a_it;
    std::vector<float>::const_iterator b_it;
    for(a_it = a.begin(), b_it = b.begin(); (a_it != a.end()) && (b_it != b.end()); ++a_it, ++b_it) {
        dot_result += ( (*a_it) * (*b_it) );    
    }   
    
    return dot_result;
    
}

vector<float> lm303dlhc::vector_cross_product(vector<float>a ,vector<float> b) {

    vector<float> result;
    
    result.push_back( (a[1] * b[2]) - (a[2] * b[1]) );
    result.push_back( (a[2] * b[0]) - (a[0] * b[2]) );
    result.push_back( (a[0] * b[1]) - (a[1] * b[0]) );  
    
    return result; 
    
}

vector<float> lm303dlhc::vector_normalize(vector<float>a) {
    
    float mag = vector_dot_product(a, a);
    a[0] = a[0]/mag;
    a[1] = a[1]/mag;
    a[2] = a[2]/mag;
    
    return a;
    
}

float lm303dlhc::get_heading( vector<float> from )
{ 

    vector<float> mag = get_mag_reading();
    float x = mag[0], y = mag[1];
    float y_offset = 10;
    
    float heading = 0;
    
    if ( (x>65000) && ( y < 91 ) ) {
        
        // in NE quadrant
        heading = 90-y-y_offset;    
        
    } else if ( ( y > 65000 && x > 65000 ) && (y > x) ) {
        
        // ES 1st part
        heading = (90 + (65535-y-y_offset) );    
    } else if ( (y > 65000 && x > 65000 ) && (x > y) ) {
        
        // ES 2nd part
        heading = (90 + (65535-y-y_offset) );    
    } else if ( (x > 0 && x < 65000) && ( y > 65000 ) ) {
        
        // SW part    
        heading = (180 + x);
    } else if ( (x > 0 && x < 65000) && ( y > 0 && y < 65000 ) ) {
        
        //WN part
        heading = 270+y;
    }

    if ( heading > 360 ) {
        heading = 0;
    }
    
    //ser.printf(" X : %f Y : %f\r\n",x,y);
    
    return heading;
    
}