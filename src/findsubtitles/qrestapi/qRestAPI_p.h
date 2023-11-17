/*==============================================================================

  Library: qRestAPI

  Copyright (c) 2010 Kitware Inc.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qRestAPI_p_h
#define __qRestAPI_p_h

// Qt includes
#include <QFile>
#if QT_VERSION >= 0x050000
#include <QHash>
#endif
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>

// qRestAPI includes
#include "qRestAPI.h"

class QIODevice;

#if (QT_VERSION < QT_VERSION_CHECK(5, 3, 0))
#ifdef QT_NO_OPENSSL
#define QRESTAPI_QT_NO_SSL
#endif
#else
#ifdef QT_NO_SSL
#define QRESTAPI_QT_NO_SSL
#endif
#endif

#ifdef QRESTAPI_QT_NO_SSL
struct QSslError{};
#endif

// --------------------------------------------------------------------------
class qRestAPIPrivate : public QObject
{
  Q_OBJECT

  Q_DECLARE_PUBLIC(qRestAPI);

  qRestAPI* const q_ptr;

private:
  static struct StaticInit
  {
    StaticInit()
    {
      qRestAPIPrivate::staticInit();
    }
  } _staticInit;

  static void staticInit();

public:
  qRestAPIPrivate(qRestAPI* object);
  ~qRestAPIPrivate();

  virtual void init();

public slots:
  void processReply(QNetworkReply* reply);
  /// Called when a query hasn't had any progress for a given TimeOut time.
  /// Note: sender() is used.
  void queryTimeOut();
  void queryProgress(qint64 bytesReceived, qint64 bytesTotal);
  void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
  void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

  void onSslErrors(QNetworkReply* reply, const QList<QSslError>& errors);

//  void onAuthenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator);

public:
  QString ServerUrl;

  QNetworkAccessManager* NetworkManager;
  int TimeOut;
  qRestAPI::RawHeaders DefaultRawHeaders;
  bool SuppressSslErrors;

  qRestAPI::ErrorType ErrorCode;
  QString ErrorString;

// In Qt5 QHash should be used. QUuid does not have a hash function in Qt4,
// so the QUuid's would be converted to QString what is expensive.
#if QT_VERSION >= 0x050000
  QHash<QUuid, qRestResult*> results;
#else
  QMap<QUuid, qRestResult*> results;
#endif
};

#endif
