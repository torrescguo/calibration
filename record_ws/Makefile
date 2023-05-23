cc:=gcc
cpp:=g++
output_folder:=Output
sources:=Source/driver/uvc_cam_sdk.c
sources+=Source/driver/bmp.c
sources+=Source/hid_control/hid_control.cpp
sources+=Source/hid_control/hid.c
sources+=Source/demo/common.cpp
includes:=Source/include
includes+=Source/hid_control/*.h

cflags_common:= -DMULTI_API_VERSION  -lusb-1.0 -ludev -lpthread  -fpermissive -w -O3

################################################################################################
demo_dispUvc_src:=Source/demo/dispUvc.cpp
demo_dispUvc=$(output_folder)/demo_dispUvc
clfags_demo_dispUvc:=$(cflags_common) -lGL -lGLU -lglut -lm `pkg-config --cflags --libs opencv` 
build_dispUvc:
		$(cpp) $(sources) $(demo_dispUvc_src) -I$(includes) -o $(demo_dispUvc) $(clfags_demo_dispUvc)
clean_dispUvc:
		rm -rf ./$(demo_dispUvc)
run_dispUvc:
		./$(demo_dispUvc)
################################################################################################
clean:
		make clean_dispUvc
	
build:
		mkdir -p $(output_folder)
		make build_dispUvc
