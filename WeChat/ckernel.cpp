#include "ckernel.h"
#include<QSettings>
#include<QApplication>
#include<QFileInfo>
#include"md5.h"
#include<QTime>

#define NetPackMap(a) m_netPackMap[a - _DEF_PACK_BASE]
void Ckernel::setNetPackMap()
{
    memset(m_netPackMap,0,sizeof(m_netPackMap));
    NetPackMap(_DEF_PACK_LOGIN_RS) = &Ckernel::slot_dealLoginRs;
    NetPackMap(_DEF_PACK_REGISTER_RS) = &Ckernel::slot_dealRegisterRs;
    NetPackMap(DEF_PACK_CREATEROOM_RS) = &Ckernel::slot_dealCreateRoomRs;
    NetPackMap(DEF_PACK_JOINROOM_RS) = &Ckernel::slot_dealJoinRoomRs;
    NetPackMap(DEF_PACK_ROOM_MEMBER) = &Ckernel::slot_dealRoomMemberRq;
    NetPackMap(DEF_PACK_LEAVEROOM_RQ) = &Ckernel::slot_dealLeaveRoomRq;
    NetPackMap(DEF_PACK_AUDIO_FRAME) = &Ckernel::slot_dealAudioFrameRq;
    NetPackMap(DEF_PACK_VIDEO_FRAME) = &Ckernel::slot_dealVidioFrameRq;
}

//初始化配置
void Ckernel::initConfig()
{
    m_serverIP = DEF_SERVERIP;

    //路径设置 exe同级的目录下 -> applicationDirPath config.ini
    QString path = QApplication::applicationDirPath() + "/config.ini";

    //判断是否存在
    QFileInfo info(path);
    QSettings settings(path,QSettings::IniFormat,NULL); //有就打开，没有就创建
    if(info.exists()){
    //加载配置文件 ip 如配置文件 设置为配置文件中的ip
        //打开配置文件
        //移动到net组
        settings.beginGroup("Net");
        //读取ip ->addr-->赋值
        QVariant ip = settings.value("ip");
        QString strIP = ip.toString();
        //结束
        settings.endGroup();
        if(!strIP.isEmpty())
            m_serverIP = strIP;
    }else{
        //没有配置文件 写入默认的ip
        settings.beginGroup("Net");
        settings.setValue("ip",m_serverIP);
        settings.endGroup();
    }
    qDebug()<<m_serverIP;
}

