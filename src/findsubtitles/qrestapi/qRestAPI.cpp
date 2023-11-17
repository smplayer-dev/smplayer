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

// Qt includes
#include <QDebug>
#include <QEventLoop>
#include <QIODevice>
#include <QSslSocket>
#include <QStringList>
#include <QTimer>
#include <QUuid>
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QJSValueIterator>
#include <QUrlQuery>
#else
#include <QScriptValueIterator>
#endif

// qRestAPI includes
#include "qRestAPI.h"
#include "qRestAPI_p.h"

#include "qRestResult.h"

// --------------------------------------------------------------------------
// Static file local error messages
static QString unknownErrorStr = "Unknown error";
static QString unknownUuidStr = "Unknown uuid %1";
static QString timeoutErrorStr = "Request timed out";

// --------------------------------------------------------------------------
// qRestAPIPrivate methods

qRestAPIPrivate::StaticInit qRestAPIPrivate::_staticInit;

// --------------------------------------------------------------------------
qRestAPIPrivate::qRestAPIPrivate(qRestAPI* object)
  : q_ptr(object)
  , NetworkManager(NULL)
  , TimeOut(0)
  , SuppressSslErrors(true)
  , ErrorCode(qRestAPI::UnknownError)
  , ErrorString(unknownErrorStr)
{
}

// --------------------------------------------------------------------------
qRestAPIPrivate::~qRestAPIPrivate()
{
  NetworkManager->deleteLater();
}

// --------------------------------------------------------------------------
void qRestAPIPrivate::staticInit()
{
  qRegisterMetaType<QUuid>("QUuid");
  qRegisterMetaType<QList<QVariantMap> >("QList<QVariantMap>");
}

// --------------------------------------------------------------------------
void qRestAPIPrivate::init()
{
  this->NetworkManager = new QNetworkAccessManager();
  QObject::connect(this->NetworkManager, SIGNAL(finished(QNetworkReply*)),
                   this, SLOT(processReply(QNetworkReply*)));
#ifndef QRESTAPI_QT_NO_SSL
  if (QSslSocket::supportsSsl())
    {
    QObject::connect(this->NetworkManager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
          this, SLOT(onSslErrors(QNetworkReply*, QList<QSslError>)));
    }
#endif
}

// --------------------------------------------------------------------------
void qRestAPI::setHttpNetworkProxy(const QNetworkProxy &proxy)
{
  Q_D(qRestAPI);

  if (d->NetworkManager != NULL)
    {
    d->NetworkManager->setProxy(proxy);
    }
}

// --------------------------------------------------------------------------
QNetworkReply* qRestAPI::sendRequest(QNetworkAccessManager::Operation operation,
    const QUrl& url,
    const qRestAPI::RawHeaders& rawHeaders,
    const QByteArray &data)
{
  Q_D(qRestAPI);
  QNetworkRequest queryRequest;
  queryRequest.setUrl(url);

  for (QMapIterator<QByteArray, QByteArray> it(d->DefaultRawHeaders); it.hasNext();)
    {
    it.next();
    queryRequest.setRawHeader(it.key(), it.value());
    }

  for (QMapIterator<QByteArray, QByteArray> it(rawHeaders); it.hasNext();)
    {
    it.next();
    queryRequest.setRawHeader(it.key(), it.value());
    }

  QNetworkReply* queryReply;
  switch (operation)
    {
    case QNetworkAccessManager::GetOperation:
      queryReply = d->NetworkManager->get(queryRequest);
      break;
    case QNetworkAccessManager::DeleteOperation:
      queryReply = d->NetworkManager->deleteResource(queryRequest);
      break;
    case QNetworkAccessManager::PutOperation:
      queryReply = d->NetworkManager->put(queryRequest, data);
      break;
    case QNetworkAccessManager::PostOperation:
      queryReply = d->NetworkManager->post(queryRequest, data);
      break;
    case QNetworkAccessManager::HeadOperation:
      queryReply = d->NetworkManager->head(queryRequest);
      break;
    default:
      // TODO
      return 0;
    }

  if (d->TimeOut > 0)
    {
    QTimer* timeOut = new QTimer(queryReply);
    timeOut->setSingleShot(true);
    QObject::connect(timeOut, SIGNAL(timeout()),
                     d, SLOT(queryTimeOut()));
    timeOut->start(d->TimeOut);
    }

  QUuid queryId = QUuid::createUuid();
  queryReply->setProperty("uuid", queryId.toString());

  qRestResult* result = new qRestResult(queryId);
  d->results[queryId] = result;
//  QObject::connect(this, SIGNAL(resultReceived(QUuid,QList<QVariantMap>)),
//                   result, SLOT(setResult(QList<QVariantMap>)));
//  QObject::connect(this, SIGNAL(errorReceived(QUuid,QString)),
//                   result, SLOT(setError(QString)));

  return queryReply;
}

