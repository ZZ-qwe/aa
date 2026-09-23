#pragma once

#include<memory.h>

#define _DEF_BUFFER         (4096)
#define _DEF_CONTENT_SIZE	(1024)
#define _MAX_SIZE           (40)
#define _MAX_PATH           (260)

//自定义协议   先写协议头 再写协议结构
//登录 注册 获取好友信息 添加好友 聊天 发文件 下线请求
#define DEF_SERVERIP     "192.168.3.239"
#define DEF_PORT         8000
#define _DEF_PACK_BASE	(10000)
#define _DEF_PACK_COUNT (100)

//注册
#define _DEF_PACK_REGISTER_RQ	(_DEF_PACK_BASE + 0 )
#define _DEF_PACK_REGISTER_RS	(_DEF_PACK_BASE + 1 )
//登录
#define _DEF_PACK_LOGIN_RQ	(_DEF_PACK_BASE + 2 )
#define _DEF_PACK_LOGIN_RS	(_DEF_PACK_BASE + 3 )
//创建房间
#define DEF_PACK_CREATEROOM_RQ  (_DEF_PACK_BASE + 4 )
#define DEF_PACK_CREATEROOM_RS   (_DEF_PACK_BASE + 5 )
//加入房间
#define DEF_PACK_JOINROOM_RQ      (_DEF_PACK_BASE + 6 )
#define DEF_PACK_JOINROOM_RS     (_DEF_PACK_BASE + 7 )
//房间列表请求
#define DEF_PACK_ROOM_MEMBER     (_DEF_PACK_BASE + 8 )
//音频数据
#define DEF_PACK_AUDIO_FRAME     (_DEF_PACK_BASE + 9)
//视频数据
#define DEF_PACK_VIDEO_FRAME      (_DEF_PACK_BASE + 10 )
//退出房间请求
#define DEF_PACK_LEAVEROOM_RQ       (_DEF_PACK_BASE + 11 )
//退出房间回复
#define DEF_PACK_LEAVEROOM_RS      (_DEF_PACK_BASE + 12 )
//音频注册
#define DEF_PACK_AUDIO_REGISTER     (_DEF_PACK_BASE + 13 )
//视频注册
#define DEF_PACK_VIDEO_REGISTER      (_DEF_PACK_BASE + 14 )


//返回的结果
//注册请求的结果
#define tel_is_exist		(0)
#define register_success	(1)
#define name_is_exist       (2)
//登录请求的结果
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)

//添加好友的结果
#define no_this_user        (0)
#define user_refuse         (1)
#define user_offline        (2)
#define add_success         (3)

#define MAX_SIZE    (60)
typedef int PackType;

//协议结构
//注册请求
typedef struct STRU_REGISTER_RQ
{
    STRU_REGISTER_RQ()
    {

        m_nType = (_DEF_PACK_REGISTER_RQ);
        memset( m_tel  , 0, MAX_SIZE);
        memset( m_name  , 0, MAX_SIZE);
        memset( m_password , 0, MAX_SIZE);
    }
    //需要手机号码 , 密码, 昵称
    PackType m_nType;
    char m_tel[MAX_SIZE];
    char m_name[MAX_SIZE];
    char m_password[MAX_SIZE];

}STRU_REGISTER_RQ;

//注册回复
typedef struct STRU_REGISTER_RS
{
    //回复结果
    STRU_REGISTER_RS()
    {
        m_nType = (_DEF_PACK_REGISTER_RS);
        m_lResult = 0;
    }
    PackType m_nType;
    int m_lResult;

}STRU_REGISTER_RS;


//协议结构
//登录请求
typedef struct STRU_LOGIN_RQ
{
    STRU_LOGIN_RQ()
    {
        m_nType = _DEF_PACK_LOGIN_RQ;
        memset (m_tel,0,MAX_SIZE);
        memset (m_password,0,MAX_SIZE);
    }
    PackType m_nType;//包类型
    char m_tel[MAX_SIZE] ;//用户ID
    char m_password[MAX_SIZE]; //密码
} STRU_LOGIN_RQ;

//登录回复
typedef struct STRU_LOGIN_RS
{
    // 需要 结果 , 用户的id
    STRU_LOGIN_RS()
    {
        m_nType = (_DEF_PACK_LOGIN_RS) ;
        m_userid = 0;
        m_lResult = 0;
        memset(m_name,0,MAX_SIZE);
    }
    PackType m_nType;
    int m_lResult;
    int m_userid;
    char m_name[MAX_SIZE];
}STRU_LOGIN_RS;

//创建房间请求
typedef struct STRU_CREATEROOM_RQ
{
    STRU_CREATEROOM_RQ()
    {
        m_nType = DEF_PACK_CREATEROOM_RQ;
        m_UserID = 0;
    }
    PackType m_nType;
    int m_UserID;
}STRU_CREATEROOM_RQ;

//创建房间回复
typedef struct STRU_CREATEROOM_RS
{
    STRU_CREATEROOM_RS()
    {
        m_nType = DEF_PACK_CREATEROOM_RS;
        m_lResult = 0;
        m_RoomID = 0;
    }
    PackType m_nType;
    int m_lResult;
    int m_RoomID;
}STRU_CREATEROOM_RS;

//加入房间请求
typedef struct STRU_JOINROOM_RQ
{
    STRU_JOINROOM_RQ()
    {
        m_nType = DEF_PACK_JOINROOM_RQ;
        m_UserID = 0;
        m_RoomID = 0;
    }
    PackType m_nType;
    int m_UserID;
    int m_RoomID;
}STRU_JOINROOM_RQ;

//加入房间回复
typedef struct STRU_JOINROOM_RS
{
    STRU_JOINROOM_RS()
    {
        m_nType = DEF_PACK_JOINROOM_RS;
        m_lResult = 0;
        m_RoomID = 0;
    }
    PackType m_nType;
    int m_lResult;
    int m_RoomID;
}STRU_JOINROOM_RS;

//房间成员请求
typedef struct STRU_PACK_ROOM_MEMBER
{
    STRU_PACK_ROOM_MEMBER()
    {
        m_nType = DEF_PACK_ROOM_MEMBER;
        m_UserID = 0;
        memset(m_szUser,0,MAX_SIZE);
    }
    PackType m_nType;
    int m_UserID;
    char m_szUser[MAX_SIZE];
}STRU_PACK_ROOM_MEMBER;

//离开房间请求
typedef struct STRU_LEAVEROOM_RQ
{
    STRU_LEAVEROOM_RQ()
    {
        m_nType = DEF_PACK_LEAVEROOM_RQ;
        m_UserID = 0;
        m_RoomID = 0;
        memset(szUserName,0,MAX_SIZE);
    }
    PackType m_nType;
    int m_UserID;
    int m_RoomID;
    char szUserName[MAX_SIZE];
}STRU_LEAVEROOM_RQ;


//注册音频
struct STRU_AUDIO_REGISTER
{
    STRU_AUDIO_REGISTER():m_nType(DEF_PACK_AUDIO_REGISTER)
    {
        m_userid = 0;

    }

    PackType m_nType; //包类型
    int m_userid;

};


//注册视频
struct STRU_VIDEO_REGISTER
{
    STRU_VIDEO_REGISTER():m_nType(DEF_PACK_VIDEO_REGISTER)
    {
        m_userid = 0;

    }

    PackType m_nType; //包类型
    int m_userid;

};

