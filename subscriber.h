#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "user.h"

//This class holds the information about a subcsriber inherited by User
class Subscriber : public User {
public:
    Subscriber(); //Constructors
    Subscriber(QString name, double remainingMoney);

public:
    double remainingMoney; //Remaining money
    int ID; //ID
};

#endif // SUBSCRIBER_H
