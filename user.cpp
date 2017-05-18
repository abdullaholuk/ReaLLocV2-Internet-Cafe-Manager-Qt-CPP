#include "user.h"

//Constructs
User::User() {
    name = "";
}

//Deconstructs
User::User(QString name) {
    this->name = name;
}
