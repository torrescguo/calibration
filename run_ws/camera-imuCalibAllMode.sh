

echo "\n***************************Start imu-camera Calibration****************************\n"
echo "Calibrating all supported modes:"

echo "------------------------"
echo "--   pinhole-radtan   --"
echo "--   pinhole-equi     --"
echo "--    pinhole-fov     --"
echo "--     omni-none      --"
echo "--    omni-radtan     --"
echo "--     eucm-none      --"
echo "--      ds-none       --"
echo "------------------------"

echo "\n***************************Start calibration pinhole-radtan********************\n"
sh camera_imu_calibrate.sh pinhole-radtan
echo "\n***************************Finish calibration pinhole-radtan*******************\n"

echo "\n***************************Start calibration pinhole-equi**********************\n"
sh camera_imu_calibrate.sh pinhole-equi
echo "\n***************************Finish calibration pinhole-equi*********************\n"

echo "\n***************************Start calibration pinhole-fov***********************\n"
sh camera_imu_calibrate.sh pinhole-fov
echo "\n***************************Finish calibration pinhole-fov**********************\n"

echo "\n***************************Start calibration omni-none*************************\n"
sh camera_imu_calibrate.sh omni-none
echo "\n***************************Finish calibration omni-none************************\n"

echo "\n***************************Start calibration omni-radtan***********************\n"
sh camera_imu_calibrate.sh omni-radtan
echo "\n***************************Finish calibration omni-radtan**********************\n"

echo "\n***************************Start calibration eucm-none*************************\n"
sh camera_imu_calibrate.sh eucm-none
echo "\n***************************Finish calibration eucm-none************************\n"

echo "\n***************************Start calibration ds-none***************************\n"
sh camera_imu_calibrate.sh ds-none
echo "\n***************************Finish calibration ds-none**************************\n"


echo "\n***************************Finish all imu-camera Calibration************************\n"