Ckernel::Ckernel(QObject *parent) : QObject(parent)
  ,m_id(0),m_roomid(0)
{
    setNetPackMap();
    initConfig();
    //主功能页面
    m_pWeChatDlg = new WeChatDialog;
    connect(m_pWeChatDlg,SIGNAL(SIG_close()),this,SLOT(slot_destroy()));
    connect(m_pWeChatDlg,SIGNAL(SIG_createRoom()),this,SLOT(slot_createRoom()));
    connect(m_pWeChatDlg,SIGNAL(SIG_joinRoom()),this,SLOT(slot_joinRoom()));




    //登录界面
    m_pLoginDlg = new LoginDialog;
    connect(m_pLoginDlg,SIGNAL(SIG_loginCommit(QString,QString)),this,SLOT(slot_loginCommit(QString,QString)));
    connect( m_pLoginDlg , SIGNAL(SIG_close()),this, SLOT(slot_destroy()) );
    connect(m_pLoginDlg,SIGNAL(SIG_registerCommit(QString,QString,QString)),this,SLOT(slot_registerCommit(QString,QString,QString)));

    m_pRoomDlg = new RoomDialog();
    connect(m_pRoomDlg,SIGNAL(SIG_close()),this,SLOT(slot_quitRoom()));
    connect(m_pRoomDlg,SIGNAL(SIG_AudioStart()),this,SLOT(slot_startAudio()));
    connect(m_pRoomDlg,SIGNAL(SIG_AudioPause()),this,SLOT(slot_pauseAudio()));

    connect(m_pRoomDlg,SIGNAL(SIG_VidioStart()),this,SLOT(slot_startVidio()));
    connect(m_pRoomDlg,SIGNAL(SIG_VidioPause()),this,SLOT(slot_pauseVidio()));

    connect(m_pRoomDlg,SIGNAL(SIG_ScreenStart()),this,SLOT(slot_startScreen()));
    connect(m_pRoomDlg,SIGNAL(SIG_ScreenPause()),this,SLOT(slot_pauseScreen()));


    //添加网络
    m_pClient = new TcpClientMediator;
    m_pClient->OpenNet(m_serverIP.toStdString().c_str(),DEF_PORT);
    connect(m_pClient,SIGNAL(SIG_ReadyData(uint,char*,int)),this,SLOT(slot_dealData(uint,char*,int)));

    //音频和视频的连接
    for(int i =0;i<2;i++)
    {
        m_pAVClient[i] = new TcpClientMediator;
        m_pAVClient[i]->OpenNet(m_serverIP.toStdString().c_str(),DEF_PORT);
        connect(m_pAVClient[i],SIGNAL(SIG_ReadyData(uint,char*,int)),this,SLOT(slot_dealData(uint,char*,int)));
    }


    m_pAudioRead = new AudioRead;
    connect(m_pAudioRead,SIGNAL(SIG_AudioFrame(QByteArray)),this,SLOT(slot_AudioFrame(QByteArray)));

    m_pVidioRead = new VidioRead;
    connect(m_pVidioRead,SIGNAL(SIG_sendVidioFrame(QImage)),this,SLOT(slot_sendVidioFrame(QImage)));

    m_pScreenRead = new ScreenRead;
    connect(m_pScreenRead,SIGNAL(SIG_getScreenFrame(QImage)),this,SLOT(slot_sendVidioFrame(QImage)));

    m_pSendVidioWorker = QSharedPointer<SendVidioWorker>(new SendVidioWorker);
    connect(this,SIGNAL(SIG_SendVidio(char*,int)),m_pSendVidioWorker.data(),SLOT(slot_sendVidio(char*,int)));

    //设置萌拍效果
    connect(m_pRoomDlg,SIGNAL(SIG_setMoji(int)),m_pVidioRead, SLOT(slot_setMoji(int)));


    m_pLoginDlg->show();
}

//回收
void Ckernel::slot_destroy()
{
    qDebug()<<__func__;
    if(m_pWeChatDlg){
        m_pWeChatDlg->hide();
        delete m_pWeChatDlg;
        m_pWeChatDlg = NULL;
    }
    if(m_pLoginDlg){
        m_pLoginDlg->hide();
        delete m_pLoginDlg;
        m_pLoginDlg = NULL;
    }
    if(m_pAudioRead){
        m_pAudioRead->pause();
        delete m_pAudioRead;
        m_pAudioRead = NULL;
    }
    if(m_pRoomDlg){
        m_pRoomDlg->hide();
        delete m_pRoomDlg;
        m_pRoomDlg = NULL;
    }
    if(m_pClient)
    {
        m_pClient->CloseNet();
        delete m_pClient;
        m_pClient = NULL;
    }
    exit(0);
}

//获取MD5加密密文
#define MD5_KEY  (1234)
static std::string GetMD5(QString value)
{
    QString str = QString("%1_%2").arg(value).arg(MD5_KEY);
    std::string strSrc = str.toStdString();
    MD5 md5(strSrc);
    return md5.toString();
}

//提交登录信息
void Ckernel::slot_loginCommit(QString tel, QString pass)
{
    qDebug()<<__func__;
    std::string strTel = tel.toStdString();
    //std::string strPassword = pass.toStdString();


    STRU_LOGIN_RQ rq;
    strcpy(rq.m_tel,strTel.c_str());

    std::string strPassMD5 = GetMD5(pass);
    qDebug()<<strPassMD5.c_str();
    strcpy(rq.m_password,strPassMD5.c_str());

    m_pClient->SendData(0,(char*)&rq,sizeof(rq));
}

