#ifndef FILTER
#define MEDIAN
#endif

#ifndef FILTER_RESOLUTION
#define FILTER_RESOLUTION 4
#endif

#define PI_CONST 3.14159265359
#define MPU_DEFAULT_ADDR 0xD0

#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

#ifdef GYRO
struct gyro_data{
    uint6_t gx,
    gy,
    gz;
};
#endif

struct angle_data{
    uint16_t x,
    y,
    z;
};


class mpu6050{
public:
    mpu6050(uint8_t addr);
    angle_data read_accel();

    #ifdef GYRO
    gyro_data read_gyro();
    #endif
private:
    uint8_t addr;

    void mpu_init();
    void send_command(uint8_t command);
    void read_data();
    void median(uint16_t x);
    void average(uint16_t x);

    struct raw_data{
        int16_t x,
        y,
        z;

        int16_t gx,
        gy,
        gz;
    };
    
};