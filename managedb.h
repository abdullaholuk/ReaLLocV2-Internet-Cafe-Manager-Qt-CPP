#ifndef MANAGEDB_H
#define MANAGEDB_H

#include <QtSql>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

//This class holds the operations about database
class ManageDB
{
public:
    ManageDB(); //Constructors
    void openDatabase(); //Opens database
    void closeDatabase(); //Closes database

public:
    QSqlDatabase database; //Database ( SQLite )
};

#endif // MANAGEDB_H
