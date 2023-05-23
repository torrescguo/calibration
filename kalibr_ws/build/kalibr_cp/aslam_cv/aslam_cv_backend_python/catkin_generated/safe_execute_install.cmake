execute_process(COMMAND "/home/hubery/hubery_ff/calibration/kalibr_ws/build/kalibr_cp/aslam_cv/aslam_cv_backend_python/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/hubery/hubery_ff/calibration/kalibr_ws/build/kalibr_cp/aslam_cv/aslam_cv_backend_python/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
