#include "clogic.h"
#include"packdef.h"
#include"TCPKernel.h"

void CLogic::setNetPackMap()
{
    NetPackMap(_DEF_PACK_REGISTER_RQ)        = &CLogic::RegisterRq;
    NetPackMap(_DEF_PACK_LOGIN_RQ)           = &CLogic::LoginRq;
    NetPackMap(DEF_PACK_CREATEROOM_RQ)       = &CLogic::CreateRoomRq;
    NetPackMap(DEF_PACK_JOINROOM_RQ)         = &CLogic::JoinRoomRq;
    NetPackMap(DEF_PACK_LEAVEROOM_RQ)        = &CLogic::LeaveRoomRq;
    NetPackMap(DEF_PACK_AUDIO_FRAME)         = &CLogic::AudioFrameRq;
    NetPackMap(DEF_PACK_VIDEO_FRAME)         = &CLogic::VidioFrameRq;

    NetPackMap(DEF_PACK_VIDEO_REGISTER)         = &CLogic::VideoRegister;
    NetPackMap(DEF_PACK_AUDIO_REGISTER)         = &CLogic::AudioRegister;

}

#define _DEF_COUT_FUNC_    cout << "clientfd:"<< clientfd << __func__ << endl;

//注册
void CLogic::RegisterRq(sock_fd clientfd,char* szbuf,int nlen)
{
    //cout << "clientfd:"<< clientfd << __func__ << endl;
    _DEF_COUT_FUNC_;

    //1.拆包
    STRU_REGISTER_RQ *rq = (STRU_REGISTER_RQ*)szbuf;
    STRU_REGISTER_RS rs;
    //获取tel password name

    //查表 t_user 根据tel 查tel
    char sqlStr[1024] = {0}/*""*/;
    sprintf(sqlStr,"select tel from t_user where tel = '%s';",rq->m_tel);
    list<string>resList;
    if(!m_sql->SelectMysql(sqlStr,1,resList))
    {
        printf("SelectMysql error: %s \n",sqlStr);
        return;
    }

    //有 user存在
    if(resList.size() >0 )
    {
        rs.m_lResult = tel_is_exist;
    }else
    {
        //没有 查表 t_user 根据name 查name name有没有
        char sqlStr[1024] = {0}/*""*/;
        resList.clear();
        sprintf(sqlStr,"select name from t_user where name = '%s';",rq->m_name);

        if(!m_sql->SelectMysql(sqlStr,1,resList))
        {
            printf("SelectMysql error2: %s \n",sqlStr);
            return;
        }
        if(resList.size() >0 ){
            //有 name存在
            rs.m_lResult = tel_is_exist;
        }else
        {
            //没有 写表tel pass name头 像和签名的默认值 返回注册成功
            rs.m_lResult = register_success;
            sprintf(sqlStr,"insert into t_user(tel,password,name,icon,feeling) values ('%s' ,'%s','%s','%d','%s');"
                    ,rq->m_tel,rq->m_password,rq->m_name ,1 ,"anthony");
            if(!m_sql->UpdataMysql(sqlStr))
            {
                printf("UpdataMysql error :%s\n",sqlStr);
            }
        }

    }
    m_tcp->SendData(clientfd,(char*)&rs,sizeof(rs));

}


// 登录
void CLogic::LoginRq (sock_fd clientfd ,char* szbuf,int nlen)
{
    //    cout << "clientfd:"<< clientfd << **func** << endl;
    _DEF_COUT_FUNC_
    // 拆包 // 手机号 密码
    STRU_LOGIN_RQ *rq = (STRU_LOGIN_RQ*) szbuf;
    STRU_LOGIN_RS rs;
    // 根据 tel 查 pass 和 id
    char strsql [1024] = "";
    list<string> lstRes;
    sprintf (strsql,"select password,id,name from t_user where tel = '% s';",rq->m_tel);
    if (!m_sql->SelectMysql (strsql,3,lstRes))
    {
        printf ("select error :% s\n",strsql);
        return;
    }
    if (lstRes.size () == 0){
        // 查不到 返回 没此用户
    } else {
        // 查到了 pass 是否一致
        if (strcmp (rq->m_password,lstRes.front ().c_str ()) != 0)
        {// 不一致 返回密码错误
            rs.m_lResult = password_error;
        } else {
            lstRes.pop_front ();
            // 一致  sock 要保存起来 id->sock 映射 (为了通信)
            int id = atoi (lstRes.front ().c_str ());
            lstRes.pop_front ();
            //sock 要保存起来 id->sock 映射 (为了通信)
            UserInfo * pInfo = new UserInfo;
            pInfo->m_id = id;
            pInfo->m_roomid = 0;
            pInfo->m_sockfd = clientfd;
            strcpy (pInfo->m_userName,lstRes.front ().c_str ());
            lstRes.pop_front ();
            // 判断 id 是否在线，在线 强制下线，不在线添加
            if (m_mapIDToUserInfo.IsExist (pInfo->m_id))
            {
                // 强制下线
            }
            m_mapIDToUserInfo.insert (pInfo->m_id,pInfo);
            // 写返回包 返回带 id 和结果
            rs.m_userid = id;
            rs.m_lResult = login_success;
            strcpy (rs.m_name,pInfo->m_userName);
        }
    }
    SendData ( clientfd, (char*)&rs, sizeof rs );
}
//创建房间

