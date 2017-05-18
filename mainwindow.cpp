#include "mainwindow.h"
#include "ui_mainwindow.h"

//Constructs main window and initialize computer and ui
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    for(int i = 1; i <= numOfComputers; i++) {
        Computer * cmpt = this->findChild<QGroupBox *>("computers")->
                findChild<Computer *>(QString("computer").append (QString::number(i)));

        cmpt->setPCNumber ();
        connect(cmpt, SIGNAL(clicked(Computer *)), this, SLOT(showInfoMenu(Computer *)));
    }

    ui->progressBar->setValue (0);
    ui->progressBar->setVisible ("false");

    lastSelected = NULL;

    ui->addTime->setVisible (false);
    ui->add->setChecked (true);
    dailyIncome = 0;
    subOperation = 0;

    connect(ui->addSub, SIGNAL(clicked(bool)), this, SLOT(setSubAddOperation()));
    connect(ui->findSub, SIGNAL(clicked(bool)), this, SLOT(setSubFindOperation()));
    connect(ui->addSubMoney, SIGNAL(clicked(bool)), this, SLOT(setSubAddMoneyOperation()));

    ui->subID->setVisible (false);
    ui->readSubID->setVisible (false);
    ui->subMoney->setVisible (false);
    ui->readSubMoney->setVisible (false);
    ui->subName->setVisible (false);
    ui->readSubName->setVisible (false);
    ui->amount->setVisible (false);
    ui->readAmount->setVisible(false);
    ui->doSth->setVisible (false);
}

//Saves dailyIncome to table and deconstructs main window
MainWindow::~MainWindow() {
    db.openDatabase ();
    QSqlQuery qry;
    qry.prepare ("SELECT * from incomeTable WHERE Day = 0");
    qry.exec ();
    double total = 0;
    if(qry.next ())
        total = dailyIncome + qry.value (1).toDouble ();
    qry.prepare ("UPDATE incomeTable SET Income = (:income) WHERE DAY = 0");
    qry.bindValue (":income", total);
    qry.exec ();

    qry.prepare ("SELECT * FROM incomeTable ORDER BY Day DESC LIMIT 1");
    qry.exec();
    int lastInd = 0;
    if(qry.next())
        lastInd = qry.value (0).toInt ();

    qry.prepare ("INSERT INTO incomeTable (Day, Income) VALUES((:day), (:income))");
    qry.bindValue (":day", lastInd + 1);
    qry.bindValue (":income", dailyIncome);
    qry.exec ();
    db.closeDatabase ();
    delete ui;
}

//Adds a subscriber to table
void MainWindow::addSubscriber(QString name, double money) {
    db.openDatabase ();
    QSqlQuery qry;
    qry.prepare ("SELECT * FROM subInfo ORDER BY ID DESC LIMIT 1");
    qry.exec();

    int lastAddID = 0;
    if(qry.next())
        lastAddID = qry.value(0).toInt ();

    qry.prepare("INSERT INTO subInfo (ID, Name, Remain) VALUES((:ID),(:name),(:remain))");
    qry.bindValue (":ID", lastAddID + 1);
    qry.bindValue (":name", name);
    qry.bindValue (":remain", money);
    qry.exec ();
    QMessageBox::information (this, "Subsriber Added", name + " is added to list.\nSubscriber ID : "
                              +QString::number (lastAddID + 1) + "\nSubscriber Remaining Money : " +
                              QString::number (money, 'd', 2));
    db.closeDatabase ();
}

//Searches a subscriber from table
QSqlQuery MainWindow::findSubscriber(QString name) {
    db.openDatabase ();
    QSqlQuery qry;
    qry.prepare("SELECT * from subInfo WHERE Name = (:name) ");
    qry.bindValue (":name", name);
    qry.exec ();
    db.closeDatabase ();
    return qry;
}

