
#camera  and distortion model
# cameraModels = { 'pinhole-radtan': acvb.DistortedPinhole,
#                  'pinhole-equi':   acvb.EquidistantPinhole,
#                  'pinhole-fov':    acvb.FovPinhole,
#                  'omni-none':      acvb.Omni,
#                  'omni-radtan':    acvb.DistortedOmni,
#                  'eucm-none':      acvb.ExtendedUnified,
#                  'ds-none':        acvb.DoubleSphere}

# --bag-from-to :bag start and end time
# --target :april_6x6_ds80.yaml  calibration board type and specification
# --topics : use "rosbag info test.bag"  to view bag's topics
# --show-extraction： to display calibration process
# --approx-sync 0.04


mode=$1
rosrun kalibr kalibr_calibrate_imu_camera --target april_6x6_ds80_A3.yaml --cam ./result/visio/${mode}/imu-camera-camchain.yaml --imu ./result/noise_calib/imu_noise.yaml --bag ./bag/imu-camera.bag --bag-from-to 3 48 --show-extraction --max-iter 30

mv ./bag/*.txt ./bag/*.pdf ./bag/*.yaml ./result/imu_visio/${mode}/

