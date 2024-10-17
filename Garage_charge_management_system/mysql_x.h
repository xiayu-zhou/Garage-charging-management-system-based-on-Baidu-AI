#ifndef MYSQL_X_H
#define MYSQL_X_H

#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDateTime>

class MYSQL_X : public QObject
{
    Q_OBJECT
public:
    explicit MYSQL_X(QObject *parent = nullptr);

    bool insertNmaeAndCarin(QString name,QString carin);

    bool findCarin(QString name);

    bool insertCarout(QString name, QString carout);

    bool insertCarName(const QString &CarName);

    bool isCarNameExists(const QString &CarName);

signals:

    void sig_finish_car(QString carin,QString carout,QString h);

public slots:

private:
    QSqlDatabase db;
};

#endif // MYSQL_X_H
