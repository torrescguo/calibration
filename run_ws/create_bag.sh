
############################## dataset############################

############################ topic Notes #########################

# cam0 cam1 imu0.csv

######################## only visio calibration ##################

# cam0 and cam1 ---monocular only one topic,binocular need two topics

###################### camera and imu calibaration ###############

# imu noise calibration,only imu0 topic is enough
# imu and camera calibration ,cam0 cam1 and imu0 are necessary

rosrun kalibr kalibr_bagcreater --folder ./dataset/. --output-bag ./bag/imu-camera.bag
