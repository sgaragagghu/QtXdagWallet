#include "PwdDialog.h"
#include "ui_pwddialog.h"

#include <QKeyEvent>

PwdDialog::PwdDialog(QWidget *parent, PWD_DLG_TYPE type) :
    QDialog(parent),
    mDlgType(type),
    ui(new Ui::PwdDialog)
{
    ui->setupUi(this);

    m_pLable = new QLabel;
    m_pLEPwd = new PwdLineEdit;
    m_pPBOK = new QPushButton("OK");

    m_pLable->setFixedSize(100,25);
    m_pLEPwd->setFixedSize(200,25);
    m_pPBOK->setFixedSize(60,25);
    m_pLEPwd->setEchoMode(QLineEdit::Password);
    m_pLable->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pHBLInputPwd = new QHBoxLayout;
    m_pHBLInputPwd->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pHBLInputPwd->addWidget(m_pLable);
    m_pHBLInputPwd->addWidget(m_pLEPwd);

    m_pHBLButton = new QHBoxLayout;
    m_pHBLButton->addWidget(m_pPBOK,Qt::AlignHCenter);

    m_pVBLGlobal = new QVBoxLayout;
    m_pVBLGlobal->setAlignment(Qt::AlignHCenter);
    m_pVBLGlobal->addLayout(m_pHBLInputPwd);
    m_pVBLGlobal->addLayout(m_pHBLButton);

    this->setLayout(m_pVBLGlobal);
    m_pLEPwd->installEventFilter(this);
    m_pPBOK->installEventFilter(this);

    setWindowIcon(QIcon(":/icon/xdagwallet.ico"));
    setWindowTitle("Dagger Wallet(XDAG)");
    setFixedSize(320,70);

    switch(type){
        case DLG_TYPE_PWD:
            m_pLable->setText("input password");
        break;
        case DLG_SET_PWD:
            m_pLable->setText("set password");
        break;
        case DLG_RETYPE_PWD:
            m_pLable->setText("confirm password");
        break;
        case DLG_TYPE_RDM:
            m_pLable->setText("set random keys");
        break;
    }

    connect(m_pPBOK,SIGNAL(clicked(bool)),this,SLOT(onBtnOKClicked()));
}

PwdDialog::~PwdDialog()
{
    delete ui;
}

void PwdDialog::closeDialog()
{
    //avoid symbol conflict with socket close
    this->close();
}

void PwdDialog::onBtnOKClicked()
{
    QString str = m_pLEPwd->text();

    switch(mDlgType){
        case DLG_TYPE_PWD:
            emit sendTypePwd(str);
        break;
        case DLG_SET_PWD:
            emit sendSetPwd(str);
        break;
        case DLG_RETYPE_PWD:
            emit sendRetypePwd(str);
        break;
        case DLG_TYPE_RDM:
            emit sendRdm(str);
        break;
    }
}

bool PwdDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_pLEPwd || obj == m_pPBOK){

        switch (event->type()) {
            case QEvent::MouseMove:
            case QEvent::MouseButtonDblClick:
                break;
            case QEvent::KeyPress:
            {
                QKeyEvent *pKeyEvent = static_cast<QKeyEvent*>(event);

                if(pKeyEvent->matches(QKeySequence::SelectAll)
                        || pKeyEvent->matches(QKeySequence::Copy)
                        || pKeyEvent->matches(QKeySequence::Paste))
                {
                    return true;
                }
                if(pKeyEvent->key() == Qt::Key_Tab ||
                        pKeyEvent->key() == Qt::Key_Left ||
                        pKeyEvent->key() == Qt::Key_Right ||
                        pKeyEvent->key() == Qt::Key_Up ||
                        pKeyEvent->key() == Qt::Key_Down){
                    return true;
                }
            }
            default:
                break;
        }
    }

    return QDialog::eventFilter(obj, event);
}

void PwdDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Tab:
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}
