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
#include "qxtfilelock.h"
#include "qxtfilelock_p.h"
/**
 * \class QxtFileLock QxtFileLock
 * \ingroup QxtCore
 * \brief The QxtFileLock class provides a crossplattform way to lock a QFile.
 *
 * It supports the range locking of a file. The File will take parentship of the lock.<br>
 * The lock gets cleaned up with the QFile, and it is released when the QFile is closed.<br>
 *
 * Example usage:
 * \code
 * off_t lockstart = 0x10;
 * off_t locklength = 30
 *
 * QFile file("test.lock");
 *
 * //the lock gets deleted when file is cleaned up
 * QxtFileLock * writeLock = new QxtFileLock(&file,lockstart,locklength,QxtFileLock::WriteLock);
 * if(file.open(QIODevice::ReadWrite))
 * {
 *     if(writeLock->lock())
 *     {
 *          // some write operations
 *         writeLock->unlock();
 *     }
 *      else
 *          //lock failed
 * }
 * \endcode
 * \note QxtFileLock behaves different than normal unix locks on *nix. A thread can writelock the region of a file only ONCE if it uses two  different handles.
 *           A different thread can not writelock a region that is owned by a other thread even if it is the SAME process.
 * \note On *nix this class uses fctnl to lock the file. This may not be compatible to other locking functions like flock and lockf
 * \note Please do not mix QxtFileLock and native file lock calls on the same QFile. The behaviour is undefined
 * \note QxtFileLock lives in the same thread as the passed QFile
 * \warning due to a refactoring issues of QFile this class will not work with Qt from 4.3 on. This will be fixed in 4.3.2
 * \warning not part of 0.2.4


*/

/**
 * @enum QxtFileLock::mode
 * @brief The Mode of the lock
 */

/**
 * @var QxtFileLock::mode QxtFileLock::ReadLock
 * @brief A non blocking read lock
 */

/**
 * @var QxtFileLock::mode QxtFileLock::WriteLock
 * @brief A non blocking write lock
 */

/**
 * @var QxtFileLock::mode QxtFileLock::ReadLockWait
 * @brief A  blocking read lock. The lock() function will block until the lock is created.
 */

/**
 * @var QxtFileLock::mode QxtFileLock::WriteLockWait
 * @brief A blocking write lock. The lock() function will block until the lock is created.
 */

QxtFileLockPrivate::QxtFileLockPrivate()  : offset(0), length(0), mode(QxtFileLock::WriteLockWait), isLocked(false)
{
}

/**
 * Contructs a new QxtFileLock. The lock is not activated.
 * @param file the file that should be locked
 * @param offset the offset where the lock starts
 * @param length the length of the lock
 * @param mode the lockmode
 */
QxtFileLock::QxtFileLock(QFile *file,const off_t offset,const off_t length,const QxtFileLock::Mode mode) : QObject(file)
{
    QXT_INIT_PRIVATE(QxtFileLock);
    connect(file,SIGNAL(aboutToClose()),this,SLOT(unlock()));
    qxt_d().offset = offset;
    qxt_d().length = length;
    qxt_d().mode = mode;
}

QxtFileLock::~QxtFileLock()
{
    unlock();
}

/**
 *@return the offset of the lock
 */
off_t QxtFileLock::offset() const
{
    return qxt_d().offset;
}

/**
 * @return true if the lock is active otherwise it returns false
 */
bool QxtFileLock::isActive() const
{
    return qxt_d().isLocked;
}

/**
 * @return the length of the lock
 */
off_t QxtFileLock::length() const
{
    return qxt_d().length;
}

/**
 * the file the lock is created on
 */
QFile * QxtFileLock::file() const
{
    return qobject_cast<QFile *>(parent());
}

/**
 * @return the mode of the lock
 */
QxtFileLock::Mode QxtFileLock::mode() const
{
    return qxt_d().mode;
}
