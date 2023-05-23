#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "uvc_cam_sdk.h"
#include "bmp.h"
#include "../hid_control/hid_control.hpp"
#include <misc.h>
#include <common.h>
#include <geometry.h>
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <semaphore.h>
#include <dispUvc.h>
#include <fstream>
#include <stdint.h>

using namespace std;
#ifdef HEAP_STAT
#define MAX_HEAP_ENTRY 1000
int  heapCurrent;
int  heapMax;
int  heapNum;
int  maxHeapNum;  
// static HidControl hid_control;
long  addr_table[MAX_HEAP_ENTRY];
int   size_table[MAX_HEAP_ENTRY];
// static double t = 0;
extern void heapStatInit (int size);
#endif

#ifdef MULTI_API_VERSION
bool g_is_new_fw = false;
#endif

#define CAPTURE_IMAGE_FPS (50)
#define WAIT_USLEEP_COFF  (1000*1000/50)
HidControl hid_control;

// #define XR50

//camera image buff
typedef struct IMAGE_FRAME_t
{
    uint8_t IMG_BUFF[WIDTH*HEIGHT*CAM_NUMS];
    uint64_t imgTimeStampNs;
}IMAGE_FRAME;

#define IMAGE_BUFF_SIZE 100
uint8_t IMG_BUFF[IMAGE_BUFF_SIZE][WIDTH*HEIGHT*CAM_NUMS];

IMAGE_FRAME img_frame[IMAGE_BUFF_SIZE];

uint32_t buf_wt = 0;
uint32_t buf_rd = 0;

// imu data buff
uint32_t imu_wt = 0;
uint32_t imu_rd = 0;

#define IMU_BUFF_SIZE 2000
imu_saveData imu_param[IMU_BUFF_SIZE];

bool isSaveData = true;

//save imu data path
std::string IMU_PATH = "../data/imu0.csv";
ofstream csv;


std::string PATH = "../data/camtime.csv";
ofstream csv_cam;

//thread define
pthread_t th_Camera;
sem_t sem_th_Camera;

pthread_t th_SaveImg;
sem_t sem_th_SaveImg;

pthread_t th_SaveImu;
sem_t sem_th_SaveImu;

// thread priority

// #define IMU_SAVE_PRI       101
// #define IMG_HANDLER_PRI    108
// #define IMG_SAVE_PRI       105



void start_uvc_camera()
{
    char path_f408_firstNode[50];
    udev_search_node(path_f408_firstNode,0);
    uvc_camera_sdk_init(path_f408_firstNode,640,800,1);
}
void save_data_toBMP(const char * path,uint8_t * mem,uint32_t width,uint32_t height)
{
    uint8_t *rgb_data = (uint8_t*)malloc(width*height*3);
    for(uint32_t i = 0;i<height;i++)
    {
        for(uint32_t j = 0;j<width;j++)
        {
            rgb_data[i*width*3+j*3+0] = mem[i*width+j];
            rgb_data[i*width*3+j*3+1] = mem[i*width+j];
            rgb_data[i*width*3+j*3+2] = mem[i*width+j];
        }
    }
    save_bgr_bmp(path,rgb_data,width,height);
    free(rgb_data);
}
void save_data_toFile(const char * path,uint8_t * mem,uint32_t bytes)
{
    FILE * fp = fopen(path,"wb");
    fwrite(mem,sizeof(uint8_t),bytes,fp);
    close(fp);
}

