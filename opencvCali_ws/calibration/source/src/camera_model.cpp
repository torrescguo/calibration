#include "../include/camera_model.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/types_c.h>


cv::Mat rmap[2][2];

void CAMERA_MODEL_API::ucm_radtan_distortion(float *param,float *r,int idx)
{
    //fx fy cx cy，w
    float fx       = param[0];
    float fy       = param[1];
    float cx       = param[2];
    float cy       = param[3];
    float xi       = param[4];
    float k1       = param[5];
    float k2       = param[6];
    float p1       = param[7];
    float p2       = param[8];

    int width = 640;
    int height = 400;


    float *map_x = (float*)malloc(sizeof(float) * width * height);
    float *map_y = (float*)malloc(sizeof(float) * width * height);

    for (int u = 0; u < height; u++)
    {
        for (int v = 0; v < width; v++)
        {
            float x, y;
            float z = 1;

            float fx1 = fx/(1+xi);
            float fy1 = fy/(1+xi);

            x = ((float)v - cx) / fx1;
            y = ((float)u - cy) / fy1;

            #if 1
            // Binocular polar correction

            // Binocular polar correction
            cv::Mat r_mat = cv::Mat(cv::Size(3,3), CV_32FC1, r);

            float p[3] = {x,y,z}; // x,y,z camera coordinates 

            cv::Mat pose = cv::Mat(cv::Size(1,3), CV_32FC1, p);

            cv::Mat r_inv = r_mat.inv();
            pose = r_inv * pose;

            float x_temp = pose.ptr<float>(0)[0];
            float y_temp = pose.ptr<float>(1)[0];
            float z_temp = pose.ptr<float>(2)[0];

            
            float norm = sqrt(x_temp * x_temp + y_temp * y_temp + z_temp * z_temp);

            float xx_temp = x_temp/norm;
            float yy_temp = y_temp/norm;
            float zz_temp = z_temp/norm + xi;


            float xx_distorted = xx_temp/zz_temp;
            float yy_distorted = yy_temp/zz_temp;

            float r = sqrt(xx_distorted * xx_distorted + yy_distorted * yy_distorted);

            float x_distorted = xx_distorted*(1+k1*r*r+k2*r*r*r*r) + 2*p1*xx_distorted*yy_distorted + p2*(r*r+2*xx_distorted*xx_distorted);
            float y_distorted = yy_distorted*(1+k1*r*r+k2*r*r*r*r) + p1*(r*r+2*yy_distorted*yy_distorted)+2*p2*xx_distorted*yy_distorted;
        
            #else // not Binocular polar correction

            float d = sqrt(x*x+y*y+z*z);
            
            float z_d = z/d + xi;

            float xx_distorted = x/(d*z_d);
            float yy_distorted = y/(d*z_d);

            float r = sqrt(xx_distorted*xx_distorted+yy_distorted*yy_distorted);

            
            float x_distorted = xx_distorted*(1+k1*r*r+k2*r*r*r*r) + 2*p1*xx_distorted*yy_distorted + p2*(r*r+2*xx_distorted*xx_distorted);
            float y_distorted = yy_distorted*(1+k1*r*r+k2*r*r*r*r) + p1*(r*r+2*yy_distorted*yy_distorted)+2*p2*xx_distorted*yy_distorted;
        
            #endif

            float u_distorted = fx*x_distorted + cx;
            float v_distorted = fy*y_distorted + cy;

            map_x[v + u * width] = u_distorted;
            map_y[v + u * width] = v_distorted;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rmap[idx][0].ptr<float>(i,j)[0] = *(map_x + i * width + j);//(float)j;
            rmap[idx][1].ptr<float>(i,j)[0] = *(map_y + i * width + j);//(float)(400-i);
            
        }
    }

    free(map_x);
    free(map_y);
}


