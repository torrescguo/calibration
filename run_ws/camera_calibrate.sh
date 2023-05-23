

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
#pinhole-radtan
rosrun kalibr kalibr_calibrate_cameras --target april_6x6_ds80_A3.yaml --bag ./bag/imu-camera.bag --models ${mode} ${mode} --topics /cam0/image_raw /cam1/image_raw --show-extraction --bag-from-to 3 53 --approx-sync 0.005

mv ./bag/*results*.txt ./bag/*report*.pdf ./bag/*camchain*.yaml ./result/visio/${mode}/

