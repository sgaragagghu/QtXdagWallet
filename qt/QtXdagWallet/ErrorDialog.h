#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>
#include <QPushButton>

typedef enum tag_ERR_DLG_TYPE{
    DLG_PWD_ERR,
}ERR_DLG_TYPE;

namespace Ui {
class ErrorDialog;
}

class ErrorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorDialog(QWidget *parent = 0,ERR_DLG_TYPE mDlgType);
    ~ErrorDialog();

private:
    Ui::ErrorDialog *ui;
    QPushButton *m_pBtnOK;
    ERR_DLG_TYPE mDlgType;

private slots:
    void onBtnClicked();
};

#endif // ERRORDIALOG_H
