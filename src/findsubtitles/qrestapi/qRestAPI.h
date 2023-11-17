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

#ifndef __qRestAPI_h
#define __qRestAPI_h

// Qt includes
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
#include <QJSValue>
#else
#include <QScriptValue>
#endif
#include <QUuid>
#include <QVariant>

#include "qRestAPI_Export.h"

template <class Key, class T> class QMap;
typedef QMap<QString, QVariant> QVariantMap;

class QNetworkReply;
class qRestAPIPrivate;

class qRestResult;

/// qRestAPI is a simple interface class to communicate with web services
/// through a public RESTful API.
/// Requests are sent to the server and answers reported back.
/// qRestAPI works in synchronous or asynchronous way.
/// The class should be adopted to specific web services by subclassing. The
/// derived class should define how to construct the requests and how to
/// interpret the responses.
/// The library provides a sample implementation to interact with Midas servers.
/// Usage:
/// <code>
/// qRestAPI* midas = new qMidasAPI();
/// midas->setServerUrl("http://slicer.kitware.com/midas3");
/// connect(midas, SIGNAL(resultReceived(QUuid,QList<QVariantMap>)),
///         myApp, SLOT(processResult(QUuid,QList<QVariantMap>)));
/// midas->query("midas.version");
/// ...
/// delete midas;
/// </code>
class qRestAPI_EXPORT qRestAPI : public QObject
{
  Q_OBJECT

  /// URL of the web application. E.g. "http://slicer.kitware.com/midas3"
  Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl)

  /// Max time to wait until last progress of a query
  Q_PROPERTY(int timeOut READ timeOut WRITE setTimeOut)

  /// Default raw headers to be set for each requests. E.g. it can be used to set
  /// the user-agent or authentication information.
  Q_PROPERTY(RawHeaders defaultRawHeaders READ defaultRawHeaders WRITE setDefaultRawHeaders)

  /// Suppress SSL errors. Can be used to bypass self-signed certificates.
  Q_PROPERTY(bool suppressSslErrors READ suppressSslErrors WRITE setSuppressSslErrors)

  typedef QObject Superclass;

