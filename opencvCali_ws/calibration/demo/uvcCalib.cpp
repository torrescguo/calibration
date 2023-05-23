
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <stdio.h>
#include "../source/include/calibration.h"
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
/*CHESS BOARD SIZE*/
#define SQUARE_W_CM     (1.12f)
#define SQUARE_H_CM     (1.12f)
#define CHESS_ROW_NB    (6)
#define CHESS_COL_NB    (9)
#define CAPTURE_IMAGE_FPS (50)
#define WAIT_USLEEP_COFF  (1000*1000/50)



using namespace std;
using namespace cv;
using namespace opencvMonoFECalib;
using namespace CAMERA_MODEL_API;


// read yaml  
#define INTRINSICS_NUMS 7
#define DISTORTION_NUMS 7

static int fix_times = 0;

string isActiveDewarp = "true";

enum intrinsics_num
{
  PINHOLE_NUM = 4,
  OMNI_NUM = 5,
  DS_NMU = 6,
  EUCM_NUM = 6
};

enum distortion_num
{
  NONE_NUM = 0,
  FOV_NUM = 1,
  RADTAN_NUM = 4,
  EQUI_NUM = 4
};

// remap map definition
extern cv::Mat rmap[2][2];

// #define SHOW_RAW
#define ENABLE_OPENCV_CALIB
void start_uvc_camera()
{
    char path_f408_firstNode[50];
    udev_search_node(path_f408_firstNode,0);
    uvc_camera_sdk_init(path_f408_firstNode,UVC_CAM_WIDTH,UVC_CAM_HEIGHT*UVC_CAM_NB,1);
}
void switch_model_dewarp(int camera_model,float *intr1,float *intr2,float *r0,float *r2)
{
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
}



void read_test_yaml(string s_yaml)
{

  cv::FileStorage fs2(s_yaml, cv::FileStorage::READ);
  
  int frameCount = (int)fs2["frameCount"];
  
  std::string data;
  fs2["calibrationDate"] >> data;
  
  cv::Mat cameraMatrix2, distCoeffs2;
  fs2["cameraMatrix"] >> cameraMatrix2;
  fs2["distCoeffs"] >> distCoeffs2;
  
  cout << "frameCount : " << frameCount << endl
       << "calibration date : " << data << endl
       << "camera Matrix : " << cameraMatrix2 <<endl
       << "distCoeffs : " << distCoeffs2 << endl;
  cv::FileNode features = fs2["features"];

  cv::FileNodeIterator it = features.begin(), it_end = features.end();
  int idx = 0;

  std::vector<uchar> lbpval;

  for(; it != it_end; ++it, idx++)
  {
    cout << "features # " << idx << ":";
    cout << "x = " << (int)(*it)["x"] << ", y = " << (int)(*it)["y"] << ", lbp : (";
    (*it)["lbp"] >> lbpval;
    for(int i = 0; i < (int)lbpval.size(); i++)
    {
      cout << " " << (int)lbpval[i];
    }
    cout << ")" <<endl;
  }
  fs2.release();
}

void write_test_yaml(string s_yaml)
{
  //数据写入
  cv::FileStorage fs(s_yaml, cv::FileStorage::WRITE);

  fs << "frameCount" << 5;

  time_t rawtime;
  time(&rawtime);
  fs<< "calibrationDate" << asctime(localtime(&rawtime));

  cv::Mat cameraMatrix = (
   cv::Mat_<double>(3, 3)
   << 1000, 0, 320, 0, 1000, 240, 0, 0, 1
  );

  cv::Mat distCoeffs = (
    cv::Mat_<double>(5, 1)
    << 0.1, 0.01, -0.001, 0, 0
  );
 
  fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;
 
  fs << "features" << "[";
  for(int i = 0; i < 3; i++)
  {
    int x = rand() % 640;
    int y = rand() % 480;
    uchar lbp = rand() % 256; 
    fs << "{:" << "x" << x << "y" << y << "lbp" << "[:";
    for(int j = 0; j < 8; j++)
    {
      fs << ((lbp >> j) & 1);
    }
    fs << "]" << "}";
  }
 
  fs << "]";
  fs.release();
  
}

// # cameraModels = { 'pinhole-radtan': acvb.DistortedPinhole,
// #                  'pinhole-equidistant':   acvb.EquidistantPinhole,
// #                  'pinhole-fov':    acvb.FovPinhole,
// #                  'omni-none':      acvb.Omni,
// #                  'omni-radtan':    acvb.DistortedOmni,
// #                  'eucm-none':      acvb.ExtendedUnified,
// #                  'ds-none':        acvb.DoubleSphere}



