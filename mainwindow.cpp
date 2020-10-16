/* The is the Full Screen (FS) version of the genweather app.
 *   This program interigates the openweathermap.org website to get JSON info from it.
 *  The response looks something like this:
 *  {
 * "lat":30.22,
 * "lon":-95.36,
 * "timezone":"America/Chicago",
 * "timezone_offset":-18000,
 * "current":{"dt":1602692411,"sunrise":1602678180,"sunset":1602719476,"temp":77.56,"feels_like":82.8,"pressure":1017,"humidity":71,"dew_point":67.41,
 *            "uvi":6.84,"clouds":0,"visibility":10000,"wind_speed":1.99,"wind_deg":185,"wind_gust":4,"weather":[{"id":800,"main":"Clear",
 *            "description":"clear sky","icon":"01d"}]
 * }
 * }
 *
 * This version looks a little different because it interigates the website a little differently
 * in order to get wind speed , wind gust and more.
 * From this response I extract weather data and display it in a window on a raspberry pi
 * Written by Larry Bonnette October 2020
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this); // set up timer
    connect(timer,SIGNAL(timeout()),this,SLOT(getWeather()));
    timer->start(180000); // Starting the 3 minute timer
    getWeather(); // Get weather from weather station for the first time
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getWeather() // This function will get kicked off when timer times out openweather.com only allows 60 queries per hour (for free) so getting the weather
                              // every 3 minutes burns up only 20 queries
{
    QDate date = QDate::currentDate();
    ui->date_lbl->setText(date.toString("MMM-dd-yyyy"));
    QTime time = QTime::currentTime();
    ui->time_lbl->setText(time.toString("h:mm AP t")); //formats time to HH:MM in AM/PM and time Zone

    // Network
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString("http://api.openweathermap.org/data/2.5/onecall?lat=30.22&lon=-95.36&exclude=hourly,minutely,alerts,daily&units=imperial&appid={Add you own key}") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll(); // strReply holds the full data stream from weather station

        //qDebug() << strReply;

        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());  //Convert JSON from website to JSON Document

        QJsonObject root_obj = jsonResponse.object(); // Convert JSON Doc to Object
        QVariantMap root_map = root_obj.toVariantMap(); // Create a root map
        QVariantMap current_map = root_map["current"].toMap(); // key with temperature data in it is called "current" convert it to a map

        //ui->incomingtxtlbl->setText(strReply); // Places all of the weather station text into a large label.


        ui->temp_lbl->setText(current_map["temp"].toString() + " F"); // Extract the temperature from the "current_map" key "temp" and place it in a label.

        ui->humid_lbl->setText(current_map["humidity"].toString() + " %");

        QString dir = current_map["wind_deg"].toString(); // Current wind direction

         //qDebug() << dir;

        // convert compass degrees to N,S,E,W text

         double bdir = dir.toDouble();

         if (bdir >= 0 && bdir < 22){
             ui->dir_lbl->setText("North");
         }
         else if (bdir >= 22 && bdir < 45){
             ui->dir_lbl->setText("NNE");
         }
         else if (bdir >= 45 && bdir < 67){
             ui->dir_lbl->setText("NE");
         }
         else if (bdir >= 67 && bdir < 90){
             ui->dir_lbl->setText("ENE");
         }
         else if (bdir >= 90 && bdir < 112){
             ui->dir_lbl->setText("East");
         }
         else if (bdir >= 112 && bdir < 135){
             ui->dir_lbl->setText("ESE");
         }
         else if (bdir >= 135 && bdir < 157){
             ui->dir_lbl->setText("SE");
         }
         else if (bdir >= 157 && bdir < 180){
             ui->dir_lbl->setText("SSE");
         }
         else if (bdir >= 180 && bdir < 202){
             ui->dir_lbl->setText("South");
         }
         else if (bdir >= 202 && bdir < 225){
             ui->dir_lbl->setText("SSW");
         }
         else if (bdir >= 225 && bdir < 247){
             ui->dir_lbl->setText("SW");
         }
         else if (bdir >= 247 && bdir < 270){
             ui->dir_lbl->setText("WSW");
         }
         else if (bdir >= 270 && bdir < 292){
             ui->dir_lbl->setText("West");
         }
         else if (bdir >= 292 && bdir < 315){
             ui->dir_lbl->setText("WNW");
         }
         else if (bdir >= 315 && bdir < 337){
             ui->dir_lbl->setText("NW");
         }
         else if (bdir >= 337 && bdir < 359){
             ui->dir_lbl->setText("NNW");
         }


         ui->speed_lbl->setText(current_map["wind_speed"].toString());

         ui->gust_lbl->setText(current_map["wind_gust"].toString());
         qDebug() << current_map["wind_gust"].toString();

        QString press = current_map["pressure"].toString();
         double bpress = press.toDouble();  //Convert  string to double
         bpress = (bpress * 0.029530); //Convert milibar to inches of mercury
         QString npress = QString::number(bpress,'f',2);  //Convert double to string to display in label the "(num,'f',2)" formats for 2 decimal places only.

         ui->press_lbl->setText(npress);  // display pressure in inches
}
}

