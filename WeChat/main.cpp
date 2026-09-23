#include "ckernel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug()<<"main :"<<QThread::currentThreadId();
    QApplication a(argc, argv);
    //WeChatDialog w;
    //w.show();
    Ckernel::GetInstance();
    return a.exec();
}
