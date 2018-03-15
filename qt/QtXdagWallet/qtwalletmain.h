#ifndef QTWALLETMAIN_H
#define QTWALLETMAIN_H

#include "PwdDialog.h"
#include "XdagWalletProcessThread.h"
#include "xdagcommondefine.h"

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QAction>
#include <QMenu>
#include <QTranslator>

namespace Ui {
class QtWalletMain;
}

class QtWalletMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtWalletMain(QWidget *parent = 0);
    ~QtWalletMain();

private:
    Ui::QtWalletMain *ui;

    void initUI();
    void translateUI(XdagCommonDefine::EN_XDAG_UI_LANG lang);
    void initWorkThread();
    void initSignal();

    QLabel *m_pLBPool;
    QLineEdit *m_pLEPool;
    QPushButton *m_pPBConnect;
    QHBoxLayout *m_pHBLPool;

    QLabel *m_pLBBalance;
    QLineEdit *m_pLEBalance;
    QLabel *m_pLBAccount;
    QLineEdit *m_pLEAccount;
    QHBoxLayout *m_pHBLAccount;

    QLabel *m_pLBTransfer;
    QLineEdit *m_pLESendAmount;
    QLabel *m_pLBTo;
    QLineEdit *m_pLERecvAddress;
    QPushButton *m_pPBXfer;
    QHBoxLayout *m_pHBLTransfer;

    //Global VBoxLayout
    QVBoxLayout *m_pVBLGlobal;

    //Language
    QMenu *m_pQMLanguage;
    QAction *m_pQAEnglish;
    QAction *m_pQAChinese;
    QAction *m_pQARussian;
    QAction *m_pQAFrench;
    QAction *m_pQAGermany;
    QAction *m_pQAJanpanese;
    QAction *m_pQAKorean;

    //work threads
    XdagWalletProcessThread *m_pXdagWalletProcessThread;

    //password input dialog
    PwdDialog *m_pDLPwdType;
    PwdDialog *m_pDLPwdReType;
    PwdDialog *m_pDLRdmType;

    //translator
    QTranslator *m_pTranslator;

    XdagCommonDefine::EN_XDAG_UI_LANG mLanguage;

    void InitWalletUpdateUI(UpdateUiInfo info);
private slots:
    void onButtonGoClicked();
    void onButtonXferClicked();
    void onChangeLanguage(QAction *);
    void onPwdTyped(QString pwd);
    void onPwdSeted(QString pwd);
    void onPwdReTyped(QString pwd);
    void onRdmTyped(QString rdm);
};

#endif // QTWALLETMAIN_H