//log imu for different mode
void imu_log_mode2_3(uint8_t imu_raw_new[60],double t)
{
    HIDoutData_new_rotation_m2_m3 *imu_mode2_3=(HIDoutData_new_rotation_m2_m3 *)imu_raw_new;
    std::cout.width(5);
    std::cout.setf(std::ios::fixed);
    std::cout<<std::setprecision(1)<< "IMU1:freq:" <<std::setw(7)<< (float) (1./t) << "; values: ";
    std::cout<< std::setprecision(1)<<((double)imu_mode2_3->imu_time / double(1e6)) <<"; "<< std::setprecision(0)<<((double)imu_mode2_3->camera_time / double(1e3)) <<"; "<< std::setprecision(1)<<imu_mode2_3->frameCount<<"; ";
    std::cout.setf(std::ios::showpos);
    std::cout<< std::setprecision(5)<<imu_mode2_3->q_e.quarternion[0]<<"; "<<std::setprecision(5)<<imu_mode2_3->q_e.quarternion[1]<<"; "<<std::setprecision(5)<<imu_mode2_3->q_e.quarternion[2]<<"; "<<std::setprecision(5)<<imu_mode2_3->q_e.quarternion[3]<<"; ";
    std::cout<< std::setprecision(0)<<imu_mode2_3->q_e.extra[0]<<std::setprecision(0)<<"; "<<imu_mode2_3->q_e.extra[1]<<"; ";
    std::cout<< std::setprecision(5)<<imu_mode2_3->linear_accel.x<<"; "<<std::setprecision(5)<<imu_mode2_3->linear_accel.y<<"; "<<std::setprecision(5)<<imu_mode2_3->linear_accel.z<<"; ";
    std::cout<< std::setprecision(2)<<imu_mode2_3->temperatureData<<std::endl;
}
void imu_log_mode1_0(uint8_t imu_raw_new[60],double t)
{
    HIDoutData_new *imu_mode1=(HIDoutData_new *)imu_raw_new;
    // t = now() - t;
    std::cout.width(5);
    std::cout.setf(std::ios::fixed);
    std::cout<<std::setprecision(1)<< "IMU1:freq:" <<std::setw(7)<< (float) (1./t)  << "; values: ";
    std::cout<<std::setprecision(1)<<((double)imu_mode1->imu_time / double(1e9)) << "; "<<std::setprecision(1)<<((double)imu_mode1->camera_time / double(1e3)) << "; "<<std::setprecision(0)<<imu_mode1->frameCount<<"; ";
    std::cout.setf(std::ios::showpos);
    std::cout <<std::setprecision(5)<<-imu_mode1->gyroData.axisData.x<<"; "<<std::setprecision(5)<<imu_mode1->gyroData.axisData.y<<"; "<<std::setprecision(5)<<imu_mode1->gyroData.axisData.z<<"; ";
    std::cout <<std::setprecision(5)<<-imu_mode1->accelData.axisData.x<<"; "<<std::setprecision(5)<<imu_mode1->accelData.axisData.y<<"; "<<std::setprecision(5)<<imu_mode1->accelData.axisData.z<<"; ";
    std::cout <<std::setprecision(2)<<imu_mode1->temperatureData<<std::endl;
}
void log_imu_new(uint8_t imu_raw_new[60],int p1,double t)//13 elements
{
    int imu_mode=p1;
    switch(imu_mode)
    {
      case 3:imu_log_mode2_3(imu_raw_new,t);break;
      case 2:imu_log_mode2_3(imu_raw_new,t);break;
      case 1:imu_log_mode1_0(imu_raw_new,t);break;
      default:std::cout<<"INVALID IMU MODE! "<<std::endl;break;
    }
}
void log_imu(uint8_t imu_raw[60],double t)//9 elements
{
    HIDoutData *myHidData = (HIDoutData *)&imu_raw;
    std::cout.width(5);
    std::cout.setf(std::ios::fixed);
    std::cout.setf(std::ios::showpos);
    std::cout <<std::setprecision(1)<< "IMU1:freq:" <<std::setw(7)<< (float) (1./t) << "; values: ";
    std::cout <<std::setprecision(1)<<(float)(double(myHidData->tymeStamp) / double(1e9))<< "; "<<std::setprecision(0)<<(float)myHidData->frameCount<<"; ";
    std::cout <<std::setprecision(5)<<-myHidData->gyroData.axisData.y <<"; "<<std::setprecision(5)<<myHidData->gyroData.axisData.x <<"; "<<std::setprecision(5)<<myHidData->gyroData.axisData.z<<"; ";
    std::cout <<std::setprecision(5)<<myHidData->accelData.axisData.y<<"; "<<std::setprecision(5)<<myHidData->accelData.axisData.x<<"; "<<std::setprecision(5)<<myHidData->accelData.axisData.z<<"; ";
}




