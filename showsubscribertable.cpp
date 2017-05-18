#include "showsubscribertable.h"
#include "ui_showsubscribertable.h"

ShowSubscriberTable::ShowSubscriberTable(QWidget *parent) : QDialog(parent), ui(new Ui::ShowSubscriberTable) {
    ui->setupUi(this);
    db.openDatabase ();
    QSqlQueryModel * modal = new QSqlQueryModel();

    QSqlQuery * query = new QSqlQuery(db.database);

    query->prepare ("SELECT * FROM subInfo");
    query->exec ();

    modal->setQuery (*query);
    ui->subInfoTable->setModel (modal);

    db.closeDatabase ();
}

ShowSubscriberTable::~ShowSubscriberTable() {
    delete ui;
}
