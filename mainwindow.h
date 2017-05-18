#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QListIterator>
#include <QObject>
#include <QDebug>
#include <QPixmap>
#include <QLineEdit>
#include <QLabel>
#include <QRadioButton>

#include "computer.h"
#include "startcomputer.h"
#include "user.h"
#include "subscriber.h"
#include "showsubscribertable.h"
#include "managedb.h"
#include "showdailyincometable.h"

#define numOfComputers 24

namespace Ui {
class MainWindow;
}

//This class holds the operation about main window
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0); //Constructors
    ~MainWindow();
    void addSubscriber(QString name, double money); //Add subscribers to list
    QSqlQuery findSubscriber(QString name); //Search subscriber in list

public slots:
    void showInfoMenu(Computer *); //Shows informations about computer which was clicked last

private slots:
    void on_buttonOpenClose_clicked(); //Starts and shutdowns computer
    void on_actionShowSubInfo_triggered();
    void on_add15min_clicked(); //Adding time operations
    void on_add30min_clicked();
    void on_add60min_clicked();
    void on_add90min_clicked();
    void on_add120min_clicked();
    void on_add180min_clicked();
    void setSubAddOperation(); //Subscriber operations
    void setSubFindOperation();
    void setSubAddMoneyOperation();
    void on_doSth_clicked();

    void on_actionShowDailyIncome_triggered();

private:
    Ui::MainWindow *ui; //Main window
    Computer * lastSelected; //Last clicked computer
    ManageDB db; //Database operations
    QTime currentTime;//Current time
    double dailyIncome; //Daily income
    double cost; //Cost
    int subOperation; //Subscriber operation type(Add subscriber = 1 / Search subscriber = 2 / Add money to subscriber = 3
};

#endif // MAINWINDOW_H
