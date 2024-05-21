#include "trayicon.h"
#include <QMutex>
#include <QMenu>
#include <QApplication>
#include <windows.h>

QScopedPointer<TrayIcon> TrayIcon::self;

TrayIcon* TrayIcon::Instance(){
    if(self.isNull()){
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if(self.isNull()){
            self.reset(new TrayIcon);
        }
    }
    return self.data();
}

TrayIcon::TrayIcon(QObject *parent)
    : QObject{parent}
{
    mainWidget = 0;
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/mintray.ico"));
    menu = new QMenu;
}

QWidget* TrayIcon::getMainWidget() const
{
    return this->mainWidget;

}

void TrayIcon::setMainWidget(QWidget *mainWidget)
{
    this->mainWidget = mainWidget;
    menu->addAction("退出",this,SLOT(closeAll()));
    trayIcon->setContextMenu(menu);

}

void TrayIcon::showMainWidget()
{
    if(mainWidget)
    {
        mainWidget->showNormal();
    }
}

void TrayIcon::hideMainWidget(){
    if(mainWidget){
        mainWidget->hide();
    }
}

void TrayIcon::closeAll()
{
    trayIcon->hide();
    trayIcon->deleteLater();
    qApp->exit();
}

bool TrayIcon::getVisible() const
{
    return trayIcon->isVisible();
}
void TrayIcon::setVisible(bool visible)
{
    trayIcon->setVisible(visible);
}
