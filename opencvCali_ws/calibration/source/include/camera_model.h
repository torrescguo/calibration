#pragma once 

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

namespace CAMERA_MODEL_API
{
    void ucm_radtan_distortion(float *param,float *r,int idx);
    void ucm_none_distortion(float *param,float *r,int idx);
    void eucm_none_distortion(float *param,float *r,int idx);

    void pinhole_fov_distortion(float *param,float *r,int idx);
    void pinhole_equi_distortion(float *param,float *r,int idx);
    void pinhole_radtan_distortion(float *param,float *r,int idx);
    void ds_none_distortion(float *param,float *r,int idx);
}




