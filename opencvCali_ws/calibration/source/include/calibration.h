#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/types_c.h>
#include <fstream>
#include <unistd.h>
#include <pthread.h>
namespace opencvMonoFECalib
{
    using namespace std;
    using namespace cv;
    typedef struct CALIB_RESUILT_t
    {
        cv::Mat intrinsic_matrix;
        cv::Mat distortion_coeffs;
        std::vector<cv::Vec3d> rotation_vectors;    
	    std::vector<cv::Vec3d> translation_vectors;  
    }CALIB_RESULT;
    typedef struct CALIB_APP_DATA_t
    {
        /*gloabl input data*/
        uint32_t cameraID;
        uint32_t pixelW;
        uint32_t pixelH;
        uint32_t cbCol;
        uint32_t cbRow;
        uint8_t * rawData;
        float squareW_cm;
        float squareH_cm;
        /*calibration steps data*/
        uint32_t pushRawGrayImgIndex;
        uint32_t patternfoundImgIndex;
        uint32_t sucessImgIndex;
        vector < vector < Point2f > >  corners_Seq;
        Size board_size;
        Size square_size;
        vector<Point2f> corners;
        vector<Mat>  image_Seq;
        pthread_t th_extract_feature;
        vector < vector < Point3f > >  object_Points;   
        uint32_t count;
        CALIB_RESULT calibration_results;
    }CALIB_APP_DATA;
    class monoFisheyeCalib
    {
        public: CALIB_APP_DATA  calibData_p;
        public:monoFisheyeCalib(uint32_t pixelW,uint32_t pixelH,uint32_t cbCol,uint32_t cbRow,float squareW_cm,float squareH_cm,uint32_t camID);
        public:~monoFisheyeCalib();
        public:void startCalibration();
        public:void endCalibration();
        public:int pushRawImageToCalib(uint8_t * grayRawDataPtr);
        public:void printCalibrationResults();
    };
};
namespace opencvStereoFECalib
{
    using namespace std;
    using namespace cv;
    typedef struct CALIB_RESUILT_t
    {
        cv::Mat K1, K2, R;
        cv::Mat T;
        cv::Mat D1, D2;
        cv::Mat R1, R2, P1, P2, Q;
    }CALIB_RESULT;
    typedef struct CALIB_APP_DATA_t
    {
        /*gloabl input data*/
        uint32_t cameraID;
        uint32_t pixelW;
        uint32_t pixelH;
        uint32_t cbCol;
        uint32_t cbRow;
        uint8_t * rawData_l;
        uint8_t * rawData_r;
        float squareW_cm;
        float squareH_cm;
        /*calibration steps data*/
        uint32_t pushRawGrayImgIndex;
        uint32_t patternfoundImgIndex;
        uint32_t sucessImgIndex;
        vector < vector < Point2f > >  corners_Seq_l;
        vector < vector < Point2f > >  corners_Seq_r;
        Size board_size;
        Size square_size;
        vector<Point2f> corners_l;
        vector<Point2f> corners_r;
        pthread_t th_extract_feature;
        vector < vector < Point3d > >  object_Points;   
        uint32_t count;
        CALIB_RESULT calibration_results;
    }CALIB_APP_DATA;
    class stereoFisheyeCalib
    {
        public: CALIB_APP_DATA  calibData_p;
        public:stereoFisheyeCalib(uint32_t pixelW,uint32_t pixelH,uint32_t cbCol,uint32_t cbRow,float squareW_cm,float squareH_cm,uint32_t camID);
        public:~stereoFisheyeCalib();
        public:void startCalibration();
        public:double endCalibration(int enable_input_matrix);
        public:void projectionGenerate();
        public:int pushRawImageToCalib(uint8_t * grayRawData_l_Ptr,uint8_t * grayRawData_r_Ptr);
        public:void printCalibrationResults(double error);
    };
};