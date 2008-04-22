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
/**
\class QxtPimpl QxtPimpl
\ingroup QxtCore
\brief Hide private details of a class

Application code generally doesn't have to be concerned about hiding its
implementation details, but when writing library code it is important to
maintain a constant interface, both source and binary. Maintaining a constant
source interface is easy enough, but keeping the binary interface constant
means moving implementation details into a private class. The PIMPL, or
d-pointer, idiom is a common method of implementing this separation. QxtPimpl
offers a convenient way to connect the public and private sides of your class.

\section start Getting Started
Before you declare the public class, you need to make a forward declaration
of the private class. The private class must have the same name as the public
class, followed by the word Private. For example, a class named MyTest would
declare the private class with:
\code
class MyTestPrivate;
\endcode

\subsection pub The Public Class
Generally, you shouldn't keep any data members in the public class without a
good reason. Functions that are part of the public interface should be declared
in the public class, and functions that need to be available to subclasses (for
calling or overriding) should be in the protected section of the public class.
To connect the private class to the public class, include the
QXT_DECLARE_PRIVATE macro in the private section of the public class. In the
example above, the private class is connected as follows:
\code
private:
    QXT_DECLARE_PRIVATE(MyTest);
\endcode

Additionally, you must include the QXT_INIT_PRIVATE macro in the public class's
constructor. Continuing with the MyTest example, your constructor might look
like this:
\code
MyTest::MyTest() {
    // initialization
    QXT_INIT_PRIVATE(MyTest);
}
\endcode

\subsection priv The Private Class
As mentioned above, data members should usually be kept in the private class.
This allows the memory layout of the private class to change without breaking
binary compatibility for the public class. Functions that exist only as
implementation details, or functions that need access to private data members,
should be implemented here.

To define the private class, inherit from the template QxtPrivate class, and
include the QXT_DECLARE_PUBLIC macro in its public section. The template
parameter should be the name of the public class. For example:
\code
class MyTestPrivate : public QxtPrivate<MyTest> {
public:
    MyTestPrivate();
    QXT_DECLARE_PUBLIC(MyTest);
};
\endcode

\section cross Accessing Private Members
Use the qxt_d() function (actually a function-like object) from functions in
the public class to access the private class. Similarly, functions in the
private class can invoke functions in the public class by using the qxt_p()
function (this one's actually a function).

For example, assume that MyTest has methods named getFoobar and doBaz(),
and MyTestPrivate has a member named foobar and a method named doQuux().
The code might resemble this example:
\code
int MyTest::getFoobar() {
    return qxt_d().foobar;
}

void MyTestPrivate::doQuux() {
    qxt_p().doBaz(foobar);
}
\endcode
*/

#ifndef QXTPIMPL_H
#define QXTPIMPL_H

/*! \relates QxtPimpl
 * Declares that a public class has a related private class.
 *
 * This shuold be put in the private section of the public class. The parameter is the name of the public class.
 */
#define QXT_DECLARE_PRIVATE(PUB) friend class PUB##Private; QxtPrivateInterface<PUB, PUB##Private> qxt_d;
/*! \relates QxtPimpl
 * Declares that a private class has a related public class.
 *
 * This may be put anywhere in the declaration of the private class. The parameter is the name of the public class.
 */
#define QXT_DECLARE_PUBLIC(PUB) friend class PUB;
/*! \relates QxtPimpl
 * Initializes resources owned by the private class.
 *
 * This should be called from the public class's constructor,
 * before qxt_d() is used for the first time. The parameter is the name of the public class.
 */
#define QXT_INIT_PRIVATE(PUB) qxt_d.setPublic(this);

#ifdef QXT_DOXYGEN_RUN
/*! \relates QxtPimpl
 * Returns a reference to the private class.
 *
 * This function is only available in a class using \a QXT_DECLARE_PRIVATE.
 */
QxtPrivate<PUB>& qxt_d();

/*! \relates QxtPimpl
 * Returns a const reference to the private class.
 *
 * This function is only available in a class using \a QXT_DECLARE_PRIVATE.
 * This overload will be automatically used in const functions.
 */
const QxtPrivate<PUB>& qxt_d();

/*! \relates QxtPimpl
 * Returns a reference to the public class.
 *
 * This function is only available in a class using \a QXT_DECLARE_PUBLIC.
 */
PUB& qxt_p();

/*! \relates QxtPimpl
 * Returns a const reference to the public class.
 *
 * This function is only available in a class using \a QXT_DECLARE_PUBLIC.
 * This overload will be automatically used in const functions.
 */
const PUB& qxt_p();
#endif

#ifndef QXT_DOXYGEN_RUN
template <typename PUB>
class QxtPrivate
{
public:
    virtual ~QxtPrivate()
    {}
    inline void QXT_setPublic(PUB* pub)
    {
        qxt_p_ptr = pub;
    }

protected:
    inline PUB& qxt_p()
    {
        return *qxt_p_ptr;
    }
    inline const PUB& qxt_p() const
    {
        return *qxt_p_ptr;
    }

private:
    PUB* qxt_p_ptr;
};

template <typename PUB, typename PVT>
class QxtPrivateInterface
{
    friend class QxtPrivate<PUB>;
public:
    QxtPrivateInterface()
    {
        pvt = new PVT;
    }
    ~QxtPrivateInterface()
    {
        delete pvt;
    }

    inline void setPublic(PUB* pub)
    {
        pvt->QXT_setPublic(pub);
    }
    inline PVT& operator()()
    {
        return *static_cast<PVT*>(pvt);
    }
    inline const PVT& operator()() const
    {
        return *static_cast<PVT*>(pvt);
    }
private:
    QxtPrivate<PUB>* pvt;
};
#endif

#endif
