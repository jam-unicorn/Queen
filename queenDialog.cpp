#include "queenDialog.h"
#include "./ui_queendialog.h"
#include "trayicon.h"

#include <QProcess>

queenDialog::queenDialog(std::filesystem::path program_dir, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::queenDialog)
    , program_dir{program_dir}
{
    ui->setupUi(this);
    hook.installHook();
//    connect(&hook, SIGNAL(setWindowOpen(bool)), this, SLOT(openWindow(bool)));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::ToolTip);
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
    if (auto script = program_dir / "action.ps1"; std::filesystem::exists(script)) {
        QStringList args;
        args.append("-NoProfile");
        args.append("-File");
        args.append(QString::fromStdString(script.string()));
        args.append("-Text");
        args.append(hook.getText());
        QProcess::startDetached(QString("powershell.exe"), args);
    } else {
        qDebug()<<hook.getText();
    }
    this->hide();
}

