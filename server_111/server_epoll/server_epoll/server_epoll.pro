QT += core
QT -= gui

CONFIG += c++11

INCLUDEPATH += /home/colin/server_111/include

HEADERS += \
    /home/colin/server_111/include/block_epoll_net.h \
    /home/colin/server_111/include/clogic.h \
    /home/colin/server_111/include/err_str.h \
    /home/colin/server_111/include/Mysql.h \
    /home/colin/server_111/include/packdef.h \
    /home/colin/server_111/include/TCPKernel.h \
    /home/colin/server_111/include/Thread_pool.h

SOURCES += \
    /home/colin/server_111/src/block_epoll_net.cpp \
    /home/colin/server_111/src/clogic.cpp \
    /home/colin/server_111/src/err_str.cpp \
    /home/colin/server_111/src/main.cpp \
    /home/colin/server_111/src/Mysql.cpp \
    /home/colin/server_111/src/TCPKernel.cpp \
    /home/colin/server_111/src/Thread_pool.cpp

LIBS += -lpthread
LIBS += -lmysqlclient

TARGET = server_epoll
TEMPLATE = app
