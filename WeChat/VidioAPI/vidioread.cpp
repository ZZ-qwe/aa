#include "vidioread.h"
#include<QMessageBox>
#include<QDebug>
#include<QPainter>

VidioRead::VidioRead(QObject *parent) : QObject(parent)

{
    m_timer = new QTimer;
    //connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_getVidioFrame()));
    m_pVidioWorker = QSharedPointer<VidioWorker>(new VidioWorker);
    m_pVidioWorker.data()->slot_setInfo(this);

    connect(m_timer,SIGNAL( timeout() ),m_pVidioWorker.get(),SLOT(slot_dowork()));

    //需要将xlm文件加入 exe同级目录里面
    MyFaceDetect::FaceDetectInit();

    //加载萌拍的图片
    m_moji= 0;

    m_tuer.load(":/images/tuer.png");
    m_hat.load(":/images/hat.png");

}

VidioRead::~VidioRead()
{
    if(m_timer)
    {
        m_timer->stop();
        delete m_timer;
        m_timer  = NULL;
    }
}

//摄像头采集图像流程
void VidioRead::slot_getVidioFrame()
{
    //qDebug()<<"slot_getVidioFrame: "<<QThread::currentThreadId();
    //从摄像头对象读取数据
    Mat frame; //mat 对象存储图像
    if( !cap.read(frame) )
    {
        return;
    }
    //Mat 图像不是显示和传输的格式
    //需要格式转换 opencv采集的格式是BGR ,显示格式为RGB
    //将 opencv 采集的BGR的图片类型转化为RGB24的类型

    //-----------------------------------------------
    //加人脸识别，萌拍功能 opencv图像处理 todo
    //获取摄像头 识别人脸的位置，返回位置对应的矩形框
    //人脸识别失败使用上一次人脸矩形
    std::vector<Rect> faces;
    //存储上一次识别的矩形
    //m_vecLastFace

    if(m_moji != 0)
        MyFaceDetect::detectAndDisplay(frame,faces);

    cvtColor(frame,frame,CV_BGR2RGB);
    //定义QImage对象, 用于发送数据以及图片显示
    QImage image ((unsigned const
    char*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888);
    //将道具绘制到图片上
    QImage tmpImg;
    switch (m_moji)
    {
    case moji_tuer:
        tmpImg = m_tuer;
        break;
    case moji_hat:
        tmpImg = m_hat;
        break;
    }

    if(faces.size()>0)
        m_vecLastFace = faces;

    //将道具绘制到图片上
    if(m_moji == moji_hat||m_moji == moji_tuer)
    {
        //QPainter 使用
        QPainter paint(&image);
        //遍历所有人脸的矩形，画出道具
        for(int i = 0;i<m_vecLastFace.size();i++)
        {
            Rect rct = m_vecLastFace[i];
            int x = rct.x + rct.width*0.5 - tmpImg.width()*0.5 + 20;
            int y = rct.y - tmpImg.height();
            QPoint p(x,y);
            paint.drawImage(p,tmpImg);
        }
    }

    //转化为大小更小的图片
    image = image.scaled( IMAGE_WIDTH,IMAGE_HEIGHT, Qt::KeepAspectRatio );
    //发送图片
    Q_EMIT SIG_sendVidioFrame( image );
}

void VidioRead::slot_openVidio()
{
    m_timer->start(1000/FRAME_RATE -10 );
    //打开摄像头
    cap.open(0);//打开默认摄像头
    if(!cap.isOpened()){
        QMessageBox::information(NULL,tr("提示"),tr("视频没有打开"));
        return;
    }
}

void VidioRead::slot_closeVidio()
{
    //关闭摄像头
    m_timer->stop();
    if(cap.isOpened())
        cap.release();
}

void VidioRead::slot_setMoji(int newMoji)
{
    m_moji = newMoji;
}
