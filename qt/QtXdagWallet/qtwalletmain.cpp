#include "QtWalletMain.h"
#include "ui_qtwalletmain.h"
#include "UiNotifyMessage.h"
#include "UpdateUiInfo.h"

#include <QMessageBox>
#include <QDialog>
QWaitCondition g_condPwdTyped;
QWaitCondition g_condPwdSeted;
QWaitCondition g_condPwdReTyped;
QWaitCondition g_condRdmTyped;
QWaitCondition g_condUiNotified;
QMutex g_Mutex;
QMap<QString,QString> g_MsgMap;
QQueue<UiNotifyMessage> g_MsgQueue;

#define MAX_XDAG_ADDRESS_LEN 64
#define MAX_XDAG_AMOUNT_LEN 32
#define MAX_XDAG_POOL_ADDR_LEN 128

#define QLINE_EDIT_HEIGHT 25
#define POOL_LINEEDIT_LEN 360
#define AMOUNT_LINEEDIT_LEN 100
#define ADRESS_LINEEDIT_LEN 200
#define QLABEL_LEN 60
#define QLABLE_HEIGHT 25
#define QPUSHBUTTON_LEN     60
#define QPUSHBUTTON_HEIGHT  25


QtWalletMain::QtWalletMain(QWidget *parent) :
    QMainWindow(parent),
    m_pDLPwdType(NULL),m_pDLPwdReType(NULL),m_pDLRdmType(NULL),
    ui(new Ui::QtWalletMain)
{
    initUI();
    initWorkThread();
    initSignal();
}

QtWalletMain::~QtWalletMain()
{
    delete ui;
}

