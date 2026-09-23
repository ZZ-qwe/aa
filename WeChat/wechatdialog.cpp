#include "wechatdialog.h"
#include "ui_wechatdialog.h"
#include<QMessageBox>
WeChatDialog::WeChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WeChatDialog)
{
    ui->setupUi(this);
    setWindowTitle("视频会议v1.0");
}

WeChatDialog::~WeChatDialog()
{
    qDebug()<<__func__;
    delete ui;
}

//关闭事件
void WeChatDialog::closeEvent(QCloseEvent *event)
{
    qDebug()<<__func__;
    if(QMessageBox::question(this,"提示","是否要退出？") == QMessageBox::Yes)
    {
        Q_EMIT SIG_close();
        event->accept();//执行
    }else
    {
        event->ignore();//忽略
    }
//    event->accept();//执行
    //    event->ignore();//忽略
}

void WeChatDialog::setInfo(QString name, int icon)
{//设置名字和头像（头像默认值先不设置）
    ui->lb_name ->setText( name );
}

//创建会议
void WeChatDialog::on_pb_create_clicked()
{
    Q_EMIT SIG_createRoom();
}

//加入会议
void WeChatDialog::on_pb_join_clicked()
{
    Q_EMIT SIG_joinRoom();
}

//不要点我
void WeChatDialog::on_pb_bool3_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("警告");
    msgBox.setText("<span style='color:red'>你已引爆炸弹，即将爆炸，迅速退出</span>");
    msgBox.exec();

}

