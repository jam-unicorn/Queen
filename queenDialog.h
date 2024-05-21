#ifndef QUEENDIALOG_H
#define QUEENDIALOG_H

#include <QDialog>
#include "hook.h"

QT_BEGIN_NAMESPACE
namespace Ui { class queenDialog; }
QT_END_NAMESPACE

class queenDialog : public QDialog
{
    Q_OBJECT

public:
    queenDialog(QWidget *parent = nullptr);
    ~queenDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::queenDialog *ui;
    Hook hook;
    bool isOpen = false;
//private slots:
//    void openWindow(bool flag);
};
#endif // QUEENDIALOG_H
