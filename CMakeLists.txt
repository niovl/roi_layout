CMAKE_MINIMUM_REQUIRED(VERSION 0.0)
PROJECT(roi_layout)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -pthread")


SET(CMAKE_CXX_COMPILER "g++")
# CPU_BASELINE=SSE2

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

set(OpenCV_LIBS opencv_core opencv_highgui opencv_imgcodecs opencv_imgproc opencv_videoio opencv_features2d opencv_videostab)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src)


