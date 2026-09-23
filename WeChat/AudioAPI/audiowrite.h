#ifndef AUDIOWRITE_H
#define AUDIOWRITE_H

#include <QObject>
#include"world.h"

class AudioWrite : public QObject
{
    Q_OBJECT
public:
    explicit AudioWrite(QObject *parent = nullptr);
    ~AudioWrite();

signals:

public slots:
    void slot_playAudio(QByteArray ba);

private:
    QAudioOutput * m_audio_out;
    QAudioInput *  m_audio_in;//采集声卡，音频输出
    QIODevice* m_buffer_out; //对应的缓冲
    QAudioFormat format;

    //SPEEX相关全局变量
    SpeexBits bits_dec;
    void *Dec_State;

};

#endif // AUDIOWRITE_H
