#include "XdagMutex.h"

XdagMutex::XdagMutex()
{

}

void XdagMutex::lock(const char* lockFile = __FILE__,int lockLine = __LINE__)
{
    this->lockFile = lockFile;
    this->lockLine = lockLine;

    QMutex::lock();
}

void XdagMutex::unlock(const char* unlockFile = __FILE__,int unlockLine = __LINE__)
{
    this->unlockFile = unlockFile;
    this->unlockLine = unlockLine;

    QMutex::unlock();
}
