#include "managedb.h"

ManageDB::ManageDB() {
}

//Opens and connects to database
void ManageDB::openDatabase() {
    database = QSqlDatabase::addDatabase ("QSQLITE");
    QString path(QDir::currentPath ());
    path.append(QDir::separator()).append("database.db");
    path = QDir::toNativeSeparators(path);
    database.setDatabaseName (path);
    database.open ();

    if(!database.isOpen ()) {
        QMessageBox::critical (NULL, "Hata!", "Veritabani açılamadı.");
        exit(0);
    }

    if(database.tables ().isEmpty ()) {
        //If database empty, creates tables
        database.exec ("CREATE  TABLE subInfo (ID INTEGER PRIMARY KEY  NOT NULL  UNIQUE , Name VARCHAR, Remain DOUBLE)");
        database.exec ("CREATE  TABLE incomeTable (Day INTEGER PRIMARY KEY  NOT NULL  UNIQUE , Income DOUBLE)");
        QSqlQuery qry;
        qry.prepare("INSERT INTO incomeTable (Day, Income) VALUES((:day),(:income))");
        qry.bindValue (":day", 0);
        qry.bindValue (":income", 0);
        qry.exec ();
    }
}

//Closes and disconnects database
void ManageDB::closeDatabase() {
    database.close ();
    database.removeDatabase(QSqlDatabase::defaultConnection);

}
