#include "mpu.hpp"

void mpu6050::average(uint16_t x){
    static uint16_t arr[ARR_LEN];
    for(int i = 0; i < ARR_LEN; i++){
        arr[i] = arr[i+1];
    }
    arr[ARR_LEN - 1] = x;
    uint16_t average = 0;

    for(int i = 0; i < ARR_LEN; i++){
        average +=arr[i];
    }
    return (uint16_t)average/ARR_LEN;
}

void mpu6050::median(uint16_t x){
    static uint16_t buff[3];
    static uint16_t count = 0;
    buff[count] = x;
    if (++count > 2) count = 0;

    uint16_t a = buff[0], b = buff[1], c = buff[2], middle;

    if((a <= b) && (a <= c)){
        middle = (b <= c) ? b : c;
    }else{
        if((b <= a) && (b <= c)){
            middle = (a <= c) ? a : c;
        }else{
            middle = (a <= b) ? a : b;
        }
    }
    return middle;
}

mpu6050::mpu6050(uint8_t addr){
    uint8_t check, data;

    HAL_I2C_Mem_Read(&hi2c1, MPU_DEFAULT_ADDR, WHO_AM_I_REG, 1, &check, 1, 1000);

    if(check == 104){
        data = 0;
        HAL_I2C_Mem_Write(&hi2c1, MPU_DEFAULT_ADDR, PWR_MGMT_1_REG, 1, &data, 1, 1000);

        data = 0x07;
        HAL_I2C_Mem_Write(&hi2c1, MPU_DEFAULT_ADDR, SMPLRT_DIV_REG, 1, &data, 1, 1000);

        data = 0x00;
        HAL_I2C_Mem_Write(&hi2c1, MPU_DEFAULT_ADDR, ACCEL_CONFIG_REG, 1, &data, 1, 1000);

        data = 0x00;
        HAL_I2C_Mem_Write(&hi2c1, MPU_DEFAULT_ADDR, GYRO_CONFIG_REG, 1, &data, 1, 1000);
    }
}

angle_data mpu6050::read_accel(){
    angle_data result;
    uint8_t data[6];
    HAL_I2C_Mem_Read (&hi2c1, MPU_DEFAULT_ADDR, ACCEL_XOUT_H_REG, 1, data, 6, 1000);

    raw_data.x = (int16_t)(data[0] << 8 | data [1]);
    raw_data.y = (int16_t)(data[2] << 8 | data [3]);
    raw_data.z = (int16_t)(data[4] << 8 | data [5]);

    result.x = raw_data.x/16384.0;
    result.y = raw_data.y/16384.0;
    result.z = raw_data.z/16384.0;

    #ifdef MEDIAN
    result.x = median(result.x);
    result.y = median(result.y);
    result.z = median(result.z);
    #endif
}

#ifdef GYRO
gyro_data mpu6050::read_gyro(){
    gyro_data result;
    uint8_t Rec_Data[6];

    HAL_I2C_Mem_Read (&hi2c1, MPU_DEFAULT_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6, 1000);

    raw_data.gx = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
    raw_data.gy = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
    raw_data.gz = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

    result.gx = raw_data.gx/131.0;
    result.gy = raw_data.gy/131.0;
    result.gz = raw_data.gz/131.0;
}
#endif