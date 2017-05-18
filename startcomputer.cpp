#include "startcomputer.h"
#include "ui_startcomputer.h"

//Constructs and connects signals and slots
StartComputer::StartComputer(QWidget *parent) : QDialog(parent), ui(new Ui::StartComputer) {
    ui->setupUi(this);
    connect(this->ui->normal, SIGNAL(clicked(bool)), this, SLOT(setUserNormal()));
    connect(ui->subscriber, SIGNAL(clicked(bool)), this, SLOT(setUserSubscriber()));
    connect(ui->withTimer, SIGNAL(clicked(bool)), this, SLOT(setLoginWithTimer()));
    connect(ui->withoutTimer, SIGNAL(clicked(bool)), this, SLOT(setLoginWithoutTimer()));
    connect (ui->readMinute, SIGNAL(textChanged(QString)), this, SLOT(setFee(QString)));
    ui->withTimer->setChecked (true);
    ui->normal->setChecked (true);
}

//Deconstructs
StartComputer::~StartComputer() {
    delete ui;
}

//Takes informations about user and checks they are valid
bool StartComputer::takeInfo(Computer *cmptr) {
    int ret = exec();
    if(ret == QDialog::Accepted) {
        db.openDatabase ();
        if(userType == 1) {
            //Subscriber
            QSqlQuery qry;
            qry.prepare ("SELECT * from subInfo where ID = (:ID)");
            qry.bindValue (":ID",user);
            qry.exec ();
            if(qry.next ()) {
                if(loginType == 0) {
                    //Logins with time
                    double remaining = qry.value (2).toDouble () - cost;
                    if(remaining < cost) {
                        QMessageBox::critical (this, "Insufficient Remaining Money",
                                     "Please load money to your account.");
                        return false;
                    }
                    cmptr->userType = 1;
                    cmptr->loginType = 0;
                    cmptr->sub.ID = qry.value (0).toInt ();
                    cmptr->sub.name = qry.value (1).toString ();
                    cmptr->sub.remainingMoney = remaining;
                    cmptr->timer->start (time*60*1000);
                    cmptr->minute = time;
                    cmptr->startTime = QTime::currentTime ();
                    cmptr->cost = cost;
                    qry.prepare ("UPDATE subInfo SET Remain = (:cost) WHERE ID = (:ID)");
                    qry.bindValue (":cost", remaining);
                    qry.bindValue (":ID", user);
                    qry.exec ();
                }
                else {
                    //Logins without time
                    cmptr->userType = 1;
                    cmptr->loginType = 1;

                    cmptr->sub.ID = qry.value (0).toInt ();
                    cmptr->sub.name = qry.value (1).toString ();
                    cmptr->sub.remainingMoney = qry.value(2).toDouble ();
                    cmptr->startTime = QTime::currentTime ();
                }
            }
            else {
                QMessageBox::critical (this, "Error", "This subscriber is not exist in table.");
                return false;
            }
        }
        else {
            //Normal user
            if(loginType == 0) {
                //Login with time
                cmptr->userType = 0;
                cmptr->loginType = 0;
                cmptr->user.name = user;
                cmptr->minute = time;
                cmptr->timer->start (time*60*1000);
                cmptr->cost = cost;
                cmptr->startTime = QTime::currentTime ();
            }
            else {
                //Login without time
                cmptr->userType = 0;
                cmptr->loginType = 1;
                cmptr->user.name = user;
                cmptr->startTime = QTime::currentTime ();
            }
        }
        db.closeDatabase ();
        return true;
    }
    return false;
}

void StartComputer::setUserNormal() {
    ui->username->setText ("User Name :");
}

void StartComputer::setUserSubscriber() {
    ui->username->setText ("Subscriber ID :");
}

void StartComputer::setLoginWithTimer() {
    ui->minute->setVisible (true);
    ui->readMinute->setVisible (true);
    ui->fee->setVisible (true);
    ui->writeFee->setVisible (true);
}

void StartComputer::setLoginWithoutTimer() {
    ui->minute->setVisible (false);
    ui->readMinute->setVisible (false);
    ui->fee->setVisible (false);
    ui->writeFee->setVisible (false);
}

//Calculates fee
void StartComputer::setFee(QString fee) {
    int time = fee.toInt ();
    double cost;
    int userType = (ui->normal->isChecked ()) ? 0 : 1;
    if(time <= 0)
        return;
    if(userType == 0)
        cost = (time % 15) ? ((time / 15) + 1) * 0.50 + 0.5  : (time / 15) * 0.50 + 0.5 ;
    else
        cost = (time % 15) ? ((time / 15) + 1) * 0.40 + 0.4 : (time / 15) * 0.40 + 0.4 ;

    ui->writeFee->setText (QString::number (cost,'d',2).append (" TL"));
}

void StartComputer::on_buttonBox_accepted() {
    if(ui->readUserName->text ().isEmpty ()) {
        QMessageBox::critical (this, "Error", "User name/Subscriber ID cannot be empty.");
        StartComputer::reject ();
    }
    userType = (ui->normal->isChecked ()) ? 0 : 1;
    loginType = (ui->withTimer->isChecked ()) ? 0 : 1;
    user = ui->readUserName->text ();
    time = 0;
    if(loginType == 0) {
        if(ui->readMinute->text ().isEmpty ()) {
            QMessageBox::critical (this, "Error", "Minute can not be empty for login with time.");
            StartComputer::reject ();
        }
        time = ui->readMinute->text ().toInt ();
        if(time <= 0) {
            QMessageBox::critical (this, "Error", "Please write valid minute.");
            StartComputer::reject ();
        }

        if(userType == 0)
            cost = (time % 15) ? ((time / 15) + 1) * 0.50: (time / 15) * 0.50;
        else
            cost = (time % 15) ? ((time / 15) + 1) * 0.40: (time / 15) * 0.40;
    }
}
