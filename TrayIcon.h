#ifndef TRAYICON_H
#define TRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>

class QMenu;

class TrayIcon : public QObject
{
    Q_OBJECT
public:
    static TrayIcon* Instance();
    explicit TrayIcon(QObject *parent = nullptr);
private:
    static QScopedPointer<TrayIcon> self;
    QWidget *mainWidget;
    QMenu* menu;
    QSystemTrayIcon* trayIcon;
public:
    QWidget* getMainWidget() const;
    void setMainWidget(QWidget* mainWidget);    
    bool getVisible() const;
    void setVisible(bool visible);
public slots:
    void closeAll();
    void showMainWidget();
    void hideMainWidget();
signals:
    void trayIconExit();
};

#endif // TRAYICON_H