void imu_save_mode2_3(uint8_t imu_raw_new[60],double t)
{
    HIDoutData_new_rotation_m2_m3 *imu_mode2_3=(HIDoutData_new_rotation_m2_m3 *)imu_raw_new;
    printf("The mode have no gyroData!\n");

    // std::cout.width(5);
    // std::cout.setf(std::ios::fixed);
    // std::cout<<std::setprecision(1)<< "IMU1:freq:" <<std::setw(7)<< (float) (1./t) << "; values: ";
    // std::cout<< std::setprecision(1)<<((double)imu_mode2_3->imu_time / double(1e6)) <<"; "<< std::setprecision(0)<<((double)imu_mode2_3->camera_time / double(1e3)) <<"; "<< std::setprecision(1)<<imu_mode2_3->frameCount<<"; ";
    // std::cout.setf(std::ios::showpos);
    // std::cout<< std::setprecision(5)<<imu_mode2_3->q_e.quarternion[0]<<"; "<<std::setprecision(5)<<imu_mode2_3->q_e.quarternion[1]<<"; "<<std::setprecision(5)<<imu_mode2_3->q_e.quarternion[2]<<"; "<<std::setprecision(5)<<imu_mode2_3->q_e.quarternion[3]<<"; ";
    // std::cout<< std::setprecision(0)<<imu_mode2_3->q_e.extra[0]<<std::setprecision(0)<<"; "<<imu_mode2_3->q_e.extra[1]<<"; ";
    // std::cout<< std::setprecision(5)<<imu_mode2_3->linear_accel.x<<"; "<<std::setprecision(5)<<imu_mode2_3->linear_accel.y<<"; "<<std::setprecision(5)<<imu_mode2_3->linear_accel.z<<"; ";
    // std::cout<< std::setprecision(2)<<imu_mode2_3->temperatureData<<std::endl;
}

void imu_save_mode1_0(uint8_t imu_raw_new[60],uint64_t timeStampNs)
{
    HIDoutData_new *imu_mode1=(HIDoutData_new *)imu_raw_new;
    float const G_ =9.80665;
    #ifndef XR50
        csv << timeStampNs << "," 
            << imu_mode1->gyroData.axisData.y << "," << -imu_mode1->gyroData.axisData.x << "," << imu_mode1->gyroData.axisData.z << ","
            << imu_mode1->accelData.axisData.y * G_ << "," << -imu_mode1->accelData.axisData.x*G_ << "," << imu_mode1->accelData.axisData.z * G_ << "\n";
    #else
        csv << timeStampNs << "," 
            << -imu_mode1->gyroData.axisData.y << "," << imu_mode1->gyroData.axisData.x << "," << imu_mode1->gyroData.axisData.z << ","
            << -imu_mode1->accelData.axisData.y * G_ << "," << imu_mode1->accelData.axisData.x*G_ << "," << imu_mode1->accelData.axisData.z * G_ << "\n";
    #endif
    // raw direction
    // csv << timeStampNs << "," 
    //     << imu_mode1->gyroData.axisData.x << "," << imu_mode1->gyroData.axisData.y << "," << imu_mode1->gyroData.axisData.z << ","
    //     << imu_mode1->accelData.axisData.x * G_ << "," << imu_mode1->accelData.axisData.y*G_ << "," << imu_mode1->accelData.axisData.z * G_ << "\n";
        
}


void save_imu_data(imu_saveData * imu_param_p)
{
    int imu_mode=imu_param_p->p1;
    switch(imu_mode)
    {
      case 3:imu_save_mode2_3(imu_param_p->imu_raw,imu_param_p->imuTimestampNs);break;
      case 2:imu_save_mode2_3(imu_param_p->imu_raw,imu_param_p->imuTimestampNs);break;
      case 1:imu_save_mode1_0(imu_param_p->imu_raw,imu_param_p->imuTimestampNs);break;
      default:std::cout<<"INVALID IMU MODE! "<<std::endl;break;
    }
}

