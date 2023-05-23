#include <stdint.h>
#include "../include/orbCalibration.h"
#include <opencv2/opencv.hpp>
#include <string.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace cv;



void get_orbcalib_cfg(int calib_method,cv::FileStorage fs,string s_calib_model,ORB_CALIB_CFG_ALL * orb_cfg)
{
   

    //intrinsic
    float k1[9] = {0.0};
    float k2[9] = {0.0};

    //extrinsic
    float t2[3] = {0.0};
    float r2[9] = {0.0};


    // tbc tbc2 
    float t_imu_1[3] = {0.0};
    float t_imu_2[3] = {0.0};
    float r_imu_1[9] = {0.0};
    float r_imu_2[9] = {0.0};

    float camera_bf;
    // imu noise and freq
    IMU_CALIB_INFO imu_calib_info_temp;
    
    imu_calib_info_temp.imuFreq = (int)fs["IMU.Frequency"];
    fs["IMU.NoiseGyro"] >> imu_calib_info_temp.imu_noise.NoiseGyro;
    fs["IMU.NoiseAcc"] >> imu_calib_info_temp.imu_noise.NoiseAcc;
    fs["MU.GyroWalk"] >> imu_calib_info_temp.imu_noise.GyroWalk;
    fs["IMU.AccWalk"] >> imu_calib_info_temp.imu_noise.AccWalk;
    fs["IMU.timeshift_cam1_imu"] >> imu_calib_info_temp.imu_timeshift.timeShift_cam1_imu;
    fs["IMU.timeshift_cam2_imu"] >> imu_calib_info_temp.imu_timeshift.timeShift_cam2_imu;

    fs["Camera.bf"] >> camera_bf;

    fs["Camera.fx"] >> k1[0];
    fs["Camera.fy"] >> k1[1];
    fs["Camera.cx"] >> k1[2];
    fs["Camera.cy"] >> k1[3];

    fs["Camera2.fx"] >> k2[0];
    fs["Camera2.fy"] >> k2[1];
    fs["Camera2.cx"] >> k2[2];
    fs["Camera2.cy"] >> k2[3];

    cout << k1[0] << endl;
    cout << k1[1] << endl;
    cout << k1[2] << endl;
    cout << k1[3] << endl;
    
    // imu-camera tbc
    {
        Mat tbc_mat,tbc2_mat;

        fs["Tbc"] >> tbc_mat;
        fs["Tbc1"] >> tbc2_mat;
        
        for (int i = 0;i < 3;i++)
        {
            t_imu_1[i] = tbc_mat.ptr<float>(i,3)[0];
            t_imu_2[i] = tbc2_mat.ptr<float>(i,3)[0];
        }

        for (int i = 0;i < 3;i++)
        {
            for (int j = 0;j < 3;j++)
            {
                r_imu_1[i*3+j] = tbc_mat.ptr<float>(i,j)[0];
                r_imu_2[i*3+j] = tbc2_mat.ptr<float>(i,j)[0];
            }
            
        }
    }

    // tlr

    {
        Mat tlr_mat;

        fs["Trl"] >> tlr_mat;
        
        for (int i = 0;i < 3;i++)
        {
            t2[i] = tlr_mat.ptr<float>(i,3)[0];
        }

        for (int i = 0;i < 3;i++)
        {
            for (int j = 0;j < 3;j++)
            {
                r2[i*3+j] = tlr_mat.ptr<float>(i,j)[0];
            }
            
        }
    }


    if ((s_calib_model == "PinHole-radtan") || (s_calib_model == "pinhole-radtan"))
    {
        fs["Camera.k1"] >> k1[4];
        fs["Camera.k2"] >> k1[5];
        fs["Camera.p1"] >> k1[6];
        fs["Camera.p2"] >> k1[7];

        fs["Camera2.k1"] >> k2[4];
        fs["Camera2.k2"] >> k2[5];
        fs["Camera2.p1"] >> k2[6];
        fs["Camera2.p2"] >> k2[7];
        orb_cfg->cameraModel = PINHOLE_RATAN;

    }
    else if (s_calib_model == "pinhole-equidistant")
    {
        fs["Camera.k1"] >> k1[4];
        fs["Camera.k2"] >> k1[5];
        fs["Camera.k3"] >> k1[6];
        fs["Camera.k4"] >> k1[7];

        fs["Camera2.k1"] >> k2[4];
        fs["Camera2.k2"] >> k2[5];
        fs["Camera2.k3"] >> k2[6];
        fs["Camera2.k4"] >> k2[7];
        orb_cfg->cameraModel = PINHOLE_EQUI;
    }
    else if (s_calib_model == "pinhole-fov")
    {
        fs["Camera.w"] >> k1[4];
        fs["Camera2.w"] >> k2[4];
        orb_cfg->cameraModel = PINHOLE_FOV;
        
    }
    else if (s_calib_model == "omni-none")
    {
        fs["Camera.xi"] >> k1[4];

        fs["Camera2.xi"] >> k2[4];
        orb_cfg->cameraModel = OMNI_NONE;
    }
    else if (s_calib_model == "omni-radtan")
    {
        fs["Camera.xi"] >> k1[4];
        fs["Camera.k1"] >> k1[5];
        fs["Camera.k2"] >> k1[6];
        fs["Camera.p1"] >> k1[7];
        fs["Camera.p2"] >> k1[8];

        fs["Camera2.xi"] >> k2[4];
        fs["Camera2.k1"] >> k2[5];
        fs["Camera2.k2"] >> k2[6];
        fs["Camera2.p1"] >> k2[7];
        fs["Camera2.p2"] >> k2[8];
        orb_cfg->cameraModel = OMNI_RADTAN;
    }
    else if (s_calib_model == "eucm-none")
    {
        fs["Camera.alpha"] >> k1[4];
        fs["Camera.beta"]  >> k1[5];
        

        fs["Camera2.alpha"] >> k2[4];
        fs["Camera2.beta"]  >> k2[5];
        orb_cfg->cameraModel = EUCM_NONE;
    
    }
    else if (s_calib_model == "ds-none")
    {
        fs["Camera.xi"]     >> k1[4];
        fs["Camera.alpha"]  >> k1[5];
        

        fs["Camera2.xi"]     >> k2[4];
        fs["Camera2.alpha"]  >> k2[5];
        orb_cfg->cameraModel = DS_NONE;
    }

    // copy to orb calibration cfg

    orb_cfg->camera_bf = camera_bf;
    memcpy(orb_cfg->orb_intr1.k,k1,sizeof(float)*9);
    memcpy(orb_cfg->orb_intr2.k,k2,sizeof(float)*9);

    memcpy(orb_cfg->orb_extr_tbc.t,t_imu_1,sizeof(float)*3);
    memcpy(orb_cfg->orb_extr_tbc.r,r_imu_1,sizeof(float)*9);
    memcpy(orb_cfg->orb_extr_tbc2.t,t_imu_2,sizeof(float)*3);
    memcpy(orb_cfg->orb_extr_tbc2.r,r_imu_2,sizeof(float)*9);

    memcpy(orb_cfg->orb_extr_tlr.r,r2,sizeof(float)*9);
    memcpy(orb_cfg->orb_extr_tlr.t,t2,sizeof(float)*3);

    memcpy(&orb_cfg->imu_calib_info,&imu_calib_info_temp,sizeof(IMU_CALIB_INFO));


}