void generate_orbsalm3_yaml(cv::FileStorage fs2,cv::FileStorage fs,string calib_mode)
{

  // intrinsics 
  // pinhole fx fy cx cy
  // omni    xi fx fy cx cy
  // ds      xi alpha fx fy cx cy
  // eucm    alpha beta fx fy cx cy
  double intrinsics_data[INTRINSICS_NUMS] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  double intrinsics_data2[INTRINSICS_NUMS] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};

  //distortion
  // radtan k1 k2 r1 r2
  // equidistant   k1 k2 k3 k4
  // fov    w
  // none   
  double distortion_data[DISTORTION_NUMS] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  double distortion_data2[DISTORTION_NUMS] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};


  if (calib_mode == "pinhole-radtan")
  {
    //fx fy cx cy
    for (int i = 0;i < PINHOLE_NUM;i++)
    {
      fs2["cam0"]["intrinsics"][i] >> intrinsics_data[i];
      fs2["cam1"]["intrinsics"][i] >> intrinsics_data2[i];
    }
    // k1 k2
    for (int i = 0;i < 2;i++)
    {
      fs2["cam0"]["distortion_coeffs"][i] >> distortion_data[i];
      fs2["cam1"]["distortion_coeffs"][i] >> distortion_data2[i];
    }
    //r1 r2
    for (int i = 2;i < 4;i++)
    {
      fs2["cam0"]["distortion_coeffs"][i] >> distortion_data[i+2];
      fs2["cam1"]["distortion_coeffs"][i] >> distortion_data2[i+2];
    }

    cout << "Use pinhole-radtan" << endl;
  }
  else if (calib_mode == "pinhole-equidistant")
  {

    // fx fy cx cy
    for (int i = 0;i < PINHOLE_NUM;i++)
    {
      fs2["cam0"]["intrinsics"][i] >> intrinsics_data[i];
      fs2["cam1"]["intrinsics"][i] >> intrinsics_data2[i];
    }
    // k1 k2 k3 k4
    for (int i = 0;i < EQUI_NUM;i++)
    {
      fs2["cam0"]["distortion_coeffs"][i] >> distortion_data[i];
      fs2["cam1"]["distortion_coeffs"][i] >> distortion_data2[i];
    }

    cout << "Use pinhole-equidistant" << endl;
  }
  else if (calib_mode == "pinhole-fov")
  {

    cout << "Entry pinhole-fov" << endl;
    //fx fy cx cy
    for (int i = 0;i < PINHOLE_NUM;i++)
    {
      fs2["cam0"]["intrinsics"][i] >> intrinsics_data[i];
      fs2["cam1"]["intrinsics"][i] >> intrinsics_data2[i];
    }
    cout << "Entry pinhole-fov---" << endl;
    //w
    {
      fs2["cam0"]["distortion_coeffs"][0] >> distortion_data[6];
      fs2["cam1"]["distortion_coeffs"][0] >> distortion_data2[6];

      cout << distortion_data[6] << endl;
      cout << distortion_data2[6] << endl;
    }
    
    cout << "Use pinhole-fov" << endl;
  }
  else if (calib_mode == "omni-none")
  {

    //fx fy cx cy
    for (int i = 1;i < OMNI_NUM;i++)
    {
      fs2["cam0"]["intrinsics"][i] >> intrinsics_data[i-1];
      fs2["cam1"]["intrinsics"][i] >> intrinsics_data2[i-1];
    }

    //xi
    fs2["cam0"]["intrinsics"][0] >> intrinsics_data[4];
    fs2["cam1"]["intrinsics"][0] >> intrinsics_data2[4];

    // distortion none

    cout << "Use omni-none" << endl;
  }
  else if (calib_mode == "omni-radtan")
  {

    //fx fy cx cy
    for (int i = 1;i < OMNI_NUM;i++)
    {
      fs2["cam0"]["intrinsics"][i] >> intrinsics_data[i-1];
      fs2["cam1"]["intrinsics"][i] >> intrinsics_data2[i-1];
    }

    //xi
    fs2["cam0"]["intrinsics"][0] >> intrinsics_data[4];
    fs2["cam1"]["intrinsics"][0] >> intrinsics_data2[4];

    // k1 k2
    for (int i = 0;i < 2;i++)
    {
      fs2["cam0"]["distortion_coeffs"][i] >> distortion_data[i];
      fs2["cam1"]["distortion_coeffs"][i] >> distortion_data2[i];
    }
    //r1 r2
    for (int i = 2;i < 4;i++)
    {
      fs2["cam0"]["distortion_coeffs"][i] >> distortion_data[i+2];
      fs2["cam1"]["distortion_coeffs"][i] >> distortion_data2[i+2];
    }

    cout << "Use omni-radtan" << endl;
  }
  else if (calib_mode == "eucm-none")
  {

    //fx fy cx cy
    for (int i = 2;i < EUCM_NUM;i++)
    {
      fs2["cam0"]["intrinsics"][i] >> intrinsics_data[i-2];
      fs2["cam1"]["intrinsics"][i] >> intrinsics_data2[i-2];
    }

    //alpha beta
    for (int i = 0;i < 2;i++)
    {
      fs2["cam0"]["intrinsics"][i] >> intrinsics_data[i+5];
      fs2["cam1"]["intrinsics"][i] >> intrinsics_data2[i+5];
    }
    
    cout << "Use eucm-none" << endl;
  }
  else if (calib_mode == "ds-none")
  {
    
//fx fy cx cy
    for (int i = 2;i < DS_NMU;i++)
    {
      fs2["cam0"]["intrinsics"][i] >> intrinsics_data[i-2];
      fs2["cam1"]["intrinsics"][i] >> intrinsics_data2[i-2];
    }

    // xi alpha
    for (int i = 0;i < 2;i++)
    {
      fs2["cam0"]["intrinsics"][i] >> intrinsics_data[i+4];
      fs2["cam1"]["intrinsics"][i] >> intrinsics_data2[i+4];
    }
    cout << "Use ds-none" << endl;
  }

  {
    //fx fy cx cy xi alpha beta
// write new yaml
     //camera 1
    // fs << "#" << "fx fy cx cy xi alpha beta";
    fs.writeComment("Camera left : fx fy cx cy xi alpha beta",false);
    fs << "CameraWWWfx" << intrinsics_data[0];
    fs << "CameraWWWfy" << intrinsics_data[1];
    fs << "CameraWWWcx" << intrinsics_data[2];
    fs << "CameraWWWcy" << intrinsics_data[3];
    fs << "CameraWWWxi" << intrinsics_data[4];
    fs << "CameraWWWalpha" << intrinsics_data[5];
    fs << "CameraWWWbeta" << intrinsics_data[6];
    // k1 k2 k3 k4 p1(r1) p2(r2) w
    fs.writeComment("Camera left : k1 k2 k3 k4 p1(r1) p2(r2) w",false);
    fs << "CameraWWWk1" << distortion_data[0];
    fs << "CameraWWWk2" << distortion_data[1];
    fs << "CameraWWWk3" << distortion_data[2];
    fs << "CameraWWWk4" << distortion_data[3];
    fs << "CameraWWWp1" << distortion_data[4];
    fs << "CameraWWWp2" << distortion_data[5];
    fs << "CameraWWWw" << distortion_data[6];

    //camera 2
    fs.writeComment("Camera right : fx fy cx cy xi alpha beta",false);
    fs << "Camera2WWWfx"    << intrinsics_data2[0];
    fs << "Camera2WWWfy"    << intrinsics_data2[1];
    fs << "Camera2WWWcx"    << intrinsics_data2[2];
    fs << "Camera2WWWcy"    << intrinsics_data2[3];
    fs << "Camera2WWWxi"    << intrinsics_data2[4];
    fs << "Camera2WWWalpha" << intrinsics_data2[5];
    fs << "Camera2WWWbeta"  << intrinsics_data2[6];

    fs.writeComment("Camera right : k1 k2 k3 k4 p1(r1) p2(r2) w",false);
    fs << "Camera2WWWk1" << distortion_data2[0];
    fs << "Camera2WWWk2" << distortion_data2[1];
    fs << "Camera2WWWk3" << distortion_data2[2];
    fs << "Camera2WWWk4" << distortion_data2[3];
    fs << "Camera2WWWp1" << distortion_data2[4];
    fs << "Camera2WWWp2" << distortion_data2[5];
    fs << "Camera2WWWw"  << distortion_data2[6];
  }

}
void read_calibration_yaml(string s_yaml_noise,string s_yaml,string s_out_yaml)
{

  cv::FileStorage fs2(s_yaml, cv::FileStorage::READ);
  cv::FileStorage fs(s_out_yaml, cv::FileStorage::WRITE);
  cv::FileStorage fs_n(s_yaml_noise,cv::FileStorage::READ);
  
  int cam_overlaps_value;
  cam_overlaps_value = (int)fs2["cam0"]["cam_overlaps"][0];
  cout << "cam0:" << endl;
  cout << "cam_overlaps:" << cam_overlaps_value << endl;

  string fill_str = "WWW";

  
  // get calibration models
  string camera_model_data;
  fs2["cam0"]["camera_model"] >> camera_model_data;

  string distortion_model;
  fs2["cam0"]["distortion_model"] >> distortion_model;

  string camera_distortion_model;
  camera_distortion_model = camera_model_data+"-"+distortion_model;
  cout << camera_distortion_model << endl;


  fs << "CameraWWWtype" << camera_model_data;
  fs << "DistortionWWWtype" << distortion_model;
  fs << "EnableDewarp" << isActiveDewarp;

  

  generate_orbsalm3_yaml(fs2,fs,camera_distortion_model);

  fs.writeComment("Camera resolution",false);
  fs << "CameraWWWwidth" << (int)fs2["cam0"]["resolution"][0];
  fs << "CameraWWWheight" << (int)fs2["cam0"]["resolution"][1];

  fs.writeComment("Camera frames per second",false);
  fs << "CameraWWWfps" << 30.0;

  fs.writeComment("Color order of the images (0: BGR, 1: RGB. It is ignored if images are grayscale)",false);
  fs << "CameraWWWRGB" << 1;

  fs.writeComment("Image scale, it changes the image size to be processed (<1.0: reduce, >1.0: increase)",false);
  fs << "CameraWWWimageScale" << 1;

  fs.writeComment("Close/Far threshold. Baseline times.",false);
  fs << "ThDepth" << 80.0;

  fs.writeComment("stereo baseline times fx. = fx * baseline /m",false);
  fs << "CameraWWWbf" << 16.78;

  // tbc ,from imu to left camera
  fs.writeComment("Tbc , from imu to left camera",false);
  {
    float t_cam_imu[4][4];
    for (int i = 0;i < 4;i++)
    {
      for (int j = 0;j < 4;j++)
      {
        t_cam_imu[i][j] = (float)fs2["cam0"]["T_cam_imu"][i][j];
      }
    }
    cv::Mat k1_mat = cv::Mat(cv::Size(4,4), CV_32FC1, t_cam_imu);

    fs << "Tbc" << k1_mat;
  
  }

  // tbc1 ,from imu to right camera
  fs.writeComment("Tbc1 , from imu to right camera",false);
  {
    float t_cam_imu[4][4];
    for (int i = 0;i < 4;i++)
    {
      for (int j = 0;j < 4;j++)
      {
        t_cam_imu[i][j] = (float)fs2["cam1"]["T_cam_imu"][i][j];
      }
    }
    cv::Mat k1_mat = cv::Mat(cv::Size(4,4), CV_32FC1, t_cam_imu);

    fs << "Tbc1" << k1_mat;
  
  }

  fs.writeComment("Trl , from left camera to right camera",false);

  {
    float t_cn_cnm1[4][4];
    for (int i = 0;i < 4;i++)
    {
      for (int j = 0;j < 4;j++)
      {
        t_cn_cnm1[i][j] = (float)fs2["cam1"]["T_cn_cnm1"][i][j];
      }
    }
    cv::Mat k1_mat = cv::Mat(cv::Size(4,4), CV_32FC1, t_cn_cnm1);

    fs << "Trl" << k1_mat;
  
  }
  
  //imu noise
  fs.writeComment("timeshift from cam1 to imu",false);
  fs << "IMUWWWtimeshift_cam1_imu" << (float)fs2["cam0"]["timeshift_cam_imu"];

  fs.writeComment("timeshift from cam2 to imu",false);
  fs << "IMUWWWtimeshift_cam2_imu" << (float)fs2["cam1"]["timeshift_cam_imu"];

  fs.writeComment("Do not insert KFs when recently lost",false);
  fs << "InsertKFsWhenLost" << 0;

  fs.writeComment("IMU noise (Use those from VINS-mono)",false);
  fs << "IMUWWWNoiseGyro"    << (float)fs_n["imu0"]["gyroscope_noise_density"];
  fs << "IMUWWWNoiseAcc"     << (float)fs_n["imu0"]["accelerometer_noise_density"];
  fs << "IMUWWWGyroWalk"     << (float)fs_n["imu0"]["gyroscope_random_walk"];
  fs << "IMUWWWAccWalk"      << (float)fs_n["imu0"]["accelerometer_random_walk"];

  fs.writeComment("IMU Frequency)",false);
  fs << "IMUWWWFrequency"    << (int)fs_n["imu0"]["update_rate"];


  fs.writeComment("",false);
  fs.writeComment("ORB Parameters",false);

  fs.writeComment("ORB Extractor: Number of features per image",false);
  fs << "ORBextractorWWWnFeatures"     << 1500;

  fs.writeComment("ORB Extractor: Scale factor between levels in the scale pyramid",false);
  fs << "ORBextractorWWWscaleFactor"   << 1.2;

  fs.writeComment("ORB Extractor: Number of levels in the scale pyramid",false);
  fs << "ORBextractorWWWnLevels"       << 8;


  fs.writeComment("Image is divided in a grid. At each cell FAST are extracted imposing a minimum response.",false);
  fs.writeComment("Firstly we impose iniThFAST. If no corners are detected we impose a lower value minThFAST",false);
  fs.writeComment("You can lower these values if your images have low contrast",false);
  fs << "ORBextractorWWWiniThFAST"     << 20;
  fs << "ORBextractorWWWminThFAST"     << 7;

  fs.writeComment("Viewer Parameters",false);
  fs << "ViewerWWWKeyFrameSize"        << 0.05;
  fs << "ViewerWWWKeyFrameLineWidth"   << 1;
  fs << "ViewerWWWGraphLineWidth"      << 0.9;
  fs << "ViewerWWWPointSize"           << 2;
  fs << "ViewerWWWCameraSize"          << 8e-2;
  fs << "ViewerWWWCameraLineWidth"     << 3;
  fs << "ViewerWWWViewpointX"          << 0;
  fs << "ViewerWWWViewpointY"          << -0.7;
  fs << "ViewerWWWViewpointZ"          << -3.5;
  fs << "ViewerWWWViewpointF"          << 500;

  
  fs2.release();

  fs_n.release();

  fs.release();
}

