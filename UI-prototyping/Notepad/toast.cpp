#include "toast.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QPropertyAnimation>

Toast::Toast(QWidget *parent, const QString &message, int timeoutMs)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(message, this);
    label->setStyleSheet("QLabel { background-color: rgba(40, 167, 70, 220);"
                         "color: white; padding: 10px; border-radius: 8px; }");
    layout->addWidget(label);
    setLayout(layout);
    adjustSize();

    QTimer::singleShot(timeoutMs, [this] () {
        QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");

        anim->setDuration(800);
        anim->setStartValue(1);
        anim->setEndValue(0);

        connect(anim, &QPropertyAnimation::finished, this, &QWidget::close);

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    });
}

void ToastManager::showMessage(QWidget *parent, const QString &message, int timeoutMs)
{
    if (!parent) {
        return;
    }

    Toast *toast = new Toast(parent, message, timeoutMs);
    activeToasts.append(toast);

    positionToasts(parent);

    toast->show();

    QObject::connect(toast, &QWidget::destroyed, [=] () {
        activeToasts.removeOne(toast);
        positionToasts(parent);
    });
}

void ToastManager::positionToasts(QWidget *parent)
{
    if (!parent) {
        return;
    }

    int margin = 20;
    int spacing = 10;

    QPoint parentBottomRight = parent->mapToGlobal(QPoint(parent->width(), parent->height()));

    int y = parentBottomRight.y() - margin;

    for (int i = activeToasts.size() - 1; i >= 0; i--) {
        Toast *toast = activeToasts[i];

        toast->adjustSize();
        y -= toast->height();
        toast->move(parentBottomRight.x() - toast->width() - margin, y);
        y -= spacing;
    }
}
