#include "showdailyincometable.h"
#include "ui_showdailyincometable.h"

ShowDailyIncomeTable::ShowDailyIncomeTable(QWidget *parent) : QDialog(parent),ui(new Ui::ShowDailyIncomeTable) {
    ui->setupUi(this);
    db.openDatabase ();
    QSqlQueryModel * modal = new QSqlQueryModel();

    QSqlQuery * query = new QSqlQuery(db.database);

    query->prepare ("SELECT * FROM incomeTable");
    query->exec ();

    modal->setQuery (*query);
    ui->dailyIncomeTable->setModel (modal);

    db.closeDatabase ();
}

ShowDailyIncomeTable::~ShowDailyIncomeTable() {
    delete ui;
}
