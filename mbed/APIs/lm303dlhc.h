#ifndef LM303DLHC
#define LM303DLHC

#include <vector>

#define M_PI 3.14159265358979323846

// Address of accelerometer registers
#define ADDR_ACC_READ   0x33
#define ADDR_ACC_WRITE  0x32

// Accelerometer control register addresses
#define CTRL_REG1_A 0x20
#define CTRL_REG4_A 0x23

// Accelerometer control configurations
#define CNTRL_REG1_1HZ              0x17
#define CNTRL_REG1_10HZ             0x27
#define CNTRL_REG1_25HZ             0x37
#define CNTRL_REG1_50HZ             0x47
#define CNTRL_REG1_100HZ            0x57
#define CNTRL_REG1_200HZ            0x67
#define CNTRL_REG1_400HZ            0x77
#define CNTRL_REG1_LOW_POW_1_62KHZ  0x87
// The following mode: for low_power mode runs 5.376KHz, normal 1.344KHz
#define CNTRL_REG1_LOW_POW_5_37KHZ_NORM_1_34KHZ 0x97

#define CNTRL_REG4_LSBLOWER_2G_HIGHRES      0x08
#define CNTRL_REG4_LSBLOWER_4G_HIGHRES      0x18
#define CNTRL_REG4_LSBLOWER_8G_HIGHRES      0x28
#define CNTRL_REG4_LSBLOWER_16G_HIGHRES     0x38

#define CNTRL_REG4_LSBHIGHER_2G_HIGHRES     0x48
#define CNTRL_REG4_LSBHIGHER_4G_HIGHRES     0x58
#define CNTRL_REG4_LSBHIGHER_8G_HIGHRES     0x68
#define CNTRL_REG4_LSBHIGHER_16G_HIGHRES    0x78

// Accelerometer data addresses
#define OUT_X_L_A   0x28
#define OUT_X_H_A   0x29
#define OUT_Y_L_A   0x2A
#define OUT_Y_H_A   0x2B
#define OUT_Z_L_A   0x2C
#define OUT_Z_H_A   0x2D


// Address of magnetometer registers
const int addr_mag_write = 0x3c;
const int addr_mag_read = 0x3d;

//Magnetometer data addresses
enum REG_ADDRS {
    /* --- Mag --- */
    CRA_REG_M   = 0x00,
    CRB_REG_M   = 0x01,
    MR_REG_M    = 0x02,
    OUT_X_H_M   = 0x03,
    OUT_X_L_M   = 0x04,
    OUT_Y_H_M   = 0x07,
    OUT_Y_L_M   = 0x08,
    OUT_Z_H_M   = 0x05,
    OUT_Z_L_M   = 0x06,
};

class lm303dlhc {
    
    private:
        I2C sensor;
    
    public:
        lm303dlhc(PinName sda, PinName scl);
        float get_acc_x_reading();
        float get_acc_y_reading();
        float get_acc_z_reading();
        vector<float> get_acc_reading();
        vector<float> get_mag_reading();
        float vector_dot_product(vector<float>a ,vector<float> b);
        vector<float> vector_cross_product(vector<float>a ,vector<float> b);
        vector<float> vector_normalize(vector<float>a);
        float get_heading( vector<float> from );
};

#endif