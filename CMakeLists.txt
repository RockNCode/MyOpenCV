#SET(OPENCV_C_SAMPLES_REQUIRED_DEPS opencv_core opencv_flann opencv_imgproc )
#    opencv_highgui opencv_ml opencv_video opencv_objdetect opencv_photo opencv_nonfree
#    opencv_features2d opencv_calib3d opencv_legacy opencv_contrib)

#ocv_check_dependencies(${OPENCV_C_SAMPLES_REQUIRED_DEPS})

#project(DisplayImage)

#add_executable(DisplayImage DisplayImage.cpp)

#install (TARGETS DisplayImage DESTINATION bin)

cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
#project( LoadSave )
find_package( OpenCV REQUIRED )
add_executable( DisplayImage DisplayImage.cpp )
add_executable( LoadSave LoadSave.cpp )
add_executable( videoInput video-input-psnr-ssim.cpp )
add_executable( videoBlackWhite video-input-black_n_white.cpp )
add_executable( countobjects countobjects.cpp )

target_link_libraries( DisplayImage ${OpenCV_LIBS} )
target_link_libraries( LoadSave ${OpenCV_LIBS} )
target_link_libraries( videoInput ${OpenCV_LIBS} )
target_link_libraries( countobjects ${OpenCV_LIBS} )
target_link_libraries( videoBlackWhite ${OpenCV_LIBS} )