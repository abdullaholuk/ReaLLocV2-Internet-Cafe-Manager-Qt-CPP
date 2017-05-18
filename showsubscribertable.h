#ifndef SHOWSUBSCRIBERTABLE_H
#define SHOWSUBSCRIBERTABLE_H

#include <QDialog>
#include "managedb.h"

namespace Ui {
class ShowSubscriberTable;
}

//This class shows the subscriber table with a dialog screen
class ShowSubscriberTable : public QDialog
{
    Q_OBJECT

public:
    explicit ShowSubscriberTable(QWidget *parent = 0); //Constructors
    ~ShowSubscriberTable();

private:
    Ui::ShowSubscriberTable *ui; //Screen
    ManageDB db; //Database operations
};

#endif // SHOWSUBSCRIBERTABLE_H
