#include "queenDialog.h"
#include "./ui_queendialog.h"
#include "trayicon.h"

queenDialog::queenDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::queenDialog)
{
    ui->setupUi(this);
    hook.installHook();
//    connect(&hook, SIGNAL(setWindowOpen(bool)), this, SLOT(openWindow(bool)));
    this->setWindowFlags(Qt::FramelessWindowHint);
    TrayIcon::Instance()->setMainWidget(this);
    this->hide();
    TrayIcon::Instance()->setVisible(true);
}

queenDialog::~queenDialog()
{
    delete ui;
    hook.uninstallHook();
}


void queenDialog::on_pushButton_clicked()
{
    qDebug()<<hook.getText();
    this->hide();
}

