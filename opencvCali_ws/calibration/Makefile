cc:=g++

src:=source/src/*.cpp
src+=demo/uvcCalib.cpp

ccflag:=-DMULTI_API_VERSION -DPLATFORM_AMD64 -w -O3 -lGL -lGLU -lglut -lm `pkg-config --cflags --libs opencv` -lusb-1.0  -ludev -lpthread  -fpermissive
exe_uvc_demo:=output/uvc_calibDemo
build:
		mkdir -p output
		$(cc) $(src) -Isource/include $(ccflag) -o $(exe_uvc_demo)
clean:
		rm -rf output
run_uvc:
		./$(exe_uvc_demo)