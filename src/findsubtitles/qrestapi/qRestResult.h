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

#ifndef __qRestResult_h
#define __qRestResult_h

// qRestAPI includes
#include "qRestAPI.h"

#include "qRestAPI_Export.h"

class QIODevice;

// --------------------------------------------------------------------------
class qRestAPI_EXPORT qRestResult : public QObject
{
  Q_OBJECT

  friend class qRestAPI;
  friend class qRestAPIPrivate;

  QUuid QueryId;
  QByteArray Reponse;
  QList<QVariantMap> Result;
  QString Error;
  qRestAPI::ErrorType ErrorCode;

  QMap<QByteArray, QByteArray> RawHeaders;

  bool done;
  QIODevice* ioDevice;

public:
  qRestResult(const QUuid& queryId, QObject* parent = 0);
  virtual ~qRestResult();

  const QUuid& queryId() const;

  bool waitForDone();

  const QList<QVariantMap>& results() const;
  const QVariantMap result() const;
  // FIXME: for consistency with the qRestAPI class, this method should be called errorString()
  const QString& error() const;
  // FIXME: for consistency with the qRestAPI class, this method should be called error()
  qRestAPI::ErrorType errorType() const;

  QByteArray rawHeader(const QByteArray& name) const;
  QMap<QByteArray, QByteArray> rawHeaders() const;

  QByteArray response()const;

public slots:
  void setResult();
  void setResult(const QList<QVariantMap>& result); // FIXME: should be called setResults(), see getters
  void setError(const QString& error, qRestAPI::ErrorType errorType = qRestAPI::UnknownError);

  void downloadReadyRead();
  void downloadFinished();
  void uploadFinished();

signals:
  void ready();

private:

  void setRawHeader(const QByteArray& name, const QByteArray& value);

};

#endif