//get os current system time
//get sec and nsec
int64_t getSystemTime() {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return (int64_t) now.tv_sec*1000000000LL + now.tv_nsec;
}
//only get sec
int64_t getSystemTime_s() {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return (int64_t) now.tv_sec;
}

// log calibration for movidius
void calibration_log(HidControl hid_control)
{
    DeviceCalibration calib;
    hid_control.get_calibration(calib);

    std::cout << " CALIBRATION LOADED FROM HID : " << std::endl;
    for (int i = 0;i < 7;i++)
    {
        printf("K1[%d]: %f\n",i,calib.K1[i]);
    }
    for (int i = 0;i < 9;i++)
    {
        printf("R1[%d]: %f\n",i,calib.R1[i]);
    }
    for (int i = 0;i < 3;i++)
    {
        printf("T1[%d]: %f\n",i,calib.T1[i]);
    }

    for (int i = 0;i < 7;i++)
    {
        printf("K2[%d]: %f\n",i,calib.K2[i]);
    }
    for (int i = 0;i < 9;i++)
    {
        printf("R2[%d]: %f\n",i,calib.R2[i]);
    }
    for (int i = 0;i < 3;i++)
    {
        printf("T2[%d]: %f\n",i,calib.T2[i]);
    }
    
    for (int i = 0;i < 3;i++)
    {
        printf("gyro_bias[%d]: %f\n",i,calib.gyro_bias[i]);
    }
    for (int i = 0;i < 3;i++)
    {
        printf("accel_bias[%d]: %f\n",i,calib.accel_bias[i]);
    }
    printf("time_shift: %f\n",calib.time_shift);
}

// some thread handler 

void *SaveImg_handler(void *)
{

    printf("SaveImg_handler Thread init success!\n");

    // csv_cam.open(PATH);
    
    
    static uint64_t cnt = 0;
    while (isSaveData)
    {
         sem_wait(&sem_th_SaveImg);
        //  if (csv_cam.is_open())
         
         uint8_t *img_start = img_frame[buf_rd].IMG_BUFF;
         uint64_t img_time = img_frame[buf_rd].imgTimeStampNs;

        //  if (csv_cam.is_open())
        cnt++;
        //  if (cnt == 1)
        //  {
        
        //     csv_cam << "timestamp" << "\n";
        //  }
        //  else
        //  {
            
        //     csv_cam << img_time << "\n";
        //  }

           printf("Img time---%lld \n",img_time);
         cv::Mat image_l = cv::Mat(cv::Size(WIDTH,HEIGHT),CV_8UC1,img_start);
         cv::Mat image_r = cv::Mat(cv::Size(WIDTH,HEIGHT),CV_8UC1,img_start + WIDTH*HEIGHT);
         
         int64_t start =getSystemTime();
         char path[300];
         char path1[300];
         sprintf(path,"../data/cam0/%lld.png",img_time);
         
         sprintf(path1,"../data/cam1/%lld.png",img_time);

         
         if (img_time > 1e+9)
         {
            cv::imwrite(path,image_l);
            cv::imwrite(path1,image_r);
         }
         
         int64_t end =getSystemTime();
        //  printf("Save img laenty = %lld\n",end-start);
         
         //swap buff
         buf_rd = (buf_rd + 1) % IMAGE_BUFF_SIZE;
    }
    
}

