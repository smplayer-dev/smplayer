#ifndef GLOBALDATACLASS_H
#define GLOBALDATACLASS_H
#include <QPointer>
#include "videolayermac.h"

class GlobalDataClass
{
public:
    GlobalDataClass();
    QPointer<VideoLayerMac> gl;
};

extern GlobalDataClass globaldata;

#endif // GLOBALDATACLASS_H
