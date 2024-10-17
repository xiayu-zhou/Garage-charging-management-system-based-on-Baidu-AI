#include "mysql_x.h"

MYSQL_X::MYSQL_X(QObject *parent) : QObject(parent)
{
   db = QSqlDatabase::addDatabase("QMYSQL");
    if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
          qDebug() << "MySQL driver not available";
    }
    db.setHostName("192.168.1.132");
    //db.setHostName("119.23.59.142");
    db.setDatabaseName("car_recognition");
    db.setUserName("root");
    db.setPassword("123456");
//    if (db.open()) {
//        qDebug() << "Connected to database";
//        // 执行数据库操作
//    } else {
//        qDebug() << "Failed to connect to database:" << db.lastError().text();
//        // 错误处理
    //    }
}

bool MYSQL_X::insertNmaeAndCarin(QString name, QString carin)
{
    if (!db.open()) {
        qDebug() << "Error: Failed to open database";
        return false;
    }

    QSqlQuery query;
    QString sql = "INSERT INTO cartime (carname, carin) VALUES (:carname, :carin)";
    query.prepare(sql);
    query.bindValue(":carname", name);
    query.bindValue(":carin", carin);

    if (!query.exec()) {
        qDebug() << "Error: Failed to insert data into cartime table:" << query.lastError().text();
        return false;
    }

    db.close();
    return true;
}

bool MYSQL_X::findCarin(QString name)
{
    if (!db.open()) {
        qDebug() << "Error: Failed to open database";
        return false;
    }

    QSqlQuery query;
    // 构造SQL查询语句，查找carin有值但carout为空的记录
    QString sql = "SELECT * FROM cartime WHERE carname = :carName AND carin IS NOT NULL AND carout IS NULL";
    query.prepare(sql);
    query.bindValue(":carName", name);

    // 执行查询
    if (!query.exec()) {
        qDebug() << "Error: Failed to execute query:" << query.lastError().text();
        db.close();
        return false;
    }

    // 检查是否有结果返回
    if (query.next()) {
        // 如果有结果，表示找到了符合条件的记录
        db.close();
        return true;
    }

    // 如果没有结果，表示没有找到符合条件的记录
    db.close();
    return false;
}

bool MYSQL_X::insertCarout(QString name, QString carout)
{
    if (!db.open()) {
        qDebug() << "Error: Failed to open database";
        return false;
    }

    QSqlQuery queryFind;
    QString sqlFind = "SELECT id, carin FROM cartime WHERE carname = :carName AND carin IS NOT NULL AND carout IS NULL";
    queryFind.prepare(sqlFind);
    queryFind.bindValue(":carName", name);

    if (!queryFind.exec()) {
        qDebug() << "Error: Failed to execute find query:" << queryFind.lastError().text();
        db.close();
        return false;
    }

    // 检查是否有结果返回
    if (!queryFind.next()) {
        // 如果没有结果，表示没有找到符合条件的记录
        db.close();
        qDebug() << "No incomplete record found for car name:" << name;
        return false;
    }

    // 第二步：更新carout的值
    int recordId = queryFind.value(0).toInt(); // 假设id是第一个字段
    QString car_in_str = queryFind.value(1).toString();
    car_in_str.replace('T',' ');
    int dotIndex = car_in_str.indexOf('.');
    if (dotIndex != -1) {
        car_in_str = car_in_str.left(dotIndex); // 移除毫秒部分
    }
    QDateTime carin_t = QDateTime::fromString(car_in_str,"yyyy-MM-dd HH:mm:ss");
    QDateTime carout_t = QDateTime::fromString(carout,"yyyy-MM-dd HH:mm:ss");
    qDebug() << "carin_t =  " << carin_t << "   carout_t = " << carout_t
             << "   car_in_str = " << car_in_str << "    carout" << carout;
    qint64 secsDifference = carin_t.secsTo(carout_t);
    qDebug() << "secsDifference = " << secsDifference;
    int hoursDifference = secsDifference / (60 * 60); // 1小时 = 60分钟 = 3600秒
    qDebug() << "Difference in hours:" << hoursDifference;

    QSqlQuery queryUpdate;
    QString sqlUpdate = "UPDATE cartime SET carout = :newCarOut, money = :newMoney WHERE id = :recordId";
    queryUpdate.prepare(sqlUpdate);
    queryUpdate.bindValue(":newCarOut", carout); // 假设carout是QDateTime或QString的合适表示
    queryUpdate.bindValue(":newMoney", hoursDifference * 2); // 假设newMoney是您要设置的新金额值
    queryUpdate.bindValue(":recordId", recordId);

    if (!queryUpdate.exec()) {
        qDebug() << "Error: Failed to execute update query:" << queryUpdate.lastError().text();
        db.close();
        return false;
    }

    emit sig_finish_car(car_in_str,carout,QString::number(hoursDifference * 2));

    // 更新成功
    db.close();
    qDebug() << "Updated carout for car name:" << name;
    return true;
}

bool MYSQL_X::insertCarName(const QString &CarName)
{
    if (!db.open()) {
        qDebug() << "Error: Failed to open database";
        return false;
    }
    QSqlQuery query;

    // 构建SQL插入语句
    QString sql = "INSERT INTO OffendingVehicles (carname) VALUES (:carname)";

    // 准备查询
    query.prepare(sql);

    // 绑定参数
    query.bindValue(":carname", CarName);

    // 执行查询
    if (!query.exec()) {
        // 处理错误
        qDebug() << "Error inserting car name:" << query.lastError().text();
        db.close();
        return false;
    }

    db.close();

    // 插入成功
    return true;
}

bool MYSQL_X::isCarNameExists(const QString &CarName)
{

    if (!db.open()) {
        qDebug() << "Error: Failed to open database";
        return false;
    }
    QSqlQuery query;

    // 构建SQL查询语句
    QString sql = "SELECT COUNT(*) FROM OffendingVehicles WHERE carname = :carname";

    // 准备查询
    query.prepare(sql);

    // 绑定参数
    query.bindValue(":carname", CarName);

    // 执行查询
    if (!query.exec()) {
        // 处理错误
        qDebug() << "Error executing query:" << query.lastError().text();
        db.close();
        return false;
    }

    // 获取查询结果
    if (query.next()) {
        // 获取COUNT(*)的结果
        int count = query.value(0).toInt();
        db.close();
        // 如果计数大于0，表示存在匹配的carname
        return count > 0;
    }

    db.close();
    // 如果没有结果或发生错误，返回false
    return false;
}
