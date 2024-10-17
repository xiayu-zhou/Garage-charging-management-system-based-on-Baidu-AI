#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QThread *parent = nullptr);

    void setFileName(QString str);

    void setUrl(QString str);


signals:

public slots:

protected:
    void run() override;

private:
    QNetworkRequest myrequset;

    QString url;
    QFile file_name;

    QNetworkAccessManager *httpmanager;
};

#endif // MYTHREAD_H
