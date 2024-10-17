#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtTextToSpeech/QTextToSpeech>
#include <opencv2/opencv.hpp>
#include <onnxruntime/core/session/onnxruntime_cxx_api.h>
#include <json/json.h>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMovie>
#include <curl/curl.h>
#include <QFile>
#include <openssl/opensslconf.h>
#include <QFileDialog>
#include <QTextToSpeech>
//#include <ekho.h>
#include <ekho_typedef.h>
#include <QSqlDatabase>
#include <QDateTime>
#include <QProcess>
#include "mysql_x.h"

//#define SAY(str) process.start("ekho \"" + str + "\"  -s -7")

using namespace cv;
using namespace Ort;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

    void init();

    void initSignal();

    void openCamera();

    void startTime();

    void showImageTolabel(Mat &frame);

    void defImageColor();

    QString getNowTime();

    ~Widget();

public slots:
    void open_caamera_clicked();

    void getPicture();

    void screenshotRecognition();

    void read_ack(QNetworkReply* r);

    void open_file_clicked();

    void update_finish_car(QString carin,QString carout,QString h);

    void on_biaojiweig_clicked();

private:
    Ui::Widget *ui;

    QTimer *time_X;

    Mat src;

    VideoCapture cap;

    bool open_camera_flag = false;

    Mat frame;


    QNetworkAccessManager *httpmanager;
    QTextToSpeech *speeker;

    QString path;

    QTextToSpeech *m_speech;

    MYSQL_X mysql;

    QString CarName;
    //Ekho ekho_tts;

    QProcess process;
};

#endif // WIDGET_H
