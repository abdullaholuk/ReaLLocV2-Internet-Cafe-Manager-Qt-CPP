#include "subscriber.h"

//Constructs
Subscriber::Subscriber() : User() {
    remainingMoney = 0;
}

//Deconstructs
Subscriber::Subscriber(QString name, double remainingMoney) : User(name) {
    this->remainingMoney = remainingMoney;
}