//Shows information menu
void MainWindow::showInfoMenu(Computer * cmpt)
{
    currentTime = QTime::currentTime ();

    ui->writePCNumber->setText (QString::number (cmpt->PCNumber));

    if(cmpt->status == 1) {
        ui->buttonOpenClose->setText ("Shutdown Computer");
        ui->outputStart->setText (cmpt->startTime.toString ());

        int time = (currentTime.msec() - cmpt->startTime.msec()) / 6000;
        if(cmpt->userType == 0) //Calculates cost according to user type -> 60 min = 2 TL for subscriber , 3 TL for normal user
            cost = (time % 15) ? ((time / 15) + 1) * 0.50 + 0.5: (time / 15) * 0.50 + 0.5;
        else
            cost = (time % 15) ? ((time / 15) + 1) * 0.40 + 0.4: (time / 15) * 0.40 + 0.4;

        if(cmpt->loginType == 0) {
            //Login with time info menu tools
            ui->progressBar->setVisible (true);
            ui->progressBar->setValue (cmpt->timer->remainingTime () / (600 * cmpt->minute) );
            ui->outputRemainingTime->setText (QString::number(cmpt->timer->remainingTime () / 60000 + 1).append (" Min"));
            ui->addTime->setVisible (true);
            ui->writeRemainingTime->setText ("Remaining Time :");
        }
        else {
            //Login without time info menu tools
            ui->progressBar->setVisible (false);
            ui->progressBar->setVisible (false);
            ui->addTime->setVisible (false);
            ui->writeRemainingTime->setText ("Fee :");
            ui->outputRemainingTime->setText (QString::number (cost, 'd',2).append(" TL"));
            ui->outputSubsID->setText (QString::number ((cmpt->sub.ID)));
        }

        if(cmpt->userType == 0) {
            //Normal user info menu tools
            ui->outputName->setText (cmpt->user.name);
            ui->outputSubRemaining->setVisible (false);
            ui->writeSubRemaining->setVisible (false);
            ui->writeID->setVisible (false);
            ui->outputSubsID->setVisible (false);
        }
        else {
            //Subscriber menu tools
            ui->outputName->setText (cmpt->sub.name);
            ui->outputSubRemaining->setVisible (true);
            ui->writeSubRemaining->setVisible (true);
            ui->writeID->setVisible (true);
            ui->outputSubsID->setVisible (true);
            db.openDatabase ();
            QSqlQuery qry;
            qry.prepare ("SELECT * from subInfo WHERE Name = (:name)");
            qry.bindValue (":name", cmpt->sub.name);
            qry.exec ();
            if(qry.next ())
                cmpt->sub.remainingMoney = qry.value (2).toDouble ();//Calculates current subsriber remaining money
            db.closeDatabase ();
            ui->outputSubRemaining->setText (QString::number (cmpt->sub.remainingMoney, 'd', 2 ));
            ui->outputSubsID->setText (QString::number(cmpt->sub.ID));
        }
    }

    else {
        //Empty info menu
        ui->outputSubRemaining->setVisible (true);
        ui->writeSubRemaining->setVisible (true);
        ui->writeID->setVisible (true);
        ui->outputSubsID->setVisible (true);
        ui->outputName->setText ("");
        ui->outputSubRemaining->setText ("");
        ui->outputSubsID->setText ("");
        ui->outputStart->setText ("");
        ui->outputRemainingTime->setText ("");
        ui->addTime->setVisible (false);
        ui->progressBar->setVisible (false);
        ui->buttonOpenClose->setText ("Start Computer");
    }

    cmpt->findChild<QLabel *>(QString("icomputer").append (QString::number (cmpt->PCNumber)))
            ->setStyleSheet ("background-color: rgb(140, 205, 255);"); //Coloured background after click


    if(lastSelected != NULL && lastSelected != cmpt) {
        lastSelected->findChild<QLabel *>(QString("icomputer").append (QString::number (lastSelected->PCNumber)))
                ->setStyleSheet ("");
        lastSelected = cmpt;
    }
    else
        lastSelected = cmpt;

}

//Starts or shutdowns computer
void MainWindow::on_buttonOpenClose_clicked() {
    StartComputer uiStartComputer;
    if(lastSelected == NULL)
        return ;
    else if(lastSelected->status == 0){
        if(uiStartComputer.takeInfo(lastSelected) == true)
             lastSelected->changeComputerStatus ();
    }
    else {
        dailyIncome += cost;
        lastSelected->shutDownComputer ();
    }
    showInfoMenu (lastSelected);
}

//Shows subscriber table
void MainWindow::on_actionShowSubInfo_triggered() {
    ShowSubscriberTable table;
    table.exec ();
}

//Adding time
void MainWindow::on_add15min_clicked() {
    if(ui->add->isChecked ())
        lastSelected->addTime (15);
    else
        lastSelected->addTime (-15);
    showInfoMenu (lastSelected);
}

void MainWindow::on_add30min_clicked() {
    if(ui->add->isChecked ())
        lastSelected->addTime (30);
    else
        lastSelected->addTime (-30);
    showInfoMenu (lastSelected);
}

void MainWindow::on_add60min_clicked() {
    if(ui->add->isChecked ())
        lastSelected->addTime (60);
    else
        lastSelected->addTime (-60);
    showInfoMenu (lastSelected);
}

void MainWindow::on_add90min_clicked() {
    if(ui->add->isChecked ())
        lastSelected->addTime (90);
    else
        lastSelected->addTime (-90);
    showInfoMenu (lastSelected);
}

void MainWindow::on_add120min_clicked() {
    if(ui->add->isChecked ())
        lastSelected->addTime (120);
    else
        lastSelected->addTime (-120);
    showInfoMenu (lastSelected);
}

void MainWindow::on_add180min_clicked() {
    if(ui->add->isChecked ())
        lastSelected->addTime (180);
    else
        lastSelected->addTime (-180);
    showInfoMenu (lastSelected);
}

