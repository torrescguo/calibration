#pragma once
#include <stdint.h>


#define WIDTH 640
#define HEIGHT 400

#define CAM_NUMS 2

typedef struct imu_saveData_t
{
    uint8_t imu_raw[60];
    int p1;
    double t;
    uint64_t imuTimestampNs;
}imu_saveData;