int get_orb_calib_result(ORB_CALIB_CFG_ALL * orb_cfg_all,char* in_file)
{

    cv::FileStorage fs(in_file, cv::FileStorage::READ);

    string s_camera_model,s_distortion_model;

    fs["Camera.type"] >> s_camera_model;
    fs["Distortion.type"] >> s_distortion_model;

    string s_calib_model = s_camera_model + "-" + s_distortion_model;
    cout << s_calib_model << endl;

    // read yaml orb calibration to write falsh
    get_orbcalib_cfg(orb_cfg_all->calibType,fs,s_calib_model,orb_cfg_all);

    fs.release();
    
}

int get_opencv_calib_result(OPENCV_CALIB_RESULT * opencv_calib_params,char *in_file)
{



    float K1[9]   = {0.0};
    float D1[4]   = {0.0};// rows 4 cols 1
    float P1[12]  = {0.0};// rows 3 cols 4
    float R1[9]   = {0.0};
    float K2[9]   = {0.0};
    float D2[4]   = {0.0};// rows 4 cols 1
    float P2[12]  = {0.0};// rows 3 cols 4
    float R2[9]   = {0.0};
    float T[3]    = {0.0};
    float Q[16]   = {0.0};
    float R[9]    = {0.0};


    cv::FileStorage fs(in_file, cv::FileStorage::READ);

    {
        Mat K1_mat,K2_mat,R1_mat,R2_mat,R_mat;
        Mat D1_mat,D2_mat,P1_mat,P2_mat,T_mat,Q_mat;

        fs["K1"] >> K1_mat;
        fs["K2"] >> K2_mat;
        fs["R1"] >> R1_mat;
        fs["R2"] >> R2_mat;
        fs["R"] >> R_mat;

        for (int i = 0;i < 3;i++)
        {
            for (int j = 0;j < 3;j++)
            {
                K1[i*3+j] = K1_mat.ptr<double>(i,j)[0];
                K2[i*3+j] = K2_mat.ptr<double>(i,j)[0];

                R1[i*3+j] = R1_mat.ptr<double>(i,j)[0];
                R2[i*3+j] = R2_mat.ptr<double>(i,j)[0];
                R[i*3+j] = R_mat.ptr<double>(i,j)[0];
            }
            
        }

        fs["D1"] >> D1_mat;
        fs["D2"] >> D2_mat;
        fs["P1"] >> P1_mat;
        fs["P2"] >> P2_mat;
        fs["Q"] >> Q_mat;
        fs["T"] >> T_mat;

        for (int i = 0;i < 4;i++)
        {
            D1[i] = D1_mat.ptr<double>(i,0)[0];
            D2[i] = D2_mat.ptr<double>(i,0)[0];
        }

        for (int i = 0;i < 3;i++)
        {
            for (int j = 0;j < 4;j++)
            {
                P1[i*4+j] = P1_mat.ptr<double>(i,j)[0];
                P2[i*4+j] = P2_mat.ptr<double>(i,j)[0];
            }
            
        }

        for (int i = 0;i < 3;i++)
        {
            T[i] = T_mat.ptr<double>(i,0)[0];
            
        }

        for (int i = 0;i < 4;i++)
        {
            for (int j = 0;j < 4;j++)
            {
                Q[i*4+j] = Q_mat.ptr<double>(i,j)[0];
            }
            
        }
    }

    memcpy(opencv_calib_params->D1,D1,sizeof(float)*4);
    memcpy(opencv_calib_params->D2,D2,sizeof(float)*4);

    memcpy(opencv_calib_params->K1,K1,sizeof(float)*9);
    memcpy(opencv_calib_params->K2,K2,sizeof(float)*9);
    memcpy(opencv_calib_params->R1,R1,sizeof(float)*9);
    memcpy(opencv_calib_params->R2,R2,sizeof(float)*9);
    memcpy(opencv_calib_params->R,R,sizeof(float)*9);

    memcpy(opencv_calib_params->P1,P1,sizeof(float)*12);
    memcpy(opencv_calib_params->P2,P2,sizeof(float)*12);

    memcpy(opencv_calib_params->Q,Q,sizeof(float)*16);
    memcpy(opencv_calib_params->T,T,sizeof(float)*3);


    fs.release();

}