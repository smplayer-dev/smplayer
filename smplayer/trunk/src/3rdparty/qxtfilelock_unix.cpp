/****************************************************************************
**
** Copyright (C) Qxt Foundation. Some rights reserved.
**
** This file is part of the QxtCore module of the Qt eXTension library
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of th Common Public License, version 1.0, as published by
** IBM.
**
** This file is provided "AS IS", without WARRANTIES OR CONDITIONS OF ANY
** KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
** WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY OR
** FITNESS FOR A PARTICULAR PURPOSE.
**
** You should have received a copy of the CPL along with this file.
** See the LICENSE file and the cpl1.0.txt file included with the source
** distribution for more information. If you did not receive a copy of the
** license, contact the Qxt Foundation.
**
** <http://libqxt.sourceforge.net>  <foundation@libqxt.org>
**
****************************************************************************/
#ifndef WIN32

#include "qxtfilelock.h"
#include "qxtfilelock_p.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
/*include pthread to make errno threadsafe*/
#include <pthread.h>
#include <errno.h>
#include <QPointer>
#include <QMutableLinkedListIterator>
#include <QDebug>

/*!
 * \internal this class is used on *nix to register all locks created by a process and to let locks on *nix act like locks on windows
 */
class QxtFileLockRegistry
{
public:
    bool registerLock(QxtFileLock *lock );
    bool removeLock(QxtFileLock *lock );
    static QxtFileLockRegistry& instance();

private:
    QLinkedList < QPointer<QxtFileLock> > procLocks;
    QMutex registryMutex;
    QxtFileLockRegistry();
};

QxtFileLockRegistry::QxtFileLockRegistry()
{}

QxtFileLockRegistry& QxtFileLockRegistry::instance()
{
    static QxtFileLockRegistry instance;
    return instance;
}

/*!
 * \internal this function locks the lockregistry and checks if there is a collision between the process locks
 * \internal if there is no collision it inserts the lock into the registry and returns
 * \internal return true for success
 */
bool QxtFileLockRegistry::registerLock(QxtFileLock * lock )
{
    QMutexLocker locker(&this->registryMutex);

    QFile *fileToLock = lock ->file();

    if (fileToLock)
    {
        struct stat fileInfo;
        if ( fstat(fileToLock->handle(),&fileInfo) < 0 )
            return false;

        int newLockStart = lock ->offset();
        int newLockEnd = lock ->offset()+lock ->length();

        QMutableLinkedListIterator< QPointer<QxtFileLock> >iterator(this->procLocks);

        while (iterator.hasNext())
        {
            QPointer<QxtFileLock> currLock = iterator.next();
            if (currLock && currLock->file() && currLock->file()->isOpen())
            {
                struct stat currFileInfo;

                /*first check if the current lock is on the same file*/
                if ( fstat(currLock->file()->handle(),&currFileInfo) < 0 )
                {
                    /*that should never happen because a closing file should remove all locks*/
                    Q_ASSERT(false);
                    continue;
                }

                if (currFileInfo.st_dev == fileInfo.st_dev && currFileInfo.st_ino == fileInfo.st_ino)
                {
                    /*same file, check if our locks are in conflict*/
                    int currLockStart = currLock->offset();
                    int currLockEnd = currLock->offset()+currLock->length();

                    /*do we have to check for threads here?*/
                    if (newLockEnd >= currLockStart  && newLockStart <= currLockEnd)
                    {
                        qDebug()<<"we may have a collision";
                        qDebug()<<newLockEnd<<" >= "<<currLockStart<<"  &&  "<<newLockStart<<" <= "<<currLockEnd;

                        /*same lock region if one of both locks are exclusive we have a collision*/
                        if (lock ->mode() == QxtFileLock::WriteLockWait || lock ->mode() == QxtFileLock::WriteLock ||
                                    currLock->mode() == QxtFileLock::WriteLockWait || currLock->mode() == QxtFileLock::WriteLock)
                            {
                                qDebug()<<"Okay if this is not the same thread using the same handle there is a collision";
                                /*the same thread  can lock the same region with the same handle*/

                                qDebug()<<"! ("<<lock ->thread()<<" == "<<currLock->thread()<<" && "<<lock ->file()->handle()<<" == "<<currLock->file()->handle()<<")";

                                if (! (lock ->thread() == currLock->thread() && lock ->file()->handle() == currLock->file()->handle()))
                                    {
                                        qDebug()<<"Collision";
                                        return false;
                                    }
                            }
                    }
                }
            }
            else //remove dead locks
                iterator.remove();
        }
        qDebug()<<"The lock is okay";
        /*here we can insert the lock into the list and return*/
        procLocks.append(QPointer<QxtFileLock>(lock ));
        return true;

    }

    return false;
}

bool QxtFileLockRegistry::removeLock(QxtFileLock * lock )
{
    QMutexLocker locker(&this->registryMutex);
    procLocks.removeAll(lock );
    return true;
}

bool QxtFileLock::unlock()
{
    if (file() && file()->isOpen() && isActive())
    {
        /*first remove real lock*/
        int lockmode,  locktype;
        int result = -1;
        struct  flock lockDesc;

        lockmode = F_SETLK;
        locktype = F_UNLCK;

        errno = 0;
        do
        {
            lockDesc.l_type = locktype;
            lockDesc.l_whence = SEEK_SET;
            lockDesc.l_start = qxt_d().offset;
            lockDesc.l_len = qxt_d().length;
            lockDesc.l_pid = 0;
            result = fcntl (this->file()->handle(), lockmode, &lockDesc);
        }
        while (result && errno == EINTR);

        QxtFileLockRegistry::instance().removeLock(this);
        qxt_d().isLocked = false;
        return true;
    }
    return false;
}

bool QxtFileLock::lock ()
{
    if (file() && file()->isOpen() && !isActive())
    {
        /*this has to block if we can get no lock*/

        bool locked = false;

        while (1)
        {
            locked = QxtFileLockRegistry::instance().registerLock(this);
            if (locked)
                break;
            else
            {
                if (qxt_d().mode == ReadLockWait || qxt_d().mode == WriteLockWait)
                    usleep(1000 * 5);
                else
                    return false;
            }
        }

        /*now get real lock*/
        int lockmode,
        locktype;

        int result = -1;

        struct  flock lockDesc;

        switch (qxt_d().mode)
        {
        case    ReadLock:
            lockmode = F_SETLK;
            locktype = F_RDLCK;
            break;

        case    ReadLockWait:
            lockmode = F_SETLKW;
            locktype = F_RDLCK;
            break;

        case    WriteLock:
            lockmode = F_SETLK;
            locktype = F_WRLCK;
            break;

        case    WriteLockWait:
            lockmode = F_SETLKW;
            locktype = F_WRLCK;
            break;

        default:
            QxtFileLockRegistry::instance().removeLock(this);
            return (false);
            break;
        }

        errno = 0;
        do
        {
            lockDesc.l_type = locktype;
            lockDesc.l_whence = SEEK_SET;
            lockDesc.l_start = qxt_d().offset;
            lockDesc.l_len = qxt_d().length;
            lockDesc.l_pid = 0;
            result = fcntl (this->file()->handle(), lockmode, &lockDesc);
        }
        while (result && errno == EINTR);

        /*we dot get the lock unregister from lockregistry and return*/
        if (result == -1)
        {
            QxtFileLockRegistry::instance().removeLock(this);
            return false;
        }

        qxt_d().isLocked = true;
        return true;
    }
    return false;
}
#endif