// --------------------------------------------------------------------------
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
QVariantMap qRestAPI::scriptValueToMap(const QJSValue& value)
#else
QVariantMap qRestAPI::scriptValueToMap(const QScriptValue& value)
#endif
{
#if QT_VERSION >= 0x040700
  return value.toVariant().toMap();
#else
  QVariantMap result;
  for (QScriptValueIterator it(value); it.hasNext();)
    {
    it.next();
    result.insert(it.name(), it.value().toVariant());
    }
  return result;
#endif
}

// --------------------------------------------------------------------------
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
void qRestAPI::appendScriptValueToVariantMapList(QList<QVariantMap>& result, const QJSValue& data)
#else
void qRestAPI::appendScriptValueToVariantMapList(QList<QVariantMap>& result, const QScriptValue& data)
#endif
{
  QVariantMap map = scriptValueToMap(data);
  if (!map.isEmpty())
    {
    result << scriptValueToMap(data);
    }
}

// --------------------------------------------------------------------------
QVariantMap qRestAPI::qVariantMapFlattened(const QVariantMap& map)
{
  QVariantMap output;
  foreach(const QString& key, map.keys())
    {
    QVariant value = map.value(key);
    if (value.canConvert<QVariantMap>())
      {
      value = QVariant::fromValue(QVariantList() << value.toMap());
      }
    if (value.canConvert<QVariantList>())
      {
      foreach(const QVariant& item, value.toList())
        {
        QVariantMap subMap = qRestAPI::qVariantMapFlattened(item.toMap());
        foreach(const QString& subKey, subMap.keys())
          {
          output.insert(QString("%1.%2").arg(key).arg(subKey), subMap.value(subKey));
          }
        }
      }
    else
      {
      output.insert(key, value);
      }
    }
  return output;
}

// --------------------------------------------------------------------------
void qRestAPIPrivate::processReply(QNetworkReply* reply)
{
  Q_Q(qRestAPI);
  QUuid queryId(reply->property("uuid").toString());

  qRestResult* restResult = results[queryId];
  Q_ASSERT(restResult);

  if (reply->error() != QNetworkReply::NoError)
    {
    qRestAPI::ErrorType errorCode = qRestAPI::NetworkError;
    switch (reply->error())
      {
    case QNetworkReply::TimeoutError:
      errorCode = qRestAPI::TimeoutError;
      break;
    case QNetworkReply::SslHandshakeFailedError:
      errorCode = qRestAPI::SslError;
      break;
    case QNetworkReply::AuthenticationRequiredError:
      errorCode = qRestAPI::AuthenticationError;
      break;
    default:
      ;
      }
    restResult->setError(queryId.toString() + ": " +
                         QString::number(static_cast<int>(reply->error())) + ": " +
                         reply->errorString(),
                         errorCode);
    }
  else
    {
    restResult->Reponse = reply->readAll();
    q->parseResponse(restResult, restResult->response());
    }

  #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    foreach(const QNetworkReply::RawHeaderPair& rawHeaderPair, reply->rawHeaderPairs())
      {
      restResult->setRawHeader(rawHeaderPair.first, rawHeaderPair.second);
      }
  #else
    foreach(const QByteArray& headerName, reply->rawHeaderList())
      {
      restResult->setRawHeader(headerName, reply->rawHeader(headerName));
      }
  #endif

  reply->close();
  reply->deleteLater();

  q->emit finished(queryId);
}

void qRestAPIPrivate::onSslErrors(QNetworkReply* reply, const QList<QSslError>& errors)
{
#ifdef QRESTAPI_QT_NO_SSL
  Q_UNUSED(reply)
  Q_UNUSED(errors)
#else
  if (!this->SuppressSslErrors)
    {
    QString errorString;
    foreach (const QSslError& error, errors)
      {
      if (!errorString.isEmpty())
        {
        errorString.append(", ");
        }
      errorString.append(error.errorString());
      }

    QString plural(errors.empty() ? " has" : "s have");
    QString error = QString("SSL error%1 occurred: %2").arg(plural).arg(errorString);

    QUuid queryId(reply->property("uuid").toString());
    qRestResult* restResult = results[queryId];

    restResult->setError(error, qRestAPI::SslError);
    }
  else
    {
    reply->ignoreSslErrors();
    }
#endif
}