//提交注册信息
void Ckernel::slot_registerCommit(QString tel, QString pass, QString name)
{
    qDebug()<<__func__;
    std::string strTel = tel.toStdString();
    //std::string strPassword = pass.toStdString();
    //中文
    std::string strName = name.toStdString();//格式 utf8

    STRU_REGISTER_RQ rq;
    strcpy(rq.m_tel,strTel.c_str());

    std::string strPassMD5 = GetMD5(pass);
    qDebug()<<strPassMD5.c_str();

    //兼容中文 utf8 QString->std::string--> char*
    strcpy(rq.m_name,strName.c_str());
    strcpy(rq.m_password,strPassMD5.c_str());

    m_pClient->SendData(0,(char*)&rq,sizeof(rq));
}

//网络信息处理
void Ckernel::slot_dealData(uint sock, char *buf, int nlen)
{
    qDebug()<<__func__;
    int type = *(int*)buf;
    if(type >= _DEF_PACK_BASE && type < _DEF_PACK_BASE + _DEF_PACK_COUNT)
    {
        //取得协议头，根据协议映射关系，找到函数指针
        PFUN pf = NetPackMap(type);
        if(pf)
        {
            (this->*pf)(sock,buf,nlen);
        }
    }
    delete[]buf;
}
//登录回复处理
void Ckernel::slot_dealLoginRs(uint sock, char *buf, int nlen)
{
    qDebug()<<__func__;
    //.拆包
    STRU_LOGIN_RS *rs = (STRU_LOGIN_RS*) buf;
    //根据返回结果得到不同信息
    switch(rs->m_lResult)
    {
    case user_not_exist:
        QMessageBox::about(m_pLoginDlg,"提示","用户不存在，登录失败");
        break;
    case password_error:

        QMessageBox::about(m_pLoginDlg,"提示","密码错误，登录失败");
        break;
    case login_success:
        {
//            QString strName = QString("用户[%1]登录成功").arg(rs->m_name);
//            QMessageBox::about(m_pLoginDlg,"提示",strName);
            //id 记录
            m_name = QString::fromStdString(rs->m_name);
            m_pWeChatDlg->setInfo(m_name);
            m_id = rs->m_userid;
            //ui跳转
            m_pLoginDlg->hide();
            m_pWeChatDlg->showNormal();

            //注册 视频和音频的fd
            STRU_AUDIO_REGISTER rq_audio;
            rq_audio.m_userid = m_id;
            STRU_VIDEO_REGISTER rq_video;
            rq_video.m_userid = m_id;

            m_pAVClient[audio_client]->SendData(0,(char*)&rq_audio,sizeof(rq_audio));
            m_pAVClient[vidio_client]->SendData(0,(char*)&rq_video,sizeof(rq_video));

        }
        break;
    }

}
//注册回复处理
void Ckernel::slot_dealRegisterRs(uint sock, char *buf, int nlen)
{
    //拆包
    STRU_REGISTER_RS *rs = (STRU_REGISTER_RS *) buf;
    //根据不同的结果 弹出不同的提示窗
    switch (rs->m_lResult)
    {
    case tel_is_exist:
        QMessageBox::about(m_pLoginDlg,"提示","手机号已被注册，注册失败");
        break;
    case register_success:
        QMessageBox::about(m_pLoginDlg,"提示","注册成功");
        break;
    case name_is_exist:
        QMessageBox::about(m_pLoginDlg,"提示","昵称已被注册，注册失败");
        break;
    default:break;
    }
}


//创建房间
void Ckernel::slot_createRoom()
{
    //判断是否再房间内 m_roomid
    if(m_roomid !=0){
        QMessageBox::about(m_pWeChatDlg,"提示","在房间内，无法创建，先退出");
        return ;
    }
    //发命令 创建房间
    STRU_CREATEROOM_RQ rq;
    rq.m_UserID = m_id;

    m_pClient->SendData(0,(char *)&rq,sizeof(rq));
}

