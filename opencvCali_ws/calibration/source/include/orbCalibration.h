#pragma once

#include <stdint.h>
#include <string.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/types_c.h>


enum CAMERA_MODEL
{
    PINHOLE_RATAN,
    PINHOLE_EQUI,
    PINHOLE_FOV,
    OMNI_NONE,
    OMNI_RADTAN,
    EUCM_NONE,
    DS_NONE,

};

enum CALIBRATION_MODEL
{
    IMU_CAMERA_CALIB,
    SINGLE_CAMERA,
    DEFAULT_OLD_OMNI,
};

typedef struct INTRINSIC_t
{
    // fx fy cx cy + mirror+ distortion params
    float k[9];
    
}INTRINSIC;



typedef struct EXTRINSIC_t
{
    float r[9];
    float t[3];

}EXTRINSIC;

typedef struct orb_calib_cfg_t
{
    int cameraModel; // seven camera model
    int calibType; // imu-camera or visio
    INTRINSIC orb_intrinsic1;
    EXTRINSIC orb_extrinsic1;
    INTRINSIC orb_intrinsic2;
    EXTRINSIC orb_extrinsic2;
}orb_calib_cfg;


typedef struct IMU_NOISE_WALK_INFO_t
{
    float NoiseGyro;
    float NoiseAcc;
    float GyroWalk;
    float AccWalk;
}IMU_NOISE_INFO;

typedef struct IMU_TIMESHIFT_t
{
    float timeShift_cam1_imu;// base imu
    float timeShift_cam2_imu;// base imu
}IMU_TIMESHIFT;
typedef struct IMU_CALIB_INFO_t
{
    int imuFreq;
    IMU_TIMESHIFT imu_timeshift;
    IMU_NOISE_INFO imu_noise;
}IMU_CALIB_INFO;

typedef struct ORB_CALIB_CFG_ALL_t
{
    int cameraModel; // seven camera model
    int calibType; // imu-camera or visio
    float camera_bf; // fx * baseline
    float P1[12];// rows:3 cols:4 base camera1
    float P2[12];// rows:3 cols:4 base camera2
    INTRINSIC orb_intr1;
    // EXTRINSIC orb_extr_trl;
    EXTRINSIC orb_extr_tbc;
    INTRINSIC orb_intr2;
    EXTRINSIC orb_extr_tlr;
    EXTRINSIC orb_extr_tbc2;
    IMU_CALIB_INFO imu_calib_info;
}ORB_CALIB_CFG_ALL;




typedef struct OPENCV_CALIB_RESULT_t
{
    float K1[9];
    float D1[4];// rows 4 cols 1
    float P1[12];// rows 3 cols 4
    float R1[9];
    float K2[9];
    float D2[4];// rows 4 cols 1
    float P2[12];// rows 3 cols 4
    float R2[9];
    float T[3];
    float Q[16];
    float R[9];
}OPENCV_CALIB_RESULT;


typedef struct ORB_CALIB_RESULT_t
{
    //input
    cv::Mat K1;//2x3
    cv::Mat K2;
    cv::Mat D1;//3x3
    cv::Mat D2;
    cv::Mat R;
    cv::Mat T;
    //output
    cv::Mat R1;
    cv::Mat R2;
    cv::Mat P1;
    cv::Mat P2;
    cv::Mat prInv1;
    cv::Mat prInv2;
    cv::Mat Q;
    cv::Mat Tlr;

}ORB_CALIB_RESULT;

int get_orb_calib_result(ORB_CALIB_CFG_ALL * orb_cfg_all,char *in_file);
int get_opencv_calib_result(OPENCV_CALIB_RESULT * opencv_calib_params,char *in_file);