void CLogic::CreateRoomRq(int clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d CreateRoomRq\n", clientfd);
    //拆包
    STRU_CREATEROOM_RQ *rq = (STRU_CREATEROOM_RQ*)szbuf;
    //随机数得到房间号，看有没有房间号，可能循环随机map roomid-> list
    int roomid = 0;
    do{
        roomid = rand()%99999999+1;
    }while(m_mapIDToRoomID.IsExist(roomid));
    list<int>lst;
    lst.push_back(rq->m_UserID);
    m_mapIDToRoomID.insert(roomid,lst);
    printf("roomid : %d\n",roomid);
    //回复
    STRU_CREATEROOM_RS rs;
    rs.m_RoomID = roomid;
    rs.m_lResult = 1;

    SendData(clientfd,(char*)&rs,sizeof(rs));
}

//加入房间
void CLogic::JoinRoomRq(int clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d JoinRoomRq\n", clientfd);
    STRU_JOINROOM_RQ *rq = (STRU_JOINROOM_RQ*)szbuf;
    STRU_JOINROOM_RS rs;
    //查看房间是否存在
    if(!m_mapIDToRoomID.IsExist(rq->m_RoomID))
    {//不存在返回失败
        rs.m_lResult = 0;
        SendData(clientfd,(char*)&rs,sizeof(rs));
        return ;
    }
    //存在返回成功
    rs.m_lResult = 1;
    rs.m_RoomID = rq->m_RoomID;
    SendData(clientfd,(char*)&rs,sizeof(rs));

    if(!m_mapIDToUserInfo.IsExist(rq->m_UserID)) return;
    UserInfo*joiner = m_mapIDToUserInfo.find(rq->m_UserID);

    STRU_PACK_ROOM_MEMBER joinrq;
    joinrq.m_UserID = rq->m_UserID;
    strcpy(joinrq.m_szUser,joiner->m_userName);

    //给自己用于更新自己的信息
    SendData(clientfd,(char*)&joinrq,sizeof(joinrq));
    //根据房间号拿到房间成员列表
    list<int> lstRoomMem = m_mapIDToRoomID.find(rq->m_RoomID);
    //遍历列表--互换信息

    for(auto ite = lstRoomMem.begin();ite!=lstRoomMem.end();++ite)
    {
        int Memid = *ite;
        if(!m_mapIDToUserInfo.IsExist(Memid)) continue;
        UserInfo*memInfo = m_mapIDToUserInfo.find(Memid);

        STRU_PACK_ROOM_MEMBER memrq;
        memrq.m_UserID = memInfo->m_id;
        strcpy(memrq.m_szUser,memInfo->m_userName);
        //把加入人的信息发给每一个房间内成员
        SendData(memInfo->m_sockfd, (char*)&joinrq, sizeof(joinrq));
        //房间内成员每个人信息发给加入人
        SendData(clientfd, (char*)&memrq, sizeof(memrq));

    }
    //加入人 添加到房间列表
    lstRoomMem.push_back(rq->m_UserID);
    m_mapIDToRoomID.insert(rq->m_RoomID,lstRoomMem);
}

