#include "ErrorDialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QWidget *parent, ERR_DLG_TYPE type) :
    QDialog(parent),
    mDlgType(type),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);

    m_pLBText = new QLabel;
    m_pLBText->setText(getTextFromDlgType(this->mDlgType));
    m_pLBText->setAlignment(Qt::AlignHCenter);
    m_pBtnOK = new QPushButton;
    m_pBtnOK->setText(tr("OK"));
    m_pBtnOK->setFixedSize(60,25);

    m_pVBLGlobal = new QVBoxLayout;
    m_pVBLGlobal->setAlignment(Qt::AlignCenter);
    m_pVBLGlobal->addWidget(m_pLBText,Qt::AlignHCenter);
    m_pVBLGlobal->addWidget(m_pBtnOK,Qt::AlignHCenter);

    this->setLayout(m_pVBLGlobal);
    connect(m_pBtnOK,SIGNAL(clicked()),this,SLOT(onBtnClicked()));

    setFixedSize(320,70);
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

QString ErrorDialog::getTextFromDlgType(ERR_DLG_TYPE type)
{
    switch (type) {
        case DLG_PWD_ERR:
            return tr("password error");
        case DLG_PWD_FORMAT_ERR:
            return tr("password foramt error \n length 8-18 consist at leat 1 charactor 1 number");
        case DLG_PWD_NOT_THE_SAME:
            return tr("password not the same");
        case DLG_NOTHING_TO_TRANSFER:
            return tr("nothing transfer");
        case DLG_BALANCE_TOO_SMALL:
            return tr("balance too small");
        case DLG_INVALID_RECV_ADDRESS:
            return tr("invalid receive address");
        default:
            return tr("unkown error");
    }
    return tr("unkown error");
}

void ErrorDialog::onBtnClicked()
{
    switch(this->mDlgType){
        case DLG_PWD_ERR:
        case DLG_PWD_NOT_THE_SAME:
        case DLG_PWD_FORMAT_ERR:
        case DLG_NOTHING_TO_TRANSFER:
        case DLG_BALANCE_TOO_SMALL:
        case DLG_INVALID_RECV_ADDRESS:
            this->close();
        break;
    }
}