void CAMERA_MODEL_API::ucm_none_distortion(float *param,float *r,int idx)
{

    //fx fy cx cy，w
    float fx       = param[0];
    float fy       = param[1];
    float cx       = param[2];
    float cy       = param[3];
    float xi       = param[4];

    int width = 640;
    int height = 400;


    float *map_x = (float*)malloc(sizeof(float) * width * height);
    float *map_y = (float*)malloc(sizeof(float) * width * height);

    for (int u = 0; u < height; u++)
    {
        for (int v = 0; v < width; v++)
        {
            float x, y;
            float z = 1;

            float fx1 = fx/(1+xi);
            float fy1 = fy/(1+xi);

            x = ((float)v - cx) / fx1;
            y = ((float)u - cy) / fy1;

            #if 1
            // Binocular polar correction
            cv::Mat r_mat = cv::Mat(cv::Size(3,3), CV_32FC1, r);

            float p[3] = {x,y,z}; // x,y,z camera coordinates 

            cv::Mat pose = cv::Mat(cv::Size(1,3), CV_32FC1, p);

            cv::Mat r_inv = r_mat.inv();
            pose = r_inv * pose;

            float x_temp = pose.ptr<float>(0)[0];
            float y_temp = pose.ptr<float>(1)[0];
            float z_temp = pose.ptr<float>(2)[0];

            
            float norm = sqrt(x_temp * x_temp + y_temp * y_temp + z_temp * z_temp);

            float xx_temp = x_temp/norm;
            float yy_temp = y_temp/norm;
            float zz_temp = z_temp/norm + xi;

            float x_distorted = xx_temp/zz_temp;
            float y_distorted = yy_temp/zz_temp;
        
            #else // not Binocular polar correction

            float d = sqrt(x*x+y*y+z*z);
            
            float z_d = z/d + xi;

            float x_distorted = x/(d*z_d);
            float y_distorted = y/(d*z_d);
            #endif

            float u_distorted = fx*x_distorted + cx;
            float v_distorted = fy*y_distorted + cy;

            map_x[v + u * width] = u_distorted;
            map_y[v + u * width] = v_distorted;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rmap[idx][0].ptr<float>(i,j)[0] = *(map_x + i * width + j);//(float)j;
            rmap[idx][1].ptr<float>(i,j)[0] = *(map_y + i * width + j);//(float)(400-i);
            
        }
    }

    free(map_x);
    free(map_y);


}


void CAMERA_MODEL_API::eucm_none_distortion(float *param,float *r,int idx)
{

    //fx fy cx cy，w
    float fx       = param[0];
    float fy       = param[1];
    float cx       = param[2];
    float cy       = param[3];
    float alpha    = param[4];
    float beta     = param[5];

    int width = 640;
    int height = 400;

    float *map_x = (float*)malloc(sizeof(float) * width * height);
    float *map_y = (float*)malloc(sizeof(float) * width * height);

    for (int u = 0; u < height; u++)
    {
        for (int v = 0; v < width; v++)
        {
            float x, y;
            float z = 1;

            x = ((float)v - cx) / fx;
            y = ((float)u - cy) / fy;
            #if 1
            // Binocular polar correction
            cv::Mat r_mat = cv::Mat(cv::Size(3,3), CV_32FC1, r);

            float p[3] = {x,y,z}; // x,y,z camera coordinates 

            cv::Mat pose = cv::Mat(cv::Size(1,3), CV_32FC1, p);

            cv::Mat r_inv = r_mat.inv();
            pose = r_inv * pose;

            float x_temp = pose.ptr<float>(0)[0];
            float y_temp = pose.ptr<float>(1)[0];
            float z_temp = pose.ptr<float>(2)[0];

            
            float norm = sqrt(beta*(x_temp * x_temp + y_temp * y_temp) + z_temp * z_temp);

            float x_distorted = x_temp/(alpha*norm + (1-alpha)*z_temp);
            float y_distorted = y_temp/(alpha*norm + (1-alpha)*z_temp);

            #else

            float d = sqrt(beta*(x*x+y*y)+z*z);

            float x_distorted = x/(alpha*d+(1-alpha)*z);
            float y_distorted = y/(alpha*d+(1-alpha)*z);

            #endif

            float u_distorted = fx*x_distorted + cx;
            float v_distorted = fy*y_distorted + cy;

            map_x[v + u * width] = u_distorted;
            map_y[v + u * width] = v_distorted;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rmap[idx][0].ptr<float>(i,j)[0] = *(map_x + i * width + j);//(float)j;
            rmap[idx][1].ptr<float>(i,j)[0] = *(map_y + i * width + j);//(float)(400-i);
            
        }
    }

    free(map_x);
    free(map_y);


}



