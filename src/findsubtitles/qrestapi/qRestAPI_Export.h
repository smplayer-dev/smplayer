

// .NAME __qRestAPI_Export - manage Windows system differences
// .SECTION Description
// The __qRestAPI_Export captures some system differences between Unix
// and Windows operating systems. 

#ifndef __qRestAPI_Export_h
#define __qRestAPI_Export_h

#include <QtCore/qglobal.h>

//#cmakedefine qRestAPI_STATIC
#define qRestAPI_STATIC

#if !defined(qRestAPI_STATIC)
#  if defined(Q_OS_WIN) || defined(Q_OS_SYMBIAN)
#    if defined(qRestAPI_EXPORTS)
#      define qRestAPI_EXPORT Q_DECL_EXPORT
#    else
#      define qRestAPI_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#else 
#  define qRestAPI_EXPORT
#endif

#if !defined(qRestAPI_EXPORT)
#  if !defined(qRestAPI_STATIC)
#    define qRestAPI_EXPORT Q_DECL_EXPORT
#  else
#    define qRestAPI_EXPORT
#  endif
#endif

#endif

