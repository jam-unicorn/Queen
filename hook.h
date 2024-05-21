#ifndef HOOK_H
#define HOOK_H

#include <QObject>
#include <QWidget>

class Hook : public QObject
{
    Q_OBJECT
public:
    explicit Hook(QObject *parent = nullptr);
    void installHook();
    void uninstallHook();
    void sendSignal(bool);
    QString getText() const;
    void setText(QString& text);
private:
    QString text;
signals:
    void setWindowOpen(bool);
};

#endif // HOOK_H
