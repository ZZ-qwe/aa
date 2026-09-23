#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include"wechatdialog.h"
#include"QDebug"
#include"TcpClientMediator.h"
#include"packdef.h"
#include"logindialog.h"
#include"demodialog.h"
#include"roomdialog.h"
#include"audioread.h"
#include"audiowrite.h"
#include"vidioread.h"
#include"screenread.h"
#include"threadworker.h"
//协议映射表使用的类型
class Ckernel;
typedef void (Ckernel::*PFUN)(uint sock,char*buf,int nlen);

class SendVidioWorker;


class Ckernel : public QObject
{
    Q_OBJECT
public:
    explicit Ckernel(QObject *parent = nullptr);

    //单例
    static Ckernel* GetInstance()
    {
        static Ckernel kernel;
        return & kernel;
    }

signals:
    void SIG_SendVidio(char* buf,int nlen);

public slots:
    void setNetPackMap();
    void slot_destroy();
    //初始化配置
    void initConfig();
    //提交登录信息
    void slot_loginCommit(QString tel,QString pass);
    //提交注册信息
    void slot_registerCommit(QString tel,QString pass,QString name);

    //创建/加入房间槽
    void slot_createRoom();
    void slot_joinRoom();

    //退出房间
    void slot_quitRoom();

    void slot_startAudio();
    void slot_pauseAudio();

    void slot_startVidio();
    void slot_pauseVidio();

    void slot_startScreen();
    void slot_pauseScreen();

    //刷新图片显示
    void slot_refreshVidio(int id , QImage& img);

    //发送音频祯
    void slot_AudioFrame(QByteArray ba);

    //发送视频帧
    void slot_sendVidioFrame(QImage img);

    //多线程发送视频
    void slot_SendVidio(char* buf,int nlen);

    //网络信息处理
    void slot_dealData(uint sock,char*buf,int nlen);
    //登录回复处理
    void slot_dealLoginRs(uint sock,char*buf,int nlen);
    //注册回复处理
    void slot_dealRegisterRs(uint sock,char*buf,int nlen);
    //创建房间回复
    void slot_dealCreateRoomRs(uint sock,char*buf,int nlen);
    //加入房间回复处理
    void slot_dealJoinRoomRs(uint sock,char*buf,int nlen);
    //房间成员请求
    void slot_dealRoomMemberRq(uint sock,char*buf,int nlen);
    //离开房间的请求处理
    void slot_dealLeaveRoomRq(uint sock,char*buf,int nlen);
    //音频帧处理
    void slot_dealAudioFrameRq(uint sock,char*buf,int nlen);
    //视频帧处理
    void slot_dealVidioFrameRq(uint sock,char*buf,int nlen);


private:
    WeChatDialog *   m_pWeChatDlg;
    INetMediator *   m_pClient;
    LoginDialog *    m_pLoginDlg;
    RoomDialog *     m_pRoomDlg;

    QString          m_serverIP;

    //协议映射表
    PFUN m_netPackMap[_DEF_PACK_COUNT];


    int m_id;
    int m_roomid;
    QString m_name;

    ///
    /// 音频 1个采集 多个播放 每一个房间成员 1：1 map映射
    ///
    AudioRead *m_pAudioRead;
    std::map<int, AudioWrite*> m_mapIDToAudioWrite;
    ///
    /// 视频采集

    VidioRead* m_pVidioRead;

    ScreenRead* m_pScreenRead;

    enum client_type{audio_client = 0,vidio_client};
    INetMediator* m_pAVClient[2];



    QSharedPointer<SendVidioWorker> m_pSendVidioWorker;


};

class SendVidioWorker: public ThreadWorker
{
    Q_OBJECT
public slots:
    void slot_sendVidio(char*buf,int nlen){
        Ckernel::GetInstance()->slot_SendVidio(buf,nlen);
    }
};

#endif // CKERNEL_H