//创建房间回复
void Ckernel::slot_dealCreateRoomRs(uint sock, char *buf, int nlen)
{
    //拆包
    STRU_CREATEROOM_RS* rs = (STRU_CREATEROOM_RS*) buf;
    //房间号 显示到界面  跳转界面
    if(m_pRoomDlg == nullptr)
    {
        qDebug() << "m_pRoomDlg是空指针！";
        return;
    }
    m_pRoomDlg->slot_setInfo(QString::number(rs->m_RoomID));
    //服务器没有把个人信息发送给你，作为第一个进入会议的

    //把自己的信息放入房间里 做显示
    UserShow* user = new UserShow;
    connect(user,SIGNAL(SIG_itemClicked(int,QString)),m_pRoomDlg,SLOT(slot_setBigImgId(int,QString)));

    user->slot_setInfo(m_id,m_name);
    m_pRoomDlg->slot_addUserShow(user);

    m_roomid = rs->m_RoomID;
    m_pRoomDlg->showNormal();

    //音频初始化
    m_pRoomDlg->slot_setAudioCheck(false);
    //视频初始化
    m_pRoomDlg->slot_setVidioCheck(false);
}



#include<QInputDialog>
#include<QRegExp>
//加入房间
void Ckernel::slot_joinRoom()
{
    //判断是否再房间内
    if(m_roomid !=0){
        QMessageBox::about(m_pWeChatDlg,"提示","在房间内，无法加入，先退出");
        return ;
    }
    //弹出窗口 填房间号
    QString strRoom = QInputDialog::getText(m_pWeChatDlg,"加入房间","输入房间号");
    QRegExp exp("^[0-9]\{1,8\}$");
    if(! exp.exactMatch(strRoom))
    {
        QMessageBox::about(m_pWeChatDlg,"提示","房间号输入不合法,不超过8的房间号");
        return;
    }
    qDebug()<<strRoom;
    //发命令 加入房间
    STRU_JOINROOM_RQ rq;
    rq.m_UserID = m_id;
    rq.m_RoomID =strRoom.toInt();
    m_pClient->SendData(0,(char *)&rq,sizeof(rq));
}


//加入房间回复处理
void Ckernel::slot_dealJoinRoomRs(uint sock, char *buf, int nlen)
{
    //拆包
    STRU_JOINROOM_RS* rs = (STRU_JOINROOM_RS*)buf;
    //根据结果 失败 提示
    if(rs->m_lResult == 0)
    {
        QMessageBox::about(m_pWeChatDlg,"提示","房间id不存在，加入失败");
        return;
    }
    //成功

    //房间号  显示到页面  跳转
    m_pRoomDlg->slot_setInfo(QString::number(rs->m_RoomID));
    //跳转，m_roomid设置
    m_roomid = rs->m_RoomID;
    m_pRoomDlg->showNormal();

    //音频初始化
    m_pRoomDlg->slot_setAudioCheck(false);
    //视频初始化

    m_pRoomDlg->slot_setVidioCheck(false);

}

//房间成员请求
void Ckernel::slot_dealRoomMemberRq(uint sock, char *buf, int nlen)
{
    qDebug()<<__func__;
    //拆包
    STRU_PACK_ROOM_MEMBER*rq = (STRU_PACK_ROOM_MEMBER*)buf;
//    rq->m_UserID;
//    rq->m_szUser;
    //创建用户对应的控件
    UserShow* user = new UserShow;
    connect(user,SIGNAL(SIG_itemClicked(int,QString)),m_pRoomDlg,SLOT(slot_setBigImgId(int,QString)));
    user->slot_setInfo(rq->m_UserID,QString::fromStdString(rq->m_szUser));
    m_pRoomDlg->slot_addUserShow(user);

    //音频的内容
    AudioWrite *aw = NULL;
    //为每个人创建播放音频的对象
    if(m_mapIDToAudioWrite.count(rq->m_UserID) == 0)
    {
        aw = new AudioWrite;
        m_mapIDToAudioWrite[rq->m_UserID] = aw;
    }

    //视频的内容
}