//Subscriber add operation
void MainWindow::setSubAddOperation() {
    subOperation = 1;
    ui->subID->setVisible (false);
    ui->readSubID->setVisible (false);
    ui->subMoney->setVisible (false);
    ui->readSubMoney->setVisible (false);
    ui->subName->setVisible (true);
    ui->readSubName->setVisible (true);
    ui->amount->setVisible (true);
    ui->readAmount->setVisible(true);
    ui->readSubName->setText ("");
    ui->readSubID->setText ("");
    ui->readSubMoney->setText ("");
    ui->readAmount->setText ("");
    ui->doSth->setVisible (true);
    ui->amount->setText ("Money :");
    ui->doSth->setText ("Add");
}

//Subscriber find operation
void MainWindow::setSubFindOperation() {
    subOperation = 2;
    ui->subID->setVisible (false);
    ui->readSubID->setVisible (false);
    ui->subMoney->setVisible (false);
    ui->readSubMoney->setVisible (false);
    ui->subName->setVisible (true);
    ui->readSubName->setVisible (true);
    ui->amount->setVisible (false);
    ui->readAmount->setVisible(false);
    ui->readSubName->setText ("");
    ui->readSubID->setText ("");
    ui->readSubMoney->setText ("");
    ui->readAmount->setText ("");
    ui->doSth->setVisible (true);
    ui->doSth->setText ("Find");
}

//Subscriber add operation
void MainWindow::setSubAddMoneyOperation() {
    subOperation = 3;
    ui->subID->setVisible (true);
    ui->readSubID->setVisible (true);
    ui->subMoney->setVisible (false);
    ui->readSubMoney->setVisible (false);
    ui->subName->setVisible (false);
    ui->readSubName->setVisible (false);
    ui->amount->setVisible (true);
    ui->readAmount->setVisible(true);
    ui->doSth->setVisible (true);
    ui->readSubName->setText ("");
    ui->readSubID->setText ("");
    ui->readSubMoney->setText ("");
    ui->readAmount->setText ("");
    ui->amount->setText ("Added Money :");
    ui->doSth->setText ("Add");
}

void MainWindow::on_doSth_clicked() {
    QString name;
    int ID;
    double money;
    QSqlQuery qry;
    if(subOperation == 1) {
        //Adds subscriber to list
        name = ui->readSubName->text ();
        name = name.toUpper ();
        if(name == "")
            QMessageBox::critical (this, "Error", "Subscriber name can not be empty.");
        else {
            money = ui->readAmount->text ().toDouble ();
            qry = findSubscriber (name);
            if(qry.next())
                QMessageBox::critical (this, "Error", "Subscriber has already existed.");
            else
                addSubscriber (name, money);
        }
    }
    else if(subOperation == 2) {
        //Searches subscriber from table
        name = ui->readSubName->text ();
        name = name.toUpper ();
        if(name == "")
            QMessageBox::critical (this, "Error", "Subscriber name can not be empty.");
        else {
            qry = findSubscriber (name);
            if(!qry.next())
                QMessageBox::critical (this, "Hata", "This subscriber is not exist in table.");
            else
            {
                ui->subID->setVisible (true);
                ui->readSubID->setVisible (true);
                ui->readSubID->setText (qry.value (0).toString ());

                ui->readSubName->setText (qry.value(1).toString ());

                ui->subMoney->setVisible (true);
                ui->readSubMoney->setVisible (true);
                ui->readSubMoney->setText (qry.value (2).toString ());
            }
        }
    }
    else if(subOperation == 3){
        //Adds money to subscriber
        ID = ui->readSubID->text ().toInt ();
        money = ui->readAmount->text ().toDouble ();
        db.openDatabase ();
        QSqlQuery qry;
        qry.prepare ("SELECT * from subInfo WHERE ID = (:ID)");
        qry.bindValue (":ID", ID);
        qry.exec ();
        if(!qry.next ())
            QMessageBox::critical (this, "Error", "This subscriber is not exist in table.");
        else {
            name = qry.value (1).toString ();
            money += qry.value(2).toDouble ();
            if(money < 0)
                QMessageBox::critical (this, "Error", "New remaining money cannot be smaller than zero.");
            else {
                qry.prepare ("UPDATE subInfo SET Remain = (:remain) WHERE ID = (:ID)");
                qry.bindValue (":remain", money);
                qry.bindValue (":ID",ID);
                qry.exec ();
                qry.next ();
                QMessageBox::information (this, "Money Added", "Subscriber ID: "
                                          +QString::number (ID) + "\nRemaining Money : " +
                                          QString::number (money, 'd', 2));
            }
        }
        db.closeDatabase ();
    }
    subOperation = 0;
    ui->doSth->setVisible (false);
}

void MainWindow::on_actionShowDailyIncome_triggered()
{
    ShowDailyIncomeTable table;
    table.exec ();
}
