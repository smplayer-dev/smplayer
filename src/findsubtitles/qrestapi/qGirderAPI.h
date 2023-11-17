/*==============================================================================

  Library: qRestAPI

  Copyright (c) 2021 Kitware Inc.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.

==============================================================================*/

#ifndef __qGirderAPI_h
#define __qGirderAPI_h

#include "qRestAPI.h"

#include "qRestAPI_Export.h"

class qRestAPI_EXPORT qGirderAPI : public qRestAPI
{
  Q_OBJECT

  typedef qRestAPI Superclass;

public:
  explicit qGirderAPI(QObject*parent = 0);
  virtual ~qGirderAPI();

  static bool parseGirderAPIv1Response(const QByteArray& response, QList<QVariantMap>& result);

  static bool parseGirderAPIv1Response(qRestResult* restResult, const QByteArray& response);

protected:
  void parseResponse(qRestResult* restResult, const QByteArray& response);

private:
  Q_DISABLE_COPY(qGirderAPI);
};

#endif
