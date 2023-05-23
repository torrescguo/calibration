
#find_package(libusb-1.0 REQUIRED)
set(LIBUSB_1_LIBRARIES  /usr/lib/x86_64-linux-gnu/libusb-1.0.so)
add_library(hid_control hid_control.cpp hid.c)
target_link_libraries(hid_control ${LIBUSB_1_LIBRARIES})


