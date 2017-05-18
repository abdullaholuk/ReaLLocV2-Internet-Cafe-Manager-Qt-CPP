#include "addsubscriber.h"
#include "ui_addsubscriber.h"

AddSubscriber::AddSubscriber(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSubscriber)
{
    ui->setupUi(this);
}

AddSubscriber::~AddSubscriber()
{
    delete ui;
}
