#include "calibration.h"
#include "orbCalibration.h"
opencvMonoFECalib::monoFisheyeCalib::monoFisheyeCalib(uint32_t pixelW,uint32_t pixelH,uint32_t cbCol,uint32_t cbRow,float squareW_cm,float squareH_cm,uint32_t camID)
{
    this->calibData_p.pixelW=pixelW;
    this->calibData_p.pixelH=pixelH;
    this->calibData_p.cbCol=cbCol;
    this->calibData_p.cbRow=cbRow;
    this->calibData_p.squareW_cm=squareW_cm;
    this->calibData_p.squareH_cm=squareH_cm;
    this->calibData_p.patternfoundImgIndex=0;
    this->calibData_p.pushRawGrayImgIndex=0;
    this->calibData_p.sucessImgIndex=0;
    this->calibData_p.square_size=Size(squareW_cm,squareH_cm);
    this->calibData_p.board_size=Size(cbCol,cbRow);
    this->calibData_p.cameraID=camID;
    this->calibData_p.count=0;
    this->calibData_p.patternfoundImgIndex=0;
    this->calibData_p.rawData = (uint8_t*)malloc(pixelW*pixelH*sizeof(uint8_t));
}
void opencvMonoFECalib::monoFisheyeCalib::startCalibration()
{
    
}
void opencvMonoFECalib::monoFisheyeCalib::endCalibration()
{
	for (int t = 0; t<this->calibData_p.patternfoundImgIndex; t++)
	{
	    vector<Point3f> tempPointSet;
	    for (int i = 0; i<this->calibData_p.board_size.height; i++)
	    {
	        for (int j = 0; j<this->calibData_p.board_size.width; j++)
	        {
	                /* 假设定标板放在世界坐标系中z=0的平面上 */
	                Point3f tempPoint;
	                tempPoint.y = i*this->calibData_p.square_size.height;
	                tempPoint.x = j*this->calibData_p.square_size.width;
	                tempPoint.z = 0;
	                tempPointSet.push_back(tempPoint);
	        }
	    }
	    this->calibData_p.object_Points.push_back(tempPointSet);
	}
    Size image_size = this->calibData_p.image_Seq[0].size();
    int flags = 0;
	flags |= cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC;
	// flags |= cv::fisheye::CALIB_CHECK_COND;
	flags |= cv::fisheye::CALIB_FIX_SKEW;
    //flags |= cv::fisheye::CALIB_FIX_K3 ;
    //flags |= cv::fisheye::CALIB_FIX_K4;
    printf("this->calibData_p.object_Points.size()=%d\n",this->calibData_p.object_Points.size());
    printf("this->calibData_p.corners_Seq.size()=%d\n",this->calibData_p.corners_Seq.size());
	fisheye::calibrate(this->calibData_p.object_Points, this->calibData_p.corners_Seq, image_size, this->calibData_p.calibration_results.intrinsic_matrix, this->calibData_p.calibration_results.distortion_coeffs, this->calibData_p.calibration_results.rotation_vectors, this->calibData_p.calibration_results.translation_vectors, flags, cv::TermCriteria(3, 20, 1e-6));
	cout<<"定标完成！\n";
}
int opencvMonoFECalib::monoFisheyeCalib::pushRawImageToCalib(uint8_t * grayRawDataPtr)
{
    memcpy(this->calibData_p.rawData,grayRawDataPtr,this->calibData_p.pixelW*this->calibData_p.pixelH*sizeof(uint8_t));
    cv::Mat image = cv::Mat(cv::Size(this->calibData_p.pixelW,this->calibData_p.pixelH), CV_8UC1, this->calibData_p.rawData);
    // Size board_size = Size(3,3);
    this->calibData_p.pushRawGrayImgIndex++;
    bool patternfound=false;
    {
        this->calibData_p.corners.clear();
        uint32_t imgW = this->calibData_p.pixelW,imgH = this->calibData_p.pixelH;
        printf("imgW=%d,imgH=%d\n",imgW,imgH);
        cv::Mat image = cv::Mat(cv::Size(imgW,imgH), CV_8UC1, this->calibData_p.rawData);
        cout<<"findChessboardCorners start" << endl;
        patternfound = findChessboardCorners(image,  this->calibData_p.board_size, this->calibData_p.corners,CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE+CALIB_CB_FAST_CHECK);
        cout<<"findChessboardCorners ok" << endl;
        if(patternfound)
        {
            printf("this->calibData_p.patternfoundImgIndex=%d\n",this->calibData_p.patternfoundImgIndex);
            this->calibData_p.patternfoundImgIndex++;
            cornerSubPix(image, this->calibData_p.corners, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            this->calibData_p.corners_Seq.push_back(this->calibData_p.corners);
            this->calibData_p.count = this->calibData_p.count + this->calibData_p.corners.size();
            this->calibData_p.image_Seq.push_back(image);
        }
    }
    return ((patternfound)? 1 : 0);
}
opencvMonoFECalib::monoFisheyeCalib::~monoFisheyeCalib()
{
    free(this->calibData_p.rawData);
}
void opencvMonoFECalib::monoFisheyeCalib::printCalibrationResults()
{
    char path[100];
    sprintf(path,"./cam%d_intrinsics.yml",this->calibData_p.cameraID);
    cv::FileStorage fs(path, true);
    if (fs.isOpened())
    {
            fs << "M1" << this->calibData_p.calibration_results.intrinsic_matrix << "D1" << this->calibData_p.calibration_results.distortion_coeffs;
            fs.release();
    }
    else
        cout << "Error: can not save the intrinsic parameters\n";

	    cout<<" [ " << "cam_" << this->calibData_p.cameraID << " ] " <<"开始保存定标结果………………"<<endl;

	    cout<<" [ " << "cam_" << this->calibData_p.cameraID << " ] " <<"相机内参数矩阵："<<endl;
	    cout<<this->calibData_p.calibration_results.intrinsic_matrix<<endl;
	    cout<<" [ " << "cam_" << this->calibData_p.cameraID << " ] " <<"畸变系数：\n";
	    cout<<this->calibData_p.calibration_results.distortion_coeffs<<endl;
}
//stereo
opencvStereoFECalib::stereoFisheyeCalib::stereoFisheyeCalib(uint32_t pixelW,uint32_t pixelH,uint32_t cbCol,uint32_t cbRow,float squareW_cm,float squareH_cm,uint32_t camID)
{
    this->calibData_p.pixelW=pixelW;
    this->calibData_p.pixelH=pixelH;
    this->calibData_p.cbCol=cbCol;
    this->calibData_p.cbRow=cbRow;
    this->calibData_p.squareW_cm=squareW_cm;
    this->calibData_p.squareH_cm=squareH_cm;
    this->calibData_p.patternfoundImgIndex=0;
    this->calibData_p.pushRawGrayImgIndex=0;
    this->calibData_p.sucessImgIndex=0;
    this->calibData_p.square_size=Size(squareW_cm,squareH_cm);
    this->calibData_p.board_size=Size(cbCol,cbRow);
    this->calibData_p.cameraID=camID;
    this->calibData_p.count=0;
    this->calibData_p.patternfoundImgIndex=0;
    this->calibData_p.rawData_l = (uint8_t*)malloc(pixelW*pixelH*sizeof(uint8_t));
    this->calibData_p.rawData_r = (uint8_t*)malloc(pixelW*pixelH*sizeof(uint8_t));
    // this->calibData_p.corners_Seq_l.reserve(300);
    // this->calibData_p.corners_Seq_r.reserve(300);
}
void opencvStereoFECalib::stereoFisheyeCalib::startCalibration()
{
    
}


void opencvStereoFECalib::stereoFisheyeCalib::projectionGenerate()
{
    Size image_size(640,400);
 
    
    {
        printf("*********************************\n");
        ORB_CALIB_CFG_ALL orb_cfg_all;
        
        char path_temp[100] = "cougar2085v4_pinhole_equi.yml";
        cout << "Hello Hello Hello" << endl;
        get_orb_calib_result(&orb_cfg_all,path_temp);

        float fx1 = orb_cfg_all.orb_intr1.k[0];
        float fy1 = orb_cfg_all.orb_intr1.k[1];
        float cx1 = orb_cfg_all.orb_intr1.k[2];
        float cy1 = orb_cfg_all.orb_intr1.k[3];

        float fx2 = orb_cfg_all.orb_intr2.k[0];
        float fy2 = orb_cfg_all.orb_intr2.k[1];
        float cx2 = orb_cfg_all.orb_intr2.k[2];
        float cy2 = orb_cfg_all.orb_intr2.k[3];

        float k1_1 = orb_cfg_all.orb_intr1.k[4];
        float k2_1 = orb_cfg_all.orb_intr1.k[5];
        float p1_1 = orb_cfg_all.orb_intr1.k[6];
        float p2_1 = orb_cfg_all.orb_intr1.k[7];

        float k1_2 = orb_cfg_all.orb_intr2.k[4];
        float k2_2 = orb_cfg_all.orb_intr2.k[5];
        float p1_2 = orb_cfg_all.orb_intr2.k[6];
        float p2_2 = orb_cfg_all.orb_intr2.k[7];

        float kk1[9] = {fx1,0,cx1,
                        0,fy1,cy1,
                        0,0,1};
        float kk2[9] = {fx2,0,cx2,
                        0,fy2,cy2,
                        0,0,1};

        float dd1[4] = {k1_1,k2_1,p1_1,p2_1};
        float dd2[4] = {k1_2,k2_2,p1_2,p2_2};


        cv::Mat k1_mat = cv::Mat(cv::Size(3,3), CV_32FC1, kk1);
        cv::Mat k2_mat = cv::Mat(cv::Size(3,3), CV_32FC1, kk2);
        cv::Mat d1_mat = cv::Mat(cv::Size(1,4), CV_32FC1, dd1);
        cv::Mat d2_mat = cv::Mat(cv::Size(1,4), CV_32FC1, dd2);
        cv::Mat R_mat = cv::Mat(cv::Size(3,3), CV_32FC1,  orb_cfg_all.orb_extr_tlr.r);
        cv::Mat T_mat = cv::Mat(cv::Size(1,3), CV_32FC1,  orb_cfg_all.orb_extr_tlr.t);

        for (int i = 0;i < 9;i++)
        {
            cout << "kk1[" << i << "]" << kk1[i] << endl;
        }

        this->calibData_p.calibration_results.K1 = cv::Mat(cv::Size(3,3), CV_32FC1, kk1);
        this->calibData_p.calibration_results.D1 = cv::Mat(cv::Size(1,4), CV_32FC1, dd1);
        this->calibData_p.calibration_results.K2 = cv::Mat(cv::Size(3,3), CV_32FC1, kk2);
        this->calibData_p.calibration_results.D2 = cv::Mat(cv::Size(1,4), CV_32FC1, dd2);
        this->calibData_p.calibration_results.R  = cv::Mat(cv::Size(3,3), CV_32FC1,  orb_cfg_all.orb_extr_tlr.r);
        this->calibData_p.calibration_results.T  = cv::Mat(cv::Size(1,3), CV_32FC1,  orb_cfg_all.orb_extr_tlr.t);

        fisheye::stereoRectify(this->calibData_p.calibration_results.K1, this->calibData_p.calibration_results.D1, this->calibData_p.calibration_results.K2, this->calibData_p.calibration_results.D2, image_size, this->calibData_p.calibration_results.R, this->calibData_p.calibration_results.T, this->calibData_p.calibration_results.R1, this->calibData_p.calibration_results.R2, this->calibData_p.calibration_results.P1, this->calibData_p.calibration_results.P2,
                           this->calibData_p.calibration_results.Q, 1024, image_size, 0.0, 1.1);
        
    }
    
        
}
double opencvStereoFECalib::stereoFisheyeCalib::endCalibration(int enbale_input_param)
{
    Size image_size(this->calibData_p.pixelW,this->calibData_p.pixelH);
    int flags = 0;
    flags |= cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC;
    flags |= cv::fisheye::CALIB_CHECK_COND;
    flags |= cv::fisheye::CALIB_FIX_SKEW;
    flags |= cv::fisheye::CALIB_FIX_K3;
    flags |= cv::fisheye::CALIB_FIX_K4;
    printf("this->calibData_p.object_Points.size()=%d\n",this->calibData_p.object_Points.size());
    printf("this->calibData_p.corners_Seq_l.size()=%d\n",this->calibData_p.corners_Seq_l.size());
	printf("this->calibData_p.corners_Seq_r.size()=%d\n",this->calibData_p.corners_Seq_r.size());
    vector<vector<Point2d> > left_img_points;
    vector<vector<Point2d> > right_img_points;
    // left_img_points.reserve(this->calibData_p.corners_Seq_l.size());
    // right_img_points.reserve(this->calibData_p.corners_Seq_l.size());
    for (int k = 0; k < this->calibData_p.corners_Seq_l.size(); k++)
    {
        vector<Point2d> v1, v2;
        for (int j = 0; j < this->calibData_p.corners_Seq_l[k].size(); j++)
        {
            v1.push_back(Point2d((double)this->calibData_p.corners_Seq_l[k][j].x, (double)this->calibData_p.corners_Seq_l[k][j].y));
            v2.push_back(Point2d((double)this->calibData_p.corners_Seq_r[k][j].x, (double)this->calibData_p.corners_Seq_r[k][j].y));
        }
        left_img_points.push_back(v1);
        right_img_points.push_back(v2);
    }
    double calib_rms_error = fisheye::stereoCalibrate(this->calibData_p.object_Points, left_img_points, right_img_points,this->calibData_p.calibration_results.K1,this->calibData_p.calibration_results.D1,this->calibData_p.calibration_results.K2,this->calibData_p.calibration_results.D2,image_size, this->calibData_p.calibration_results.R, this->calibData_p.calibration_results.T,  flags, cv::TermCriteria(3, 20, 1e-6));
	std::cout << "#############  RMS:" << calib_rms_error << "  #############" << std::endl;

    if (enbale_input_param)
    {
        printf("*********************************\n");
        ORB_CALIB_CFG_ALL orb_cfg_all;
        
        char path_temp[100] = "cougar2085v4_pinhole_equi.yml";
        cout << "Hello Hello Hello" << endl;
        get_orb_calib_result(&orb_cfg_all,path_temp);

        float fx1 = orb_cfg_all.orb_intr1.k[0];
        float fy1 = orb_cfg_all.orb_intr1.k[1];
        float cx1 = orb_cfg_all.orb_intr1.k[2];
        float cy1 = orb_cfg_all.orb_intr1.k[3];

        float fx2 = orb_cfg_all.orb_intr2.k[0];
        float fy2 = orb_cfg_all.orb_intr2.k[1];
        float cx2 = orb_cfg_all.orb_intr2.k[2];
        float cy2 = orb_cfg_all.orb_intr2.k[3];

        float k1_1 = orb_cfg_all.orb_intr1.k[4];
        float k2_1 = orb_cfg_all.orb_intr1.k[5];
        float p1_1 = 0;//orb_cfg_all.orb_intr1.k[6];
        float p2_1 = 0;//orb_cfg_all.orb_intr1.k[7];

        float k1_2 = orb_cfg_all.orb_intr2.k[4];
        float k2_2 = orb_cfg_all.orb_intr2.k[5];
        float p1_2 = 0;//orb_cfg_all.orb_intr2.k[6];
        float p2_2 = 0;//orb_cfg_all.orb_intr2.k[7];

        float kk1[9] = {fx1,0,cx1,
                        0,fy1,cy1,
                        0,0,1};
        float kk2[9] = {fx2,0,cx2,
                        0,fy2,cy2,
                        0,0,1};

        float dd1[4] = {k1_1,k2_1,p1_1,p2_1};
        float dd2[4] = {k1_2,k2_2,p1_2,p2_2};


        cv::Mat k1_mat = cv::Mat(cv::Size(3,3), CV_32FC1, kk1);
        cv::Mat k2_mat = cv::Mat(cv::Size(3,3), CV_32FC1, kk2);
        cv::Mat d1_mat = cv::Mat(cv::Size(1,4), CV_32FC1, dd1);
        cv::Mat d2_mat = cv::Mat(cv::Size(1,4), CV_32FC1, dd2);
        cv::Mat R_mat = cv::Mat(cv::Size(3,3), CV_32FC1,  orb_cfg_all.orb_extr_tlr.r);
        cv::Mat T_mat = cv::Mat(cv::Size(1,3), CV_32FC1,  orb_cfg_all.orb_extr_tlr.t);

        for (int i = 0;i < 9;i++)
        {
            cout << "kk1[" << i << "]" << kk1[i] << endl;
        }

        this->calibData_p.calibration_results.K1 = cv::Mat(cv::Size(3,3), CV_32FC1, kk1);
        this->calibData_p.calibration_results.D1 = cv::Mat(cv::Size(1,4), CV_32FC1, dd1);
        this->calibData_p.calibration_results.K2 = cv::Mat(cv::Size(3,3), CV_32FC1, kk2);
        this->calibData_p.calibration_results.D2 = cv::Mat(cv::Size(1,4), CV_32FC1, dd2);
        this->calibData_p.calibration_results.R  = cv::Mat(cv::Size(3,3), CV_32FC1,  orb_cfg_all.orb_extr_tlr.r);
        this->calibData_p.calibration_results.T  = cv::Mat(cv::Size(1,3), CV_32FC1,  orb_cfg_all.orb_extr_tlr.t);

        // fisheye::stereoRectify(k1_mat, d1_mat, k2_mat, d2_mat, image_size, R_mat, T_mat, this->calibData_p.calibration_results.R1, this->calibData_p.calibration_results.R2, this->calibData_p.calibration_results.P1, this->calibData_p.calibration_results.P2,
        //                    this->calibData_p.calibration_results.Q, 1024, image_size, 0.0, 1.1);

        fisheye::stereoRectify(this->calibData_p.calibration_results.K1, this->calibData_p.calibration_results.D1, this->calibData_p.calibration_results.K2, this->calibData_p.calibration_results.D2, image_size, this->calibData_p.calibration_results.R, this->calibData_p.calibration_results.T, this->calibData_p.calibration_results.R1, this->calibData_p.calibration_results.R2, this->calibData_p.calibration_results.P1, this->calibData_p.calibration_results.P2,
                           this->calibData_p.calibration_results.Q, 1024, image_size, 0.0, 1.1);
        
    }
    else
    {
        fisheye::stereoRectify(this->calibData_p.calibration_results.K1, this->calibData_p.calibration_results.D1, this->calibData_p.calibration_results.K2, this->calibData_p.calibration_results.D2, image_size, this->calibData_p.calibration_results.R, this->calibData_p.calibration_results.T, this->calibData_p.calibration_results.R1, this->calibData_p.calibration_results.R2, this->calibData_p.calibration_results.P1, this->calibData_p.calibration_results.P2,
                           this->calibData_p.calibration_results.Q, 1024, image_size, 0.0, 1.1);
    }
        
    cout<<"定标完成！\n";
    return calib_rms_error;
}
int opencvStereoFECalib::stereoFisheyeCalib::pushRawImageToCalib(uint8_t * grayRawData_l_Ptr,uint8_t * grayRawData_r_Ptr)
{
    memcpy(this->calibData_p.rawData_l,grayRawData_l_Ptr,this->calibData_p.pixelW*this->calibData_p.pixelH*sizeof(uint8_t));
    memcpy(this->calibData_p.rawData_r,grayRawData_r_Ptr,this->calibData_p.pixelW*this->calibData_p.pixelH*sizeof(uint8_t));
    cv::Mat image_l = cv::Mat(cv::Size(this->calibData_p.pixelW,this->calibData_p.pixelH), CV_8UC1, this->calibData_p.rawData_l);
    cv::Mat image_r = cv::Mat(cv::Size(this->calibData_p.pixelW,this->calibData_p.pixelH), CV_8UC1, this->calibData_p.rawData_r);
    Size image_size = image_l.size();
    // Size board_size = Size(3,3);
    this->calibData_p.pushRawGrayImgIndex++;
    uint32_t corners_standard = this->calibData_p.cbCol*this->calibData_p.cbRow;
    bool patternfound=false;
    {
        this->calibData_p.corners_l.clear();
        this->calibData_p.corners_r.clear();
        uint32_t imgW = this->calibData_p.pixelW,imgH = this->calibData_p.pixelH;
        printf("imgW=%d,imgH=%d\n",imgW,imgH);
        cout<<"findChessboardCorners start" << endl;
        bool patternfound_l = findChessboardCorners(image_l,  this->calibData_p.board_size, this->calibData_p.corners_l,CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE+CALIB_CB_FAST_CHECK);
        bool patternfound_r = findChessboardCorners(image_r,  this->calibData_p.board_size, this->calibData_p.corners_r,CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE+CALIB_CB_FAST_CHECK);
        patternfound = patternfound_l & patternfound_r;
        cout<<"findChessboardCorners ok" << endl;
        if(patternfound)
        {
            cornerSubPix(image_l, this->calibData_p.corners_l, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            cornerSubPix(image_r, this->calibData_p.corners_r, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            if((this->calibData_p.corners_l.size()==corners_standard)&&(this->calibData_p.corners_r.size()==corners_standard))
            {
            printf("this->calibData_p.patternfoundImgIndex=%d\n",this->calibData_p.patternfoundImgIndex);
            this->calibData_p.patternfoundImgIndex++;
            this->calibData_p.corners_Seq_l.push_back(this->calibData_p.corners_l);
            this->calibData_p.corners_Seq_r.push_back(this->calibData_p.corners_r);
            this->calibData_p.count = this->calibData_p.count + this->calibData_p.corners_l.size() + this->calibData_p.corners_r.size();
            printf("this->calibData_p.corners_l.size()=%d\n",this->calibData_p.corners_l.size());
            printf("this->calibData_p.corners_r.size()=%d\n",this->calibData_p.corners_r.size());
            printf("diff=%d\n",abs(this->calibData_p.corners_l.size()-this->calibData_p.corners_r.size()));
            vector<Point3d> obj;
            for (int k = 0; k < this->calibData_p.board_size.height; ++k)
            {   
                for (int j = 0; j < this->calibData_p.board_size.width; ++j)
                { 
                    obj.push_back(Point3d(double((double)j * this->calibData_p.squareW_cm), double((double)k * this->calibData_p.squareW_cm), 0));
                }
            }
            this->calibData_p.object_Points.push_back(obj);
            }
            else
            {
                patternfound=false;
            }
        }
    }
    return ((patternfound)? 1 : 0);
}
opencvStereoFECalib::stereoFisheyeCalib::~stereoFisheyeCalib()
{
    free(this->calibData_p.rawData_l);
    free(this->calibData_p.rawData_r);
}
void opencvStereoFECalib::stereoFisheyeCalib::printCalibrationResults(double error)
{
    char path[100];
    sprintf(path,"./cam%d_steroCalibration.yml",this->calibData_p.cameraID);
    cv::FileStorage fs(path, true);
    {
        if (fs.isOpened())
        {

            fs << "ERROR " << error;
            fs << "K1" << this->calibData_p.calibration_results.K1 << "D1" << this->calibData_p.calibration_results.D1 << "K2" << this->calibData_p.calibration_results.K2 << "D2" << this->calibData_p.calibration_results.D2;
            fs << "P1" << this->calibData_p.calibration_results.P1 << "P2" << this->calibData_p.calibration_results.P2;
            fs << "R" << this->calibData_p.calibration_results.R << "T" << this->calibData_p.calibration_results.T << "R1" << this->calibData_p.calibration_results.R1 << "R2" << this->calibData_p.calibration_results.R2 << "Q" << this->calibData_p.calibration_results.Q;
            fs.release();
        }
        else
        {   
            cout << "Error: can not save the intrinsic parameters\n";
        }



    }
}