void CAMERA_MODEL_API::pinhole_fov_distortion(float *param,float *r,int idx)
{

    //fx fy cx cy，w
    float fx = param[0];
    float fy = param[1];
    float cx = param[2];
    float cy = param[3];
    float w  = param[4];

    int width = 640;
    int height = 400;

    float *map_x = (float*)malloc(sizeof(float) * width * height);
    float *map_y = (float*)malloc(sizeof(float) * width * height);

    for (int u = 0; u < height; u++)
    {
        for (int v = 0; v < width; v++)
        {
            float x, y;
            // calc_xy(u, v, &x, &y);
            float z = 1;
            x = ((float)v - cx) / fx;
            y = ((float)u - cy) / fy;

            #if 1
            // Binocular polar correction
            cv::Mat r_mat = cv::Mat(cv::Size(3,3), CV_32FC1, r);

            float p[3] = {x,y,z}; // x,y,z camera coordinates 

            cv::Mat pose = cv::Mat(cv::Size(1,3), CV_32FC1, p);

            cv::Mat r_inv = r_mat.inv();
            pose = r_inv * pose;

            float x_temp = pose.ptr<float>(0)[0];
            float y_temp = pose.ptr<float>(1)[0];
            float z_temp = pose.ptr<float>(2)[0];

            
            float norm = sqrt(x_temp * x_temp + y_temp * y_temp + z_temp * z_temp);

            float xx_temp = x_temp/norm;
            float yy_temp = y_temp/norm;
            float zz_temp = z_temp/norm;

            float xx_distorted = xx_temp/zz_temp;
            float yy_distorted = yy_temp/zz_temp;

            float r = sqrt(xx_distorted * xx_distorted + yy_distorted * yy_distorted);

            float r_d = (1/w) * atan(2*r*tan(w/2));

            float x_distorted = xx_distorted*r_d/r;
            float y_distorted = yy_distorted*r_d/r;
            #else
            // lens distortion correction
            float r = sqrt(x*x+y*y);

            float r_d = (1/w) * atan(2*r*tan(w/2));

            float x_distorted = x*r_d/r;
            float y_distorted = y*r_d/r;

            #endif

            float u_distorted = fx*x_distorted + cx;
            float v_distorted = fy*y_distorted + cy;

            map_x[v + u * width] = u_distorted;
            map_y[v + u * width] = v_distorted;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rmap[idx][0].ptr<float>(i,j)[0] = *(map_x + i * width + j);//(float)j;
            rmap[idx][1].ptr<float>(i,j)[0] = *(map_y + i * width + j);//(float)(400-i);
            
        }
    }

    free(map_x);
    free(map_y);


}