void fix_yaml(string s_yaml_ori)
{
  fix_times++;
	ifstream inFile;
	inFile.open(s_yaml_ori);
	if (!inFile)
	{
		cout << s_yaml_ori << "read fail!" << endl;
		exit(1);
	}

  // char s_yaml_out_temp[50];

  // sprintf(s_yaml_out_temp,"./data/%d_temp.yaml",fix_times);

	ofstream outFile;
  string s_yaml_out = to_string(fix_times) + "temp.yaml";
  // for (int i = 0;i < sizeof(s_yaml_out_temp);i++)
  // {
  //   s_yaml_out.push_back(s_yaml_out_temp[i]);

  //   // s_yaml_out.insert(0, 1, s_yaml_out_temp[i]);
  // }
  cout << s_yaml_out << endl;
  // s_yaml_out = "./data/temp.yaml";
	outFile.open(s_yaml_out);

  
	string line;
	bool flag = false;

  ifstream outTemp;
	outTemp.open(s_yaml_out);
	if (!outTemp)
	{
		cout << s_yaml_ori << "read fail!" << endl;
		exit(1);
	}

  string yaml_fomat = "%YAML:1.0";
  string yaml_lines = "---";
  int lineCnt = 1;

	while (getline(inFile, line))
	{
    if ((lineCnt == 1) && (line.data() != yaml_fomat))
    {
      outFile << yaml_fomat << endl;
      outFile << yaml_lines << endl;
    }
      
    {
      if ((line.find("T_cn_cnm1") != string::npos) || (line.find("T_cam_imu") != string::npos) || (line.find("T_i_b") != string::npos))
      {
        flag = true;
        outFile << line << endl;
        for (int i = 0; i < 4; i++)
        {
          getline(inFile, line);
          outFile << "  " << line << endl;
        }
      }
      else if (flag)
      {
        flag = false;
        outFile << line << endl;
      }
      else
        outFile << line << endl;
    }

    lineCnt++;  

	}

	inFile.close();
  outFile.close();


  // rename output file
  const char *str1 = s_yaml_out.data();
  const char *str2 = s_yaml_ori.data();

  char s_system_in[200];
  sprintf(s_system_in,"%s %s %s","mv",str1,str2);
  printf(s_system_in);
  system(s_system_in);
  cout << endl;
}


