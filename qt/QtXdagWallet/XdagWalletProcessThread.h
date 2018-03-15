#ifndef WALLETINITTHREAD_H
#define WALLETINITTHREAD_H

#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>

#include "../../xdaglib/wrapper/qtwrapper.h"
#include "../../xdaglib/client/xdagmain.h"
#include "UiNotifyMessage.h"
#include "UpdateUiInfo.h"

static QObject *gCallBackObject;

class XdagWalletProcessThread : public QThread
{
    Q_OBJECT

public:
    explicit XdagWalletProcessThread(QObject *parent);
    ~XdagWalletProcessThread();

    void setPoolAddr(const char* poolAddr);
    const char* getPoolAddr(void);

    void setMutex(QMutex *mutex);
    QMutex* getMutex(void);

    void setMsgMap(QMap<QString,QString> *map);
    QMap<QString,QString>* getMsgMap();

    void setCondPwdTyped(QWaitCondition *cond);
    void setCondPwdSeted(QWaitCondition *cond);
    void setCondPwdReTyped(QWaitCondition *cond);
    void setCondRdmTyped(QWaitCondition *cond);
    void setCondUiNotified(QWaitCondition *cond);

    QWaitCondition* getCondPwdTyped(void);
    QWaitCondition* getCondPwdSeted(void);
    QWaitCondition* getCondPwdReTyped(void);
    QWaitCondition* getCondRdmTyped(void);
    QWaitCondition* getCondUiNotified(void);

    void setMsgQueue(QQueue<UiNotifyMessage> *msgQueue);

    //emit ui signal
    void emitUISignal(UpdateUiInfo info);

    //for xdag library call back
    static st_xdag_app_msg* XdagWalletProcessCallback(const void *call_back_object,st_xdag_event *event);

    //process ui notify message
    void processUiNotifyMessage(UiNotifyMessage &msg);

    //stop the thread
    void stop();

    void waitPasswdTyped();
    void wakePasswdTyped();
    void waitPasswdSeted();
    void wakePasswdSeted();
    void waitPasswdRetyped();
    void wakePasswdRetyped();
    void waitRdmTyped();
    void wakeRdmTyped();

protected:
    virtual void run();

signals:
    //emit wallet process signal
    void XdagWalletProcessSignal(UpdateUiInfo info);

private:
    //Mutex Lock
    QMutex *m_pMutex;
    //Condition wait password typed
    QWaitCondition *m_pCondPwdTyped;
    //Condition wait password seted
    QWaitCondition *m_pCondPwdSeted;
    //Condition wait password re-typed
    QWaitCondition *m_pCondPwdReTyped;
    //Condition wait password re-typed
    QWaitCondition *m_pCondRdmTyped;
    //Condition wait ui notify
    QWaitCondition *m_pUiNotified;

    //Msg Map
    QMap<QString,QString> *m_pMsgMap;

    //Msg queue in main loop
    QQueue<UiNotifyMessage> *m_pMsgQueue;

    //flag is need stop
    bool m_bNeedStopped;

    //pool addr
    QString mPoolAddr;
};

#endif // WALLETINITTHREAD_H