void *Camera_handler(void *)
{

    double t = 0;

    printf("hello dispUvc\n");
    start_uvc_camera();
    uvc_camera_sdk_stream_start(1000*1000);

    uint64_t grab_times=0;
    // sem_wait(&sem_th_Camera);

    sem_init(&sem_th_SaveImg, 0, 0);
    pthread_create(&th_SaveImg,NULL,SaveImg_handler,NULL);
    // pthread_setschedprio(th_SaveImg,IMG_SAVE_PRI);
    while(1)
    {
        
         camera_t * uvc_frame=uvc_camera_sdk_stream_captured_once();
         
         //get timestamp
         uint32_t *offset = (uint32_t*)(uvc_frame->head.start + WIDTH*HEIGHT*CAM_NUMS);
         uint32_t timeStampUs = *(offset + 1);
         img_frame[buf_wt].imgTimeStampNs = (uint64_t)timeStampUs * (uint64_t)1000;
         memcpy(img_frame[buf_wt].IMG_BUFF,uvc_frame->head.start,WIDTH*HEIGHT*CAM_NUMS);
         

         sem_post(&sem_th_SaveImg);
        
         //update circle buff
         buf_wt = (buf_wt + 1) % IMAGE_BUFF_SIZE;
         cv::Mat image_cv_mat = cv::Mat(cv::Size(640,800), CV_8UC1, uvc_frame->head.start);
        
		 cv::imshow("UVC FRAMES", image_cv_mat);
         if (cv::waitKey(20) == 27 )
         {
		    break;
		 }
        
        // usleep(WAIT_USLEEP_COFF);
    }
    
    uvc_camera_sdk_stream_stop();
}


void *SaveImu_handler(void *)
{

    printf("SaveImu thread init success!\n");
    static uint64_t frame_nums = 0;
    csv.open(IMU_PATH);
    while (isSaveData)
    {
        sem_wait(&sem_th_SaveImu);

        if (csv.is_open())
        {
            frame_nums++;
            if(frame_nums == 1)
            {
                csv << "timestamp"
                    << ","
                    << "omega_x"
                    << ","
                    << "omega_y"
                    << ","
                    << "omega_z"
                    << ","
                    << "alpha_x"
                    << ","
                    << "alpha_y"
                    << ","
                    << "alpha_z"
                    << "\n";
                
            }
            else
            {
                //  csv << Zhangsan.age << "," << Zhangsan.height << "," << Zhangsan.weight << "," << Zhangsan.score
                //     << "\n";
                // if (frame_nums < 120000)
                {
                    if (imu_param[imu_rd].imuTimestampNs >1e+9)
                        save_imu_data(&imu_param[imu_rd]);
                }
                // else
                // {
                //     isSaveData = false;
                //     // exit(0);
                // }
                

            }

            // if (frame_nums > 1000000) {
            // csv.close();
            // }

        }

        // printf("Imu time---%lld\n",imu_param[imu_rd].imuTimestampNs);

        imu_rd = (imu_rd + 1) % IMU_BUFF_SIZE;
            
    }
    
}