//退出房间
void CLogic::LeaveRoomRq(int clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d LeaveRoomRq\n", clientfd);
    //拆包
    STRU_LEAVEROOM_RQ *rq = (STRU_LEAVEROOM_RQ *)szbuf;
    //看房间是否存在
    if(!m_mapIDToRoomID.IsExist(rq->m_RoomID)) return;
    //如果房间存在，可以获得用户列表
    list<int> lst = m_mapIDToRoomID.find(rq->m_RoomID);
    //遍历每个用户，
    for(auto ite = lst.begin();ite!=lst.end();)
    {
        int userid = *ite;
        //是不是自己，如果是自己 从列表中去除--从房间移除了
        if(userid == rq->m_UserID)
        {
            ite = lst.erase(ite);
        }else
        {
            //用户是否在线，在线转发
            if(m_mapIDToUserInfo.IsExist(userid))
            {
                UserInfo *info = m_mapIDToUserInfo.find(userid);
                SendData(info->m_sockfd,szbuf,nlen);
            }
            ++ite;
        }
    }

    //列表是否节点数为0->map项去掉
    if(lst.size() == 0)
    {
        m_mapIDToRoomID.erase(rq->m_RoomID);
        return;
    }
    //更新房间成员列表
    m_mapIDToRoomID.insert(rq->m_RoomID,lst);


}
//处理音频帧
void CLogic::AudioFrameRq(int clientfd, char *szbuf, int nlen)
{
    //printf("clientfd:%d AudioFrameRq\n", clientfd);
    //拆包
    char * tmp = szbuf;
    //跳过type
    tmp += sizeof(int);
    //读取 userid
    int uid = *(int*)tmp;
    //跳过 userid
    tmp += sizeof(int);
    //获取 roomid
    int roomid = *(int*)tmp ; //按照int取数据

    //看房间是否存在
    if(!m_mapIDToRoomID.IsExist(roomid)) return;
    list<int> lst = m_mapIDToRoomID.find(roomid);
    //获取成员列表
    for(auto ite = lst.begin();ite!=lst.end();++ite)
    {
    //看是否在线转发
        int userid = *ite;
        //屏蔽掉自己 不转发
        if(uid == userid) continue;
        if(!m_mapIDToUserInfo.IsExist(userid)) continue;
        UserInfo* userinfo = m_mapIDToUserInfo.find(userid);
        //原样转发
        SendData(userinfo->m_audiofd,szbuf,nlen);
    }

}
//处理视频帧
void CLogic::VidioFrameRq(int clientfd, char *szbuf, int nlen)
{

    //拆包
    char * tmp = szbuf;
    //跳过type
    tmp += sizeof(int);
    //读取 userid
    int uid = *(int*)tmp;
    //跳过 userid
    tmp += sizeof(int);
    //获取 roomid
    int roomid = *(int*)tmp ; //按照int取数据



    //看房间是否存在
    if(!m_mapIDToRoomID.IsExist(roomid)) return;
    list<int> lst = m_mapIDToRoomID.find(roomid);

    //获取成员列表
    for(auto ite = lst.begin();ite!=lst.end();++ite)
    {
    //看是否在线转发
        int userid = *ite;
        //屏蔽掉自己 不转发
        if(uid == userid) continue;
        if(!m_mapIDToUserInfo.IsExist(userid)) continue;
        UserInfo* userinfo = m_mapIDToUserInfo.find(userid);
        //原样转发
        SendData(userinfo->m_videofd,szbuf,nlen);

    }
}

//audio_register
void CLogic::AudioRegister(int clientfd, char *szbuf, int nlen)
{
    //拆包
    STRU_AUDIO_REGISTER* rq = (STRU_AUDIO_REGISTER*)szbuf;
    int userid = rq->m_userid;
    //m_userid->node refresh fd

    if(m_mapIDToUserInfo.IsExist(userid))
    {
        UserInfo*info = m_mapIDToUserInfo.find(userid);
        info->m_audiofd = clientfd;
    }
}

//video_register
void CLogic::VideoRegister(int clientfd, char *szbuf, int nlen)
{
    //拆包
    STRU_VIDEO_REGISTER* rq = (STRU_VIDEO_REGISTER*)szbuf;
    int userid = rq->m_userid;
    //m_userid->node refresh fd

    if(m_mapIDToUserInfo.IsExist(userid))
    {
        UserInfo*info = m_mapIDToUserInfo.find(userid);
        info->m_videofd = clientfd;
    }
}