static bool FileStringReplace(ifstream &instream, ofstream &outstream)
{
	string str;
	size_t pos = 0;
	while (getline(instream, str)) 
	{
		pos = str.find("WWW"); 
		if (pos != string::npos)
		{
			str = str.replace(pos, 3, "."); 
			outstream << str << endl;
			continue;
		}
		outstream << str << endl;
	}
	return true;
}

void fix_out_yaml(string s_yaml_in,string s_yaml_out)
{

	ifstream instream(s_yaml_in); // instream.open(file_path) 默认以ifstream::in打开
	ofstream outstream(s_yaml_out); // outstream.open(out_path) 默认以ostream::out打开,文件内容会被丢弃,可使用app模式(写指针会被定位到文件末尾)
	FileStringReplace(instream, outstream);
	instream.close();
	outstream.close();



}

  

void projectionGenerate(ORB_CALIB_RESULT *orb_calib_res_t,string path_ori)
{
    
    Size image_size(UVC_CAM_WIDTH,UVC_CAM_HEIGHT);

    ORB_CALIB_CFG_ALL orb_cfg_all;

    get_orb_calib_result(&orb_cfg_all,(char*)path_ori.data());

    double fx1 = (double)orb_cfg_all.orb_intr1.k[0];
    double fy1 = (double)orb_cfg_all.orb_intr1.k[1];
    double cx1 = (double)orb_cfg_all.orb_intr1.k[2];
    double cy1 = (double)orb_cfg_all.orb_intr1.k[3];

    double fx2 = (double)orb_cfg_all.orb_intr2.k[0];
    double fy2 = (double)orb_cfg_all.orb_intr2.k[1];
    double cx2 = (double)orb_cfg_all.orb_intr2.k[2];
    double cy2 = (double)orb_cfg_all.orb_intr2.k[3];

    double k1_1 = (double)orb_cfg_all.orb_intr1.k[4];
    double k2_1 = (double)orb_cfg_all.orb_intr1.k[5];
    double p1_1 = (double)orb_cfg_all.orb_intr1.k[6];
    double p2_1 = (double)orb_cfg_all.orb_intr1.k[7];

    double k1_2 = (double)orb_cfg_all.orb_intr2.k[4];
    double k2_2 = (double)orb_cfg_all.orb_intr2.k[5];
    double p1_2 = (double)orb_cfg_all.orb_intr2.k[6];
    double p2_2 = (double)orb_cfg_all.orb_intr2.k[7];

    double kk1[9] = {fx1,0,cx1,
                    0,fy1,cy1,
                    0,0,1};
    double kk2[9] = {fx2,0,cx2,
                    0,fy2,cy2,
                    0,0,1};

    double dd1[4] = {k1_1,k2_1,p1_1,p2_1};
    double dd2[4] = {k1_2,k2_2,p1_2,p2_2};

    double r[9] = {0.0};
    double t[3] = {0.0};
    for (int i = 0;i < 9;i++)
    {
        r[i] = orb_cfg_all.orb_extr_tlr.r[i];
    }
    for (int i = 0;i < 3;i++)
    {
        t[i] = orb_cfg_all.orb_extr_tlr.t[i];
    }
    t[0] = t[0]*100;
    cout << "base line:" << t[0] << endl;
    // cam0 to cam1
    double trl[16] = {0.0};
    trl[15] = 1.;
    
    cv::Mat Trl = cv::Mat(cv::Size(4,4), CV_64FC1,trl);
    for (int i = 0;i < 3;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            Trl.ptr<double>(i,j)[0] = (double)orb_cfg_all.orb_extr_tlr.r[i*3+j];
        }
    }

    for (int i = 0;i < 3;i++)
    {
        Trl.ptr<double>(i,3)[0] = (double)orb_cfg_all.orb_extr_tlr.t[i];
    }

    
    orb_calib_res_t->K1 = cv::Mat(cv::Size(3,3), CV_64FC1, kk1);
    orb_calib_res_t->K2 = cv::Mat(cv::Size(3,3), CV_64FC1, kk2);
    orb_calib_res_t->D1 = cv::Mat(cv::Size(1,4), CV_64FC1, dd1);
    orb_calib_res_t->D2 = cv::Mat(cv::Size(1,4), CV_64FC1, dd2);
    orb_calib_res_t->R = cv::Mat(cv::Size(3,3), CV_64FC1,  r);
    orb_calib_res_t->T = cv::Mat(cv::Size(1,3), CV_64FC1,  t);

    for (int i = 0;i < 3;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            cout << "K1[" << i*3+j << "]:" << orb_calib_res_t->K1.ptr<double>(i,j)[0] << endl;;
        }
    }
    cout << "********************process*******************" << endl;
    // cam1 to cam0
    orb_calib_res_t->Tlr = Trl.inv();
    
    cout << orb_calib_res_t->K1.ptr<double>(0,0)[0] << "\n" << orb_calib_res_t->K1.ptr<double>(0,2)[0] << endl;
    fisheye::stereoRectify(orb_calib_res_t->K1, orb_calib_res_t->D1, 
                            orb_calib_res_t->K2, orb_calib_res_t->D2, image_size, orb_calib_res_t->R,
                            orb_calib_res_t->T, orb_calib_res_t->R1, orb_calib_res_t->R2, 
                            orb_calib_res_t->P1, orb_calib_res_t->P2,
                            orb_calib_res_t->Q, 1024, image_size, 0.0, 1.1);
    
    orb_calib_res_t->prInv1 = (orb_calib_res_t->P1.colRange(0,3) * orb_calib_res_t->R1).inv();
    orb_calib_res_t->prInv2 = (orb_calib_res_t->P2.colRange(0,3) * orb_calib_res_t->R2).inv();
                            
    for (int i = 0;i < 3;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            cout << "K1[" << i*3+j << "]:" << orb_calib_res_t->K1.ptr<double>(i,j)[0] << endl;;
        }
    }
    char path[100];
    sprintf(path,"../cam%d_steroCalibration.yml",0);
    cv::FileStorage fs(path, true);
    {
        if (fs.isOpened())
        {
            fs << "K1" << orb_calib_res_t->K1 << "D1" << orb_calib_res_t->D1 << "K2" << orb_calib_res_t->K2 << "D2" << orb_calib_res_t->D2;
            fs << "P1" << orb_calib_res_t->P1 << "P2" << orb_calib_res_t->P2;
            fs << "R" << orb_calib_res_t->R << "T" << orb_calib_res_t->T << "R1" << orb_calib_res_t->R1 << "R2" << orb_calib_res_t->R2 << "Q" << orb_calib_res_t->Q;
            fs << "prInv1" << orb_calib_res_t->prInv1;
            fs << "prInv2" << orb_calib_res_t->prInv2;
            fs << "Tlr" << orb_calib_res_t->Tlr;
            fs.release();
        }
        else
        {   
            cout << "Error: can not save the intrinsic parameters\n" << endl;
        }

    }

    cout << "**************************new yaml******************************" << endl;
    cout << path_ori << endl;
    cv::FileStorage fs_s(path_ori, cv::FileStorage::APPEND);
    
    if (!fs_s.isOpened())
    {
        cout << "Open yaml fail!" << endl;
    }
    
    orb_calib_res_t->R1.convertTo(orb_calib_res_t->R1,CV_32FC1);
    orb_calib_res_t->R2.convertTo(orb_calib_res_t->R2,CV_32FC1);
    orb_calib_res_t->P1.convertTo(orb_calib_res_t->P1,CV_32FC1);
    orb_calib_res_t->P2.convertTo(orb_calib_res_t->P2,CV_32FC1);
    orb_calib_res_t->Q.convertTo(orb_calib_res_t->Q,CV_32FC1);
    orb_calib_res_t->Tlr.convertTo(orb_calib_res_t->Tlr,CV_32FC1);
    orb_calib_res_t->prInv1.convertTo(orb_calib_res_t->prInv1,CV_32FC1);
    orb_calib_res_t->prInv2.convertTo(orb_calib_res_t->prInv2,CV_32FC1);

    fs_s << "r1"      << orb_calib_res_t->R1;
    fs_s << "r2"      << orb_calib_res_t->R2;
    fs_s << "p1"      << orb_calib_res_t->P1;
    fs_s << "p2"      << orb_calib_res_t->P2;
    fs_s << "q"       << orb_calib_res_t->Q;
    fs_s << "Tlr"     << orb_calib_res_t->Tlr;
    fs_s << "prInv1"  << orb_calib_res_t->prInv1;
    fs_s << "prInv2"  << orb_calib_res_t->prInv2;
    
    fs_s.release();
    
        
}

