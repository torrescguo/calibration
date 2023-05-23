#include "calibration.h"
#include "uvc_cam_sdk.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "../source/include/camera_model.h"
#include "../source/include/orbCalibration.h"
/*UVC*/
#define UVC_CAM_WIDTH   (640)
#define UVC_CAM_HEIGHT  (400)
#define UVC_CAM_NB      (2)
/*CHESS BOARD SIZE*/
#define SQUARE_W_CM     (2.3f)
#define SQUARE_H_CM     (2.3f)
#define CHESS_ROW_NB    (6)
#define CHESS_COL_NB    (9)
#define CAPTURE_IMAGE_FPS (50)
#define WAIT_USLEEP_COFF  (1000*1000/50)



using namespace std;
using namespace cv;
using namespace opencvMonoFECalib;
using namespace CAMERA_MODEL_API;

extern cv::Mat rmap[2][2];

// #define ENABLE_OPENCV_CALIB

void start_uvc_camera()
{
    char path_f408_firstNode[50];
    udev_search_node(path_f408_firstNode,0);
    uvc_camera_sdk_init(path_f408_firstNode,UVC_CAM_WIDTH,UVC_CAM_HEIGHT*UVC_CAM_NB,1);
}

int main_stereo(int argc,char * argv[])
{
    start_uvc_camera();
    uvc_camera_sdk_stream_start(1000*1000);
    opencvStereoFECalib::stereoFisheyeCalib calibObjS=opencvStereoFECalib::stereoFisheyeCalib(UVC_CAM_WIDTH,UVC_CAM_HEIGHT,CHESS_COL_NB,CHESS_ROW_NB,SQUARE_W_CM,SQUARE_H_CM,0);
    int record_frames=10;
    const uint32_t subAreaFactor=1;
    uint32_t subAreaCornersNb[4]={0,0,0,0};
    float    unitX = UVC_CAM_WIDTH/subAreaFactor;
    float    unitY = UVC_CAM_HEIGHT/subAreaFactor;
    int      subAreaFrames = record_frames/(subAreaFactor*subAreaFactor);
    bool     continueDet=true;
#ifdef  ENABLE_OPENCV_CALIB 
    while(continueDet)
    {
        camera_t * uvc_frame=uvc_camera_sdk_stream_captured_once();
        int found=calibObjS.pushRawImageToCalib(uvc_frame->head.start,uvc_frame->head.start+UVC_CAM_WIDTH*UVC_CAM_HEIGHT);
        cv::Mat image_cv_mat = cv::Mat(cv::Size(UVC_CAM_WIDTH,UVC_CAM_HEIGHT*2), CV_8UC1, uvc_frame->head.start);
        //draw found kp
        if(found)
        {
            vector<Point2f>  corners_l = calibObjS.calibData_p.corners_l;
            for(auto coord : corners_l)
            {
                cv::circle(image_cv_mat, coord, 4, cv::Scalar(255,255,0), 1, cv::LINE_AA);
            }
            vector<Point2f>  corners_r = calibObjS.calibData_p.corners_r;
            for(auto coord : corners_r)
            {
                Point2f kp_((float)(coord.x),(float)(coord.y+UVC_CAM_HEIGHT));
                cv::circle(image_cv_mat, kp_, 4, cv::Scalar(255,255,0), 1, cv::LINE_AA);
            }
            {
                float center_p_l_x=0,center_p_l_y=0;
                for(auto coord : corners_l)
                {
                    center_p_l_x +=coord.x;
                    center_p_l_y +=coord.y;
                }
                center_p_l_x /= corners_l.size(),center_p_l_y /= corners_r.size();
                uint32_t subId_x = center_p_l_x/unitX;
                uint32_t subId_y = center_p_l_y/unitY;
                (subAreaCornersNb[subId_x*subAreaFactor+subId_y])++;
            }
            if((subAreaCornersNb[0] > subAreaFrames) && (subAreaCornersNb[1] > subAreaFrames) && (subAreaCornersNb[2] > subAreaFrames) && (subAreaCornersNb[3] > subAreaFrames))
            {
                continueDet=false;
            }
        }    
		// cvui::update();
		cv::imshow("UVC FRAMES", image_cv_mat);
        if (cv::waitKey(20) == 27 ) {break;}
        usleep(WAIT_USLEEP_COFF);
    }
    double error=calibObjS.endCalibration();
    calibObjS.printCalibrationResults(error);
#endif
    //
    printf("validating the calibration data\n");
    #ifdef ENABLE_OPENCV_CALIB
    // Dual channel
    fisheye::initUndistortRectifyMap(calibObjS.calibData_p.calibration_results.K1, calibObjS.calibData_p.calibration_results.D1, calibObjS.calibData_p.calibration_results.R1, calibObjS.calibData_p.calibration_results.P1,cv::Size(UVC_CAM_WIDTH,UVC_CAM_HEIGHT) , CV_16SC2, rmap[0][0], rmap[0][1]);
    fisheye::initUndistortRectifyMap(calibObjS.calibData_p.calibration_results.K2, calibObjS.calibData_p.calibration_results.D2, calibObjS.calibData_p.calibration_results.R2, calibObjS.calibData_p.calibration_results.P2,cv::Size(UVC_CAM_WIDTH,UVC_CAM_HEIGHT) , CV_16SC2, rmap[1][0], rmap[1][1]);
    std::cout << rmap[0][0].size().width << rmap[0][0].size().height << std::endl;
    std::cout << rmap[0][1].size().width << rmap[0][1].size().height << std::endl;
    std::cout << rmap[1][0].type() << std::endl;
    std::cout << rmap[1][1].ptr<int16_t>(8,9)[0] << std::endl;
    std::cout << rmap[1][1].ptr<int16_t>(8,9)[1] << std::endl;
    #else
    rmap[0][0] = cv::Mat(cv::Size(640,400), CV_32FC1);
    rmap[0][1] = cv::Mat(cv::Size(640,400), CV_32FC1);
    rmap[1][0] = cv::Mat(cv::Size(640,400), CV_32FC1);
    rmap[1][1] = cv::Mat(cv::Size(640,400), CV_32FC1);
    

    float r0[9] = {1,0,0,
                   0,1,0,
                   0,0,1};

    ORB_CALIB_CFG_ALL orb_calibration_in;
    string s_yaml = string(argv[1]);
   
    get_orb_calib_result(&orb_calibration_in,argv[1]);
    int calib_method = orb_calibration_in.calibType;
    int camera_model = orb_calibration_in.cameraModel;

    cout << "Camera mdoel :" << camera_model << "Calib method :" << calib_method << endl;

    float *intr1 = orb_calibration_in.orb_intr1.k;
    float *intr2 = orb_calibration_in.orb_intr2.k;
    float *r2 = orb_calibration_in.orb_extr_tlr.r;

    for (int i = 0;i < 9;i++)
    {
        printf("K1[%d] = %f\n",i,intr1[i]);
    }
    for (int i = 0;i < 9;i++)
    {
        printf("K2[%d] = %f\n",i,intr2[i]);
    }

    for (int i = 0;i < 9;i++)
    {
        printf("R2[%d] = %f\n",i,r2[i]);
    }

    switch (camera_model)
    {
        case PINHOLE_RATAN:
        {
            pinhole_radtan_distortion(intr1,r0,0);
            pinhole_radtan_distortion(intr2,r2,1);

        }break;

        case PINHOLE_EQUI:
        {
            pinhole_equi_distortion(intr1,r0,0);
            pinhole_equi_distortion(intr2,r2,1);

        }break;

        case PINHOLE_FOV:
        {
            pinhole_fov_distortion(intr1,r0,0);
            pinhole_fov_distortion(intr2,r2,1);

        }break;

        case OMNI_NONE:
        {
            ucm_none_distortion(intr1,r0,0);
            ucm_none_distortion(intr2,r2,1);

        }break;

        case OMNI_RADTAN:
        {
            ucm_radtan_distortion(intr1,r0,0);
            ucm_radtan_distortion(intr2,r2,1);

        }break;

        case EUCM_NONE:
        {
            eucm_none_distortion(intr1,r0,0);
            eucm_none_distortion(intr2,r2,1);

        }break;

        case DS_NONE:
        {
            ds_none_distortion(intr1,r0,0);
            ds_none_distortion(intr2,r2,1);

        }break;

        default:
            break;

    }
    
    #endif

    std::cout << "******** Done Calibration ********\n" << std::endl;
    
    
    uvc_camera_sdk_stream_stop();
    start_uvc_camera();
    uvc_camera_sdk_stream_start(1000*1000);
    while(1)
    {
        camera_t * uvc_frame=uvc_camera_sdk_stream_captured_once();
        cv::Mat imageL = cv::Mat(cv::Size(UVC_CAM_WIDTH,UVC_CAM_HEIGHT), CV_8UC1, uvc_frame->head.start);
        cv::Mat imageR = cv::Mat(cv::Size(UVC_CAM_WIDTH,UVC_CAM_HEIGHT), CV_8UC1, uvc_frame->head.start+UVC_CAM_WIDTH*UVC_CAM_HEIGHT);
        cv::Mat recImgL = cv::Mat(400,640,CV_8UC1);
        cv::Mat recImgR = cv::Mat(400,640,CV_8UC1); 
        
        remap(imageL, recImgL, rmap[0][0], rmap[0][1], CV_INTER_LINEAR);
        remap(imageR, recImgR, rmap[1][0], rmap[1][1], CV_INTER_LINEAR);
        cv::Mat display;
        hconcat(recImgL,recImgR,display);

        // for (int i = 40;i < 400;i+=40)
        // {
        //     cv::line(display, cv::Point(0, i), cv::Point(640*2, i), cv::Scalar(255, 255, 0,0),1,8);
        // }

        cv::imshow("UVC FRAMES", display);
        if (cv::waitKey(20) == 27 ) {break;}
        usleep(WAIT_USLEEP_COFF);
    }
    return 0;
}
int main_mono(int argc,char * argv[])
{
    start_uvc_camera();
    uvc_camera_sdk_stream_start(1000*1000);
    opencvMonoFECalib::monoFisheyeCalib calibObj_L=opencvMonoFECalib::monoFisheyeCalib(UVC_CAM_WIDTH,UVC_CAM_HEIGHT,CHESS_COL_NB,CHESS_ROW_NB,SQUARE_W_CM,SQUARE_H_CM,0);
    opencvMonoFECalib::monoFisheyeCalib calibObj_R=opencvMonoFECalib::monoFisheyeCalib(UVC_CAM_WIDTH,UVC_CAM_HEIGHT,CHESS_COL_NB,CHESS_ROW_NB,SQUARE_W_CM,SQUARE_H_CM,1);
    int record_frames=400;
    while(record_frames--)
    {
        camera_t * uvc_frame=uvc_camera_sdk_stream_captured_once();
        int found_l=calibObj_L.pushRawImageToCalib(uvc_frame->head.start);
        int found_r=calibObj_R.pushRawImageToCalib(uvc_frame->head.start+UVC_CAM_WIDTH*UVC_CAM_HEIGHT);
        cv::Mat image_cv_mat = cv::Mat(cv::Size(UVC_CAM_WIDTH,UVC_CAM_HEIGHT*2), CV_8UC1, uvc_frame->head.start);
        //draw found kp
        if(found_l)
        {
            vector<Point2f>  corners_ = calibObj_L.calibData_p.corners;
            for(auto coord : corners_)
            {
                cv::circle(image_cv_mat, coord, 4, cv::Scalar(255,255,0), 1, cv::LINE_AA);
            }
        }
        if(found_r)
        {
            vector<Point2f>  corners_ = calibObj_R.calibData_p.corners;
            for(auto coord : corners_)
            {
                Point2f kp_((float)(coord.x),(float)(coord.y+UVC_CAM_HEIGHT));
                cv::circle(image_cv_mat, kp_, 4, cv::Scalar(255,255,0), 1, cv::LINE_AA);
            }
        }        
		// cvui::update();
		cv::imshow("UVC FRAMES", image_cv_mat);
        if (cv::waitKey(20) == 27 ) {break;}
        usleep(WAIT_USLEEP_COFF);
    }
    calibObj_L.endCalibration();
    calibObj_R.endCalibration();
    calibObj_L.printCalibrationResults();
    calibObj_R.printCalibrationResults();
    return 0;
}
int main(int argc,char * argv[])
{
    main_stereo(argc,argv);
}