// --------------------------------------------------------------------------
void qRestAPIPrivate::queryProgress(qint64 bytesTransmitted, qint64 bytesTotal)
{
  Q_UNUSED(bytesTransmitted);
  Q_UNUSED(bytesTotal);
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(this->sender());
  // We received some progress so we postpone the timeout if any.
  QTimer* timer = reply->findChild<QTimer*>();
  if (timer)
    {
    timer->start();
    }
}

// --------------------------------------------------------------------------
void qRestAPIPrivate::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
  Q_Q(qRestAPI);
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(this->sender());
  QUuid queryId(reply->property("uuid").toString());
  double progress = static_cast<double>(bytesReceived) / bytesTotal;
  q->emit progress(queryId, progress);
}

// --------------------------------------------------------------------------
void qRestAPIPrivate::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
  Q_Q(qRestAPI);
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(this->sender());
  Q_ASSERT(reply);
  if (!reply)
    {
    return;
    }
  QUuid queryId(reply->property("uuid").toString());
  double progress = static_cast<double>(bytesSent) / bytesTotal;
  q->emit progress(queryId, progress);
}

// --------------------------------------------------------------------------
void qRestAPIPrivate::queryTimeOut()
{
  QTimer* timer = qobject_cast<QTimer*>(this->sender());
  Q_ASSERT(timer);
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(timer->parent());
  Q_ASSERT(reply);
  reply->abort();
  //reply->setError(QNetworkReply::TimeoutError,
  //   q->tr("Time out: No progress for %1 seconds.").arg(timer->interval()));
}

// --------------------------------------------------------------------------
// qRestAPI methods

// --------------------------------------------------------------------------
qRestAPI::qRestAPI(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qRestAPIPrivate(this))
{
  Q_D(qRestAPI);
  d->init();
}

// --------------------------------------------------------------------------
qRestAPI::~qRestAPI()
{
}

// --------------------------------------------------------------------------
QString qRestAPI::serverUrl()const
{
  Q_D(const qRestAPI);
  return d->ServerUrl;
}

// --------------------------------------------------------------------------
void qRestAPI::setServerUrl(const QString& serverUrl)
{
  Q_D(qRestAPI);
  d->ServerUrl = serverUrl;
}

// --------------------------------------------------------------------------
int qRestAPI::timeOut()const
{
  Q_D(const qRestAPI);
  return d->TimeOut;
}

// --------------------------------------------------------------------------
void qRestAPI::setTimeOut(int msecs)
{
  Q_D(qRestAPI);
  d->TimeOut = msecs;
}

// --------------------------------------------------------------------------
qRestAPI::RawHeaders qRestAPI::defaultRawHeaders()const
{
  Q_D(const qRestAPI);
  return d->DefaultRawHeaders;
}

// --------------------------------------------------------------------------
void qRestAPI::setDefaultRawHeaders(const qRestAPI::RawHeaders& defaultRawHeaders)
{
  Q_D(qRestAPI);
  d->DefaultRawHeaders = defaultRawHeaders;
}

// --------------------------------------------------------------------------
bool qRestAPI::suppressSslErrors()const
{
  Q_D(const qRestAPI);
  return d->SuppressSslErrors;
}

// --------------------------------------------------------------------------
void qRestAPI::setSuppressSslErrors(bool suppressSslErrors)
{
  Q_D(qRestAPI);
  d->SuppressSslErrors = suppressSslErrors;
}

// --------------------------------------------------------------------------
QUrl qRestAPI::createUrl(const QString& resource, const qRestAPI::Parameters& parameters)
{
  Q_D(qRestAPI);
  QUrl url(d->ServerUrl + resource);
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
  foreach(const QString& parameter, parameters.keys())
    {
    url.addQueryItem(parameter, parameters[parameter]);
    }
#else
  QUrlQuery urlQuery(url);
  foreach(const QString& parameter, parameters.keys())
    {
    urlQuery.addQueryItem(parameter, parameters[parameter]);
    }
  url.setQuery(urlQuery);
#endif
  return url;
}

// --------------------------------------------------------------------------
void qRestAPI::parseResponse(qRestResult* restResult, const QByteArray& response)
{
  Q_UNUSED(response);
  QList<QVariantMap> result;
  restResult->setResult(result);
}

