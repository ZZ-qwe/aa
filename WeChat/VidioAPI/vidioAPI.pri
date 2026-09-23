HEADERS += \
    $$PWD/common.h \
    $$PWD/myfacedetect.h \
    $$PWD/screenread.h \
    $$PWD/threadworker.h \
    $$PWD/vidioread.h

SOURCES += \
    $$PWD/myfacedetect.cpp \
    $$PWD/screenread.cpp \
    $$PWD/threadworker.cpp \
    $$PWD/vidioread.cpp

INCLUDEPATH+=D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\include\opencv2\
            D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\include

LIBS+=D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\lib\libopencv_calib3d420.dll.a\
      D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\lib\libopencv_core420.dll.a\
      D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\lib\libopencv_features2d420.dll.a\
      D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\lib\libopencv_flann420.dll.a\
      D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\lib\libopencv_highgui420.dll.a\
      D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\lib\libopencv_imgproc420.dll.a\
      D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\lib\libopencv_ml420.dll.a\
      D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\lib\libopencv_objdetect420.dll.a\
      D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\lib\libopencv_video420.dll.a\
      D:\Ccolin_later\video_meeting\project_res\opencv\opencv\opencv-release\opencv-release\lib\libopencv_videoio420.dll.a

