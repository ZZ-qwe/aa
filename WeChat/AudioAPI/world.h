#ifndef WORLD_H
#define WORLD_H

#include<QAudioInput>
#include<QAudioOutput>
#include<QIODevice>
#include<QTimer>
#include<QMessageBox>
#include<QDebug>
#include"speex/include/speex.h"

//webRtc vad 静音检测
#define USE_VAD   (1)
#include"WebRtc_Vad/webrtc_vad.h"

enum ENUM_PLAY_STATE {stopped, playing, pausing };

//设置压缩质量
#define SPEEX_QUALITY    (8)

#define USE_SPEEX        (1)

#endif // WORLD_H


