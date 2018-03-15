#include "PwdLineEdit.h"
#include <QKeyEvent>

PwdLineEdit::PwdLineEdit(QWidget *parent)
{
    this->setEchoMode(QLineEdit::Password);
}

void PwdLineEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::SelectAll)
            || event->matches(QKeySequence::Copy)
            || event->matches(QKeySequence::Paste))
    {
        return;
    }

    QLineEdit::keyPressEvent(event);
}

void PwdLineEdit::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void PwdLineEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void PwdLineEdit::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}
