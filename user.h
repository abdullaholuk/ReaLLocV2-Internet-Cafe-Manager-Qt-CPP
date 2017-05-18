#ifndef USER_H
#define USER_H

#include <QString>

//This class holds the informations about a user
class User {
public:
    User(); //Constructors
    User(QString name);

public:
    QString name;

};

#endif // USER_H
