#include "ErrorDialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QWidget *parent, en_xdag_event_type type) :
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
    this->setWindowIcon(QIcon(":/icon/xdagwallet.ico"));
    connect(m_pBtnOK,SIGNAL(clicked()),this,SLOT(onBtnClicked()));

    setFixedSize(320,100);
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

QString ErrorDialog::getTextFromDlgType(en_xdag_event_type type)
{
    switch (type) {
        case en_event_pwd_error:
            return tr("password error");
        case en_event_pwd_format_error:
            return tr("password foramt error \n length 8-18 consist at leat 1 charactor 1 number");
        case en_event_pwd_not_same:
            return tr("password not the same");
        case en_event_nothing_transfer:
            return tr("nothing transfer");
        case en_event_balance_too_small:
            return tr("balance too small");
        case en_event_invalid_recv_address:
            return tr("invalid receive address");
        default:
            return tr("unkown error");
    }
    return tr("unkown error");
}

void ErrorDialog::onBtnClicked()
{
    switch(this->mDlgType){
        case en_event_pwd_error:
        case en_event_pwd_format_error:
        case en_event_pwd_not_same:
        case en_event_nothing_transfer:
        case en_event_balance_too_small:
        case en_event_invalid_recv_address:
            this->close();
        break;
    }
}
