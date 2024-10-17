#include "MyThread.h"

MyThread::MyThread(QThread *parent) : QThread(parent)
{
    httpmanager = new QNetworkAccessManager();
}

void MyThread::setFileName(QString str)
{
    file_name.setFileName(str);
    file_name.open(QIODevice::ReadOnly);

}

void MyThread::setUrl(QString str)
{
    myrequset.setUrl(str);
    myrequset.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
}

void MyThread::run()
{
    while (1) {
        QByteArray buf = file_name.readAll();
        QByteArray buf64 = buf.toBase64().toPercentEncoding();
        QByteArray body = "image="+buf64;
        httpmanager->post(myrequset, body);
    }
}
