#ifndef STARTCOMPUTER_H
#define STARTCOMPUTER_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QRadioButton>

#include "subscriber.h"
#include "user.h"
#include "computer.h"
#include "managedb.h"

namespace Ui {
class StartComputer;
}

//This class takes the informations about a login with a dialog screen
class StartComputer : public QDialog {
    Q_OBJECT

public:
    explicit StartComputer(QWidget *parent = 0); //Constructors
    ~StartComputer();
    bool takeInfo(Computer * cmptr); //Takes informations to cmptr computer
                                     //True if informations are valid, false otherwise
private slots:
    void setUserNormal();
    void setUserSubscriber();
    void setLoginWithTimer();
    void setLoginWithoutTimer();
    void setFee(QString fee);
    void on_buttonBox_accepted();

private:
    Ui::StartComputer *ui; //Screen
    int userType; //User type(normal = 0 / subscriber = 1)
    int loginType; //Login type(with time = 0 / without time = 1)
    QString user; //User name or ID
    int time; //Duration
    double cost; //Cost
    ManageDB db; //Database operations
};

#endif // STARTCOMPUTER_H