//离开房间的请求处理
void Ckernel::slot_dealLeaveRoomRq(uint sock, char *buf, int nlen)
{
    qDebug()<<__func__ << "recv userid:" << ((STRU_LEAVEROOM_RQ*)buf)->m_UserID
           << "roomid:" << ((STRU_LEAVEROOM_RQ*)buf)->m_RoomID;

    STRU_LEAVEROOM_RQ *rq = (STRU_LEAVEROOM_RQ*) buf;
    if(rq->m_RoomID == m_roomid)
    {

        m_pRoomDlg->slot_removeUserShow(rq->m_UserID);
    }
    //去掉对应音频
    if(m_mapIDToAudioWrite.count(rq->m_UserID) > 0)
    {
        AudioWrite *pAw = m_mapIDToAudioWrite[rq->m_UserID];
        m_mapIDToAudioWrite.erase(rq->m_UserID);
        delete pAw;
    }
}
//音频帧处理
void Ckernel::slot_dealAudioFrameRq(uint sock, char *buf, int nlen)
{
    //拆包
    //音频帧
    char*tmp = buf;
    //反序列化

    int userId;
    int roomId;

    tmp += sizeof(int);

    userId = *(int*)tmp;//按照整形取
    tmp += sizeof(int);

    roomId = *(int*)tmp;
    tmp += sizeof(int);

    //跳过时间
    tmp += sizeof(int);

    tmp += sizeof(int);

    tmp += sizeof(int);

    int nbufLen = nlen - 6*sizeof (int);
    QByteArray ba(tmp,nbufLen);

    if(m_roomid == roomId)
    {
        if(m_mapIDToAudioWrite.count(userId) > 0)
        {
            AudioWrite * aw = m_mapIDToAudioWrite[userId];
            aw->slot_playAudio(ba);
        }
    }
}
//视频帧处理
void Ckernel::slot_dealVidioFrameRq(uint sock, char *buf, int nlen)
{
    char* tmp = buf;
        tmp += sizeof (int);
        int userId = *(int*)tmp;
        tmp += sizeof (int);
        int roomId = *(int*)tmp;

        tmp += sizeof (int);
        tmp += sizeof (int);
        tmp += sizeof (int);
        tmp += sizeof (int);


        int datalen = nlen - 6*sizeof (int);


        QByteArray bt(tmp,datalen);
        QImage img;
        img.loadFromData(bt);
        if(m_roomid == roomId)
            m_pRoomDlg->slot_refreshUser(userId,img);

}

//退出房间
void Ckernel::slot_quitRoom()
{
    qDebug()<<__func__;
    //发退出包
    STRU_LEAVEROOM_RQ rq;
    rq.m_UserID = m_id;
    rq.m_RoomID = m_roomid;
    std::string name = m_name.toStdString();
    strcpy(rq.szUserName,name.c_str());
    m_pClient->SendData(0,(char*)&rq,sizeof(rq));
    //关闭音频视频
    m_pAudioRead->pause();
    m_pVidioRead->slot_closeVidio();
    m_pScreenRead->slot_closeVidio();

    m_pRoomDlg->slot_setAudioCheck(false);
    m_pRoomDlg->slot_setVidioCheck(false);
    m_pRoomDlg->slot_setScreenCheck(false);

    //回收所有人的audiowrite
    for(auto ite = m_mapIDToAudioWrite.begin();ite != m_mapIDToAudioWrite.end();)
    {
        AudioWrite * pWrite = ite->second;
        ite = m_mapIDToAudioWrite.erase(ite);
        delete pWrite;
    }

    //回收资源
    m_pRoomDlg->slot_clearUserShow();
    m_roomid = 0;

}

void Ckernel::slot_startAudio()
{
    m_pAudioRead->start();
}

void Ckernel::slot_pauseAudio()
{
    m_pAudioRead->pause();
}

void Ckernel::slot_startVidio()
{
    m_pVidioRead->slot_openVidio();
}

void Ckernel::slot_pauseVidio()
{
    m_pVidioRead->slot_closeVidio();
}

void Ckernel::slot_startScreen()
{
    m_pScreenRead->slot_openVidio();
}

void Ckernel::slot_pauseScreen()
{
    m_pVidioRead->slot_closeVidio();
}

