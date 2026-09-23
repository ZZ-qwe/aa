

QT += multimedia

HEADERS += \
    $$PWD/WebRtcMoudle/analog_agc.h \
    $$PWD/WebRtcMoudle/complex_fft_tables.h \
    $$PWD/WebRtcMoudle/cpu_features_wrapper.h \
    $$PWD/WebRtcMoudle/defines.h \
    $$PWD/WebRtcMoudle/digital_agc.h \
    $$PWD/WebRtcMoudle/fft4g.h \
    $$PWD/WebRtcMoudle/gain_control.h \
    $$PWD/WebRtcMoudle/noise_suppression.h \
    $$PWD/WebRtcMoudle/noise_suppression_x.h \
    $$PWD/WebRtcMoudle/ns_core.h \
    $$PWD/WebRtcMoudle/nsx_core.h \
    $$PWD/WebRtcMoudle/nsx_defines.h \
    $$PWD/WebRtcMoudle/real_fft.h \
    $$PWD/WebRtcMoudle/resample_by_2_internal.h \
    $$PWD/WebRtcMoudle/ring_buffer.h \
    $$PWD/WebRtcMoudle/signal_processing_library.h \
    $$PWD/WebRtcMoudle/spl_inl.h \
    $$PWD/WebRtcMoudle/typedefs.h \
    $$PWD/WebRtcMoudle/windows_private.h \
    $$PWD/WebRtc_Vad/signal_processing_library.h \
    $$PWD/WebRtc_Vad/spl_inl.h \
    $$PWD/WebRtc_Vad/typedefs.h \
    $$PWD/WebRtc_Vad/vad_core.h \
    $$PWD/WebRtc_Vad/vad_filterbank.h \
    $$PWD/WebRtc_Vad/vad_gmm.h \
    $$PWD/WebRtc_Vad/vad_sp.h \
    $$PWD/WebRtc_Vad/vad_unittest.h \
    $$PWD/WebRtc_Vad/webrtc_vad.h \
    $$PWD/audioread.h \
    $$PWD/audiowrite.h \
    $$PWD/world.h


#LIBS += $$PWD/speex/lib/libspeex.lib\
#        $$PWD/WebRtc_Vad/signalProcess.lib
SOURCES += \
    $$PWD/WebRtc_Vad/vad_core.c \
    $$PWD/WebRtc_Vad/vad_filterbank.c \
    $$PWD/WebRtc_Vad/vad_gmm.c \
    $$PWD/WebRtc_Vad/vad_sp.c \
    $$PWD/WebRtc_Vad/webrtc_vad.c \
    $$PWD/audioread.cpp \
    $$PWD/audiowrite.cpp


LIBS += $$PWD/speex/lib/libspeex.lib\
        $$PWD/WebRtc_Vad/signalProcess.lib


