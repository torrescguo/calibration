#!/bin/bash
model=$1
cd ./output
./uvc_calibDemo ../data/imu_visio/${model}/imu-camera-camchain-imucam.yaml ../data/imu_visio/${model}/imu-camera-imu.yaml ../data/imu_visio/${model}/${model}_temp.yaml ../data/imu_visio_out/${model}/Cougar2085V4_${model}.yaml

rm -f ../data/imu_visio/${model}/${model}_temp.yaml