void Ckernel::slot_refreshVidio(int id,QImage& img)
{
    m_pRoomDlg->slot_refreshUser(id,img);
}

//发送音频帧
void Ckernel::slot_AudioFrame(QByteArray ba)
{
    int nPackSize = 6*sizeof(int) + ba.size();
    char* buf = new char[nPackSize];
    char*tmp = buf;
    //序列化操作
    int type = DEF_PACK_AUDIO_FRAME;

    int userId = m_id;
    int roomId = m_roomid;
    QTime tm = QTime::currentTime();
    int min = tm.minute();
    int sec = tm.second();
    int msec = tm.msec();

    *(int*)tmp = type; //按照整形存
    tmp += sizeof(int);

    *(int*)tmp = userId;
    tmp += sizeof(int);

    *(int*)tmp = roomId;
    tmp += sizeof(int);

    *(int*)tmp = min;
    tmp += sizeof(int);

    *(int*)tmp = sec;
    tmp += sizeof(int);

    *(int*)tmp = msec;
    tmp += sizeof(int);

    memcpy(tmp,ba.data(),ba.size());

    //m_pClient->SendData(0,buf,nPackSize);
    m_pAVClient[audio_client]->SendData(0,buf,nPackSize);
    delete [] buf;
}

#include<QBuffer>
//发送视频帧
void Ckernel::slot_sendVidioFrame(QImage img)
{
    //显示图片
    slot_refreshVidio(m_id , img);

    //

    //压缩
    //压缩图片从RGB24格式压缩到JPEG格式, 发送出去
    QByteArray ba;
    QBuffer qbuf(&ba); // QBuffer 与 QByteArray 字节数组联立联系
    img.save( &qbuf , "JPEG" , 50 );  //将图片的数据写入 ba
    //使用ba对象, 可以获取图片对应的缓冲区 //可以使用ba.data() , ba.size()将缓冲区发送出去
    //写视频帧 发送

    int nPackSize = 6*sizeof (int) + ba.size();
    char *buf = new char[nPackSize];
    char* tmp = buf;

    *(int*)tmp = DEF_PACK_VIDEO_FRAME;
    tmp += sizeof (int);
    *(int*)tmp = m_id;
    tmp += sizeof(int);
    *(int*)tmp = m_roomid;
    tmp += sizeof(int);

    //用于延迟过久舍弃一些帧的参考时间
    QTime tm = QTime::currentTime();
    *(int*) tmp = tm.minute();
    tmp += sizeof(int);
    *(int*) tmp = tm.second();
    tmp += sizeof(int);
    *(int*) tmp = tm.msec();
    tmp += sizeof(int);

    memcpy(tmp,ba.data(),ba.size());

    //发送是一个阻塞函数，如果服务器接收缓冲区由于数据量过大，没有及时取走缓冲区数据，
    //导致滑动窗口变小，send 函数阻塞 ，影响用户界面响应，出现未响应问题（卡顿）


    //将视频发送 变为一个信号 ，放到另外一个线程执行 todo
//    m_pClient->SendData(0,buf,nPackSize);
//    delete []buf; //修复内存泄漏

    Q_EMIT SIG_SendVidio(buf,nPackSize);

}
//多线程发送视频
void Ckernel::slot_SendVidio(char *buf, int nlen)
{
    char* tmp = buf;
    tmp += sizeof (int);
    tmp += sizeof (int);
    tmp += sizeof (int);

    int min = *(int *)tmp;
    tmp+=sizeof(int);
    int sec = *(int *)tmp;
    tmp+=sizeof(int);
    int msec = *(int *)tmp;
    tmp+=sizeof(int);

    //当前时间
    QTime ctm = QTime::currentTime();
    //数据包时间
    QTime tm(ctm.hour(),min,sec,msec);
    //发送数据包延迟超过300直接舍弃
    if(tm.msecsTo(ctm)>300)
    {
        qDebug()<<"send fail";
        delete [] buf;
        return;
    }

    //m_pClient->SendData(0,buf,nlen);
    m_pAVClient[vidio_client]->SendData(0,buf,nlen);
    delete []buf;
}

