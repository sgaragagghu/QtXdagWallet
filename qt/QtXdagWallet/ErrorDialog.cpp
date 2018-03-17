#include "ErrorDialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QWidget *parent,ERR_DLG_TYPE mDlgType) :
    QDialog(parent),mDlgType(mDlgType),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);

    m_pBtnOK = new QPushButton;
    m_pBtnOK->setText(tr("OK"));

    connect(m_pBtnOK,SIGNAL(clicked()),this,SLOT(onBtnClicked()));
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

void ErrorDialog::onBtnClicked()
{

}
