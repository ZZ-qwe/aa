#include "logindialog.h"
#include "ui_logindialog.h"
#include<QRegExp>
#include<QMessageBox>
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("注册&登录");
    ui->twpage->setCurrentIndex(0); //第一时间显示登录界面
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
//清空登录信息
void LoginDialog::on_pb_clear_clicked()
{
    ui->le_pass->setText("");
    ui->le_tel->setText("");
}


//提交登录信息
void LoginDialog::on_pb_commit_clicked()
{
    QString strTel = ui->le_tel->text();
    QString strPassword = ui->le_pass->text();
    //校验
    //非空校验 清掉空格惠普， 不能是空字符串
    QString tmpTel = strTel,tmpPass = strPassword;
    if(tmpTel.remove(" ").isEmpty()||tmpPass.remove(" ").isEmpty())
    {//isEmpty()  相当于  ""
        QMessageBox::about(this,"提示","手机号或密码不能有空格或为空");
        return;
    }
    //1.校验手机号 正则表达式 8-11位手机号
    QRegExp reg("^1[3-9][0-9]\{6,9\}$");
    bool res = reg.exactMatch(tmpTel);
    if(!res)
    {
        QMessageBox::about(this,"提示","手机号格式错误，请输入合法手机号");
        return;
    }
    //2.校验密码 长度在6-20之间
    if(strPassword.length()>20)
    {
        QMessageBox::about(this,"提示","密码长度太长，不能超过20");
        return;
    }else if(strPassword.length()<6)
    {
        QMessageBox::about(this,"提示","密码长度太短，不能少于6");
        return;
    }

    Q_EMIT SIG_loginCommit(strTel,strPassword);
}

//清空注册信息
void LoginDialog::on_pb_clear_register_clicked()
{
    ui->le_comfirm_register->setText("");
    ui->le_name_register->setText("");
    ui->le_pass_register->setText("");
    ui->le_tel_register->setText("");
}

//提交注册信息
void LoginDialog::on_pb_commit_register_clicked()
{
    QString strTel = ui->le_tel_register->text();
    QString strPassword = ui->le_pass_register->text();
    QString strConfirm = ui->le_comfirm_register->text();
    QString strName = ui->le_name_register->text();
    //校验
    //非空校验 清掉空格惠普， 不能是空字符串
    QString tmpTel = strTel,tmpPass = strPassword,tmpName = strName;
    if(tmpTel.remove(" ").isEmpty()||tmpPass.remove(" ").isEmpty()||tmpName.remove(" ").isEmpty())
    {//isEmpty()  相当于  ""
        QMessageBox::about(this,"提示","手机号或密码或昵称不能有空格或为空");
        return;
    }
    //1.校验手机号 正则表达式 8-11位手机号
    QRegExp reg("^1[3-9][0-9]\{6,9\}$");
    bool res = reg.exactMatch(tmpTel);
    if(!res)
    {
        QMessageBox::about(this,"提示","手机号格式错误，请输入合法手机号");
        return;
    }
    //2.校验密码 长度在6-20之间
    if(strPassword.length()>20)
    {
        QMessageBox::about(this,"提示","密码长度太长，不能超过20");
        return;
    }else if(strPassword.length()<6)
    {
        QMessageBox::about(this,"提示","密码长度太短，不能少于6");
        return;
    }
    //3.两次输入密码是否一致
    if(strPassword != strConfirm)
    {
        QMessageBox::about(this,"提示","两次输入的密码需要一致");
        return;
    }
    //4.昵称长度不能超过10
    if(strName.length()>10)
    {
        QMessageBox::about(this,"提示","昵称长度不能超过10");
        return ;
    }
    Q_EMIT SIG_registerCommit(strTel,strPassword,strName);
}

void LoginDialog::closeEvent(QCloseEvent *event)
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




