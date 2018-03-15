#ifndef PWDLINEEDIT_H
#define PWDLINEEDIT_H

#include <QLineEdit>

class PwdLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    PwdLineEdit(QWidget *parent = Q_NULLPTR);
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
};

#endif // PWDLINEEDIT_H