int main(int argc, char *argv[])
{

    double t = 0;
    printf("argc = %d\n",argc);
    bool new_command = Option(argc,argv)("new");
    bool dyn_calib_command = Option(argc,argv)("dyn");
    printf("dyn_calib_command = %d\n",dyn_calib_command);

    std::string DISPLAY_HW_SW_VERSION = hid_control.get_software_version();
    float sw_v = hid_control.get_software_number();
    std::cout << "software version =" << sw_v  << std::endl;
    std::cout << " DISPLAY_HW_SW_VERSION = " << DISPLAY_HW_SW_VERSION << std::endl;
    #ifdef MULTI_API_VERSION
    hid_control.set_api_version(sw_v > 0.02);
    g_is_new_fw = sw_v > 0.02;
    #endif

    //#if (NEW_IMU_ENABLE==1u)
    #ifdef MULTI_API_VERSION
    if (g_is_new_fw)
    {
    hid_control.imu_new_request();
    }
    //#else
    else
    #endif
    {
    hid_control.imu_request();
    }

    // calibration_log(hid_control);
    //#endif
    char *NEW_P1,*NEW_P2,*NEW_P3;
    static int P1=0;
    static short P2=0,P3=0;
    std::string C_Zero="0";
    std::string C_One="1";

    int func_control_dyn = 0;
    if(dyn_calib_command)
    {
    BNO_DYN_CALIB_REQUEST_t req;
    BNO_DYN_CALIB_RESPONSE_t res;
    req.p1 = (uint8_t)argv[2];
    req.p2 = (uint8_t)argv[3];
    req.not_record = 0;
    hid_control.bno_dyn_calib(req,res);
    int i=0;
    while(i++<100){
        req.not_record = 1;
        func_control_dyn = hid_control.bno_dyn_calib(req,res);
        printf("status: %d %d %d %d %d %d\n",res.acc_status,res.gyr_status,res.mag_status,res.lacc_status,res.gra_status);
        }
    }
    if(new_command)
    {
    hid_control.reset_slam();
    if (argc>=3) NEW_P1 = argv[2];// receive parameters
    else {NEW_P1=&C_One[0];}
    if (argc>=4) NEW_P2 = argv[3];
    else {NEW_P2=&C_Zero[0];}
    if (argc>=5) NEW_P3 = argv[4];
    else {NEW_P3=&C_Zero[0];}
    if(std::stoi(NEW_P1)>=0 && std::stoi(NEW_P1)<=3){P1=std::stoi(NEW_P1);}// give received value to P1 P2 P3
    if(std::stoi(NEW_P2)>=-200 && std::stoi(NEW_P2)<=400){P2=std::stoi(NEW_P2);}
    if(std::stoi(NEW_P3)>=-200 && std::stoi(NEW_P3)<=1000){P3=std::stoi(NEW_P3);}
    printf("%d %d %d\n",P1,P2,P3);
    hid_control.imu_fusion(P1,true,(short)P2,(short)P3,(short)1000);
    }  
    hid_control.reset_slam();
    sleep(1);
    hid_control.set_gain_exposure(1,100);
    int func_control = 0;

    uint64_t grab_times=0;

    sem_init(&sem_th_Camera, 0, 0);
    pthread_create(&th_Camera,NULL,Camera_handler,NULL);

    // pthread_setschedprio(th_Camera,IMG_HANDLER_PRI);

    sem_init(&sem_th_SaveImu, 0, 0);
    pthread_create(&th_SaveImu,NULL,SaveImu_handler,NULL);
    // pthread_setschedprio(th_SaveImu,IMU_SAVE_PRI);
    while (1 && !func_control)
    {
        // sem_post(&sem_th_Camera);

         int64_t rosTsNS =getSystemTime_s();// get s   system current time start from 1970

         uint32_t rosTime = ((uint64_t)rosTsNS)  & (0x00000000ffffffff);
        //  printf("Imu time--- = %lld \n",rosTime);

        // hid_control.reset_slam();
        t = now();

        #ifdef MULTI_API_VERSION
            #define  IMUSIZE 13
        #else
            #define IMUSIZE 9
        #endif
        
        float imu_data[IMUSIZE];
        //#if (NEW_IMU_ENABLE==1u)
        #ifdef MULTI_API_VERSION
        if (g_is_new_fw)
        {
            func_control=hid_control.internal_grab_imu_new_raw(imu_param[imu_wt].imu_raw);
            t = now() - t;
            HIDoutData_new *imu_mode1_log=(HIDoutData_new *)imu_param[imu_wt].imu_raw;
            //get nsec
            imu_param[imu_wt].p1 = P1;
            imu_param[imu_wt].t = t;
            imu_param[imu_wt].imuTimestampNs = ((uint64_t)imu_mode1_log->imu_time) * (1e+3);;
            sem_post(&sem_th_SaveImu);
            // printf("Imu time---%lld\n",imu_param[imu_wt].imuTimestampNs);
            // log_imu_new(imu_rawBuffer,P1,t);//13 elements
        }
        //#else
        else
        #endif
        {
            func_control=hid_control.internal_grab_imu_raw(imu_param[imu_wt].imu_raw);
            t = now() - t;
            log_imu(imu_param[imu_wt].imu_raw,t);//9 elements
        }

        imu_wt = (imu_wt + 1) % IMU_BUFF_SIZE;
        
    }
}
