#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/hubery/hubery_ff/calibration/kalibr_ws/src/kalibr_cp/aslam_cv/aslam_cv_python"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/hubery/hubery_ff/calibration/kalibr_ws/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/hubery/hubery_ff/calibration/kalibr_ws/install/lib/python2.7/dist-packages:/home/hubery/hubery_ff/calibration/kalibr_ws/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/hubery/hubery_ff/calibration/kalibr_ws/build" \
    "/usr/bin/python2" \
    "/home/hubery/hubery_ff/calibration/kalibr_ws/src/kalibr_cp/aslam_cv/aslam_cv_python/setup.py" \
     \
    build --build-base "/home/hubery/hubery_ff/calibration/kalibr_ws/build/kalibr_cp/aslam_cv/aslam_cv_python" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/hubery/hubery_ff/calibration/kalibr_ws/install" --install-scripts="/home/hubery/hubery_ff/calibration/kalibr_ws/install/bin"