// --------------------------------------------------------------------------
QUuid qRestAPI::get(const QString& resource, const Parameters& parameters, const qRestAPI::RawHeaders& rawHeaders)
{
  QUrl url = createUrl(resource, parameters);
  QNetworkReply* queryReply = sendRequest(QNetworkAccessManager::GetOperation, url, rawHeaders);
  QUuid queryId(queryReply->property("uuid").toString());
  return queryId;
}

// --------------------------------------------------------------------------
QUuid qRestAPI::get(QIODevice* output, const QString& resource, const Parameters& parameters, const qRestAPI::RawHeaders& rawHeaders)
{
  Q_D(qRestAPI);

  QUrl url = createUrl(resource, parameters);

  if (!output->open(QIODevice::WriteOnly))
    {
    QUuid queryId;
    // TODO How to raise the error? We do not have a qRestResult object yet...
    //    emit errorReceived(queryId, "Cannot open device for writing.");
    return queryId;
    }

  QNetworkReply* queryReply = sendRequest(QNetworkAccessManager::GetOperation, url, rawHeaders);
  QUuid queryId = QUuid(queryReply->property("uuid").toString());
  qRestResult* result = new qRestResult(queryId, queryReply);
  result->ioDevice = output;

  connect(queryReply, SIGNAL(downloadProgress(qint64,qint64)),
          d, SLOT(downloadProgress(qint64,qint64)));
  connect(queryReply, SIGNAL(readyRead()),
          result, SLOT(downloadReadyRead()));
  connect(queryReply, SIGNAL(finished()),
          result, SLOT(downloadFinished()));

  return queryId;
}

// --------------------------------------------------------------------------
QUuid qRestAPI::head(const QString  resource, const Parameters& parameters, const qRestAPI::RawHeaders& rawHeaders)
{
  QUrl url = createUrl(resource, parameters);
  QNetworkReply* queryReply = sendRequest(QNetworkAccessManager::HeadOperation, url, rawHeaders);
  QUuid queryId = QUuid(queryReply->property("uuid").toString());
  return queryId;
}

// --------------------------------------------------------------------------
QUuid qRestAPI::download(const QString& fileName, const QString& resource, const Parameters& parameters, const qRestAPI::RawHeaders& rawHeaders)
{
  Q_UNUSED(rawHeaders);
  Q_D(qRestAPI);

  QIODevice* output = new QFile(fileName);

  QUuid queryId = get(output, resource, parameters);

  output->setParent(d->results[queryId]);

  return queryId;
}

// --------------------------------------------------------------------------
QUuid qRestAPI::del(const QString& resource, const Parameters& parameters, const qRestAPI::RawHeaders& rawHeaders)
{
  QUrl url = createUrl(resource, parameters);
  QNetworkReply* queryReply = sendRequest(QNetworkAccessManager::DeleteOperation, url, rawHeaders);
  QUuid queryId(queryReply->property("uuid").toString());
  return queryId;
}

// --------------------------------------------------------------------------
QUuid qRestAPI::post(const QString& resource, const Parameters& parameters, const qRestAPI::RawHeaders& rawHeaders)
{
  QUrl url = createUrl(resource, parameters);
  QNetworkReply* queryReply = sendRequest(QNetworkAccessManager::PostOperation, url, rawHeaders);
  QUuid queryId(queryReply->property("uuid").toString());
  return queryId;
}

// --------------------------------------------------------------------------
QUuid qRestAPI::put(const QString& resource, const Parameters& parameters, const qRestAPI::RawHeaders& rawHeaders)
{
  QUrl url = createUrl(resource, parameters);
  QNetworkReply* queryReply = sendRequest(QNetworkAccessManager::PutOperation, url, rawHeaders);
  QUuid queryId(queryReply->property("uuid").toString());
  return queryId;
}

QUuid qRestAPI::put(QIODevice *input, const QString &resource, const qRestAPI::Parameters &parameters, const qRestAPI::RawHeaders &rawHeaders)
{
  Q_D(qRestAPI);

  QUrl url = createUrl(resource, parameters);
  if (!input->isOpen() && !input->open(QIODevice::ReadOnly))
    {
    QUuid uid = QUuid::createUuid();
    qRestResult* restResult = new qRestResult(uid);
    restResult->setError(uid.toString() + ": " +
                         "Could not open file for upload!",
                         qRestAPI::FileError);
    d->results[uid] = restResult;
    return uid;
    }

  QByteArray data = input->readAll();

  QNetworkReply* queryReply = sendRequest(QNetworkAccessManager::PutOperation, url, rawHeaders, data);
  QUuid queryId (queryReply->property("uuid").toString());

  qRestResult* result = d->results[queryId];
  result->ioDevice = input;
  connect(queryReply, SIGNAL(uploadProgress(qint64,qint64)),
          d, SLOT(uploadProgress(qint64,qint64)));
  connect(queryReply, SIGNAL(finished()),
          result, SLOT(uploadFinished()));

  return queryId;
}