int main_stereo(int argc,char * argv[])
{

    if (argc < 5)
    {
        cout << "Input params too few !" << endl;
        exit(0);
    }

    // demo file
    cout << argv[0] << endl;
    // fist argument
    cout << argv[1] << endl;
    // second argument
    cout << argv[2] << endl;
    // thrid argument
    cout << argv[3] << endl;
    // fouth argument
    cout << argv[4] << endl;
    

    //imu-camera joint
    string s_calibration_yaml = string(argv[1]);
    //imu noise
    string s_calibration_yaml_noise = string(argv[2]);

    string s_calibration__out_yaml = string(argv[3]);

    string s_calibration__out_yaml_new = string(argv[4]);


    cout << "--------------------" << endl;

    cout << s_calibration_yaml << endl;
    cout << s_calibration_yaml_noise << endl;
    cout << s_calibration__out_yaml  << endl;
    cout << s_calibration__out_yaml_new << endl;

    // s_calibration_yaml = "../data/imu_visio/omni-none/imu-camera-camchain-imucam.yaml";
    fix_yaml(s_calibration_yaml);
    fix_yaml(s_calibration_yaml_noise);

    read_calibration_yaml(s_calibration_yaml_noise,s_calibration_yaml,s_calibration__out_yaml);

    fix_out_yaml(s_calibration__out_yaml,s_calibration__out_yaml_new);

    // rm temp file
    char s_system_rm[100];
    const char *path_rm = s_calibration__out_yaml.data();
    sprintf(s_system_rm,"%s %s","rm -f",path_rm);
    printf(s_system_rm);
    system(s_system_rm);

    cout << "start generate projection matrix!" << endl;
    ORB_CALIB_RESULT orb_calib_result;
    projectionGenerate(&orb_calib_result,s_calibration__out_yaml_new);

#ifdef ENABLE_OPENCV_CALIB

    
    char path[100] = "../cam0_steroCalibration.yml";
    OPENCV_CALIB_RESULT opencv_claib_result;
    get_opencv_calib_result(&opencv_claib_result,path);

    cv::Mat k1_mat = cv::Mat(cv::Size(3,3), CV_32FC1, opencv_claib_result.K1);
    cv::Mat k2_mat = cv::Mat(cv::Size(3,3), CV_32FC1, opencv_claib_result.K2);
    cv::Mat d1_mat = cv::Mat(cv::Size(1,4), CV_32FC1, opencv_claib_result.D1);
    cv::Mat d2_mat = cv::Mat(cv::Size(1,4), CV_32FC1, opencv_claib_result.D2);
    cv::Mat r1_mat = cv::Mat(cv::Size(3,3), CV_32FC1, opencv_claib_result.R1);
    cv::Mat r2_mat = cv::Mat(cv::Size(3,3), CV_32FC1, opencv_claib_result.R2);
    // 4cols 3 cows
    cv::Mat p1_mat = cv::Mat(cv::Size(4,3), CV_32FC1, opencv_claib_result.P1);
    cv::Mat p2_mat = cv::Mat(cv::Size(4,3), CV_32FC1, opencv_claib_result.P2);


    fisheye::initUndistortRectifyMap(k1_mat, d1_mat, r1_mat, p1_mat,cv::Size(UVC_CAM_WIDTH,UVC_CAM_HEIGHT) , CV_16SC2, rmap[0][0], rmap[0][1]);
    fisheye::initUndistortRectifyMap(k2_mat, d2_mat, r2_mat, p2_mat,cv::Size(UVC_CAM_WIDTH,UVC_CAM_HEIGHT) , CV_16SC2, rmap[1][0], rmap[1][1]);

    
#else
    rmap[0][0] = cv::Mat(cv::Size(640,400), CV_32FC1);
    rmap[0][1] = cv::Mat(cv::Size(640,400), CV_32FC1);
    rmap[1][0] = cv::Mat(cv::Size(640,400), CV_32FC1);
    rmap[1][1] = cv::Mat(cv::Size(640,400), CV_32FC1);

    
    for (int i = 0; i < 400; i++)
    {
        for (int j = 0; j < 640; j++)
        {
            rmap[0][0].ptr<float>(i,j)[0] = (float)j;
            rmap[0][1].ptr<float>(i,j)[0] = (float)(400-i);
            rmap[1][0].ptr<float>(i,j)[0] = (float)j;
            rmap[1][1].ptr<float>(i,j)[0] = (float)(400-i);
            
        }
    }
    
#endif

    std::cout << "******** Done Calibration ********\n" << std::endl;
    
    
    // uvc_camera_sdk_stream_stop();
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


        #ifdef SHOW_RAW
        hconcat(imageL,imageR,display);
        #else
        hconcat(recImgL,recImgR,display);
        #endif
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

int main(int argc,char * argv[])
{
    main_stereo(argc,argv);
}