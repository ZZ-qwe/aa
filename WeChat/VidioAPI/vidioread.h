#ifndef VIDIOREAD_H
#define VIDIOREAD_H

#include <QObject>
#include<QImage>
#include<QTimer>
#include"common.h"
#include <QSharedPointer>
#include"myfacedetect.h"

#include"threadworker.h"

//图片的宽高
#define IMAGE_WIDTH (320)
#define IMAGE_HEIGHT (240)


class VidioWorker;
class VidioRead : public QObject
{
    Q_OBJECT
public:
    explicit VidioRead(QObject *parent = nullptr);

    ~VidioRead();


signals:
    void SIG_sendVidioFrame(QImage img);
public slots:
    void slot_getVidioFrame();
    void slot_openVidio();
    void slot_closeVidio();

    void slot_setMoji(int newMoji);

private:
    QTimer * m_timer;

    //opencv获取图片对象
    cv::VideoCapture cap;
    QSharedPointer <VidioWorker>m_pVidioWorker;

    std::vector<Rect> m_vecLastFace;

    enum moji_type{moji_tuer = 1,moji_hat = 2};
    int m_moji;//用于存储当前萌拍效果 1 兔耳朵 ，2 圣诞帽，3 劳大 4 .美颜作者本人
    QImage m_tuer;
    QImage m_hat;


};
class VidioWorker:public ThreadWorker
{
    Q_OBJECT
public slots:
    void slot_setInfo(VidioRead* p)
    {
        m_pVidioRead = p;
    }
    //定时器到时执行
    void slot_dowork()
    {
        m_pVidioRead->slot_getVidioFrame();
    }

private:
    VidioRead* m_pVidioRead;

};

#endif // VIDIOREAD_H