// --------------------------------------------------------------------------
QUuid qRestAPI::upload(const QString& fileName, const QString& resource, const Parameters& parameters, const qRestAPI::RawHeaders& rawHeaders)
{
  Q_D(qRestAPI);
  QIODevice* input = new QFile(fileName);

  QUuid queryId = this->put(input, resource, parameters, rawHeaders);

  input->setParent(d->results[queryId]);

  return queryId;
}

// --------------------------------------------------------------------------
bool qRestAPI::sync(const QUuid& queryId)
{
  QList<QVariantMap> result;
  return this->sync(queryId, result);
}

// --------------------------------------------------------------------------
bool qRestAPI::sync(const QUuid& queryId, QList<QVariantMap>& result)
{
  Q_D(qRestAPI);
  result.clear();
  if (d->results.contains(queryId))
    {
    bool ok = d->results[queryId]->waitForDone();
    qRestResult* queryResult = d->results.take(queryId);
    if (!ok)
      {
      QVariantMap map;
      map["queryError"] = queryResult->Error;
      queryResult->Result.push_front(map);
      d->ErrorCode = queryResult->errorType();
      d->ErrorString = queryResult->error();
      }
    result = queryResult->Result;
    delete queryResult;
    return ok;
    }
  d->ErrorCode = UnknownUuidError;
  d->ErrorString = unknownUuidStr.arg(queryId.toString());
  return false;
}

// --------------------------------------------------------------------------
QString qRestAPI::qVariantMapListToString(const QList<QVariantMap>& list)
{
  QStringList values;
  foreach(const QVariantMap& map, list)
    {
    foreach(const QString& key, map.keys())
      {
      values << QString("%1: %2").arg(key, map[key].toString());
      }
    }
  return values.join("\n");
}

// --------------------------------------------------------------------------
QString qRestAPI::qVariantToString(const QVariant &value, int indent)
{
  QString output;
  bool isVariantList = value.canConvert<QVariantList>();
  bool isVariantMap = value.canConvert<QVariantMap>();
  if ((isVariantList || isVariantMap) && indent > 0)
    {
    output += "\n";
    }
  if (isVariantList)
    {
    QVariantList list = value.toList();
    for(int idx=0; idx < list.count(); ++idx)
      {
      output.append(QString("%1%2: ").arg(QString(indent, ' ')).arg(idx));
      output.append(qRestAPI::qVariantToString(list.at(idx), indent + 2));
      }
    }
  else if (isVariantMap)
    {
    QVariantMap map = value.toMap();
    foreach(const QString& key, map.keys())
      {
      output.append(QString("%1%2: ").arg(QString(indent, ' ')).arg(key));
      output.append(qRestAPI::qVariantToString(map.value(key), indent + 2));
      }
    }
  else
    {
    return value.toString() + "\n";
    }
  return output;
}

// --------------------------------------------------------------------------
qRestResult* qRestAPI::takeResult(const QUuid& queryId)
{
  Q_D(qRestAPI);
  if (d->results.contains(queryId))
    {
    // Do /not/ try to .take() the query before calling waitForDone();
    // the latter triggers SIGNALs which access d->results.
    bool ok = d->results[queryId]->waitForDone();
    qRestResult* result = d->results.take(queryId);
    if (ok)
      {
      return result;
      }
    else
      {
      d->ErrorCode = result->errorType();
      d->ErrorString = result->error();
      delete result;
      return NULL;
      }
    }
  d->ErrorCode = UnknownUuidError;
  d->ErrorString = QString::number(UnknownUuidError);
  return NULL;
}

// --------------------------------------------------------------------------
qRestAPI::ErrorType qRestAPI::error() const
{
  Q_D(const qRestAPI);
  return d->ErrorCode;
}

// --------------------------------------------------------------------------
QString qRestAPI::errorString() const
{
  Q_D(const qRestAPI);
  return d->ErrorString;
}

//// --------------------------------------------------------------------------
//void qRestAPIPrivate::onAuthenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator)
//{
//  Q_D(qRestAPI);
//  authenticator->setUser(d->userName);
//  authenticator->setPassword(d->password);
//}
