#ifndef GLOBALDATACLASS_H
#define GLOBALDATACLASS_H
#include <QPointer>
#include "mplayerwindow.h"

class GlobalDataClass
{
public:
    GlobalDataClass();
    QPointer<MplayerLayer> gl;
};

extern GlobalDataClass globaldata;

#endif // GLOBALDATACLASS_H
