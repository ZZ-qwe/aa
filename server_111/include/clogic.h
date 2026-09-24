#ifndef CLOGIC_H
#define CLOGIC_H

#include"TCPKernel.h"
#include"packdef.h"
class CLogic
{
public:
    CLogic( TcpKernel* pkernel )
    {
        m_pKernel = pkernel;
        m_sql = pkernel->m_sql;
        m_tcp = pkernel->m_tcp;
    }
public:
    //设置协议映射
    void setNetPackMap();
    /************** 发送数据*********************/
    void SendData( sock_fd clientfd, char*szbuf, int nlen )
    {
        m_pKernel->SendData( clientfd ,szbuf , nlen );
    }
    /************** 网络处理 *********************/
    //注册
    void RegisterRq(sock_fd clientfd, char*szbuf, int nlen);
    //登录
    void LoginRq(sock_fd clientfd, char*szbuf, int nlen);
    //创建房间
    void CreateRoomRq(int clientfd,char*szbuf,int nlen);
    //加入房间
    void JoinRoomRq(int clientfd,char*szbuf,int nlen);
    //退出房间
    void LeaveRoomRq(int clientfd,char*szbuf,int nlen);
    //处理音频帧
    void AudioFrameRq(int clientfd,char*szbuf,int nlen);
    //处理视频帧
    void VidioFrameRq(int clientfd,char*szbuf,int nlen);
    //audio_register
    void AudioRegister(int clientfd,char*szbuf,int nlen);
    //video_register
    void VideoRegister(int clientfd,char*szbuf,int nlen);

    /*******************************************/

private:
    TcpKernel* m_pKernel;
    CMysql * m_sql;
    Block_Epoll_Net * m_tcp;

    //STL map不是线程安全的    需要加锁避免添加移除元素出现问题
    MyMap<int,UserInfo*> m_mapIDToUserInfo;

    MyMap<int ,list<int>> m_mapIDToRoomID;
};

#endif // CLOGIC_H