void CAMERA_MODEL_API::pinhole_equi_distortion(float *param,float *r,int idx)
{

    //fx fy cx cy，w
    float fx = param[0];
    float fy = param[1];
    float cx = param[2];
    float cy = param[3];
    float k1  = param[4];
    float k2  = param[5];
    float k3  = param[6];
    float k4  = param[7];

    int width = 640;
    int height = 400;

    float *map_x = (float*)malloc(sizeof(float) * width * height);
    float *map_y = (float*)malloc(sizeof(float) * width * height);

    for (int u = 0; u < height; u++)
    {
        for (int v = 0; v < width; v++)
        {
            float x, y;
            float z = 1;
            x = ((float)v - cx) / fx;
            y = ((float)u - cy) / fy;
        #if 1
           // Binocular polar correction
            cv::Mat r_mat = cv::Mat(cv::Size(3,3), CV_32FC1, r);

            float p[3] = {x,y,z}; // x,y,z camera coordinates 

            cv::Mat pose = cv::Mat(cv::Size(1,3), CV_32FC1, p);

            cv::Mat r_inv = r_mat.inv();
            pose = r_inv * pose;

            float x_temp = pose.ptr<float>(0)[0];
            float y_temp = pose.ptr<float>(1)[0];
            float z_temp = pose.ptr<float>(2)[0];

            
            float norm = sqrt(x_temp * x_temp + y_temp * y_temp + z_temp * z_temp);

            float xx_temp = x_temp/norm;
            float yy_temp = y_temp/norm;
            float zz_temp = z_temp/norm;

            float xx_distorted = xx_temp/zz_temp;
            float yy_distorted = yy_temp/zz_temp;

            float r = sqrt(xx_distorted * xx_distorted + yy_distorted * yy_distorted);

            // distortion process

            float theta = atan(r);
            float theta2 = theta * theta;
            float theta4 = theta2 * theta2;
            float theta6 = theta4 * theta2;
            float theta8 = theta4 * theta4;
            float thetad = theta * (1 + k1 * theta2 + k2 * theta4 + k3 * theta6 + k4 * theta8);

            float scaling = (r > 1e-8) ? thetad / r : 1.0;

            float x_distorted = x*scaling;
            float y_distorted = y*scaling;
        #else

            float r = sqrt(x*x+y*y);

            float theta = atan(r);
            float theta2 = theta * theta;
            float theta4 = theta2 * theta2;
            float theta6 = theta4 * theta2;
            float theta8 = theta4 * theta4;
            float thetad = theta * (1 + k1 * theta2 + k2 * theta4 + k3 * theta6 + k4 * theta8);

            float scaling = (r > 1e-8) ? thetad / r : 1.0;

            float x_distorted = x*scaling;
            float y_distorted = y*scaling;
        #endif
            float u_distorted = fx*x_distorted + cx;
            float v_distorted = fy*y_distorted + cy;
        

            map_x[v + u * width] = u_distorted;
            map_y[v + u * width] = v_distorted;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rmap[idx][0].ptr<float>(i,j)[0] = *(map_x + i * width + j);//(float)j;
            rmap[idx][1].ptr<float>(i,j)[0] = *(map_y + i * width + j);//(float)(400-i);
            
        }
    }

    free(map_x);
    free(map_y);


}


void CAMERA_MODEL_API::pinhole_radtan_distortion(float *param,float *r,int idx)
{

    //fx fy cx cy，w
    float fx = param[0];
    float fy = param[1];
    float cx = param[2];
    float cy = param[3];
    float k1  = param[4];
    float k2  = param[5];
    float p1  = param[6];
    float p2  = param[7];

    int width = 640;
    int height = 400;

    float *map_x = (float*)malloc(sizeof(float) * width * height);
    float *map_y = (float*)malloc(sizeof(float) * width * height);

    for (int u = 0; u < height; u++)
    {
        for (int v = 0; v < width; v++)
        {
            float x, y;
            float z =1;
           
            x = ((float)v - cx) / fx;
            y = ((float)u - cy) / fy;
            #if 1
           // Binocular polar correction
            cv::Mat r_mat = cv::Mat(cv::Size(3,3), CV_32FC1, r);

            float p[3] = {x,y,z}; // x,y,z camera coordinates 

            cv::Mat pose = cv::Mat(cv::Size(1,3), CV_32FC1, p);

            cv::Mat r_inv = r_mat.inv();
            pose = r_inv * pose;

            float x_temp = pose.ptr<float>(0)[0];
            float y_temp = pose.ptr<float>(1)[0];
            float z_temp = pose.ptr<float>(2)[0];

            
            float norm = sqrt(x_temp * x_temp + y_temp * y_temp + z_temp * z_temp);

            float xx_temp = x_temp/norm;
            float yy_temp = y_temp/norm;
            float zz_temp = z_temp/norm;

            float xx_distorted = xx_temp/zz_temp;
            float yy_distorted = yy_temp/zz_temp;

            float r = sqrt(xx_distorted * xx_distorted + yy_distorted * yy_distorted);

            float x_distorted = xx_distorted*(1+k1*r*r+k2*r*r*r*r) + 2*p1*xx_distorted*yy_distorted + p2*(r*r+2*xx_distorted*xx_distorted);
            float y_distorted = yy_distorted*(1+k1*r*r+k2*r*r*r*r) + p1*(r*r+2*yy_distorted*yy_distorted)+2*p2*xx_distorted*yy_distorted;
            #else

            float r = sqrt(x*x+y*y);

            float x_distorted = x*(1+k1*r*r+k2*r*r*r*r) + 2*p1*x*y + p2*(r*r+2*x*x);
            float y_distorted = y*(1+k1*r*r+k2*r*r*r*r) + p1*(r*r+2*y*y)+2*p2*x*y;

            #endif
            float u_distorted = fx*x_distorted + cx;
            float v_distorted = fy*y_distorted + cy;
        

            map_x[v + u * width] = u_distorted;
            map_y[v + u * width] = v_distorted;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rmap[idx][0].ptr<float>(i,j)[0] = *(map_x + i * width + j);//(float)j;
            rmap[idx][1].ptr<float>(i,j)[0] = *(map_y + i * width + j);//(float)(400-i);
            
        }
    }

    free(map_x);
    free(map_y);


}

