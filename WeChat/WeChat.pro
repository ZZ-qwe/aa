QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(./netapi/netapi.pri)
INCLUDEPATH += ./netapi

include(./md5/md5.pri)
INCLUDEPATH += ./md5

include(./AudioAPI/audioAPI.pri)
INCLUDEPATH += ./AudioAPI

include(./VidioAPI/vidioAPI.pri)
INCLUDEPATH += ./VidioAPI


SOURCES += \
    AudioAPI/WebRtcMoudle/analog_agc.c \
    AudioAPI/WebRtcMoudle/complex_bit_reverse.c \
    AudioAPI/WebRtcMoudle/complex_fft.c \
    AudioAPI/WebRtcMoudle/copy_set_operations.c \
    AudioAPI/WebRtcMoudle/cross_correlation.c \
    AudioAPI/WebRtcMoudle/digital_agc.c \
    AudioAPI/WebRtcMoudle/division_operations.c \
    AudioAPI/WebRtcMoudle/dot_product_with_scale.c \
    AudioAPI/WebRtcMoudle/downsample_fast.c \
    AudioAPI/WebRtcMoudle/energy.c \
    AudioAPI/WebRtcMoudle/fft4g.c \
    AudioAPI/WebRtcMoudle/get_scaling_square.c \
    AudioAPI/WebRtcMoudle/min_max_operations.c \
    AudioAPI/WebRtcMoudle/noise_suppression.c \
    AudioAPI/WebRtcMoudle/noise_suppression_x.c \
    AudioAPI/WebRtcMoudle/ns_core.c \
    AudioAPI/WebRtcMoudle/nsx_core.c \
    AudioAPI/WebRtcMoudle/nsx_core_c.c \
    AudioAPI/WebRtcMoudle/nsx_core_neon_offsets.c \
    AudioAPI/WebRtcMoudle/real_fft.c \
    AudioAPI/WebRtcMoudle/resample.c \
    AudioAPI/WebRtcMoudle/resample_48khz.c \
    AudioAPI/WebRtcMoudle/resample_by_2.c \
    AudioAPI/WebRtcMoudle/resample_by_2_internal.c \
    AudioAPI/WebRtcMoudle/resample_by_2_mips.c \
    AudioAPI/WebRtcMoudle/resample_fractional.c \
    AudioAPI/WebRtcMoudle/ring_buffer.c \
    AudioAPI/WebRtcMoudle/spl_init.c \
    AudioAPI/WebRtcMoudle/spl_sqrt.c \
    AudioAPI/WebRtcMoudle/spl_sqrt_floor.c \
    AudioAPI/WebRtcMoudle/splitting_filter.c \
    AudioAPI/WebRtcMoudle/vector_scaling_operations.c \
    ckernel.cpp \
    logindialog.cpp \
    main.cpp \
    roomdialog.cpp \
    usershow.cpp \
    wechatdialog.cpp

HEADERS += \
    AudioAPI/WebRtcMoudle/analog_agc.h \
    AudioAPI/WebRtcMoudle/complex_fft_tables.h \
    AudioAPI/WebRtcMoudle/cpu_features_wrapper.h \
    AudioAPI/WebRtcMoudle/defines.h \
    AudioAPI/WebRtcMoudle/digital_agc.h \
    AudioAPI/WebRtcMoudle/fft4g.h \
    AudioAPI/WebRtcMoudle/gain_control.h \
    AudioAPI/WebRtcMoudle/noise_suppression.h \
    AudioAPI/WebRtcMoudle/noise_suppression_x.h \
    AudioAPI/WebRtcMoudle/ns_core.h \
    AudioAPI/WebRtcMoudle/nsx_core.h \
    AudioAPI/WebRtcMoudle/nsx_defines.h \
    AudioAPI/WebRtcMoudle/real_fft.h \
    AudioAPI/WebRtcMoudle/resample_by_2_internal.h \
    AudioAPI/WebRtcMoudle/ring_buffer.h \
    AudioAPI/WebRtcMoudle/signal_processing_library.h \
    AudioAPI/WebRtcMoudle/spl_inl.h \
    AudioAPI/WebRtcMoudle/typedefs.h \
    AudioAPI/WebRtcMoudle/windows_private.h \
    ckernel.h \
    logindialog.h \
    roomdialog.h \
    usershow.h \
    wechatdialog.h

FORMS += \
    logindialog.ui \
    roomdialog.ui \
    usershow.ui \
    wechatdialog.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