void QtWalletMain::initUI()
{
    ui->setupUi(this);

    setWindowTitle(tr("Dagger Wallet(XDAG)"));

    setWindowIcon(QIcon(":/icon/xdagwallet.ico"));
    m_pLBPool = new QLabel(tr("Pool"));
    m_pLEPool = new QLineEdit("cn.xdag.top:8899");
    m_pPBConnect = new QPushButton(tr("Connect"));

    m_pLBPool->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pLBPool->setFixedSize(QLABEL_LEN,QLABLE_HEIGHT);
    m_pLEPool->setFixedSize(MAX_XDAG_POOL_ADDR_LEN,QLABLE_HEIGHT);
    m_pLEPool->setFixedSize(POOL_LINEEDIT_LEN,QLINE_EDIT_HEIGHT);
    m_pPBConnect->setFixedSize(QPUSHBUTTON_LEN,QPUSHBUTTON_HEIGHT);
    m_pHBLPool = new QHBoxLayout;
    m_pHBLPool->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pHBLPool->setSpacing(0);
    m_pHBLPool->addWidget(m_pLBPool);
    m_pHBLPool->addWidget(m_pLEPool);
    m_pHBLPool->addWidget(m_pPBConnect);

    m_pLBBalance = new QLabel(tr("Balance"));
    m_pLEBalance = new QLineEdit(tr("Not Ready"));
    m_pLBAccount = new QLabel(tr("Account"));
    m_pLEAccount = new QLineEdit(tr("Not Ready"));

    m_pLBBalance->setAlignment(Qt::AlignLeft| Qt::AlignVCenter);
    m_pLBBalance->setFixedSize(QLABEL_LEN,QLABLE_HEIGHT);
    m_pLEBalance->setMaxLength(MAX_XDAG_AMOUNT_LEN);
    m_pLEAccount->setMaxLength(MAX_XDAG_ADDRESS_LEN);
    m_pLBAccount->setFixedSize(QLABEL_LEN,QLABLE_HEIGHT);
    m_pLBAccount->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLEBalance->setFixedSize(AMOUNT_LINEEDIT_LEN,QLINE_EDIT_HEIGHT);
    m_pLEAccount->setFixedSize(ADRESS_LINEEDIT_LEN,QLINE_EDIT_HEIGHT);
    m_pHBLAccount = new QHBoxLayout;
    m_pHBLAccount->setSpacing(0);
    m_pHBLAccount->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pHBLAccount->addWidget(m_pLBBalance);
    m_pHBLAccount->addWidget(m_pLEBalance);
    m_pHBLAccount->addWidget(m_pLBAccount);
    m_pHBLAccount->addWidget(m_pLEAccount);

    m_pLBTransfer = new QLabel(tr("Transfer"));
    m_pLESendAmount = new QLineEdit("");
    m_pLBTo = new QLabel(tr("To"));
    m_pLERecvAddress = new QLineEdit("");
    m_pPBXfer = new QPushButton(tr("Send"));

    m_pLBTransfer->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pLBTransfer->setFixedSize(QLABEL_LEN,QLABLE_HEIGHT);
    m_pLESendAmount->setMaxLength(MAX_XDAG_AMOUNT_LEN);
    m_pLERecvAddress->setMaxLength(MAX_XDAG_ADDRESS_LEN);
    m_pLBTo->setFixedSize(QLABEL_LEN,QLABLE_HEIGHT);
    m_pLBTo->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_pLESendAmount->setFixedSize(AMOUNT_LINEEDIT_LEN,QLINE_EDIT_HEIGHT);
    m_pLERecvAddress->setFixedSize(ADRESS_LINEEDIT_LEN,QLINE_EDIT_HEIGHT);
    m_pPBXfer->setFixedSize(QPUSHBUTTON_LEN,QPUSHBUTTON_HEIGHT);
    m_pHBLTransfer = new QHBoxLayout;
    m_pHBLTransfer->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pHBLTransfer->setSpacing(0);
    m_pHBLTransfer->addWidget(m_pLBTransfer);
    m_pHBLTransfer->addWidget(m_pLESendAmount);
    m_pHBLTransfer->addWidget(m_pLBTo);
    m_pHBLTransfer->addWidget(m_pLERecvAddress);
    m_pHBLTransfer->addWidget(m_pPBXfer);

    m_pVBLGlobal = new QVBoxLayout;
    m_pVBLGlobal->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pVBLGlobal->addLayout(m_pHBLPool);
    m_pVBLGlobal->addLayout(m_pHBLAccount);
    m_pVBLGlobal->addLayout(m_pHBLTransfer);

    ui->centralWidget->setLayout(m_pVBLGlobal);

    //language settings
    //default english
    mLanguage = XdagCommonDefine::EN_LANG_ENGLISH;

    m_pQMLanguage = new QMenu("Language");
    m_pQAEnglish = new QAction("English");
    m_pQAChinese = new QAction("中文");
    m_pQARussian = new QAction("язык");
    m_pQAFrench = new QAction("langue");
    m_pQAGermany = new QAction("sprache");
    m_pQAJanpanese = new QAction("Language");
    m_pQAKorean = new QAction("언어");

    m_pQMLanguage->addAction(m_pQAEnglish);
    m_pQMLanguage->addAction(m_pQAChinese);
    m_pQMLanguage->addAction(m_pQARussian);
    m_pQMLanguage->addAction(m_pQAFrench);
    m_pQMLanguage->addAction(m_pQAGermany);
    m_pQMLanguage->addAction(m_pQAJanpanese);
    m_pQMLanguage->addAction(m_pQAKorean);
    ui->menuBar->addMenu(m_pQMLanguage);

    m_pTranslator = new QTranslator;

    setFixedSize(500,160);
}