void CAMERA_MODEL_API::ds_none_distortion(float *param,float *r,int idx)
{

    //fx fy cx cy，w
    float fx = param[0];
    float fy = param[1];
    float cx = param[2];
    float cy = param[3];
    float xi  = param[4];
    float alpha  = param[5];

    int width = 640;
    int height = 400;

    float *map_x = (float*)malloc(sizeof(float) * width * height);
    float *map_y = (float*)malloc(sizeof(float) * width * height);

    for (int u = 0; u < height; u++)
    {
        for (int v = 0; v < width; v++)
        {
            float x, y;
           
            float z = 1;

            float fx1 = fx/(1+xi);
            float fy1 = fy/(1+xi);

            x = ((float)v - cx) / fx1;
            y = ((float)u - cy) / fy1;

            #if 1
            // Binocular polar correction
            cv::Mat r_mat = cv::Mat(cv::Size(3,3), CV_32FC1, r);

            float p[3] = {x,y,z}; // x,y,z camera coordinates 

            cv::Mat pose = cv::Mat(cv::Size(1,3), CV_32FC1, p);

            cv::Mat r_inv = r_mat.inv();
            pose = r_inv * pose;

            float x_temp = pose.ptr<float>(0)[0];
            float y_temp = pose.ptr<float>(1)[0];
            float z_temp = pose.ptr<float>(2)[0];

            
            float norm = sqrt(x_temp * x_temp + y_temp * y_temp + z_temp * z_temp);

            float xx_temp = x_temp/norm;
            float yy_temp = y_temp/norm;
            float zz_temp = z_temp/norm;

            float xx_distorted = xx_temp/zz_temp;
            float yy_distorted = yy_temp/zz_temp;
            
            float zz_distorted = zz_temp/zz_temp;

            float d1 = sqrt(xx_distorted*xx_distorted+yy_distorted*yy_distorted+zz_distorted*zz_distorted);
            float d2 = sqrt(xx_distorted*xx_distorted+yy_distorted*yy_distorted+(xi*d1+zz_distorted)*(xi*d1+zz_distorted));

            float x_distorted = xx_distorted/(alpha*d2+(1-alpha)*(xi*d1+zz_distorted));
            float y_distorted = yy_distorted/(alpha*d2+(1-alpha)*(xi*d1+zz_distorted));
        
            #else // not Binocular polar correction

            float d1 = sqrt(x*x+y*y+z*z);
            float d2 = sqrt(x*x+y*y+(xi*d1+z)*(xi*d1+z));

            float x_distorted = x/(alpha*d2+(1-alpha)*(xi*d1+z));
            float y_distorted = y/(alpha*d2+(1-alpha)*(xi*d1+z));

            #endif
            float u_distorted = fx*x_distorted + cx;
            float v_distorted = fy*y_distorted + cy;
        

            map_x[v + u * width] = u_distorted;
            map_y[v + u * width] = v_distorted;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            rmap[idx][0].ptr<float>(i,j)[0] = *(map_x + i * width + j);//(float)j;
            rmap[idx][1].ptr<float>(i,j)[0] = *(map_y + i * width + j);//(float)(400-i);
            
        }
    }

    free(map_x);
    free(map_y);


}