public:

  enum ErrorType
  {
    UnknownError = -1,
    /// An unknown uuid was used
    UnknownUuidError = 1,
    /// A time-out error
    TimeoutError = 2,
    /// Error related to SSL connections
    SslError = 3,
    /// Error parsing the response
    ResponseParseError = 4,
    /// The remote server requires authentication but the credentials
    /// provided were not accepted (if any).
    AuthenticationError = 5,
    /// Error is raised if a file could not be opened
    FileError = 6,
    /// General network error not covered by more specific error types
    NetworkError = 100
  };

  /// Constructs a qRestAPI object.
  explicit qRestAPI(QObject*parent = 0);
  /// Destructs a qRestAPI object.
  virtual ~qRestAPI();

  /// Type of the parameter list of a REST request.
  typedef QMap<QString, QString> Parameters;
  /// Type of the raw header list of a REST request.
  typedef QMap<QByteArray, QByteArray> RawHeaders;

  /// Returns the URL of the web application.
  QString serverUrl()const;
  /// Sets the URL of the web application.
  void setServerUrl(const QString& serverUrl);

  /// Sets the HTTP network proxy that will be used for all queries
  void setHttpNetworkProxy(const QNetworkProxy& proxy);

  /// Returns the raw headers that are set for every request.
  RawHeaders defaultRawHeaders()const;
  /// Sets the raw headers to be set for every request.
  void setDefaultRawHeaders(const RawHeaders& defaultRawHeaders);

  /// Tells if the SSL errors are suppressed or not.
  bool suppressSslErrors()const;
  /// Sets if the SSL errors are to be suppressed or not.
  void setSuppressSslErrors(bool suppressSslErrors);

  void setTimeOut(int msecs);
  int timeOut()const;

  /// Sends a GET request to the web service.
  /// The \a resource and \parameters are used to compose the URL.
  /// \a rawHeaders can be used to set the raw headers of the request to send.
  /// These headers will be set additionally to those defined by the
  /// \a defaultRawHeaders property.
  /// errorReceived() is emitted if no server is found or if the server sends
  /// errors.
  /// resultReceived() is emitted when a result is received from the server,
  /// it is fired even if errors are received.
  /// Returns a unique identifier of the posted query.
  virtual QUuid get(const QString& resource,
    const Parameters& parameters = Parameters(),
    const RawHeaders& rawHeaders = RawHeaders());

  virtual QUuid get(QIODevice* output,
    const QString& resource,
    const Parameters& parameters = Parameters(),
    const RawHeaders& rawHeaders = RawHeaders());

  /// Sends a HEAD request to the web service.
  /// The \a resource and \parameters are used to compose the URL.
  /// \a rawHeaders can be used to set the raw headers of the request to send.
  /// These headers will be set additionally to those defined by the
  /// \a defaultRawHeaders property.
  /// errorReceived() is emitted if no server is found or if the server sends
  /// errors.
  /// resultReceived() is emitted when a result is received from the server,
  /// it is fired even if errors are received.
  /// Returns a unique identifier of the posted query.
  virtual QUuid head(const QString resource,
                              const Parameters& parameters = Parameters(),
                              const RawHeaders& rawHeaders = RawHeaders());

  /// Downloads a file from the web service.
  /// \a fileName is the name of the output file.
  /// The \a resource and \parameters are used to compose the URL.
  /// \a rawHeaders can be used to set the raw headers of the request to send.
  /// These headers will be set additionally to those defined by the
  /// \a defaultRawHeaders property.
  /// errorReceived() is emitted if no server is found or if the server sends
  /// errors.
  /// resultReceived() is emitted when a result is received from the server,
  /// it is fired even if errors are received.
  /// Returns a unique identifier of the posted query.
  virtual QUuid download(const QString& fileName,
    const QString& resource,
    const Parameters& parameters = Parameters(),
    const RawHeaders& rawHeaders = RawHeaders());

  /// Sends a DELETE request to the web service.
  /// The \a resource and \parameters are used to compose the URL.
  /// \a rawHeaders can be used to set the raw headers of the request to send.
  /// These headers will be set additionally to those defined by the
  /// \a defaultRawHeaders property.
  /// errorReceived() is emitted if no server is found or if the server sends
  /// errors.
  /// resultReceived() is emitted when a result is received from the server,
  /// it is fired even if errors are received.
  /// Returns a unique identifier of the posted query.
  QUuid del(const QString& resource,
    const Parameters& parameters = Parameters(),
    const RawHeaders& rawHeaders = RawHeaders());

  /// Sends a POST request to the web service.
  /// The \a resource and \parameters are used to compose the URL.
  /// \a rawHeaders can be used to set the raw headers of the request to send.
  /// These headers will be set additionally to those defined by the
  /// \a defaultRawHeaders property.
  /// errorReceived() is emitted if no server is found or if the server sends
  /// errors.
  /// resultReceived() is emitted when a result is received from the server,
  /// it is fired even if errors are received.
  /// Returns a unique identifier of the posted query.
  QUuid post(const QString& resource,
    const Parameters& parameters = Parameters(),
    const RawHeaders& rawHeaders = RawHeaders());

  /// Sends a PUT request to the web service.
  /// The \a resource and \parameters are used to compose the URL.
  /// \a rawHeaders can be used to set the raw headers of the request to send.
  /// These headers will be set additionally to those defined by the
  /// \a defaultRawHeaders property.
  /// If an \a input is specified it will be send together with the request.
  /// errorReceived() is emitted if no server is found or if the server sends
  /// errors.
  /// resultReceived() is emitted when a result is received from the server,
  /// it is fired even if errors are received.
  /// Returns a unique identifier of the posted query.
  QUuid put(const QString& resource,
    const Parameters& parameters = Parameters(),
    const RawHeaders& rawHeaders = RawHeaders());

  virtual QUuid put(QIODevice* input,
    const QString& resource,
    const Parameters& parameters = Parameters(),
    const RawHeaders& rawHeaders = RawHeaders());

  QUuid upload(const QString& fileName,
    const QString& resource,
    const Parameters& parameters = Parameters(),
    const RawHeaders& rawHeaders = RawHeaders());

  /// Blocks until the result for the uuid \a queryId is available.
  /// Returns false if an error occured.
  /// \sa ErrorType
  /// \sa error()
  /// \sa errorString()
  bool sync(const QUuid& queryId);

  /// Blocks until the results for the uuid \a queryId is available and
  /// stores the results in the given \a result parameter.
  /// Returns false if an error occured.
  /// \sa ErrorType
  /// \sa error()
  /// \sa errorString()
  bool sync(const QUuid& queryId, QList<QVariantMap>& result);

  /// Get a qRestResult object for the specified QUuid.
  /// If the \a queryId parameter is unknown, this function
  /// returns NULL and sets the error state to ErrorType::UnknownUuid.
  qRestResult* takeResult(const QUuid& queryId);

  /// Get the error code for the last error which occured.
  ErrorType error() const;

  /// Get the error description for the last error which occured.
  QString errorString() const;

  /// Utility function that transforms a QList of QVariantMap into a string.
  /// Mostly for debug purpose.
  static QString qVariantMapListToString(const QList<QVariantMap>& variants);

  /// \brief Return an indented representation of a QVariant considering map and list.
  ///
  /// \example
  ///
  /// ```
  /// a: 1
  /// b:
  ///   b_a: 2-1
  ///   b_b: 2-2
  ///   b_c: 2-3
  /// c:
  ///   0:
  ///     c_a_1: 3-1_1
  ///     c_b_1: 3-2_1
  ///     c_c_1: 3-3_1
  ///   1:
  ///     c_a_2: 3-1_3
  ///     c_b_2: 3-2_2
  ///     c_b_3: 3-2_3
  /// ```
  static QString qVariantToString(const QVariant &value, int indent=0);

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  static QVariantMap scriptValueToMap(const QJSValue& value);
  static void appendScriptValueToVariantMapList(QList<QVariantMap>& result, const QJSValue& value);
