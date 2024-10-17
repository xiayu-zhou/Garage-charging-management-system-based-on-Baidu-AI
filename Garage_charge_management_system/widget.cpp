#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);
    init();
    initSignal();
    defImageColor();
//    foreach (QString engine, QTextToSpeech::availableEngines())
//    qDebug()<< engine;
    QLocale locale(QLocale::Chinese, QLocale::China);
    QLocale::setDefault(locale); // 这只会影响 Qt 的区域设置处理，不一定影响 TTS 引擎
    //m_speech->say("Welcome");
    //ekho_tts->speak("123123");
    //process.start("ekho \"哈可视对讲哈萨克哈桑A1231\"  -s -10");
    //process.start("ekho \"发现违规车辆，上报相关人员\"  -s -7");

}

void Widget::init()
{
//    qDebug()<<QSslSocket::sslLibraryBuildVersionString();
//    qDebug() << QSslSocket::supportsSsl();
    time_X = new QTimer(this);
    httpmanager = new QNetworkAccessManager();
    speeker = new QTextToSpeech();
    m_speech = new QTextToSpeech();
}

void Widget::initSignal()
{
    connect(ui->open_camera,&QPushButton::clicked,this,&Widget::open_caamera_clicked);
    connect(ui->jietushibie,&QPushButton::clicked,this,&Widget::screenshotRecognition);
    connect(ui->open_file,&QPushButton::clicked,this,&Widget::open_file_clicked);
    connect(time_X, &QTimer::timeout, this, &Widget::getPicture);
    connect(httpmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(read_ack(QNetworkReply*)));
    connect(&mysql,SIGNAL(sig_finish_car(QString,QString,QString)),this,SLOT(update_finish_car(QString,QString,QString)));
    connect(ui->off_ve,&QPushButton::clicked,this,&Widget::on_biaojiweig_clicked);
}

void Widget::startTime()
{
    time_X->start(30); // 每30毫秒捕获一帧
    qDebug() << "startTime";
}

void Widget::showImageTolabel(Mat &frame)
{
    if (!frame.empty()) {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // 转换为RGB格式
        QImage img((const unsigned char*)(frame.data), frame.cols, frame.rows, frame.cols*frame.channels(), QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(img));
        ui->label->setScaledContents(true); // 启用内容缩放
    }else{
        qDebug() <<"null";
    }
}

void Widget::defImageColor()
{
    frame.setTo(230);
    showImageTolabel(frame);
}

QString Widget::getNowTime()
{
    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();

    // 设置时间格式，例如 "yyyy-MM-dd HH:mm:ss"
    QString format = "yyyy-MM-dd HH:mm:ss";

    // 将时间转换为指定格式的字符串
    QString formattedTime = currentTime.toString(format);

    // 返回格式化后的时间字符串
    return formattedTime;
}

void Widget::read_ack(QNetworkReply *r)
{
    //提取json格式的返回
    QByteArray buf = r->readAll();
    qDebug() << buf;
    //提取json中的感兴趣的内容
    QJsonDocument myjson = QJsonDocument::fromJson(buf);
    QJsonObject node = myjson.object();
    QJsonObject arr = node.value("words_result").toObject();
    QString text = arr.value("number").toString();
    CarName = text;
    ui->textEdit->setText(text);
    if(text != "" || !text.isNull()){
        if(mysql.isCarNameExists(CarName)){
            qDebug() << "发现违规车辆，上报相关人员";
            process.start("ekho \"发现违规车辆，上报相关人员\"  -s -10");
            return ;
        }
        if(mysql.findCarin(text))
        {
            mysql.insertCarout(text,getNowTime());
            //m_speech->say("Have a nice trip to " + text + " car owners");
            process.start("ekho \"一路顺风" + text + "车主\"  -s -10");
        }else{
            mysql.insertNmaeAndCarin(text,getNowTime());
            //m_speech->say("Welcome to " + text + " car owners");
            process.start("ekho \"欢迎您" + text + "车主\"  -s -10");
        }
    }
}

void Widget::open_file_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择文件",QString(),"所有文件 (*);;文本文件 (*.txt)");
    if (!fileName.isEmpty()) {
        // 用户选择了文件，你可以在这里处理文件名
        qDebug() << fileName;
    }
    //向百度发送一个二维码识别请求（HTTP）
    QNetworkRequest myrequset;
    //填充请求url
    myrequset.setUrl(QUrl("https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate?access_token=24.f8374ae639ea15c6aa5e4cd429fd1f06.2592000.1720185424.282335-78726040"));
    //填充header
    myrequset.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //填充请求的body
    //1.挖出照片内容
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray buf = file.readAll();

    //2.转成base64及urlcode
    QByteArray buf64 = buf.toBase64().toPercentEncoding();
    QByteArray body = "image="+buf64;
    //发送完整的一次识别请求
    httpmanager->post(myrequset, body);
}

void Widget::update_finish_car(QString carin, QString carout, QString h)
{
    ui->h_meoney->setText("花费" + h + "元");
    ui->jilu->setText("入库 " + carin + "  出库 " + carout);
}

void Widget::on_biaojiweig_clicked()
{
    if(mysql.isCarNameExists(CarName)){
        qDebug() << "发现违规车辆，上报相关人员";
        process.start("ekho \"发现违规车辆，上报相关人员\"  -s -10");
    }else{
        mysql.insertCarName(CarName);
        qDebug() << "发现违规车辆，上报相关人员";
        process.start("ekho \"发现违规车辆，上报相关人员\"  -s -10");
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::open_caamera_clicked()
{
    qDebug() << "open_caamera_clicked";
    if(open_camera_flag){
        open_camera_flag = false;
        ui->open_camera->setText("打开摄像头");
        time_X->stop();
        defImageColor();

    }else{
        qDebug() << "open_camera_flag = true";
        open_camera_flag = true;
        ui->open_camera->setText("关闭摄像头");
        cap.open(0);
        startTime();
    }
}

void Widget::getPicture()
{
    if(!cap.isOpened()){
        qDebug() << "no open";
        return;
    }

    cap >> frame; // 从摄像头捕获一帧

    showImageTolabel(frame);
}

void Widget::screenshotRecognition()
{
    qDebug() << "Widget::screenshotRecognition";

    imwrite("Test.jpg", frame);

    //向百度发送一个二维码识别请求（HTTP）
    QNetworkRequest myrequset;
    //填充请求url
    myrequset.setUrl(QUrl("https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate?access_token=24.f8374ae639ea15c6aa5e4cd429fd1f06.2592000.1720185424.282335-78726040"));
    //填充header
    myrequset.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //填充请求的body
    //1.挖出照片内容
    QFile file("Test.jpg");
    file.open(QIODevice::ReadOnly);
    QByteArray buf = file.readAll();

    //2.转成base64及urlcode
    QByteArray buf64 = buf.toBase64().toPercentEncoding();
    QByteArray body = "image="+buf64;
    //发送完整的一次识别请求
    httpmanager->post(myrequset, body);
}
