#include "computer.h"

//Constructs a computer widget
Computer::Computer(QWidget *parent) : QWidget(parent) {
    status = 0;
    timer = new QTimer(this);
    connect (timer, SIGNAL(timeout()), this, SLOT(shutDownComputer()));

}

//Deconstructs
Computer::~Computer() {
    delete this;
}

//Calculates PC number using label name
void Computer::setPCNumber() {
    PCNumber = atoi(this->objectName ().toStdString ().substr (8,2).c_str ());

}

//Mouse clicked on computer
void Computer::mousePressEvent(QMouseEvent *event) {
    emit clicked(this);
}

//Shutdowns computer and calculates current cost
//If current cost is lower than opening cost, returns remainder money

void Computer::shutDownComputer() {
    double currentCost;
    int time = (QTime::currentTime ().msec () - startTime.msec()) / 6000; //Calculates time diffence from start and now
    if(userType == 0) //Calculates cost according to user type -> 60 min = 2 TL for subscriber , 3 TL for normal user
        currentCost = (time % 15) ? ((time / 15) + 1) * 0.50 + 0.5: (time / 15) * 0.50 + 0.5;
    else
        currentCost = (time % 15) ? ((time / 15) + 1) * 0.40 + 0.4: (time / 15) * 0.40 + 0.4;
    if(timer->isActive ())
        timer->stop ();
    if(loginType == 0) {
        if(userType == 0) {
            if(currentCost < cost) {
                //Normal user, logins with time, currentCost < startCost, we need to return remainder
                QMessageBox::information (this, "Remainder Money",
                             QString::number (cost - currentCost, 'd',2).append (" TL will be returned."));

                changeComputerStatus ();
                return ;
            }
        }
        else {
            if(currentCost < cost) {
                //Subscriber, logins with time, currentCost < startCost, we need to return to account in subscriber table
                QMessageBox::information (this, "Remainder Money",
                             QString::number (cost - currentCost, 'd',2).append (" TL returned your account."));

                db.openDatabase ();
                QSqlQuery qry;
                qry.prepare ("UPDATE subInfo SET Remain = (:cost) WHERE ID = (:ID)");
                qry.bindValue (":cost", sub.remainingMoney + (cost - currentCost));
                qry.bindValue (":ID", sub.ID);
                qry.exec ();
                db.closeDatabase ();
                changeComputerStatus ();
                return;
            }
        }
    }
    else {
        if(userType == 0) {
            //Normal user, logins without time, he needs to pay
            QMessageBox::information (this, "Fee", QString("You should pay ").append(
                                          QString::number (currentCost, 'd',2)));

            changeComputerStatus ();
            return ;
        }
        else {
            if(currentCost > sub.remainingMoney) {
                //Subscriber, logins without time, currentCost < remainingMoney, he needs to load money to account
                QMessageBox::critical (this, "Insufficient Remaining Money", "Your remaining money is missing " +
                             QString::number (cost - currentCost, 'd',2).
                             append (" TL. Please load money to your account."));
                return ;
            }
            else {
                //Subscriber, logins without time, cost will be taken his account
                QMessageBox::information (this, "Fee",
                             QString::number (currentCost, 'd',2).append (" TL will be taken from your account."));
                db.openDatabase ();
                QSqlQuery qry;
                qry.prepare ("UPDATE subInfo SET Remain = (:cost) WHERE ID = (:ID)");
                qry.bindValue (":cost", sub.remainingMoney - currentCost);
                qry.bindValue (":ID", sub.ID);
                qry.exec ();
                db.closeDatabase ();
                changeComputerStatus ();
                return ;
            }
        }
    }
    //Changes computer status
    changeComputerStatus ();
}

//Adds time to user
void Computer::addTime(int time) {
    int r = timer->remainingTime ();
    double addCost = 1; //Cost for add or subtract time
    if(r + time * 60 * 1000 <= 0){
        QMessageBox::critical (this, "Error", "This time is invalid for subtract from remaining time.");
        return ;
    }
    if(time < 0){
        addCost = -1;
        time = -time;
    }
    if(userType == 0)
        addCost *= (time % 15) ? ((time / 15) + 1) * 0.50 + 0.5: (time / 15) * 0.50 + 0.5;
    else
        addCost *= (time % 15) ? ((time / 15) + 1) * 0.40 + 0.4: (time / 15) * 0.40 + 0.4;
    if(addCost < 0)
        time = -time;
    if(userType == 0) {
        //Normal user add or subtract time
        timer->stop ();
        timer->start (r + time * 60 * 1000);
        minute += time;
        cost += addCost;
        if(addCost > 0) {
           QMessageBox::information (this, "Fee", QString("You should pay ").append (
                         QString::number (addCost, 'd',2).append (" TL")));
        }
        if(addCost < 0) {
            QMessageBox::information (this, "Remainder Money",
                         QString::number (addCost, 'd',2).append (" TL will be returned."));
        }
    }
    else {
        //Subscriber
        if((addCost > 0 && sub.remainingMoney >= addCost) ) {
            //Add time and remainingMoney >= addCost
            QMessageBox::information (this, "Fee",
                                      QString::number (addCost, 'd',2).append ("TL will be taken from your account."));
            db.openDatabase ();
            QSqlQuery qry;
            qry.prepare ("UPDATE subInfo SET Remain = (:cost) WHERE ID = (:ID)");
            qry.bindValue (":cost", sub.remainingMoney - addCost);
            qry.bindValue (":ID", sub.ID);
            qry.exec ();
            db.closeDatabase ();
            timer->stop ();
            timer->start (r + time * 60 * 1000);
            minute += time;
            cost += addCost;
        }
        else if(addCost < 0) {
            //Subtract time
            QMessageBox::information (this, "Remainder Money",
                         QString::number (-addCost, 'd',2).append (" TL will be returned to your account."));
            timer->stop ();
            timer->start (r + time * 60 * 1000);
            minute += time;
            cost += addCost;
            db.openDatabase ();
            QSqlQuery qry;
            qry.prepare ("UPDATE subInfo SET Remain = (:cost) WHERE ID = (:ID)");
            qry.bindValue (":cost", sub.remainingMoney - addCost);
            qry.bindValue (":ID", sub.ID);
            qry.exec ();
            db.closeDatabase ();
        }
        else if(addCost > sub.remainingMoney) {
            //Add time but addCost < remainingMoney
            QMessageBox::critical (this, "Insufficient Remaining Money", "Your remaining money is missing " +
                                    QString::number (-addCost, 'd',2).
                                    append (" TL. Please load money to your account."));

        }
    }
}

void Computer::changeComputerStatus() {
    if(status == 0) {
        status = 1;
        QPixmap  pix(":/icons/icons/monitor-icon-working.png");
        this->findChild<QLabel *>(QString("icomputer").append (QString::number (PCNumber)))->setPixmap (pix);
    }
    else {
        status = 0;
        QPixmap  pix(":/icons/icons/monitor-icon-shutdown.png");
        this->findChild<QLabel *>(QString("icomputer").append (QString::number (PCNumber)))->setPixmap (pix);
    }
}
