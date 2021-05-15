#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void getWeather();

private:
    Ui::MainWindow *ui;
    QTimer *timer; // added a timer to get weather station data every 3 minutes.
};

#endif // MAINWINDOW_H
