#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

typedef enum tag_ERR_DLG_TYPE{
    DLG_PWD_ERR,
    DLG_PWD_FORMAT_ERR,
    DLG_PWD_NOT_THE_SAME,
    DLG_NOTHING_TO_TRANSFER,
    DLG_BALANCE_TOO_SMALL,
    DLG_INVALID_RECV_ADDRESS,
    DLG_UNKOWN_ERROR
}ERR_DLG_TYPE;

namespace Ui {
class ErrorDialog;
}

class ErrorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorDialog(QWidget *parent = 0,ERR_DLG_TYPE type = DLG_UNKOWN_ERROR);
    ~ErrorDialog();

    static QString getTextFromDlgType(ERR_DLG_TYPE type);

private:
    Ui::ErrorDialog *ui;
    QLabel *m_pLBText;
    QPushButton *m_pBtnOK;
    QVBoxLayout *m_pVBLGlobal;
    ERR_DLG_TYPE mDlgType;

private slots:
    void onBtnClicked();
};

#endif // ERRORDIALOG_H
