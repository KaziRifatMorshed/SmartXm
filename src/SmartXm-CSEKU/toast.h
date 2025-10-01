#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include <QList>

class Toast : public QWidget {
    Q_OBJECT

public:
    explicit Toast(QWidget *parent, const QString &message, int timeoutMs = 5000);
};

class ToastManager {
public:
    static void showMessage(QWidget *parent, const QString &message, int timeoutMs = 5000);

private:
    static inline QList <Toast *> activeToasts;
    static void positionToasts(QWidget *parent);
};

#endif // TOAST_H
