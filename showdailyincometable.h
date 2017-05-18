#ifndef SHOWDAILYINCOMETABLE_H
#define SHOWDAILYINCOMETABLE_H

#include <QDialog>
#include "managedb.h"

namespace Ui {
class ShowDailyIncomeTable;
}

//This class shows daily income table with a dialog screen
class ShowDailyIncomeTable : public QDialog
{
    Q_OBJECT

public:
    explicit ShowDailyIncomeTable(QWidget *parent = 0); //Constructors
    ~ShowDailyIncomeTable();

private:
    Ui::ShowDailyIncomeTable *ui; //Screen
    ManageDB db; //Database operations
};

#endif // SHOWDAILYINCOMETABLE_H