void QtWalletMain::translateUI(XdagCommonDefine::EN_XDAG_UI_LANG lang)
{
    QString qmFilePath;

    switch (lang) {
    case XdagCommonDefine::EN_LANG_ENGLISH:
        qmFilePath = ":/language/english.qm";
        break;
    case XdagCommonDefine::EN_LANG_CHINESE:
        qmFilePath = ":/language/chinese.qm";
        break;
    case XdagCommonDefine::EN_LANG_RUSSIAN:
        qmFilePath = ":/language/russian.qm";
        break;
    case XdagCommonDefine::EN_LANG_GERMAN:
        qmFilePath = ":/language/germany.qm";
        break;
    case XdagCommonDefine::EN_LANG_FRENCH:
        qmFilePath = ":/language/french.qm";
        break;
    case XdagCommonDefine::EN_LANG_JANPAN:
        qmFilePath = ":/language/japanese.qm";
        break;
    case XdagCommonDefine::EN_LANG_KOREAN:
        qmFilePath = ":/language/korean.qm";
        break;
    default:
        qmFilePath = ":/language/english.qm";
        break;
    }

    m_pTranslator->load(qmFilePath);

    if(qApp->installTranslator(m_pTranslator)){
        setWindowTitle(tr("Dagger Wallet(XDAG)"));
        m_pLBPool->setText(tr("Pool"));
        m_pPBConnect->setText(tr("Connect"));

        m_pLBBalance->setText(tr("Balance"));
        m_pLBAccount->setText(tr("Account"));

        m_pLBTransfer->setText(tr("Transfer"));
        m_pLBTo->setText(tr("To"));
        m_pPBXfer->setText(tr("Send"));
        m_pQMLanguage->setTitle(tr("Language"));
    }
}

void QtWalletMain::initWorkThread(){
    m_pXdagWalletProcessThread = new XdagWalletProcessThread(this);

    m_pXdagWalletProcessThread->setMutex(&g_Mutex);
    m_pXdagWalletProcessThread->setCondPwdTyped(&g_condPwdTyped);
    m_pXdagWalletProcessThread->setCondPwdSeted(&g_condPwdSeted);
    m_pXdagWalletProcessThread->setCondPwdReTyped(&g_condPwdReTyped);
    m_pXdagWalletProcessThread->setCondRdmTyped(&g_condRdmTyped);
    m_pXdagWalletProcessThread->setCondUiNotified(&g_condUiNotified);

    m_pXdagWalletProcessThread->setMsgMap(&g_MsgMap);
    m_pXdagWalletProcessThread->setMsgQueue(&g_MsgQueue);

}

void QtWalletMain::initSignal()
{
    //connect wallet init signal and slot
    connect(m_pPBConnect,&QPushButton::clicked,this,&QtWalletMain::onButtonGoClicked);
    connect(m_pPBXfer,&QPushButton::clicked,this,&QtWalletMain::onButtonXferClicked);

    //connect wallet init thread signal and ui slot
    connect(m_pXdagWalletProcessThread,&XdagWalletProcessThread::XdagWalletProcessSignal,this,&QtWalletMain::InitWalletUpdateUI);

    //change language
    connect(m_pQMLanguage,SIGNAL(triggered(QAction *)),this,SLOT(onChangeLanguage(QAction *)));

    qRegisterMetaType<UpdateUiInfo>();
    qRegisterMetaType<UiNotifyMessage>();
}

void QtWalletMain::onButtonGoClicked()
{
    m_pXdagWalletProcessThread->setPoolAddr(m_pLEPool->text().toStdString().c_str());
    //init wallet use wallet init thread
    if(!m_pXdagWalletProcessThread->isRunning()){
        m_pXdagWalletProcessThread->start();
    }
}

void QtWalletMain::onButtonXferClicked()
{
    //do xfer coin notify user connect the pool first
    if(!m_pXdagWalletProcessThread->isRunning()){
        m_pXdagWalletProcessThread->start();
    }

    UiNotifyMessage msg;
    msg.msgType = UiNotifyMessage::EN_DO_XFER_XDAG;
    msg.account = strdup(m_pLEAccount->text().toStdString().c_str());
    msg.amount = strdup(m_pLESendAmount->text().toStdString().c_str());
    msg.msgFromThreadId = QThread::currentThreadId();

    g_MsgQueue.push_back(msg);
    g_condUiNotified.wakeAll();
}