#else
  static QVariantMap scriptValueToMap(const QScriptValue& value);
  static void appendScriptValueToVariantMapList(QList<QVariantMap>& result, const QScriptValue& value);
#endif

  /// \brief Flatten a QVariantMap of nested QVariantList, QVariantMap and QVariant.
  ///
  /// Given a dictionnary like the following:
  ///
  /// ```
  /// {
  ///   "a": "1",
  ///   "b": {
  ///     "b_a": "2-1",
  ///     "b_b": "2-2",
  ///     "b_c": "2-3"
  ///   },
  ///   "c": [
  ///     {
  ///       "c_a_1": "3-1_1",
  ///       "c_b_1": "3-2_1",
  ///       "c_c_1": "3-3_1"
  ///     },
  ///     {
  ///       "c_a_2": "3-1_2",
  ///       "c_b_2": "3-2_2"
  ///     }
  ///   ]
  /// }
  /// ```
  ///
  /// It returns this flattened dictionnary:
  ///
  /// ```
  /// {
  ///   "a": "1",
  ///   "b.b_a": "2-1",
  ///   "b.b_b": "2-2",
  ///   "b.b_c": "2-3",
  ///   "c.c_a_1": "3-1_1",
  ///   "c.c_b_1": "3-2_1",
  ///   "c.c_c_1": "3-3_1",
  ///   "c.c_a_2": "3-1_2",
  ///   "c.c_b_2": "3-2_2"
  /// }
  /// ```
  ///
  /// \warning In list of dictionnaries where the same key is found in multiple
  /// dictionnaries, only the value associated associated with the key found in
  /// the last list is kept.
  ///
  static QVariantMap qVariantMapFlattened(const QVariantMap& value);

signals:
  void finished(const QUuid& queryId);
  void progress(const QUuid& queryId, double progress);

protected:
  QNetworkReply* sendRequest(QNetworkAccessManager::Operation operation,
      const QUrl& url,
      const RawHeaders& rawHeaders = RawHeaders(),
      const QByteArray& data = QByteArray());

  virtual QUrl createUrl(const QString& method, const qRestAPI::Parameters& parameters);
  virtual void parseResponse(qRestResult* restResult, const QByteArray& response);

private:
  QScopedPointer<qRestAPIPrivate> d_ptr;

  Q_DECLARE_PRIVATE(qRestAPI);
  Q_DISABLE_COPY(qRestAPI);
};

#endif
