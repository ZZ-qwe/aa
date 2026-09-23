#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include<QCloseEvent>
#include"QDebug"
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

signals:
    void SIG_close();
    void SIG_loginCommit(QString tel,QString password);
    void SIG_registerCommit(QString strTel,QString strPassword,QString strName);

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    //关闭窗口
    void closeEvent(QCloseEvent *event);

private slots:



    void on_pb_commit_clicked();

    void on_pb_clear_register_clicked();

    void on_pb_commit_register_clicked();

    void on_pb_clear_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
