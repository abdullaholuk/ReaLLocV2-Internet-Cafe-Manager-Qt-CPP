#ifndef COMPUTER_H
#define COMPUTER_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QString>
#include <QEvent>
#include <QDebug>
#include <QPixmap>
#include <QTimer>
#include <QDateTime>
#include <cstring>
#include <cstdlib>

#include "user.h"
#include "subscriber.h"
#include "managedb.h"

//This class holds the informations and operations about computers at cafe
class Computer : public QWidget {
    Q_OBJECT
public:
    explicit Computer(QWidget *parent); //Constructors
    ~Computer();
    void setPCNumber(); //Calculates PC number
    void mousePressEvent(QMouseEvent *event); //Controls mouse click on computer
    void addTime(int time); //Add time to user
    void changeComputerStatus(); //Changes computer status

signals:
    void clicked(Computer *);

public slots:
    void shutDownComputer(); //Shutdown computer

public:
    int PCNumber; //PC number
    int status; //PC status(shutdown = 0 / open = 1)
    int loginType; //Login type(with time = 0 / without time = 1)
    int userType; //User type(normal = 0 / subscriber = 1)
    ManageDB db; //Database operations
    QTimer * timer; //Timer for login with time
    User user; //User
    Subscriber sub; //Subscriber
    QTime startTime; //PC start time
    int minute; //PC usage time
    double cost; //Cost for usage
};

#endif // COMPUTER_H