void QtWalletMain::onChangeLanguage(QAction *action)
{
    XdagCommonDefine::EN_XDAG_UI_LANG lang;

    if(action == m_pQAChinese){
        lang = XdagCommonDefine::EN_LANG_CHINESE;
    }else if(action == m_pQAEnglish){
        lang = XdagCommonDefine::EN_LANG_ENGLISH;
    }else if(action == m_pQARussian){
        lang = XdagCommonDefine::EN_LANG_RUSSIAN;
    }else if(action == m_pQAGermany){
        lang = XdagCommonDefine::EN_LANG_GERMAN;
    }else if(action == m_pQAFrench){
        lang = XdagCommonDefine::EN_LANG_FRENCH;
    }else if(action == m_pQAJanpanese){
        lang = XdagCommonDefine::EN_LANG_JANPAN;
    }else if(action == m_pQAKorean){
        lang = XdagCommonDefine::EN_LANG_KOREAN;
    }else{
        lang = XdagCommonDefine::EN_LANG_ENGLISH;
    }

    translateUI(lang);
}


void QtWalletMain::onPwdTyped(QString pwd)
{
    qDebug() << "QtWalletMain on pwd-typed run thread id: " << QThread::currentThreadId();

    g_MsgMap.insert("type-passwd",pwd);
    if(m_pDLPwdType){
        m_pDLPwdType->closeDialog();
        m_pDLPwdType = NULL;
    }
    m_pXdagWalletProcessThread->wakePasswdTyped();
}

void QtWalletMain::onPwdSeted(QString pwd)
{
    qDebug() << "QtWalletMain on pwd seted run thread id: " << QThread::currentThreadId();

    g_MsgMap.insert("set-passwd",pwd);
    if(m_pDLPwdType){
        m_pDLPwdType->closeDialog();
        m_pDLPwdType = NULL;
    }
    m_pXdagWalletProcessThread->wakePasswdSeted();
}

void QtWalletMain::onPwdReTyped(QString pwd)
{
    qDebug() << "QtWalletMain on pwd typed run thread id: " << QThread::currentThreadId();

    g_MsgMap.insert("retype-passwd",pwd);
    if(m_pDLPwdType){
        m_pDLPwdType->closeDialog();
        m_pDLPwdType = NULL;
    }
    m_pXdagWalletProcessThread->wakePasswdRetyped();
}

void QtWalletMain::onRdmTyped(QString pwd)
{
    qDebug() << "QtWalletMain on pwd re-typed run thread id: " << QThread::currentThreadId();

    g_MsgMap.insert("type-rdm",pwd);
    if(m_pDLPwdType){
        m_pDLPwdType->closeDialog();
        m_pDLPwdType = NULL;
    }
    m_pXdagWalletProcessThread->wakeRdmTyped();

}

void QtWalletMain::InitWalletUpdateUI(UpdateUiInfo info){

    switch(info.event_type){

        case en_event_type_pwd:
            m_pDLPwdType = new PwdDialog(0,DLG_TYPE_PWD);
            connect(m_pDLPwdType,SIGNAL(sendTypePwd(QString)),this,SLOT(onPwdTyped(QString)));
            m_pDLPwdType->show();
        break;

        case en_event_set_pwd:
            m_pDLPwdType = new PwdDialog(0,DLG_SET_PWD);
            connect(m_pDLPwdType,SIGNAL(sendSetPwd(QString)),this,SLOT(onPwdSeted(QString)));
            m_pDLPwdType->show();
        break;

        case en_event_retype_pwd:
            m_pDLPwdType = new PwdDialog(0,DLG_RETYPE_PWD);
            connect(m_pDLPwdType,SIGNAL(sendRetypePwd(QString)),this,SLOT(onPwdReTyped(QString)));
            m_pDLPwdType->show();
        break;

        case en_event_set_rdm:
            m_pDLPwdType = new PwdDialog(0,DLG_TYPE_RDM);
            connect(m_pDLPwdType,SIGNAL(sendRdm(QString)),this,SLOT(onRdmTyped(QString)));
            m_pDLPwdType->show();
        break;

        //update ui info
        case en_event_update_state:
            m_pLEAccount->clear();
            m_pLEBalance->clear();
            ui->statusBar->clearMessage();
            m_pLEAccount->setText(info.address);
            m_pLEBalance->setText(info.balance);
            ui->statusBar->showMessage(info.state);
        break;
    }
}




