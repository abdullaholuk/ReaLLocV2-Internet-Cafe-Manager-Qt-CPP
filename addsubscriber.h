#ifndef ADDSUBSCRIBER_H
#define ADDSUBSCRIBER_H

#include <QDialog>

namespace Ui {
class AddSubscriber;
}

class AddSubscriber : public QDialog
{
    Q_OBJECT

public:
    explicit AddSubscriber(QWidget *parent = 0);
    ~AddSubscriber();

private:
    Ui::AddSubscriber *ui;
};

#endif // ADDSUBSCRIBER_H
